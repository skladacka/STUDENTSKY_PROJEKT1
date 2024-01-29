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

#include <map>
#include <string>

class wxPropertyGridInterface;
class wxPropertyGridEvent;
class BaseObject;
class wxXmlNode;

class DmxPanTiltAbility
{
    public:
        DmxPanTiltAbility();
        virtual ~DmxPanTiltAbility();

        void AddPanTiltTypeProperties(wxPropertyGridInterface *grid);
        int OnPanTiltPropertyGridChange(wxPropertyGridInterface *grid, wxPropertyGridEvent& event, wxXmlNode* ModelXml, BaseObject* base);

        int GetPanChannel() { return pan_channel; }
        int GetTiltChannel() { return tilt_channel; }

    protected:
        int pan_channel;
        int pan_orient;
        int pan_deg_of_rot;
        float pan_slew_limit;
        int tilt_channel;
        int tilt_orient;
        int tilt_deg_of_rot;
        float tilt_slew_limit;
    
    
        class PanTiltState {
        public:
            uint32_t ms = 0;
            float pan_angle = 0.0f;
            float tilt_angle = 0.0f;
        };
        std::map<std::string, PanTiltState> panTiltStates;

    private:
};

