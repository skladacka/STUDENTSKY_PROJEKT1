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

#define MOVING_HEAD_MIN -1800
#define MOVING_HEAD_MAX  1800
#define MOVING_HEAD_DIVISOR 10
#define MOVING_HEAD_GROUP_MIN 1
#define MOVING_HEAD_GROUP_MAX 6
#define MOVING_HEAD_TIME_MIN -1000
#define MOVING_HEAD_TIME_MAX 1000
#define MOVING_HEAD_SCALE_MIN -100
#define MOVING_HEAD_SCALE_MAX 100

class DmxMotor;

class MovingHeadEffect : public RenderableEffect
{
public:
    MovingHeadEffect(int id);
    virtual ~MovingHeadEffect();
    virtual bool CanBeRandom() override
    {
        return false;
    }
    virtual void Render(Effect* effect, const SettingsMap& settings, RenderBuffer& buffer) override;
    virtual void RenameTimingTrack(std::string oldname, std::string newname, Effect* effect) override;
    virtual void SetPanelStatus(Model* cls) override;
    virtual void SetDefaultParameters() override;
    virtual bool CanRenderPartialTimeInterval() const override
    {
        return false;
    }
    virtual std::list<std::string> CheckEffectSettings(const SettingsMap& settings, AudioManager* media, Model* model, Effect* eff, bool renderCache) override;

    virtual double GetSettingVCMin(const std::string& name) const override
    {
        if (name == "E_VALUECURVE_MHPan")
            return MOVING_HEAD_MIN;
        if (name == "E_VALUECURVE_MHTilt")
            return MOVING_HEAD_MIN;
        if (name == "E_VALUECURVE_MHFanPan")
            return MOVING_HEAD_MIN;
        if (name == "E_VALUECURVE_MHFanTilt")
            return MOVING_HEAD_MIN;
        if (name == "E_VALUECURVE_MHPanOffset")
            return MOVING_HEAD_MIN;
        if (name == "E_VALUECURVE_MHTiltOffset")
            return MOVING_HEAD_MIN;
        if (name == "E_VALUECURVE_MHGroupings")
            return MOVING_HEAD_GROUP_MIN;
        if (name == "E_VALUECURVE_MHTimeOffset")
            return MOVING_HEAD_TIME_MIN;
        if (name == "E_VALUECURVE_MHPathScale")
            return MOVING_HEAD_SCALE_MIN;
        return RenderableEffect::GetSettingVCMin(name);
    }
    virtual double GetSettingVCMax(const std::string& name) const override
    {
        if (name == "E_VALUECURVE_MHPan")
            return MOVING_HEAD_MAX;
        if (name == "E_VALUECURVE_MHTilt")
            return MOVING_HEAD_MAX;
        if (name == "E_VALUECURVE_MHFanPan")
            return MOVING_HEAD_MAX;
        if (name == "E_VALUECURVE_MHFanTilt")
            return MOVING_HEAD_MAX;
        if (name == "E_VALUECURVE_MHPanOffset")
            return MOVING_HEAD_MAX;
        if (name == "E_VALUECURVE_MHTiltOffset")
            return MOVING_HEAD_MAX;
        if (name == "E_VALUECURVE_MHGroupings")
            return MOVING_HEAD_GROUP_MAX;
        if (name == "E_VALUECURVE_MHTimeOffset")
            return MOVING_HEAD_TIME_MAX;
        if (name == "E_VALUECURVE_MHPathScale")
            return MOVING_HEAD_SCALE_MAX;
        return RenderableEffect::GetSettingVCMax(name);
    }
    virtual int GetSettingVCDivisor(const std::string& name) const override
    {
        if (name == "E_VALUECURVE_MHPan")
            return MOVING_HEAD_DIVISOR;
        if (name == "E_VALUECURVE_MHTilt")
            return MOVING_HEAD_DIVISOR;
        if (name == "E_VALUECURVE_MHFanPan")
            return MOVING_HEAD_DIVISOR;
        if (name == "E_VALUECURVE_MHFanTilt")
            return MOVING_HEAD_DIVISOR;
        if (name == "E_VALUECURVE_MHPanOffset")
            return MOVING_HEAD_DIVISOR;
        if (name == "E_VALUECURVE_MHTiltOffset")
            return MOVING_HEAD_DIVISOR;
        if (name == "E_VALUECURVE_MHTimeOffset")
            return MOVING_HEAD_DIVISOR;
        if (name == "E_VALUECURVE_MHPathScale")
            return MOVING_HEAD_DIVISOR;
        return RenderableEffect::GetSettingVCDivisor(name);
    }

protected:
    virtual xlEffectPanel* CreatePanel(wxWindow* parent) override;
    virtual bool needToAdjustSettings(const std::string& version) override
    {
        return false;
    }
    void WriteCmdToPixel(DmxMotor* motor, int value, RenderBuffer &buffer);
    std::list<Model*> GetModels(Model* model);
    void UpdateFixturePositions(Model *cls);
    void GetValueCurvePosition(float& position, const std::string& settings, double eff_pos, RenderBuffer &buffer);
    void CalculatePosition(int location, float& position, wxArrayString& heads, int groupings, float offset, float& delta );
    void CalculatePathPositions(bool pan_path_active, bool tilt_path_active, float& pan_pos, float& tilt_pos, float time_offset, float path_scale, float delta, double eff_pos, const std::string& path_def);
};
