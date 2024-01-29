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

#include <list>
#include <string>

#include "BaseController.h"

struct J1SysPixelOutput;
struct J1SysSerialOutput;

class J1Sys : public BaseController
{
    #pragma region Member Variables
    int _outputs = 0;
    std::string _username = "admin";
    std::string _password = "";
    #pragma endregion

    #pragma region Encode and Decode
    char EncodeStringPortProtocol(std::string protocol) const;
    char EncodeSerialPortProtocol(std::string protocol) const;
    int DecodeProtocolSpeed(std::string protocol) const;
    #pragma endregion

    #pragma region String Port Handling
    std::string BuildStringPort(bool active, int string, char protocol, int speed, int startChannel, int universe, int pixels, wxWindow* parent) const;
    void ResetStringOutputs();
    void ReadCurrentConfig(std::vector<J1SysPixelOutput>& j);
    void DumpConfig(const std::vector<J1SysPixelOutput>& j) const;
    int GetBankSize() const;
    #pragma endregion

    #pragma region Serial Port Handling
    std::string BuildSerialPort(bool active, int port, char protocol, int speed, int universe, wxWindow* parent) const;
    void ResetSerialOutputs();
    void DumpConfig(const std::vector<J1SysSerialOutput>& j) const;
    void ReadCurrentSerialConfig(std::vector<J1SysSerialOutput>& j);
    #pragma endregion

    #pragma region Private Functions
    void Reboot();
    #pragma endregion

public:
    #pragma region Constructors and Destructors
    J1Sys(const std::string& ip, const std::string &proxy);
    virtual ~J1Sys() {}
    #pragma endregion

    #pragma region Getters and Setters
#ifndef DISCOVERYONLY
    bool SetInputUniverses(Controller* controller, OutputManager* outputManager);
    bool SetOutputs(ModelManager* allmodels, OutputManager* outputManager, Controller* controller, wxWindow* parent) override;
#endif
    virtual bool UsesHTTP() const override { return true; }
    #pragma endregion
};

