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
#include "../UtilFunctions.h"

#define MORPH_X_MIN 0
#define MORPH_X_MAX 100

#define MORPH_Y_MIN 0
#define MORPH_Y_MAX 100

#define MORPH_STARTLENGTH_MIN 0
#define MORPH_STARTLENGTH_MAX 100

#define MORPH_ENDLENGTH_MIN 0
#define MORPH_ENDLENGTH_MAX 100

#define MORPH_DURATION_MIN 0
#define MORPH_DURATION_MAX 100

#define MORPH_ACCEL_MIN -10
#define MORPH_ACCEL_MAX 10

#define MORPH_REPEAT_MIN 0
#define MORPH_REPEAT_MAX 250

#define MORPH_REPEATSKIP_MIN 1
#define MORPH_REPEATSKIP_MAX 100

#define MORPH_STAGGER_MIN -100
#define MORPH_STAGGER_MAX 100

class MorphEffect : public RenderableEffect
{
public:
    MorphEffect(int id);
    virtual ~MorphEffect();
    virtual void Render(Effect* effect, const SettingsMap& settings, RenderBuffer& buffer) override;
    virtual int DrawEffectBackground(const Effect* e, int x1, int y1, int x2, int y2, xlVertexColorAccumulator& backgrounds, xlColor* colorMask, bool ramps) override;
    virtual AssistPanel* GetAssistPanel(wxWindow* parent, xLightsFrame* xl_frame) override;
    virtual bool HasAssistPanel() override
    {
        return true;
    }
    virtual std::list<std::string> CheckEffectSettings(const SettingsMap& settings, AudioManager* media, Model* model, Effect* eff, bool renderCache) override;
    virtual void SetDefaultParameters() override;
    virtual bool CanRenderPartialTimeInterval() const override
    {
        return true;
    }
    virtual bool PressButton(const std::string& id, SettingsMap& paletteMap, SettingsMap& settings) override;

    virtual double GetSettingVCMin(const std::string& name) const override
    {
        if (StartsWith(name, "E_VALUECURVE_Morph_Start_X") || StartsWith(name, "E_VALUECURVE_Morph_End_X"))
            return MORPH_X_MIN;
        if (StartsWith(name, "E_VALUECURVE_Morph_Start_Y") || StartsWith(name, "E_VALUECURVE_Morph_End_Y"))
            return MORPH_Y_MIN;
        if (name == "E_VALUECURVE_MorphStartLength")
            return MORPH_STARTLENGTH_MIN;
        if (name == "E_VALUECURVE_MorphEndLength")
            return MORPH_ENDLENGTH_MIN;
        if (name == "E_VALUECURVE_MorphDuration")
            return MORPH_DURATION_MIN;
        if (name == "E_VALUECURVE_MorphAccel")
            return MORPH_ACCEL_MIN;
        if (name == "E_VALUECURVE_Morph_Repeat_Count")
            return MORPH_REPEAT_MIN;
        if (name == "E_VALUECURVE_Morph_Repeat_Skip")
            return MORPH_REPEATSKIP_MIN;
        if (name == "E_VALUECURVE_Morph_Stagger")
            return MORPH_STAGGER_MIN;
        return RenderableEffect::GetSettingVCMin(name);
    }

    virtual double GetSettingVCMax(const std::string& name) const override
    {
        if (StartsWith(name, "E_VALUECURVE_Morph_Start_X") || StartsWith(name, "E_VALUECURVE_Morph_End_X"))
            return MORPH_X_MAX;
        if (StartsWith(name, "E_VALUECURVE_Morph_Start_Y") || StartsWith(name, "E_VALUECURVE_Morph_End_Y"))
            return MORPH_Y_MAX;
        if (name == "E_VALUECURVE_MorphStartLength")
            return MORPH_STARTLENGTH_MAX;
        if (name == "E_VALUECURVE_MorphEndLength")
            return MORPH_ENDLENGTH_MAX;
        if (name == "E_VALUECURVE_MorphDuration")
            return MORPH_DURATION_MAX;
        if (name == "E_VALUECURVE_MorphAccel")
            return MORPH_ACCEL_MAX;
        if (name == "E_VALUECURVE_Morph_Repeat_Count")
            return MORPH_REPEAT_MAX;
        if (name == "E_VALUECURVE_Morph_Repeat_Skip")
            return MORPH_REPEATSKIP_MAX;
        if (name == "E_VALUECURVE_Morph_Stagger")
            return MORPH_STAGGER_MAX;
        return RenderableEffect::GetSettingVCMax(name);
    }

protected:
    virtual xlEffectPanel* CreatePanel(wxWindow* parent) override;
};
