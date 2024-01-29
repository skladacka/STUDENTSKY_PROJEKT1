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

#define CANDLE_AGILITY_MIN 1
#define CANDLE_AGILITY_MAX 10

#define CANDLE_WINDBASELINE_MIN 0
#define CANDLE_WINDBASELINE_MAX 255

#define CANDLE_WINDVARIABILITY_MIN 0
#define CANDLE_WINDVARIABILITY_MAX 10

#define CANDLE_WINDCALMNESS_MIN 0
#define CANDLE_WINDCALMNESS_MAX 10

class CandleEffect : public RenderableEffect
{
public:
    CandleEffect(int id);
    virtual ~CandleEffect();
    virtual void SetDefaultParameters() override;
    virtual void Render(Effect* effect, const SettingsMap& settings, RenderBuffer& buffer) override;
    virtual std::list<std::string> CheckEffectSettings(const SettingsMap& settings, AudioManager* media, Model* model, Effect* eff, bool renderCache) override;

    virtual double GetSettingVCMin(const std::string& name) const override
    {
        if (name == "E_VALUECURVE_Candle_FlameAgility")
            return CANDLE_AGILITY_MIN;
        if (name == "E_VALUECURVE_Candle_WindBaseline")
            return CANDLE_WINDBASELINE_MIN;
        if (name == "E_VALUECURVE_Candle_WindVariability")
            return CANDLE_WINDVARIABILITY_MIN;
        if (name == "E_VALUECURVE_Candle_WindCalmness")
            return CANDLE_WINDCALMNESS_MIN;
        return RenderableEffect::GetSettingVCMin(name);
    }
    virtual double GetSettingVCMax(const std::string& name) const override
    {
        if (name == "E_VALUECURVE_Candle_FlameAgility")
            return CANDLE_AGILITY_MAX;
        if (name == "E_VALUECURVE_Candle_WindBaseline")
            return CANDLE_WINDBASELINE_MAX;
        if (name == "E_VALUECURVE_Candle_WindVariability")
            return CANDLE_WINDVARIABILITY_MAX;
        if (name == "E_VALUECURVE_Candle_WindCalmness")
            return CANDLE_WINDCALMNESS_MAX;
        return RenderableEffect::GetSettingVCMax(name);
    }

protected:
    virtual xlEffectPanel* CreatePanel(wxWindow* parent) override;
    void Update(wxByte& flameprime, wxByte& flame, wxByte& wind, size_t windVariability, size_t flameAgility, size_t windCalmness, size_t windBaseline);
};
