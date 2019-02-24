#include "xLightsTimer.h"
#include <wx/thread.h>
#include <log4cpp/Category.hh>
#include <mutex>

#ifndef __WXOSX__
#define USE_THREADED_TIMER
#endif

#ifdef USE_THREADED_TIMER

class xlTimerThread : public wxThread
{
public:
    xlTimerThread(const std::string& name, int interval, bool oneshot, xLightsTimer* timer, bool log);
    void Reset(int interval, bool oneshot, const std::string& name);
    void Stop();
    void Suspend();
    void SetFudgeFactor(int ff);
    int GetInterval() const { return _interval; }
private:
    std::atomic<bool> _stop;
    std::atomic<bool> _suspend;
    std::atomic<bool> _oneshot;
    std::atomic<int> _interval;
    std::atomic<int> _suspendCount;
    int _fudgefactor;
    bool _log;
    xLightsTimer* _timer;
    std::string _name;

    // the main thread holds a lock on this mutex while the timer is running
    // it is the timer thread timing out trying to lock it that actually makes the timer work
    // when stopped or suspended the main thread releases the lock.
    // If the timer thread manages to get the lock it immediately releases it
    std::timed_mutex _waiter;

    // when the main thread holds this lock the timer thread will block ... waiting for it to be
    // released. Once the timer thread gets it it immediately releases it.
    std::mutex _suspendLock;

    void DoSleep(int millis);
    virtual ExitCode Entry() override;
};

#pragma region xlTimerTimer
xLightsTimer::xLightsTimer()
{
    _log = false;
    _suspend = false;
    _timerCallback = nullptr;
    _t = nullptr;
    _pending = false;
    _name = "";
}

xLightsTimer::~xLightsTimer()
{
    if (_t != nullptr)
    {
        _t->Stop();
    }
}

void xLightsTimer::Stop()
{
    if (_t != nullptr)
    {
        _t->Suspend();
    }
}

bool xLightsTimer::Start(int time/* = -1*/, bool oneShot/* = wxTIMER_CONTINUOUS*/, const std::string& name)
{
    static log4cpp::Category &logger_timer = log4cpp::Category::getInstance(std::string("log_timer"));
    wxStopWatch sw;

    // While there is support for one shot timers here ... it is not the most robust code and should
    // be avoided if possible
    wxASSERT(oneShot == wxTIMER_CONTINUOUS);

    if (name != "") _name = name;

    if (_t == nullptr)
    {
        logger_timer.debug("Timer thread created for %s", (const char*)_name.c_str());
        _t = new xlTimerThread(name, time, oneShot, this, _log);
        if (_t == nullptr) return false;
        _t->Create();
        _t->SetPriority(WXTHREAD_DEFAULT_PRIORITY + 1); // run it with slightly higher priority to ensure events are generated in a timely manner
        _t->Run();
    }
    else
    {
        logger_timer.info("Resetting timer %s as thread already exists.", (const char*)_name.c_str());
        Stop();
        _t->Reset(time, oneShot, _name);
    }

    logger_timer.debug("Timer %s started in %ldms", (const char*)_name.c_str(), sw.Time());

    return true;
}

void xLightsTimer::DoSendTimer() {
    if (!_pending) {
        return;
    }
    wxTimer::Notify();
    //reset pending to false AFTER sending the event so if sending takes to long, it results in a skipped frame instead of
    //infinite number of CallAfters consuming the CPU
    _pending = false;
}

void xLightsTimer::Notify() {
    if (_suspend)
    {
        return;
    }

    if (_timerCallback != nullptr)
    {
        wxTimerEvent event(*this);
        _timerCallback->TimerCallback(event);
    }
    else
    {
        _pending = true;
        CallAfter(&xLightsTimer::DoSendTimer);
    }
}

int xLightsTimer::GetInterval() const
{
    if (_t != nullptr)
    {
        return _t->GetInterval();
    }
    return -1;
}

xlTimerThread::xlTimerThread(const std::string& name, int interval, bool oneshot, xLightsTimer* timer, bool log) : wxThread(wxTHREAD_JOINABLE)
{
    static log4cpp::Category &logger_timer = log4cpp::Category::getInstance(std::string("log_timer"));

    // shouldnt be creating the timer thread with an interval less than zero
    wxASSERT(interval >= 0);

    _name = name;
    _log = log;
    _stop = false;
    _suspend = false;
    _fudgefactor = 0;
    _interval = interval;
    _timer = timer;
    _oneshot = (oneshot == wxTIMER_ONE_SHOT);
    _suspendCount = 0;

    // grab the wait lock so the timer thread has its timing behaviour
    logger_timer.debug("About to grab the waiter");
    _waiter.lock();
    logger_timer.debug("    got it");
}

void xlTimerThread::Reset(int interval, bool oneshot, const std::string& name)
{
    static log4cpp::Category &logger_timer = log4cpp::Category::getInstance(std::string("log_timer"));

    if (name != "") _name = name;

    logger_timer.debug("Timer %s reset from interval %d to interval %d %s", (const char*)_name.c_str(), (int)_interval, interval, oneshot ? "ONESHOT" : "");

    wxASSERT(_suspend == true);
    wxASSERT(_stop == false);

    int oldInterval = _interval;

    if (oldInterval == -99)
    {
        logger_timer.debug("Timer being reset was a one shot");
    }

    // only set the interval to a new value if it is greater than or equal to zero
    if (interval >= 0)
    {
        _interval = interval;
    }
    _oneshot = oneshot;

    wxStopWatch sw;

    // if this was not a one shot suspend
    if (oldInterval != -99)
    {
        // grab the wait lock so the timer thread goes back to its timing behaviour
        logger_timer.debug("About to grab the waiter");
        _waiter.lock();
        logger_timer.debug("    got it");
    }

    _suspend = false;

    // if this was not a one shot suspend
    if (oldInterval != -99)
    {
        // now release the suspend
        logger_timer.debug("About to release the suspendLock");
        _suspendLock.unlock();
        logger_timer.debug("    released");
    }

    logger_timer.debug("    Reset took %ldms", sw.Time());
}

void xlTimerThread::Suspend()
{
    static log4cpp::Category &logger_timer = log4cpp::Category::getInstance(std::string("log_timer"));

    if (_suspend) return;

    logger_timer.debug("Timer %s suspend", (const char*)_name.c_str());
    wxStopWatch sw;

    _suspend = true;

    // lock the suspend lock on the main thread so the timer thread will block until it is released
    logger_timer.debug("About to grab the suspendLock");
    _suspendLock.lock();
    logger_timer.debug("    got it");

    int sc = _suspendCount;

    // release this lock on the main thread which immediately stops the timer wait
    logger_timer.debug("About to release the waiter");
    _waiter.unlock();
    logger_timer.debug("    released");

    // this ensures the other thread saw suspended before we continue ...
    // but also even worst case we never wait more than 3ms
    int i = 0;
    while (sc == _suspendCount && i < 3)
    {
        // give the timer thread a chance to use the unlocked waiter
        wxMilliSleep(1);
        i++;
    }
    if (i == 3)
    {
        logger_timer.warn("    Waited 3 seconds for thread to lock and didnt see it ... maybe because it grabbed it early.");
    }

    logger_timer.debug("    Suspend took %ldms", sw.Time());
}

void xlTimerThread::Stop()
{
    static log4cpp::Category &logger_timer = log4cpp::Category::getInstance(std::string("log_timer"));

    if (_stop) return;

    logger_timer.debug("Timer %s stop", (const char*)_name.c_str());
    wxStopWatch sw;

    int oldInterval = _interval;
    _stop = true;
    _suspend = false;

    // release this lock on the main thread which immediately stops the timer wait
    _waiter.unlock();

    // if this was not a one shot suspend
    if (oldInterval != -99)
    {
        // also release any suspended state so the thread will exit
        _suspendLock.unlock();
    }

    // give the timer thread a chance to use the unlocked waiter
    wxMilliSleep(1);

    logger_timer.debug("    Stop took %ldms", sw.Time());
}

void xlTimerThread::DoSleep(int millis)
{
    static log4cpp::Category &logger_timer = log4cpp::Category::getInstance(std::string("log_timer"));
    if (millis > 5000)
    {
        logger_timer.debug("THREAD: DoSleep(%d)", millis);
    }

    // try to grab the lock but time out after the desired number of milliseconds
    if (_waiter.try_lock_for(std::chrono::milliseconds(millis)))
    {
        if (millis > 5000)
        {
            logger_timer.debug("THREAD: DoSleep(%d) ... timer was aborted", millis);
        }
        wxASSERT(_suspend == true || _stop == true);

        // we got the lock so release it immediately
        _waiter.unlock();
    }
    else
    {
        if (millis > 5000)
        {
            logger_timer.debug("THREAD: DoSleep(%d) ... %s timer timed out", millis, (const char*)_name.c_str());
        }
    }
}

wxThread::ExitCode xlTimerThread::Entry()
{
    static log4cpp::Category &logger_timer = log4cpp::Category::getInstance(std::string("log_timer"));

    bool log = _log;
    bool stop = _stop;
    bool oneshot = _oneshot;
    int interval = _interval;
    int fudgefactor = _fudgefactor;
    long long last = wxGetLocalTimeMillis().GetValue();

    while (!_stop)
    {
        if (_suspend)
        {
            logger_timer.debug("THREAD: Timer %s thread suspended. Interval %d", (const char *)_name.c_str(), interval);

            // If we were one shot we cant use the fancy locks because we were in this thread
            // when we suspended ... and we cant change threads as the delay will cause an issue
            if (_interval == -99)
            {
                // this was one shot so we cant use the locks ... do it oldschool
                ++_suspendCount;
                while (_suspend)
                {
                    wxMilliSleep(1);
                }
            }
            else
            {
                ++_suspendCount;

                // we look like we are in suspend mode so try to grab the suspend lock and block until we get it
                logger_timer.debug("THREAD: About to grab the suspend lock");
                _suspendLock.lock();
                logger_timer.debug("THREAD:     got it");

                // now we got it ... release it
                _suspendLock.unlock();
                logger_timer.debug("THREAD:     released it");
            }

            logger_timer.debug("THREAD: Timer %s thread unsuspended.", (const char *)_name.c_str());
            last = wxGetLocalTimeMillis().GetValue();
        }

        oneshot = _oneshot;
        interval = _interval;

        if (!_stop)
        {
            long long now = wxGetLocalTimeMillis().GetValue();
            long long toSleep = last + interval + fudgefactor - now;
            //if (log)
            //{
            //    logger_base.debug("Timer sleeping for %ldms", (std::max)(1, (int)toSleep));
            //}
            DoSleep((std::max)(1, (int)toSleep));

            last = wxGetLocalTimeMillis().GetValue();
            stop = _stop;
            bool suspend = _suspend;
            fudgefactor = _fudgefactor;
            if (!stop && !suspend)
            {
                logger_timer.debug("THREAD: Timer %s fired.", (const char *)_name.c_str());
                _timer->Notify();
            }
            if (oneshot)
            {
                logger_timer.debug("THREAD: %s ONESHOT SO AUTOMATICALLY SUSPENDING.", (const char *)_name.c_str());
                _suspend = true;
                _interval = -99;
                interval = -99;
            }
        }
    }

    logger_timer.debug("Timer %s thread exiting.", (const char *)_name.c_str());

    return wxThread::ExitCode(nullptr);
}

void xlTimerThread::SetFudgeFactor(int ff)
{
    _fudgefactor = ff;
}
#else
xLightsTimer::xLightsTimer() {}
xLightsTimer::~xLightsTimer() {}
void xLightsTimer::Stop() {wxTimer::Stop();}
bool xLightsTimer::Start(int time, bool oneShot, const std::string& name) {return wxTimer::Start(time, oneShot);};
void xLightsTimer::Notify() {wxTimer::Notify();}
int xLightsTimer::GetInterval() const { return wxTimer::GetInterval(); }
void xLightsTimer::DoSendTimer() {};
#endif
