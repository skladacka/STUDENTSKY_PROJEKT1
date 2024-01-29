/***************************************************************
 * This source files comes from the xLights project
 * https://www.xlights.org
 * https://github.com/xLightsSequencer/xLights
 * See the github commit history for a record of contributing
 * developers.
 * Copyright claimed based on commit dates recorded in Github
 * License: https://github.com/xLightsSequencer/xLights/blob/master/License.txt
 **************************************************************/

#include "OutputProcessSet.h"
#include <wx/xml/xml.h>

OutputProcessSet::OutputProcessSet(OutputManager* outputManager, wxXmlNode* node) : OutputProcess(outputManager, node)
{
    _channels = wxAtol(node->GetAttribute("Channels", "1"));
    _value = wxAtol(node->GetAttribute("Value", "0"));
}

OutputProcessSet::OutputProcessSet(const OutputProcessSet& op) : OutputProcess(op)
{
    _channels = op._channels;
    _value = op._value;
}

OutputProcessSet::OutputProcessSet(OutputManager* outputManager, std::string startChannel, size_t p1, size_t p2, const std::string& description) : OutputProcess(outputManager, startChannel, description)
{
    _channels = p1;
    _value = p2;
}

OutputProcessSet::OutputProcessSet(OutputManager* outputManager) : OutputProcess(outputManager)
{
    _channels = 1;
    _value = 0;
}

wxXmlNode* OutputProcessSet::Save()
{
    wxXmlNode* res = new wxXmlNode(nullptr, wxXML_ELEMENT_NODE, "OPSet");

    res->AddAttribute("Channels", wxString::Format(wxT("%ld"), (long)_channels));
    res->AddAttribute("Value", wxString::Format(wxT("%d"), _value));

    OutputProcess::Save(res);

    return res;
}

void OutputProcessSet::Frame(uint8_t* buffer, size_t size, std::list<OutputProcess*>& processes)
{
    size_t sc = GetStartChannelAsNumber();
    size_t chs = std::min(_channels, size - (sc - 1));

    memset(buffer + sc - 1, (uint8_t)_value, chs);
}
