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

#include "RenderableEffect.h"

#define COLOURWASH_CYCLES_MIN 0.1f
#define COLOURWASH_CYCLES_MAX 20.0f

class ColorWashEffect : public RenderableEffect
{
public:
    ColorWashEffect(int id);
    virtual ~ColorWashEffect();

    virtual void SetDefaultParameters() override;
    virtual void Render(Effect* effect, const SettingsMap& settings, RenderBuffer& buffer) override;
    virtual int DrawEffectBackground(const Effect* e, int x1, int y1, int x2, int y2, xlVertexColorAccumulator& bg, xlColor* colorMask, bool ramps) override;
    virtual wxString GetEffectString() override;
    virtual bool needToAdjustSettings(const std::string& version) override;
    virtual void adjustSettings(const std::string& version, Effect* effect, bool removeDefaults = true) override;
    virtual bool CanRenderPartialTimeInterval() const override
    {
        return true;
    }

    virtual double GetSettingVCMin(const std::string& name) const override
    {
        if (name == "E_VALUECURVE_ColorWash_Cycles")
            return COLOURWASH_CYCLES_MIN;
        return RenderableEffect::GetSettingVCMin(name);
    }
    virtual double GetSettingVCMax(const std::string& name) const override
    {
        if (name == "E_VALUECURVE_ColorWash_Cycles")
            return COLOURWASH_CYCLES_MAX;
        return RenderableEffect::GetSettingVCMax(name);
    }

protected:
    virtual void RemoveDefaults(const std::string& version, Effect* effect) override;
    virtual xlEffectPanel* CreatePanel(wxWindow* parent) override;
};
