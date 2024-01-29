/***************************************************************
 * This source files comes from the xLights project
 * https://www.xlights.org
 * https://github.com/xLightsSequencer/xLights
 * See the github commit history for a record of contributing
 * developers.
 * Copyright claimed based on commit dates recorded in Github
 * License: https://github.com/xLightsSequencer/xLights/blob/master/License.txt
 **************************************************************/

#include "ValueCurveButton.h"

#include <wx/artprov.h>
#include <wx/dcmemory.h>
#include <wx/dnd.h>

wxDEFINE_EVENT(EVT_VC_CHANGED, wxCommandEvent);

class ValueCurveButton;

class VCTextDropTarget : public wxTextDropTarget
{
public:
    VCTextDropTarget(ValueCurveButton* owner, wxString type) { _owner = owner; _type = type; };

    virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& data) override;
    virtual wxDragResult OnDragOver(wxCoord x, wxCoord y, wxDragResult def) override;

    ValueCurveButton* _owner;
    wxString _type;
};

wxDragResult VCTextDropTarget::OnDragOver(wxCoord x, wxCoord y, wxDragResult def)
{
    if (_owner->IsEnabled())
    {
        return wxDragCopy;
    }
    return wxDragNone;
}

bool VCTextDropTarget::OnDropText(wxCoord x, wxCoord y, const wxString& data)
{
    if (data == "") return false;

    if (!_owner->IsEnabled()) return false;

    if (data == "VALUECURVE_CLEAR")
    {
        _owner->SetActive(false);
    }
    else
    {
        if (!data.Contains("ID_VALUECURVE")) return false;

        ValueCurve vc(data);
        ValueCurve* pvc = _owner->GetValue();
        vc.SetId(pvc->GetId());
        vc.SetLimits(pvc->GetMin(), pvc->GetMax());
        vc.SetDivisor(pvc->GetDivisor());
        vc.FixChangedScale(pvc->GetMin(), pvc->GetMax(), 1);

        _owner->GetValue()->Deserialise(vc.Serialise());
        _owner->UpdateBitmap();
        _owner->NotifyChange();
    }

    return true;
}

ValueCurveButton::ValueCurveButton(wxWindow *parent,
    wxWindowID id,
    const wxBitmapBundle& bitmap,
    const wxPoint& pos,
    const wxSize& size,
    long style,
    const wxValidator& validator,
    const wxString& name) : wxBitmapButton(parent, id, bitmap, pos, size, style, validator, name)
{
    _vc = new ValueCurve(name.ToStdString());

    VCTextDropTarget* vcdt = new VCTextDropTarget(this, "ValueCurve");
    SetDropTarget(vcdt);
}

ValueCurveButton::~ValueCurveButton()
{
    if (_vc != nullptr)
    {
        delete _vc;
    }
}

void ValueCurveButton::SetActive(bool active)
{
    bool isActive = _vc->IsActive();
    if (isActive != active) {
        _vc->SetActive(active);
        UpdateState();
    } else {
        UpdateBitmap();
    }
}

void ValueCurveButton::ToggleActive()
{
    _vc->ToggleActive();
    UpdateState();
}

void ValueCurveButton::UpdateBitmap() {
    if (GetValue()->IsActive()) {
        RenderNewBitmap();
    } else {
        static wxBitmapBundle nsBitmap = wxArtProvider::GetBitmapBundle("xlART_valuecurve_notselected", wxART_BUTTON);
        SetBitmap(nsBitmap);
    }
}

void ValueCurveButton::UpdateState()
{
    UpdateBitmap();
    NotifyChange();
}

void ValueCurveButton::RenderNewBitmap() {
    SetBitmap(_vc->GetImage(24, 24, GetContentScaleFactor()));
}

void ValueCurveButton::SetValue(const wxString& value)
{
    _vc->Deserialise(value.ToStdString());
    UpdateState();
}

void ValueCurveButton::NotifyChange()
{
    wxCommandEvent eventVCChange(EVT_VC_CHANGED);
    eventVCChange.SetEventObject(this);
    GetParent()->ProcessWindowEvent(eventVCChange);
}

void ValueCurveButton::SetLimits(float min, float max)
{
    _vc->SetLimits(min, max);
    UpdateState();
}

ValueCurve* ValueCurveButton::GetValue()
{
    return _vc;
}
