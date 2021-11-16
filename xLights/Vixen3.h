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

#include <wx/colour.h>
#include <wx/gdicmn.h>
#include <wx/xml/xml.h>

#include <map>
#include <cmath>

struct VixenTiming
{
    VixenTiming(float s, float e, const std::string& l) : start(s), end(e), label(l) { }
    float start;
    float end;
    std::string label;
};

class VixenEffect
{
public:
    VixenEffect(float s, float e, const std::string& esid) :
        start(s), end(e), effectSettingsId(esid), type("") {}

    std::string GetPalette() const;
    std::string GetSettings() const;
    std::string GetXLightsType() const;

    float start;
    float end;
    std::string effectSettingsId;
    std::string type;
    std::map<std::string, std::string> settings;
    std::vector<wxColor> colours;
    std::vector<wxPoint> levelCurve;
};

class Vixen3 {

    wxXmlDocument _doc;
    std::string _filename;
    std::string _systemFile;
    std::map<std::string, std::list<VixenTiming>> _timingData;
    std::map<std::string, std::string> _timingType;
    std::map<std::string, std::list<VixenEffect>> _effectData;
    bool _systemFound = false;

    void ProcessNode(wxXmlNode* n, std::map<std::string, std::string>& models);

    public:

    Vixen3(const std::string& filename, const std::string& system = "");
    virtual ~Vixen3() {}

    inline static long ConvertTiming(float time, int frame)
    {
        return std::round(time * 1000.0 / (float)frame) * frame;
    }
    const std::string& GetFilename() const { return _filename; }
    bool IsSystemFound() const { return _systemFound; }

    std::list<std::string> GetTimings() const;
    std::list<VixenTiming> GetTimings(const std::string& timing) const;
    std::string GetTimingType(const std::string& timing) const;
    std::list<VixenTiming> GetRelatedTiming(const std::string& timing, const std::string& type) const;

    std::list<std::string> GetModelsWithEffects() const;
    std::list<VixenEffect> GetEffects(const std::string& model) const;
};
