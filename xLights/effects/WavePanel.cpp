/***************************************************************
 * This source files comes from the xLights project
 * https://www.xlights.org
 * https://github.com/xLightsSequencer/xLights
 * See the github commit history for a record of contributing
 * developers.
 * Copyright claimed based on commit dates recorded in Github
 * License: https://github.com/xLightsSequencer/xLights/blob/master/License.txt
 **************************************************************/

#include "WavePanel.h"
#include "EffectPanelUtils.h"
#include "WaveEffect.h"

//(*InternalHeaders(WavePanel)
#include <wx/bitmap.h>
#include <wx/bmpbuttn.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/slider.h>
#include <wx/stattext.h>
#include <wx/string.h>
#include <wx/textctrl.h>
//*)

//(*IdInit(WavePanel)
const long WavePanel::ID_STATICTEXT_Wave_Type = wxNewId();
const long WavePanel::ID_CHOICE_Wave_Type = wxNewId();
const long WavePanel::ID_BITMAPBUTTON_CHOICE_Wave_Type = wxNewId();
const long WavePanel::ID_STATICTEXT_Fill_Colors = wxNewId();
const long WavePanel::ID_CHOICE_Fill_Colors = wxNewId();
const long WavePanel::ID_BITMAPBUTTON_CHOICE_Fill_Colors = wxNewId();
const long WavePanel::ID_CHECKBOX_Mirror_Wave = wxNewId();
const long WavePanel::ID_BITMAPBUTTON_CHECKBOX_Mirror_Wave = wxNewId();
const long WavePanel::ID_STATICTEXT_Number_Waves = wxNewId();
const long WavePanel::ID_SLIDER_Number_Waves = wxNewId();
const long WavePanel::ID_VALUECURVE_Number_Waves = wxNewId();
const long WavePanel::IDD_TEXTCTRL_Number_Waves = wxNewId();
const long WavePanel::ID_BITMAPBUTTON_SLIDER_Number_Waves = wxNewId();
const long WavePanel::ID_STATICTEXT_Thickness_Percentage = wxNewId();
const long WavePanel::ID_SLIDER_Thickness_Percentage = wxNewId();
const long WavePanel::ID_VALUECURVE_Thickness_Percentage = wxNewId();
const long WavePanel::IDD_TEXTCTRL_Thickness_Percentage = wxNewId();
const long WavePanel::ID_BITMAPBUTTON_SLIDER_Thickness_Percentage = wxNewId();
const long WavePanel::ID_STATICTEXT_Wave_Height = wxNewId();
const long WavePanel::ID_SLIDER_Wave_Height = wxNewId();
const long WavePanel::ID_VALUECURVE_Wave_Height = wxNewId();
const long WavePanel::IDD_TEXTCTRL_Wave_Height = wxNewId();
const long WavePanel::ID_BITMAPBUTTON_SLIDER_Wave_Height = wxNewId();
const long WavePanel::ID_STATICTEXT_Wave_Speed = wxNewId();
const long WavePanel::IDD_SLIDER_Wave_Speed = wxNewId();
const long WavePanel::ID_VALUECURVE_Wave_Speed = wxNewId();
const long WavePanel::ID_TEXTCTRL_Wave_Speed = wxNewId();
const long WavePanel::ID_BITMAPBUTTON_SLIDER_Wave_Speed = wxNewId();
const long WavePanel::ID_STATICTEXT_Wave_Direction = wxNewId();
const long WavePanel::ID_CHOICE_Wave_Direction = wxNewId();
const long WavePanel::ID_BITMAPBUTTON_CHOICE_Wave_Direction = wxNewId();
const long WavePanel::ID_STATICTEXT1 = wxNewId();
const long WavePanel::ID_SLIDER_Wave_YOffset = wxNewId();
const long WavePanel::ID_VALUECURVE_Wave_YOffset = wxNewId();
const long WavePanel::IDD_TEXTCTRL_Wave_YOffset = wxNewId();
const long WavePanel::ID_BITMAPBUTTON_Wave_YOffset = wxNewId();
//*)

BEGIN_EVENT_TABLE(WavePanel,wxPanel)
	//(*EventTable(WavePanel)
	//*)
END_EVENT_TABLE()

WavePanel::WavePanel(wxWindow* parent) : xlEffectPanel(parent)
{
	//(*Initialize(WavePanel)
	BulkEditTextCtrl* TextCtrl1;
	BulkEditTextCtrl* TextCtrl75;
	BulkEditTextCtrl* TextCtrl76;
	BulkEditTextCtrl* TextCtrl77;
	BulkEditTextCtrlF2* TextCtrl78;
	wxFlexGridSizer* FlexGridSizer1;
	wxFlexGridSizer* FlexGridSizer2;
	wxFlexGridSizer* FlexGridSizer3;
	wxFlexGridSizer* FlexGridSizer4;
	wxFlexGridSizer* FlexGridSizer51;
	wxFlexGridSizer* FlexGridSizer5;

	Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	FlexGridSizer51 = new wxFlexGridSizer(0, 4, 0, 0);
	FlexGridSizer51->AddGrowableCol(1);
	StaticText47 = new wxStaticText(this, ID_STATICTEXT_Wave_Type, _("Wave Type"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_Wave_Type"));
	FlexGridSizer51->Add(StaticText47, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	Choice_Wave_Type = new BulkEditChoice(this, ID_CHOICE_Wave_Type, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE_Wave_Type"));
	Choice_Wave_Type->SetSelection( Choice_Wave_Type->Append(_("Sine")) );
	Choice_Wave_Type->Append(_("Triangle"));
	Choice_Wave_Type->Append(_("Square"));
	Choice_Wave_Type->Append(_("Decaying Sine"));
	Choice_Wave_Type->Append(_("Fractal/ivy"));
	FlexGridSizer51->Add(Choice_Wave_Type, 1, wxALL|wxEXPAND, 2);
	FlexGridSizer51->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BitmapButton_WaveType = new xlLockButton(this, ID_BITMAPBUTTON_CHOICE_Wave_Type, wxNullBitmap, wxDefaultPosition, wxSize(14,14), wxBU_AUTODRAW|wxBORDER_NONE, wxDefaultValidator, _T("ID_BITMAPBUTTON_CHOICE_Wave_Type"));
	BitmapButton_WaveType->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNHIGHLIGHT));
	FlexGridSizer51->Add(BitmapButton_WaveType, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticText48 = new wxStaticText(this, ID_STATICTEXT_Fill_Colors, _("Fill Colors"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_Fill_Colors"));
	FlexGridSizer51->Add(StaticText48, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	Choice_Fill_Colors = new BulkEditChoice(this, ID_CHOICE_Fill_Colors, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE_Fill_Colors"));
	Choice_Fill_Colors->SetSelection( Choice_Fill_Colors->Append(_("None")) );
	Choice_Fill_Colors->Append(_("Rainbow"));
	Choice_Fill_Colors->Append(_("Palette"));
	FlexGridSizer51->Add(Choice_Fill_Colors, 1, wxALL|wxEXPAND, 2);
	FlexGridSizer51->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BitmapButton_Wave_FillColors = new xlLockButton(this, ID_BITMAPBUTTON_CHOICE_Fill_Colors, wxNullBitmap, wxDefaultPosition, wxSize(14,14), wxBU_AUTODRAW|wxBORDER_NONE, wxDefaultValidator, _T("ID_BITMAPBUTTON_CHOICE_Fill_Colors"));
	BitmapButton_Wave_FillColors->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNHIGHLIGHT));
	FlexGridSizer51->Add(BitmapButton_Wave_FillColors, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	FlexGridSizer51->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	CheckBox_Mirror_Wave = new BulkEditCheckBox(this, ID_CHECKBOX_Mirror_Wave, _("Mirror Wave"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX_Mirror_Wave"));
	CheckBox_Mirror_Wave->SetValue(false);
	FlexGridSizer51->Add(CheckBox_Mirror_Wave, 1, wxALL|wxEXPAND, 5);
	FlexGridSizer51->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BitmapButton_Wave_MirrorWave = new xlLockButton(this, ID_BITMAPBUTTON_CHECKBOX_Mirror_Wave, wxNullBitmap, wxDefaultPosition, wxSize(14,14), wxBU_AUTODRAW|wxBORDER_NONE, wxDefaultValidator, _T("ID_BITMAPBUTTON_CHECKBOX_Mirror_Wave"));
	BitmapButton_Wave_MirrorWave->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNHIGHLIGHT));
	FlexGridSizer51->Add(BitmapButton_Wave_MirrorWave, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticText50 = new wxStaticText(this, ID_STATICTEXT_Number_Waves, _("Number of Waves"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_Number_Waves"));
	FlexGridSizer51->Add(StaticText50, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1 = new wxFlexGridSizer(0, 2, 0, 0);
	FlexGridSizer1->AddGrowableCol(0);
	Slider_Number_Waves = new BulkEditSlider(this, ID_SLIDER_Number_Waves, 900, 180, 3600, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_Number_Waves"));
	FlexGridSizer1->Add(Slider_Number_Waves, 1, wxALL|wxEXPAND, 2);
	BitmapButton_Number_WavesVC = new BulkEditValueCurveButton(this, ID_VALUECURVE_Number_Waves, GetValueCurveNotSelectedBitmap(), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxBORDER_NONE, wxDefaultValidator, _T("ID_VALUECURVE_Number_Waves"));
	FlexGridSizer1->Add(BitmapButton_Number_WavesVC, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	FlexGridSizer51->Add(FlexGridSizer1, 1, wxALL|wxEXPAND, 2);
	TextCtrl75 = new BulkEditTextCtrl(this, IDD_TEXTCTRL_Number_Waves, _("900"), wxDefaultPosition, wxDLG_UNIT(this,wxSize(25,-1)), 0, wxDefaultValidator, _T("IDD_TEXTCTRL_Number_Waves"));
	TextCtrl75->SetMaxLength(4);
	FlexGridSizer51->Add(TextCtrl75, 1, wxALL|wxEXPAND, 5);
	BitmapButton_NumberWaves = new xlLockButton(this, ID_BITMAPBUTTON_SLIDER_Number_Waves, wxNullBitmap, wxDefaultPosition, wxSize(14,14), wxBU_AUTODRAW|wxBORDER_NONE, wxDefaultValidator, _T("ID_BITMAPBUTTON_SLIDER_Number_Waves"));
	BitmapButton_NumberWaves->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNHIGHLIGHT));
	FlexGridSizer51->Add(BitmapButton_NumberWaves, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticText52 = new wxStaticText(this, ID_STATICTEXT_Thickness_Percentage, _("Thickness of Wave"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_Thickness_Percentage"));
	FlexGridSizer51->Add(StaticText52, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer2 = new wxFlexGridSizer(0, 2, 0, 0);
	FlexGridSizer2->AddGrowableCol(0);
	Slider_Thickness_Percentage = new BulkEditSlider(this, ID_SLIDER_Thickness_Percentage, 5, 0, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_Thickness_Percentage"));
	FlexGridSizer2->Add(Slider_Thickness_Percentage, 1, wxALL|wxEXPAND, 2);
	BitmapButton_Thickness_PercentageVC = new BulkEditValueCurveButton(this, ID_VALUECURVE_Thickness_Percentage, GetValueCurveNotSelectedBitmap(), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxBORDER_NONE, wxDefaultValidator, _T("ID_VALUECURVE_Thickness_Percentage"));
	FlexGridSizer2->Add(BitmapButton_Thickness_PercentageVC, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	FlexGridSizer51->Add(FlexGridSizer2, 1, wxALL|wxEXPAND, 2);
	TextCtrl76 = new BulkEditTextCtrl(this, IDD_TEXTCTRL_Thickness_Percentage, _("5"), wxDefaultPosition, wxDLG_UNIT(this,wxSize(25,-1)), 0, wxDefaultValidator, _T("IDD_TEXTCTRL_Thickness_Percentage"));
	TextCtrl76->SetMaxLength(3);
	FlexGridSizer51->Add(TextCtrl76, 1, wxALL|wxEXPAND, 5);
	BitmapButton_Wave_Thickness = new xlLockButton(this, ID_BITMAPBUTTON_SLIDER_Thickness_Percentage, wxNullBitmap, wxDefaultPosition, wxSize(14,14), wxBU_AUTODRAW|wxBORDER_NONE, wxDefaultValidator, _T("ID_BITMAPBUTTON_SLIDER_Thickness_Percentage"));
	BitmapButton_Wave_Thickness->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNHIGHLIGHT));
	FlexGridSizer51->Add(BitmapButton_Wave_Thickness, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticText55 = new wxStaticText(this, ID_STATICTEXT_Wave_Height, _("Scale Height of Wave"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_Wave_Height"));
	FlexGridSizer51->Add(StaticText55, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer3 = new wxFlexGridSizer(0, 2, 0, 0);
	FlexGridSizer3->AddGrowableCol(0);
	Slider_Wave_Height = new BulkEditSlider(this, ID_SLIDER_Wave_Height, 50, 0, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_Wave_Height"));
	FlexGridSizer3->Add(Slider_Wave_Height, 1, wxALL|wxEXPAND, 2);
	BitmapButton_Wave_HeightVC = new BulkEditValueCurveButton(this, ID_VALUECURVE_Wave_Height, GetValueCurveNotSelectedBitmap(), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxBORDER_NONE, wxDefaultValidator, _T("ID_VALUECURVE_Wave_Height"));
	FlexGridSizer3->Add(BitmapButton_Wave_HeightVC, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	FlexGridSizer51->Add(FlexGridSizer3, 1, wxALL|wxEXPAND, 2);
	TextCtrl77 = new BulkEditTextCtrl(this, IDD_TEXTCTRL_Wave_Height, _("50"), wxDefaultPosition, wxDLG_UNIT(this,wxSize(25,-1)), 0, wxDefaultValidator, _T("IDD_TEXTCTRL_Wave_Height"));
	TextCtrl77->SetMaxLength(3);
	FlexGridSizer51->Add(TextCtrl77, 1, wxALL|wxEXPAND, 5);
	BitmapButton_Wave_Height = new xlLockButton(this, ID_BITMAPBUTTON_SLIDER_Wave_Height, wxNullBitmap, wxDefaultPosition, wxSize(14,14), wxBU_AUTODRAW|wxBORDER_NONE, wxDefaultValidator, _T("ID_BITMAPBUTTON_SLIDER_Wave_Height"));
	BitmapButton_Wave_Height->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNHIGHLIGHT));
	FlexGridSizer51->Add(BitmapButton_Wave_Height, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticText189 = new wxStaticText(this, ID_STATICTEXT_Wave_Speed, _("Speed"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_Wave_Speed"));
	FlexGridSizer51->Add(StaticText189, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer4 = new wxFlexGridSizer(0, 2, 0, 0);
	FlexGridSizer4->AddGrowableCol(0);
	Slider_Wave_Speed = new BulkEditSliderF2(this, IDD_SLIDER_Wave_Speed, 1000, 0, 5000, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("IDD_SLIDER_Wave_Speed"));
	FlexGridSizer4->Add(Slider_Wave_Speed, 1, wxALL|wxEXPAND, 2);
	BitmapButton_Wave_SpeedVC = new BulkEditValueCurveButton(this, ID_VALUECURVE_Wave_Speed, GetValueCurveNotSelectedBitmap(), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxBORDER_NONE, wxDefaultValidator, _T("ID_VALUECURVE_Wave_Speed"));
	FlexGridSizer4->Add(BitmapButton_Wave_SpeedVC, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	FlexGridSizer51->Add(FlexGridSizer4, 1, wxALL|wxEXPAND, 2);
	TextCtrl78 = new BulkEditTextCtrlF2(this, ID_TEXTCTRL_Wave_Speed, _("10.00"), wxDefaultPosition, wxDLG_UNIT(this,wxSize(35,-1)), 0, wxDefaultValidator, _T("ID_TEXTCTRL_Wave_Speed"));
	TextCtrl78->SetMaxLength(5);
	FlexGridSizer51->Add(TextCtrl78, 1, wxALL|wxEXPAND, 5);
	BitmapButton_WaveSpeed = new xlLockButton(this, ID_BITMAPBUTTON_SLIDER_Wave_Speed, wxNullBitmap, wxDefaultPosition, wxSize(14,14), wxBU_AUTODRAW|wxBORDER_NONE, wxDefaultValidator, _T("ID_BITMAPBUTTON_SLIDER_Wave_Speed"));
	BitmapButton_WaveSpeed->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNHIGHLIGHT));
	FlexGridSizer51->Add(BitmapButton_WaveSpeed, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText56 = new wxStaticText(this, ID_STATICTEXT_Wave_Direction, _("Wave Direction"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_Wave_Direction"));
	FlexGridSizer51->Add(StaticText56, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	Choice_Wave_Direction = new BulkEditChoice(this, ID_CHOICE_Wave_Direction, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE_Wave_Direction"));
	Choice_Wave_Direction->SetSelection( Choice_Wave_Direction->Append(_("Right to Left")) );
	Choice_Wave_Direction->Append(_("Left to Right"));
	FlexGridSizer51->Add(Choice_Wave_Direction, 1, wxALL|wxEXPAND, 2);
	FlexGridSizer51->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BitmapButton_Wave_Direction = new xlLockButton(this, ID_BITMAPBUTTON_CHOICE_Wave_Direction, wxNullBitmap, wxDefaultPosition, wxSize(14,14), wxBU_AUTODRAW|wxBORDER_NONE, wxDefaultValidator, _T("ID_BITMAPBUTTON_CHOICE_Wave_Direction"));
	BitmapButton_Wave_Direction->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNHIGHLIGHT));
	FlexGridSizer51->Add(BitmapButton_Wave_Direction, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Y Offset"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	FlexGridSizer51->Add(StaticText1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer5 = new wxFlexGridSizer(0, 2, 0, 0);
	FlexGridSizer5->AddGrowableCol(0);
	Slider_Y_Offset = new BulkEditSlider(this, ID_SLIDER_Wave_YOffset, 0, -250, 250, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_Wave_YOffset"));
	FlexGridSizer5->Add(Slider_Y_Offset, 1, wxALL|wxEXPAND, 2);
	BitmapButton_Wave_YOffsetVC = new BulkEditValueCurveButton(this, ID_VALUECURVE_Wave_YOffset, GetValueCurveNotSelectedBitmap(), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxBORDER_NONE, wxDefaultValidator, _T("ID_VALUECURVE_Wave_YOffset"));
	FlexGridSizer5->Add(BitmapButton_Wave_YOffsetVC, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	FlexGridSizer51->Add(FlexGridSizer5, 1, wxALL|wxEXPAND, 5);
	TextCtrl1 = new BulkEditTextCtrl(this, IDD_TEXTCTRL_Wave_YOffset, _("0"), wxDefaultPosition, wxDLG_UNIT(this,wxSize(25,-1)), 0, wxDefaultValidator, _T("IDD_TEXTCTRL_Wave_YOffset"));
	TextCtrl1->SetMaxLength(4);
	FlexGridSizer51->Add(TextCtrl1, 1, wxALL|wxEXPAND, 5);
	BitmapButton_YOffset = new xlLockButton(this, ID_BITMAPBUTTON_Wave_YOffset, wxNullBitmap, wxDefaultPosition, wxSize(14,14), wxBU_AUTODRAW|wxBORDER_NONE, wxDefaultValidator, _T("ID_BITMAPBUTTON_Wave_YOffset"));
	BitmapButton_YOffset->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNHIGHLIGHT));
	FlexGridSizer51->Add(BitmapButton_YOffset, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(FlexGridSizer51);

	Connect(ID_BITMAPBUTTON_CHOICE_Wave_Type,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WavePanel::OnLockButtonClick);
	Connect(ID_BITMAPBUTTON_CHOICE_Fill_Colors,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WavePanel::OnLockButtonClick);
	Connect(ID_BITMAPBUTTON_CHECKBOX_Mirror_Wave,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WavePanel::OnLockButtonClick);
	Connect(ID_VALUECURVE_Number_Waves,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WavePanel::OnVCButtonClick);
	Connect(ID_BITMAPBUTTON_SLIDER_Number_Waves,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WavePanel::OnLockButtonClick);
	Connect(ID_VALUECURVE_Thickness_Percentage,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WavePanel::OnVCButtonClick);
	Connect(ID_BITMAPBUTTON_SLIDER_Thickness_Percentage,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WavePanel::OnLockButtonClick);
	Connect(ID_VALUECURVE_Wave_Height,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WavePanel::OnVCButtonClick);
	Connect(ID_BITMAPBUTTON_SLIDER_Wave_Height,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WavePanel::OnLockButtonClick);
	Connect(ID_VALUECURVE_Wave_Speed,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WavePanel::OnVCButtonClick);
	Connect(ID_BITMAPBUTTON_SLIDER_Wave_Speed,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WavePanel::OnLockButtonClick);
	Connect(ID_BITMAPBUTTON_CHOICE_Wave_Direction,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WavePanel::OnLockButtonClick);
	Connect(ID_VALUECURVE_Wave_YOffset,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WavePanel::OnVCButtonClick);
	Connect(ID_BITMAPBUTTON_Wave_YOffset,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WavePanel::OnLockButtonClick);
	//*)
    SetName("ID_PANEL_WAVE");

    Connect(wxID_ANY, EVT_VC_CHANGED, (wxObjectEventFunction)&WavePanel::OnVCChanged, 0, this);
    Connect(wxID_ANY, EVT_VALIDATEWINDOW, (wxObjectEventFunction)&WavePanel::OnValidateWindow, 0, this);

    BitmapButton_Number_WavesVC->GetValue()->SetLimits(WAVE_NUMBER_MIN, WAVE_NUMBER_MAX);
    BitmapButton_Thickness_PercentageVC->GetValue()->SetLimits(WAVE_THICKNESS_MIN, WAVE_THICKNESS_MAX);
    BitmapButton_Wave_HeightVC->GetValue()->SetLimits(WAVE_HEIGHT_MIN, WAVE_HEIGHT_MAX);
    BitmapButton_Wave_SpeedVC->GetValue()->SetLimits(WAVE_SPEED_MIN, WAVE_SPEED_MAX);
    BitmapButton_Wave_SpeedVC->GetValue()->SetDivisor(WAVE_SPEED_DIVISOR);
    BitmapButton_Wave_YOffsetVC->GetValue()->SetLimits(WAVE_YOFFSET_MIN, WAVE_YOFFSET_MAX);

    ValidateWindow();
}

WavePanel::~WavePanel()
{
	//(*Destroy(WavePanel)
	//*)
}

void WavePanel::ValidateWindow()
{
    wxString type = Choice_Wave_Type->GetStringSelection();
    if (type == "Fractal / ivy")
    {
        BitmapButton_Number_WavesVC->GetValue()->SetActive(false);
        BitmapButton_Number_WavesVC->Disable();
    }
    else
    {
        BitmapButton_Number_WavesVC->Enable();
    }
}

void WavePanel::OnChoice_Wave_TypeSelect(wxCommandEvent& event)
{
    ValidateWindow();
}
