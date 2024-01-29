#pragma once

/***************************************************************
 * This source files comes from the xLights project
 * https://www.xlights.org
 * https://github.com/xLightsSequencer/xLights
 * See the github commit history for a record of contributing
 * developers.
 * Copyright claimed based on commit dates recorded in Github
 * License: https://github.com/xLightsSequencer/xLights/blob/master/License.txt
 **************************************************************/

#include <string>
#include <wx/wx.h>
#include <atomic>

class ListenerManager;
class ScheduleManager;
class ListenerThread;

class ListenerBase
{
    protected:
	    bool _stop;
        ListenerManager* _listenerManager;
        ListenerThread* _thread;
        bool _isOk;
        std::atomic<int> _frameMS;
        std::string _localIP;

	public:
        ListenerBase(ListenerManager* listenerManager, const std::string& localIP);
		virtual ~ListenerBase() {}
		virtual void Start() = 0;
        virtual void Stop() = 0;
        virtual std::string GetType() const = 0;
        virtual void StartProcess(const std::string& localIP) = 0;
        virtual void StopProcess() = 0;
        bool IsOk() const { return _isOk; }
        virtual void Poll() {};
        void SetFrameMS(int frameMS) { _frameMS = frameMS; }
};

class ListenerThread : public wxThread
{
    bool _stop = false;
    ListenerBase* _listener = nullptr;
    bool _running = false;
    std::string _localIP;

public:
    ListenerThread(ListenerBase* listener, const std::string& localIP);
    virtual ~ListenerThread()
    {
        Stop();
    }

    void Stop()
    {
        _stop = true;
        Wait();
    }

    virtual void* Entry() override;
};
