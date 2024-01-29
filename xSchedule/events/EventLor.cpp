/***************************************************************
 * This source files comes from the xLights project
 * https://www.xlights.org
 * https://github.com/xLightsSequencer/xLights
 * See the github commit history for a record of contributing
 * developers.
 * Copyright claimed based on commit dates recorded in Github
 * License: https://github.com/xLightsSequencer/xLights/blob/master/License.txt
 **************************************************************/

#include "EventLor.h"
#include <wx/xml/xml.h>

EventLor::EventLor() : EventSerial()
{
    _unit_id = 1;
    _unit_id_str = "0x1";
}

EventLor::EventLor(wxXmlNode* node) : EventSerial(node)
{
    _unit_id_str = node->GetAttribute("UnitId", "0x01").ToStdString();
    _unit_id = std::strtol(_unit_id_str.c_str(), nullptr, 16);
}

void EventLor::SetUnitId(std::string unit_id)
{
    if (_unit_id_str != unit_id)
    {
        _unit_id_str = unit_id;
        _unit_id = std::strtol(unit_id.c_str(), nullptr, 16);
        _changeCount++;
    }
}

wxXmlNode* EventLor::Save()
{
    wxXmlNode* en = new wxXmlNode(nullptr, wxXML_ELEMENT_NODE, "EventLor");
    en->AddAttribute("UnitId", _unit_id_str);
    en->AddAttribute("CommPort", _commPort);
    en->AddAttribute("Speed", wxString::Format("%d", _speed));
    en->AddAttribute("Channel", wxString::Format("%d", _channel));
    EventBase::Save(en);
    return en;
}

std::string EventLor::GetParmToolTip()
{
    return EventBase::DefaultParmTooltip();
}
