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

#include "../common/xlBaseApp.h"


typedef bool (*p_xSchedule_Action)(const char* command, const wchar_t* parameters, const char* data, char* buffer, size_t bufferSize);

class RemoteFalconApp : public xlBaseApp
{
public:
    virtual bool OnInit() override;
    virtual int OnExit() override;
};

DECLARE_APP(RemoteFalconApp)
