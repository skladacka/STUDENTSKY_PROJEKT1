#ifndef DMXSERVO3D_H
#define DMXSERVO3D_H

#include "DmxServo.h"
#include "Mesh.h"

class DmxServo3d : public DmxServo
{
    public:
        DmxServo3d(wxXmlNode *node, const ModelManager &manager, bool zeroBased = false);
        virtual ~DmxServo3d();

        virtual void DrawModelOnWindow(ModelPreview* preview, DrawGLUtils::xl3Accumulator& va, const xlColor* c, float& sx, float& sy, float& sz, bool active) override;

        virtual void AddTypeProperties(wxPropertyGridInterface *grid) override;
        virtual int OnPropertyGridChange(wxPropertyGridInterface *grid, wxPropertyGridEvent& event) override;

    protected:
        virtual void InitModel() override;

        virtual void ExportXlightsModel() override;
        virtual void ImportXlightsModel(std::string filename, xLightsFrame* xlights, float& min_x, float& max_x, float& min_y, float& max_y) override;

    private:
        Mesh* static_mesh;
        Mesh* motion_mesh;
};

#endif // DMXSERVO3D_H
