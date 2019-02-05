#ifndef BUTTERFLYEFFECT_H
#define BUTTERFLYEFFECT_H

#include "RenderableEffect.h"

#define BUTTERFLY_SPEED_MIN 0
#define BUTTERFLY_SPEED_MAX 100

#define BUTTERFLY_SKIP_MIN 2
#define BUTTERFLY_SKIP_MAX 10

#define BUTTERFLY_CHUNKS_MIN 1
#define BUTTERFLY_CHUNKS_MAX 10

class ButterflyEffect : public RenderableEffect
{
    public:
        ButterflyEffect(int id);
        virtual ~ButterflyEffect();
        virtual void SetDefaultParameters() override;
        virtual void Render(Effect *effect, SettingsMap &settings, RenderBuffer &buffer) override;
        virtual bool AppropriateOnNodes() const override { return false; }
        virtual bool CanRenderPartialTimeInterval() const override { return true; }
        virtual bool SupportsRenderCache(const SettingsMap& settings) const override { return true; }

    protected:
        virtual wxPanel *CreatePanel(wxWindow *parent) override;
    private:
};

#endif // BUTTERFLYEFFECT_H
