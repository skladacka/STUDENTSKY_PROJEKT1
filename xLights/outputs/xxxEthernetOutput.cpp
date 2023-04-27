
/***************************************************************
 * This source files comes from the xLights project
 * https://www.xlights.org
 * https://github.com/smeighan/xLights
 * See the github commit history for a record of contributing
 * developers.
 * Copyright claimed based on commit dates recorded in Github
 * License: https://github.com/smeighan/xLights/blob/master/License.txt
 **************************************************************/

#include "xxxEthernetOutput.h"

#include <wx/xml/xml.h>
#include <wx/process.h>

#include "OutputManager.h"
#include "../UtilFunctions.h"
#include "../utils/ip_utils.h"

#include <log4cpp/Category.hh>

static const int32_t xxxCHANNELSPERPACKET = 1200;

#pragma region Private Functions
void xxxEthernetOutput::Heartbeat(int mode, const std::string& localIP) {

    static log4cpp::Category& logger_base = log4cpp::Category::getInstance(std::string("log_base"));

    static wxLongLong __lastTime = 0;
    static wxIPV4address __remoteAddr;
    static uint8_t __pkt[] = { 0x80, 0x01, 0x00, 0x81 };
    static wxDatagramSocket* __datagram = nullptr;

    if (mode == 1) {
        // output
        if (__datagram == nullptr) return;

        wxLongLong now = wxGetUTCTimeMillis();
        if (__lastTime + xxx_HEARTBEATINTERVAL < now) {
            __datagram->SendTo(__remoteAddr, __pkt, sizeof(__pkt));
            __lastTime = now;
        }
    }
    else if (mode == 0) {
        // initialise
        if (__datagram != nullptr) return;

        wxIPV4address localaddr;
        if (localIP == "") {
            localaddr.AnyAddress();
        }
        else {
            localaddr.Hostname(localIP);
        }
        __remoteAddr.Hostname("224.0.0.0");
        __remoteAddr.Service(xxx_PORT);

        __datagram = new wxDatagramSocket(localaddr, wxSOCKET_NOWAIT);

        if (__datagram != nullptr) {
            if (!__datagram->IsOk() || __datagram->Error() != wxSOCKET_NOERROR) {
                logger_base.error("xxxEthernetOutput: %s Error creating xxxEthernet heartbeat datagram => %d : %s.",
                    (const char*)localaddr.IPAddress().c_str(),
                    __datagram->LastError(),
                    (const char*)DecodeIPError(__datagram->LastError()).c_str());
                delete __datagram;
                __datagram = nullptr;
            }
        }
        else {
            logger_base.error("xxxEthernetOutput: %s Error creating xxxEthernet heartbeat datagram.",
                (const char*)localaddr.IPAddress().c_str());
        }
    }
    else if (mode == 9) {
        // close
        if (__datagram == nullptr) return;
        __datagram->Close();
        delete __datagram;
        __datagram = nullptr;
    }
}

void xxxEthernetOutput::OpenDatagram() {

    static log4cpp::Category& logger_base = log4cpp::Category::getInstance(std::string("log_base"));

    if (_datagram != nullptr) return;

    wxIPV4address localaddr;
    if (GetForceLocalIPToUse() == "") {
        localaddr.AnyAddress();
    }
    else {
        localaddr.Hostname(GetForceLocalIPToUse());
    }

    _datagram = new wxDatagramSocket(localaddr, wxSOCKET_NOWAIT);
    if (_datagram == nullptr) {
        logger_base.error("xxxEthernetOutput: %s Error opening datagram.", (const char*)localaddr.IPAddress().c_str());
    }
    else if (!_datagram->IsOk()) {
        logger_base.error("xxxEthernetOutput: %s Error opening datagram. Network may not be connected? OK : FALSE", (const char*)localaddr.IPAddress().c_str());
        delete _datagram;
        _datagram = nullptr;
    }
    else if (_datagram->Error() != wxSOCKET_NOERROR) {
        logger_base.error("xxxEthernetOutput: %s Error creating xxxEthernet datagram => %d : %s.", (const char*)localaddr.IPAddress().c_str(), _datagram->LastError(), (const char*)DecodeIPError(_datagram->LastError()).c_str());
        delete _datagram;
        _datagram = nullptr;
    }
}
#pragma endregion

#pragma region Constructors and Destructors
xxxEthernetOutput::xxxEthernetOutput(wxXmlNode* node, bool isActive) : IPOutput(node, isActive) {

    SetId(wxAtoi(node->GetAttribute("Id", "0")));
    if (wxAtoi(node->GetAttribute("Port", "-1")) != -1)
    {
        _universe = wxAtoi(node->GetAttribute("Port"));
    }
    _data = (uint8_t*)malloc(_channels + xxxETHERNET_PACKET_HEADERLEN + xxxETHERNET_PACKET_FOOTERLEN);
    memset(_data, 0, _channels + xxxETHERNET_PACKET_HEADERLEN + xxxETHERNET_PACKET_FOOTERLEN);
}

xxxEthernetOutput::xxxEthernetOutput() : IPOutput() {

    _universe = 1;
    _data = (uint8_t*)malloc(_channels + xxxETHERNET_PACKET_HEADERLEN + xxxETHERNET_PACKET_FOOTERLEN);
    memset(_data, 0, _channels + xxxETHERNET_PACKET_HEADERLEN + xxxETHERNET_PACKET_FOOTERLEN);
}

xxxEthernetOutput::xxxEthernetOutput(xxxEthernetOutput* output) : IPOutput(output) {

    _data = (uint8_t*)malloc(_channels);
    memset(_data, 0, _channels);
}

xxxEthernetOutput::~xxxEthernetOutput() {
    if (_data != nullptr) {
        free(_data);
        _data = nullptr;
    }
}

wxXmlNode* xxxEthernetOutput::Save() {

    wxXmlNode* node = new wxXmlNode(wxXML_ELEMENT_NODE, "network");

    node->AddAttribute("Id", wxString::Format(wxT("%i"), GetId()));

    IPOutput::Save(node);

    return node;
}
#pragma endregion

#pragma region Getters and Setters
std::string xxxEthernetOutput::GetLongDescription() const {

    std::string res = "";

    if (!_enabled) res += "INACTIVE ";
    res += "xxxEthernet {" + wxString::Format(wxT("%i"), _universe).ToStdString() + "} ";
    res += "[1-" + std::string(wxString::Format(wxT("%i"), _channels)) + "] ";
    res += "(" + std::string(wxString::Format(wxT("%i"), GetStartChannel())) + "-" + std::string(wxString::Format(wxT("%i"), GetEndChannel())) + ") ";

    return res;
}

std::string xxxEthernetOutput::GetExport() const {

    return wxString::Format(",%ld,%ld,,%s,%s,,,,%d,%i",
        GetStartChannel(),
        GetEndChannel(),
        GetType(),
        GetIP(),
        GetUniverse(),
        GetChannels());
}
#pragma endregion

#pragma region Start and Stop
bool xxxEthernetOutput::Open() {

    if (!_enabled) return true;
    if (!ip_utils::IsIPValid(_resolvedIp)) return false;

    _ok = IPOutput::Open();

    memset(_packet, 0, sizeof(_packet));

    OpenDatagram();
    _remoteAddr.Hostname(_ip.c_str());
    _remoteAddr.Service(xxx_PORT);

    Heartbeat(0, GetForceLocalIPToUse());

    return _ok && _datagram != nullptr;
}

void xxxEthernetOutput::Close() {

    Heartbeat(9, GetForceLocalIPToUse());
    if (_datagram != nullptr) {
        delete _datagram;
        _datagram = nullptr;
    }
    IPOutput::Close();
}
#pragma endregion

#pragma region Frame Handling
void xxxEthernetOutput::StartFrame(long msec) {

    static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));

    if (!_enabled) return;

    if (_datagram == nullptr && OutputManager::IsRetryOpen()) {
        OpenDatagram();
        if (_ok) {
            logger_base.debug("xxxEthernetOutput: Open retry successful");
        }
    }

    _timer_msec = msec;
}

void xxxEthernetOutput::EndFrame(int suppressFrames) {

    if (!_enabled || _suspend) return;

        if (_datagram == nullptr) return;

        if (_changed || NeedToOutput(suppressFrames)) {
            int current = 0;
            for (int i = 0; i < (_channels - 1) / xxxCHANNELSPERPACKET + 1; i++) {
                _packet[0] = 0x80;
                _packet[1] = _universe;
                _packet[2] = 0x1d; // 0x1c
                _packet[3] = (uint8_t)(((current / 3) >> 8) & 0xFF); // high start pixel
                _packet[4] = (uint8_t)((current / 3) & 0xFF); // low start pixel
                int ch = (std::min)(_channels - current, xxxCHANNELSPERPACKET);
                _packet[5] = (uint8_t)((ch >> 8) & 0xFF); // high pixels per packet
                _packet[6] = (uint8_t)(ch & 0xFF); // low pixels per packet
                memcpy(&_packet[xxxETHERNET_PACKET_HEADERLEN], &_data[current], ch);
                _packet[xxxETHERNET_PACKET_HEADERLEN + ch] = 0x81;
                _datagram->SendTo(_remoteAddr, _packet, xxxETHERNET_PACKET_HEADERLEN + ch + xxxETHERNET_PACKET_FOOTERLEN);
                current += xxxCHANNELSPERPACKET;
            }
            FrameOutput();
            Heartbeat(1, GetForceLocalIPToUse());
        }
        else {
            SkipFrame();
        }
}
#pragma endregion 

#pragma region Data Setting
void xxxEthernetOutput::SetOneChannel(int32_t channel, unsigned char data) {

    if (!_enabled) return;

    if (_data[channel] != data) {
        _data[channel] = data;
        _changed = true;
    }
}

void xxxEthernetOutput::SetManyChannels(int32_t channel, unsigned char data[], size_t size) {

    size_t chs = (std::min)(size, (size_t)(GetMaxChannels() - channel));
    if (memcmp(&_data[channel], data, chs) != 0) {
        memcpy(&_data[channel], data, chs);
        _changed = true;
    }
}

void xxxEthernetOutput::AllOff() {

    memset(_data, 0x00, _channels);
    _changed = true;
}
#pragma endregion 
