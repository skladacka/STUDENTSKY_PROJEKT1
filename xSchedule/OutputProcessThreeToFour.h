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

class OutputProcessThreeToFour : public OutputProcess
{
    size_t _nodes;
	std::string _colourOrder;

    public:

        OutputProcessThreeToFour(OutputManager* outputManager, wxXmlNode* node);
        OutputProcessThreeToFour(OutputManager* outputManager);
        OutputProcessThreeToFour(const OutputProcessThreeToFour& op);
        OutputProcessThreeToFour(OutputManager* outputManager, std::string _startChannel, size_t p1, std::string colourOrder, const std::string& description);
        virtual ~OutputProcessThreeToFour() {}
        virtual wxXmlNode* Save() override;
        virtual void Frame(uint8_t* buffer, size_t size, std::list<OutputProcess*>& processes) override;
        virtual size_t GetP1() const override { return _nodes; }
        std::string GetColourOrder() const { return _colourOrder; }
        virtual size_t GetP2() const override { return 0; }
        virtual std::string GetType() const override { return "Three To Four"; }
};

