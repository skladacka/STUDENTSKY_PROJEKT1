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

#include "OutputProcess.h"

class OutputProcessRemap : public OutputProcess
{
    size_t _to;
    size_t _channels;

    public:

        OutputProcessRemap(OutputManager* outputManager, wxXmlNode* node);
        OutputProcessRemap(OutputManager* outputManager);
        OutputProcessRemap(const OutputProcessRemap& op);
        OutputProcessRemap(OutputManager* outputManager, std::string _startChannel, size_t p1, size_t p2, const std::string& description);
        virtual ~OutputProcessRemap() {}
        virtual wxXmlNode* Save() override;
        virtual void Frame(uint8_t* buffer, size_t size, std::list<OutputProcess*>& processes) override;
        virtual size_t GetP1() const override { return _to; }
        virtual size_t GetP2() const override { return _channels; }
        virtual std::string GetType() const override { return "Remap"; }
};
