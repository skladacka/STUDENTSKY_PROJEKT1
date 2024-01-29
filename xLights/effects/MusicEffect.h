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
#include "../RenderBuffer.h"
#include <string>
#include <list>
#include <vector>

class MusicEvent;

#define MUSIC_OFFSET_MIN 0
#define MUSIC_OFFSET_MAX 100

class MusicEffect : public RenderableEffect
{
public:
    MusicEffect(int id);
    virtual ~MusicEffect();
    virtual void Render(Effect* effect, const SettingsMap& settings, RenderBuffer& buffer) override;
    void Render(RenderBuffer& buffer,
                int bars, const std::string& type, int sensitivity, bool scale, const std::string& scalenotes, int offsetx, int startnote, int endnote, const std::string& colourtreatment, bool fade, bool logarithmicX);
    virtual void SetDefaultParameters() override;
    virtual std::list<std::string> CheckEffectSettings(const SettingsMap& settings, AudioManager* media, Model* model, Effect* eff, bool renderCache) override;

    // Currently not possible but I think changes could be made to make it support partial
    // virtual bool CanRenderPartialTimeInterval() const override { return true; }

    virtual double GetSettingVCMin(const std::string& name) const override
    {
        if (name == "E_VALUECURVE_Music_Offset")
            return MUSIC_OFFSET_MIN;
        return RenderableEffect::GetSettingVCMin(name);
    }
    virtual double GetSettingVCMax(const std::string& name) const override
    {
        if (name == "E_VALUECURVE_Music_Offset")
            return MUSIC_OFFSET_MAX;
        return RenderableEffect::GetSettingVCMax(name);
    }

protected:
    void CreateEvents(RenderBuffer& buffer, std::vector<std::list<MusicEvent*>*>& events, int startNote, int endNote, int bars, int scalenotes, int sensitivity, bool logarithmicX);
    virtual xlEffectPanel* CreatePanel(wxWindow* parent) override;
    int DecodeType(const std::string& type);
    int DecodeColourTreatment(const std::string& colourtreatment);
    void RenderMorph(RenderBuffer& buffer, int x, int bars, int startNote, int endNote, std::list<MusicEvent*>& events, int colourTreatment, bool bounce, bool fade);
    void RenderCollide(RenderBuffer& buffer, int x, int bars, int startNote, int endNote, bool in, std::list<MusicEvent*>& events, int colourTreatment, bool fade);
    void RenderOn(RenderBuffer& buffer, int x, int bars, int startNote, int endNote, std::list<MusicEvent*>& events, int colourTreatment, bool fade);
    int DecodeScaleNotes(const std::string& scalenotes);
    virtual bool needToAdjustSettings(const std::string& version) override;
    virtual void adjustSettings(const std::string& version, Effect* effect, bool removeDefaults = true) override;
};
