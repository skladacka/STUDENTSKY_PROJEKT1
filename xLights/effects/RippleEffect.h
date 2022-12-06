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

#include "../Color.h"

#define RIPPLE_CYCLES_MIN 0
#define RIPPLE_CYCLES_MAX 300

#define RIPPLE_THICKNESS_MIN 1
#define RIPPLE_THICKNESS_MAX 100

#define RIPPLE_ROTATION_MIN 0
#define RIPPLE_ROTATION_MAX 360

#define RIPPLE_XC_MIN -100
#define RIPPLE_XC_MAX 100

#define RIPPLE_YC_MIN -100
#define RIPPLE_YC_MAX 100

class RippleEffect : public RenderableEffect
{
public:
    RippleEffect(int id);
    virtual ~RippleEffect();
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
        if (name == "E_VALUECURVE_Ripple_Cycles")
            return RIPPLE_CYCLES_MIN;
        if (name == "E_VALUECURVE_Ripple_Thickness")
            return RIPPLE_THICKNESS_MIN;
        if (name == "E_VALUECURVE_Ripple_Rotation")
            return RIPPLE_ROTATION_MIN;
        if (name == "E_VALUECURVE_Ripple_XC")
            return RIPPLE_XC_MIN;
        if (name == "E_VALUECURVE_Ripple_YC")
            return RIPPLE_YC_MIN;
        return RenderableEffect::GetSettingVCMin(name);
    }

    virtual double GetSettingVCMax(const std::string& name) const override
    {
        if (name == "E_VALUECURVE_Ripple_Cycles")
            return RIPPLE_CYCLES_MAX;
        if (name == "E_VALUECURVE_Ripple_Thickness")
            return RIPPLE_THICKNESS_MAX;
        if (name == "E_VALUECURVE_Ripple_Rotation")
            return RIPPLE_ROTATION_MAX;
        if (name == "E_VALUECURVE_Ripple_XC")
            return RIPPLE_XC_MAX;
        if (name == "E_VALUECURVE_Ripple_YC")
            return RIPPLE_YC_MAX;
        return RenderableEffect::GetSettingVCMax(name);
    }

protected:
    virtual xlEffectPanel* CreatePanel(wxWindow* parent) override;

private:
    void Drawcircle(RenderBuffer& buffer, int Movement, int xc, int yc, double radius, HSVValue& hsv, int Ripple_Thickness, int CheckBox_Ripple3D);
    void Drawsquare(RenderBuffer& buffer, int Movement, int x1, int x2, int y1, int y2, int Ripple_Thickness, int CheckBox_Ripple3D, HSVValue& hsv);
    void Drawtriangle(RenderBuffer& buffer, int Movement, int xc, int yc, double side, HSVValue& hsv, int Ripple_Thickness, int CheckBox_Ripple3D);
    void Drawstar(RenderBuffer& buffer, int Movement, int xc, int yc, double radius, int points, HSVValue& hsv, int Ripple_Thickness, int CheckBox_Ripple3D, float rotation);

    void Drawheart(RenderBuffer& buffer, int Movement, int xc, int yc, double radius, HSVValue& hsv, int Ripple_Thickness, int CheckBox_Ripple3D);
    void Drawpolygon(RenderBuffer& buffer, int Movement, int xc, int yc, double radius, int points, HSVValue& hsv, int Ripple_Thickness, int CheckBox_Ripple3D, float rotation);
    void Drawsnowflake(RenderBuffer& buffer, int Movement, int xc, int yc, double radius, int points, HSVValue& hsv, int Ripple_Thickness, int CheckBox_Ripple3D, double rotation);
    void Drawtree(RenderBuffer& buffer, int Movement, int xc, int yc, double radius, HSVValue& hsv, int Ripple_Thickness, int CheckBox_Ripple3D);
    void Drawcandycane(RenderBuffer& buffer, int Movement, int xc, int yc, double radius, HSVValue& hsv, int Ripple_Thickness, int CheckBox_Ripple3D);
    void Drawcrucifix(RenderBuffer& buffer, int Movement, int xc, int yc, double radius, HSVValue& hsv, int Ripple_Thickness, int CheckBox_Ripple3D);
    void Drawpresent(RenderBuffer& buffer, int Movement, int xc, int yc, double radius, HSVValue& hsv, int Ripple_Thickness, int CheckBox_Ripple3D);
};
