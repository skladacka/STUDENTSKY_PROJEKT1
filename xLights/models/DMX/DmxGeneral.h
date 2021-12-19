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

#include "DmxModel.h"
#include "DmxColorAbility.h"

class DmxGeneral : public DmxModel, public DmxColorAbility
{
    public:
        DmxGeneral(wxXmlNode *node, const ModelManager &manager, bool zeroBased = false);
        virtual ~DmxGeneral();

        virtual bool HasColorAbility() override { return true; }

        virtual void AddTypeProperties(wxPropertyGridInterface *grid) override;
        virtual int OnPropertyGridChange(wxPropertyGridInterface *grid, wxPropertyGridEvent& event) override;

        virtual void DisplayModelOnWindow(ModelPreview* preview, xlGraphicsContext *ctx,
                                          xlGraphicsProgram *solidProgram, xlGraphicsProgram *transparentProgram, bool is_3d = false,
                                          const xlColor* color = nullptr, bool allowSelected = false, bool wiring = false,
                                          bool highlightFirst = false, int highlightpixel = 0,
                                          float *boundingBox = nullptr) override;
        virtual void DisplayEffectOnWindow(ModelPreview* preview, double pointSize) override;

    
    protected:
        virtual void InitModel() override;
        virtual void DrawModel(xlVertexColorAccumulator *vac, const xlColor *c, bool allowSelected);

        virtual void ExportXlightsModel() override;
        virtual void ImportXlightsModel(std::string const& filename, xLightsFrame* xlights, float& min_x, float& max_x, float& min_y, float& max_y) override;
    private:
};
