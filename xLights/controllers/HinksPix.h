#pragma once

/***************************************************************
 * This source files comes from the xLights project
 * https://www.xlights.org
 * https://github.com/smeighan/xLights
 * See the github commit history for a record of contributing
 * developers.
 * Copyright claimed based on commit dates recorded in Github
 * License: https://github.com/smeighan/xLights/blob/master/License.txt
 **************************************************************/

#include <array>
#include <list>
#include <memory>
#include <string>

#include "BaseController.h"
#include "ControllerUploadData.h"
#include "../UtilClasses.h"

class HinksPix;
class wxJSONValue;

struct HinksPixOutput {
    HinksPixOutput(int output_) :
        output(output_),
        universe(1),
        startChannel(1),
        pixels(50),
        direction(0),
        protocol(0),
        nullPixel(0),
        colorOrder(0),
        brightness(100),
        gamma(1),
        controlerStartChannel(1),
        controlerEndChannel(150){};
    const int output;
    int universe;
    int startChannel;
    int pixels;
    int direction;
    int protocol;
    int nullPixel;
    int colorOrder;
    int brightness;
    int gamma;

    int getControllerStartChannel() const { return controlerStartChannel; }
    int getControllerEndChannel() const { return controlerEndChannel; }
    void setControllerChannels(const int startChan);

    void Dump() const;
    void SetConfig(wxString const& data);
    wxString BuildCommand() const;
    wxString BuildCommandEasyLights() const;

private:
    int controlerStartChannel;
    int controlerEndChannel;
};

struct HinksPixSerial {
    HinksPixSerial() :
        e131Universe(1),
        e131StartChannel(1),
        e131NumOfChan(512),
        e131Enabled(false),
        ddpDMXStartChannel(1),
        ddpDMXNumOfChan(512),
        ddpDMXEnabled(false),
        upload(false){};
    int e131Universe;
    int e131StartChannel;
    int e131NumOfChan;
    bool e131Enabled;
    int ddpDMXStartChannel;
    int ddpDMXNumOfChan;
    bool ddpDMXEnabled;
    bool upload;
    void Dump() const;
    void SetConfig(wxJSONValue const& data);

    wxString BuildCommand() const;
    wxString BuildCommandEasyLights(int mode) const;
};

struct HinksSmartOutput {
    HinksSmartOutput(int id_) :
        id(id_), 
        type(0), 
        portStartPixel{ 0, 0, 0, 0 } {};
    int id;
    int type;
    std::array<int, 4> portStartPixel;

    bool operator==(const HinksSmartOutput& rhs) const {
        return id == rhs.id;
    }

    void Dump() const;
    void SetConfig(wxString const& data);
    wxString BuildCommand() const;
};

struct HinksPixInputUniverse {
    HinksPixInputUniverse(int universe_, int numOfChan_) :
        index(-1),
        universe(universe_),
        numOfChan(numOfChan_),
        hinksPixStartChannel(1){};
    int index;
    int universe;
    int numOfChan;
    int32_t hinksPixStartChannel;

    bool operator==(const HinksPixInputUniverse& rhs) const {
        return universe == rhs.universe;
    }

    void Dump() const;
    wxString BuildCommand() const;
    wxString BuildCommandEasyLights() const;
};

class HinksPix : public BaseController
{
    enum class EXPType {
        Not_Present,
        Local_SPI,
        Long_Range,
        Local_AC
    };

    static constexpr int UN_PER = 6;

#pragma region Member Variables
    EXPType _EXP_Outputs[3];
    std::string _controllerType;
    int _numberOfOutputs;
    int _numberOfUniverses;
    int _MCPU_Version;

    std::vector<HinksPixOutput> _pixelOutputs;
    std::unique_ptr<HinksPixSerial> _serialOutput;
    std::vector<HinksSmartOutput> _smartOutputs[3][4];
#pragma endregion

#pragma region Encode and Decode
    int EncodeColorOrder(const std::string& colorOrder) const;
    int EncodeDirection(const std::string& direction) const;
    int EncodeStringPortProtocol(const std::string& protocol) const;
    int EncodeBrightness(int brightness) const;
    int EncodeGamma(int gamma) const;
    EXPType DecodeExpansionType(const std::string& type) const;
#pragma endregion

#pragma region Private Functions
    bool InitControllerOutputData();
    void InitExpansionBoardData(int expansion, int startport, int length);
    std::unique_ptr<HinksPixSerial> InitSerialData();

    bool UploadInputUniverses(Controller* controller, std::vector<HinksPixInputUniverse> const& inputUniverses) const;
    
    bool UploadInputUniversesEasyLights(Controller* controller, std::vector<HinksPixInputUniverse> const& inputUniverses) const;
    void UploadPixelOutputsEasyLights(bool& worked);
    wxString GetControllerE131Data(int rowIndex) const;
    wxString GetControllerData(int rowIndex, std::string const& data = std::string())const ;
    wxString GetControllerRowData(int rowIndex, std::string const& url, std::string const& data) const;
    std::map<wxString, wxString> StringToMap(wxString const& text) const;
    
    void UpdatePortData(HinksPixOutput& pd, UDControllerPort* stringData, int32_t hinkstartChan) const;
    void UpdateSerialData(HinksPixSerial& pd, UDControllerPort* serialData, int const mode, std::vector<HinksPixInputUniverse>& inputUniverses, int32_t& hinkstartChan, int& index, bool individualUniverse) const;
    void UploadPixelOutputs(bool& worked) const;
    void UpdateUniverseControlerChannels(UDControllerPort* stringData, std::vector<HinksPixInputUniverse>& inputUniverses, int32_t& hinkstartChan, int& index, bool individualUniverse);
    void UploadExpansionBoardData(int expansion, int startport, int length, bool& worked) const;
    void UploadSmartRecievers(bool& worked) const;
    void UploadSmartRecieverData(int expan, int bank, std::vector<HinksSmartOutput> const& receivers, bool& worked) const;
    void CalculateSmartRecievers(UDControllerPort* stringData);
    void SendRebootController(bool& worked) const;
    std::string GetJSONControllerData(std::string const& url, std::string const& data) const;
    bool GetControllerDataJSON(const std::string& url, wxJSONValue& val, std::string const& data) const;
    void PostToControllerNoResponce(std::string const& url, std::string const& data) const;
    static const std::string GetJSONPostURL() { return "/Xlights_PostData.cgi"; };
    static const std::string GetJSONInfoURL() { return "/XLights_BoardInfo.cgi"; };
    static const std::string GetJSONPortURL() { return "/Xlights_Board_Port_Config.cgi"; };
    static const std::string GetJSONModeURL() { return "/Xlights_Data_Mode.cgi"; };
    static const std::string GetE131URL() { return"/GetE131Data.cgi"; };
    static const std::string GetInfoURL() { return"/GetInfo.cgi"; };
    const int GetNumberOfOutputs() { return _numberOfOutputs; }
    const int GetNumberOfSerial() { return 1; }

#pragma endregion

public:
#pragma region Constructors and Destructors
    HinksPix(const std::string& ip, const std::string& fppProxy);
    virtual ~HinksPix();
#pragma endregion

#pragma region Getters and Setters
#ifndef DISCOVERYONLY
    bool SetOutputs(ModelManager* allmodels, OutputManager* outputManager, Controller* controller, wxWindow* parent) override;
#endif
    virtual bool UsesHTTP() const override { return true; }
#pragma endregion
};
