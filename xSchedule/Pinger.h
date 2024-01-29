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
#include <list>
#include <atomic>

#include "../xLights/outputs/Output.h"

#define PINGINTERVAL 60

class OutputManager;
class PingThread;
class ListenerManager;
class Controller;

class APinger
{
    PingThread* _pingThread = nullptr;
	Controller* _controller = nullptr;
	std::atomic<Output::PINGSTATE> _lastResult;
    std::string _ip;
    std::string _why;
    ListenerManager* _listenerManager = nullptr;
    std::atomic<int> _failCount;
    bool _isLocal = false;
    bool _active = true;

    void CheckLocal();

    public:

    void SetPingResult(Output::PINGSTATE result);
    bool IsOutput() const { return _controller != nullptr; }
	APinger(ListenerManager* listenerManager, Controller* controller);
	APinger(ListenerManager* listenerManager, const std::string ip, const std::string why);
	virtual ~APinger();
	Output::PINGSTATE GetPingResult() const;
    bool GetPingResult(Output::PINGSTATE state) const;
    static std::string GetPingResultName(Output::PINGSTATE state);
    Output::PINGSTATE Ping();
	std::string GetName() const;
    int GetPingInterval() const { return PINGINTERVAL; }
    void Stop();
    std::string GetIP() const { return _ip; }
    int GetFailCount() const { return  _failCount; }
    bool IsInactive() const { return !_active; }
    bool IsLocal() const
    {
        return _isLocal;
    }
};

class Pinger
{
    ListenerManager* _listenerManager = nullptr;
    std::list<APinger*> _pingers;
    bool _stopping = false;
	
	public:
		Pinger(ListenerManager* listenerManager, OutputManager* outputManager);
		virtual ~Pinger();
        std::list<APinger*> GetPingers() const { if (_stopping) return std::list<APinger*>(); return _pingers; }
        void AddIP(const std::string ip, const std::string why);
        void RemoveNonOutputIPs();
        APinger* GetPinger(const std::string& ip) const;
        APinger* GetPingerByIndex(int index) const;
};
