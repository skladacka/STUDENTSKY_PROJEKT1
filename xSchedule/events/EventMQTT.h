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

#include "EventBase.h"

class EventMQTT: public EventBase
{
    std::string _topic;
    std::string _ip;
    std::string _username;
    std::string _password;
    std::string _clientId;
    int _port;

    public:
		EventMQTT();
		EventMQTT(wxXmlNode* node);
		virtual ~EventMQTT() {}
		virtual wxXmlNode* Save() override;
		virtual std::string GetType() const override { return "MQTT"; }
        void SetTopic(std::string topic) { if (_topic != topic) { _topic = topic; _changeCount++; } }
        void SetUsername(std::string username) { if (_username != username) { _username = username; _changeCount++; } }
        void SetPassword(std::string password) { if (_password != password) { _password = password; _changeCount++; } }
        void SetClientId(std::string clientId);
        void SetDefaultClientId() { _clientId = "xSchedule"; }
        void SetBrokerIP(std::string ip) { if (_ip != ip) { _ip = ip; _changeCount++; } }
        void SetBrokerPort(int port) { if (_port != port) { _port = port; _changeCount++; } }
        std::string GetTopic() const { return _topic; }
        std::string GetUsername() const { return _username; }
        std::string GetPassword() const { return _password; }
        std::string GetClientId() const { return _clientId; }
        std::string GetBrokerIP() const { return _ip; }
        int GetBrokerPort() const { return _port; }
        virtual void Process(const std::string& topic, const std::string& data, ScheduleManager* scheduleManager) override;
        static std::string GetParmToolTip();
};

