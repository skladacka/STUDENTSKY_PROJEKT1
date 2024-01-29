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

#include "RenderableEffect.h"

#define GARLANDS_CYCLES_MIN 0
#define GARLANDS_CYCLES_MAX 20

#define GARLANDS_SPACING_MIN 1
#define GARLANDS_SPACING_MAX 100

class GarlandsEffect : public RenderableEffect
{
public:
    GarlandsEffect(int id);
    virtual ~GarlandsEffect();
    virtual void SetDefaultParameters() override;
    virtual void Render(Effect* effect, const SettingsMap& settings, RenderBuffer& buffer) override;
    virtual bool AppropriateOnNodes() const override
    {
        return false;
    }
    virtual bool CanRenderPartialTimeInterval() const override
    {
        return true;
    }

    virtual double GetSettingVCMin(const std::string& name) const override
    {
        if (name == "E_VALUECURVE_Garlands_Cycles")
            return GARLANDS_CYCLES_MIN;
        if (name == "E_VALUECURVE_Garlands_Spacing")
            return GARLANDS_SPACING_MIN;
        return RenderableEffect::GetSettingVCMin(name);
    }

    virtual double GetSettingVCMax(const std::string& name) const override
    {
        if (name == "E_VALUECURVE_Garlands_Cycles")
            return GARLANDS_CYCLES_MAX;
        if (name == "E_VALUECURVE_Garlands_Spacing")
            return GARLANDS_SPACING_MAX;
        return RenderableEffect::GetSettingVCMax(name);
    }

protected:
    virtual xlEffectPanel* CreatePanel(wxWindow* parent) override;
};
