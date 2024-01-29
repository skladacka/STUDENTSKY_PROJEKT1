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

#include <wx/wx.h>
#include <wx/bmpbndl.h>

class RenderableEffect;

class DragEffectBitmapButton : public wxBitmapButton
{
public:
    //        DragEffectBitmapButton();

    DragEffectBitmapButton(wxWindow* parent, wxWindowID id, const wxBitmap& bitmap, const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = wxBU_AUTODRAW, const wxValidator& validator = wxDefaultValidator,
        const wxString& name = wxButtonNameStr);
    virtual void DoSetSizeHints(int minW, int minH,
        int maxW, int maxH,
        int incW, int incH);
    virtual ~DragEffectBitmapButton();
    void OnMouseLeftDown(wxMouseEvent& event);
    void SetBitmap(const wxBitmapBundle& bmp);

    void SetEffect(RenderableEffect* effect, int sz = 16);

protected:
private:
    wxString(*GetStringFunction)();

    RenderableEffect* mEffect;
};

