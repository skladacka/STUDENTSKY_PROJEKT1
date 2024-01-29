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

class OutputProcessDimWhite : public OutputProcess
{
    size_t _nodes;
    int _dim;
    int _lastDim;
    uint8_t _dimTable[256];

    void BuildDimTable();

    public:

        OutputProcessDimWhite(OutputManager* outputManager, wxXmlNode* node);
        OutputProcessDimWhite(OutputManager* outputManager);
        OutputProcessDimWhite(const OutputProcessDimWhite& op);
        OutputProcessDimWhite(OutputManager* outputManager, std::string _startChannel, size_t p1, size_t p2, const std::string& description);
        virtual ~OutputProcessDimWhite() {}
        virtual wxXmlNode* Save() override;
        virtual void Frame(uint8_t* buffer, size_t size, std::list<OutputProcess*>& processes) override;
        virtual size_t GetP1() const override { return _nodes; }
        virtual size_t GetP2() const override { return _dim; }
        virtual std::string GetType() const override { return "Dim White"; }
};

