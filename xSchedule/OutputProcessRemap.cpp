/***************************************************************
 * This source files comes from the xLights project
 * https://www.xlights.org
 * https://github.com/xLightsSequencer/xLights
 * See the github commit history for a record of contributing
 * developers.
 * Copyright claimed based on commit dates recorded in Github
 * License: https://github.com/xLightsSequencer/xLights/blob/master/License.txt
 **************************************************************/

#include "OutputProcessRemap.h"
#include <wx/xml/xml.h>

OutputProcessRemap::OutputProcessRemap(OutputManager* outputManager, wxXmlNode* node) : OutputProcess(outputManager, node)
{
    _channels = wxAtol(node->GetAttribute("Channels", "1"));
    _to = wxAtol(node->GetAttribute("To", "1"));
}

OutputProcessRemap::OutputProcessRemap(const OutputProcessRemap& op) : OutputProcess(op)
{
    _channels = op._channels;
    _to = op._to;
}

OutputProcessRemap::OutputProcessRemap(OutputManager* outputManager, std::string startChannel, size_t p1, size_t p2, const std::string& description) : OutputProcess(outputManager, startChannel, description)
{
    _to = p1;
    _channels = p2;
}

OutputProcessRemap::OutputProcessRemap(OutputManager* outputManager) : OutputProcess(outputManager)
{
    _channels = 1;
    _to = 1;
}

wxXmlNode* OutputProcessRemap::Save()
{
    wxXmlNode* res = new wxXmlNode(nullptr, wxXML_ELEMENT_NODE, "OPRemap");

    res->AddAttribute("Channels", wxString::Format(wxT("%ld"), (long)_channels));
    res->AddAttribute("To", wxString::Format(wxT("%ld"), (long)_to));

    OutputProcess::Save(res);

    return res;
}

void OutputProcessRemap::Frame(uint8_t* buffer, size_t size, std::list<OutputProcess*>& processes)
{
    size_t sc = GetStartChannelAsNumber();

    if (sc == _to) return;

    size_t chs1 = std::min(_channels, size - (sc - 1));
    size_t chs2 = std::min(_channels, size - (_to - 1));
    size_t chs = std::min(chs1, chs2);

    memcpy(buffer + _to - 1, buffer + sc - 1, chs);
}
