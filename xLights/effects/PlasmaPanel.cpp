/***************************************************************
 * This source files comes from the xLights project
 * https://www.xlights.org
 * https://github.com/xLightsSequencer/xLights
 * See the github commit history for a record of contributing
 * developers.
 * Copyright claimed based on commit dates recorded in Github
 * License: https://github.com/xLightsSequencer/xLights/blob/master/License.txt
 **************************************************************/

#include "PlasmaPanel.h"
#include "EffectPanelUtils.h"
#include "PlasmaEffect.h"

//(*InternalHeaders(PlasmaPanel)
#include <wx/bitmap.h>
#include <wx/bmpbuttn.h>
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

//(*IdInit(PlasmaPanel)
const long PlasmaPanel::ID_STATICTEXT_Plasma_Color = wxNewId();
const long PlasmaPanel::ID_CHOICE_Plasma_Color = wxNewId();
const long PlasmaPanel::ID_BITMAPBUTTON_CHOICE_Plasma_Color = wxNewId();
const long PlasmaPanel::ID_STATICTEXT_Plasma_Style = wxNewId();
const long PlasmaPanel::ID_SLIDER_Plasma_Style = wxNewId();
const long PlasmaPanel::IDD_TEXTCTRL_Plasma_Style = wxNewId();
const long PlasmaPanel::ID_BITMAPBUTTON_SLIDER_Plasma_Style = wxNewId();
const long PlasmaPanel::ID_STATICTEXT_Plasma_Line_Density = wxNewId();
const long PlasmaPanel::ID_SLIDER_Plasma_Line_Density = wxNewId();
const long PlasmaPanel::IDD_TEXTCTRL_Plasma_Line_Density = wxNewId();
const long PlasmaPanel::ID_BITMAPBUTTON_SLIDER_Plasma_Line_Density = wxNewId();
const long PlasmaPanel::ID_STATICTEXT_Plasma_Speed = wxNewId();
const long PlasmaPanel::ID_SLIDER_Plasma_Speed = wxNewId();
const long PlasmaPanel::ID_VALUECURVE_Plasma_Speed = wxNewId();
const long PlasmaPanel::IDD_TEXTCTRL_Plasma_Speed = wxNewId();
const long PlasmaPanel::ID_BITMAPBUTTON_SLIDER_Plasma_Speed = wxNewId();
//*)

BEGIN_EVENT_TABLE(PlasmaPanel,wxPanel)
	//(*EventTable(PlasmaPanel)
	//*)
END_EVENT_TABLE()

PlasmaPanel::PlasmaPanel(wxWindow* parent) : xlEffectPanel(parent)
{
	//(*Initialize(PlasmaPanel)
	BulkEditTextCtrl* TextCtrl84;
	BulkEditTextCtrl* TextCtrl85;
	BulkEditTextCtrl* TextCtrl87;
	wxFlexGridSizer* FlexGridSizer139;
	wxFlexGridSizer* FlexGridSizer3;
	xlLockButton* BitmapButton_PlasmaColor;
	xlLockButton* BitmapButton_PlasmaLineDensity;
	xlLockButton* BitmapButton_PlasmaSpeed;
	xlLockButton* BitmapButton_PlasmaStyle;

	Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	FlexGridSizer139 = new wxFlexGridSizer(0, 4, 0, 0);
	FlexGridSizer139->AddGrowableCol(1);
	StaticText64 = new wxStaticText(this, ID_STATICTEXT_Plasma_Color, _("Color Choice"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_Plasma_Color"));
	FlexGridSizer139->Add(StaticText64, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 2);
	Choice_Plasma_Color = new BulkEditChoice(this, ID_CHOICE_Plasma_Color, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE_Plasma_Color"));
	Choice_Plasma_Color->SetSelection( Choice_Plasma_Color->Append(_("Normal")) );
	Choice_Plasma_Color->Append(_("Preset Colors 1"));
	Choice_Plasma_Color->Append(_("Preset Colors 2"));
	Choice_Plasma_Color->Append(_("Preset Colors 3"));
	Choice_Plasma_Color->Append(_("Preset Colors 4"));
	FlexGridSizer139->Add(Choice_Plasma_Color, 1, wxTOP|wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer139->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BitmapButton_PlasmaColor = new xlLockButton(this, ID_BITMAPBUTTON_CHOICE_Plasma_Color, wxNullBitmap, wxDefaultPosition, wxSize(14,14), wxBU_AUTODRAW|wxBORDER_NONE, wxDefaultValidator, _T("ID_BITMAPBUTTON_CHOICE_Plasma_Color"));
	BitmapButton_PlasmaColor->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNHIGHLIGHT));
	FlexGridSizer139->Add(BitmapButton_PlasmaColor, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	StaticText197 = new wxStaticText(this, ID_STATICTEXT_Plasma_Style, _("Add Twist to Effect"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_Plasma_Style"));
	FlexGridSizer139->Add(StaticText197, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 2);
	Slider_Plasma_Style = new BulkEditSlider(this, ID_SLIDER_Plasma_Style, 1, 1, 10, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_Plasma_Style"));
	FlexGridSizer139->Add(Slider_Plasma_Style, 1, wxALL|wxEXPAND, 2);
	TextCtrl84 = new BulkEditTextCtrl(this, IDD_TEXTCTRL_Plasma_Style, _("1"), wxDefaultPosition, wxDLG_UNIT(this,wxSize(20,-1)), 0, wxDefaultValidator, _T("IDD_TEXTCTRL_Plasma_Style"));
	TextCtrl84->SetMaxLength(1);
	FlexGridSizer139->Add(TextCtrl84, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BitmapButton_PlasmaStyle = new xlLockButton(this, ID_BITMAPBUTTON_SLIDER_Plasma_Style, wxNullBitmap, wxDefaultPosition, wxSize(14,14), wxBU_AUTODRAW|wxBORDER_NONE, wxDefaultValidator, _T("ID_BITMAPBUTTON_SLIDER_Plasma_Style"));
	BitmapButton_PlasmaStyle->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNHIGHLIGHT));
	FlexGridSizer139->Add(BitmapButton_PlasmaStyle, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	StaticText202 = new wxStaticText(this, ID_STATICTEXT_Plasma_Line_Density, _("Line Density"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_Plasma_Line_Density"));
	FlexGridSizer139->Add(StaticText202, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 2);
	Slider_Plasma_Line_Density = new BulkEditSlider(this, ID_SLIDER_Plasma_Line_Density, 1, 1, 10, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_Plasma_Line_Density"));
	FlexGridSizer139->Add(Slider_Plasma_Line_Density, 1, wxALL|wxEXPAND, 2);
	TextCtrl85 = new BulkEditTextCtrl(this, IDD_TEXTCTRL_Plasma_Line_Density, _("1"), wxDefaultPosition, wxDLG_UNIT(this,wxSize(20,-1)), 0, wxDefaultValidator, _T("IDD_TEXTCTRL_Plasma_Line_Density"));
	TextCtrl85->SetMaxLength(2);
	FlexGridSizer139->Add(TextCtrl85, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BitmapButton_PlasmaLineDensity = new xlLockButton(this, ID_BITMAPBUTTON_SLIDER_Plasma_Line_Density, wxNullBitmap, wxDefaultPosition, wxSize(14,14), wxBU_AUTODRAW|wxBORDER_NONE, wxDefaultValidator, _T("ID_BITMAPBUTTON_SLIDER_Plasma_Line_Density"));
	BitmapButton_PlasmaLineDensity->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNHIGHLIGHT));
	FlexGridSizer139->Add(BitmapButton_PlasmaLineDensity, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	StaticText204 = new wxStaticText(this, ID_STATICTEXT_Plasma_Speed, _("Speed"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_Plasma_Speed"));
	FlexGridSizer139->Add(StaticText204, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer3 = new wxFlexGridSizer(0, 2, 0, 0);
	FlexGridSizer3->AddGrowableCol(0);
	Slider_Plasma_Speed = new BulkEditSlider(this, ID_SLIDER_Plasma_Speed, 10, 0, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_Plasma_Speed"));
	FlexGridSizer3->Add(Slider_Plasma_Speed, 1, wxALL|wxEXPAND, 2);
	BitmapButton_Plasma_SpeedVC = new BulkEditValueCurveButton(this, ID_VALUECURVE_Plasma_Speed, GetValueCurveNotSelectedBitmap(), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxBORDER_NONE, wxDefaultValidator, _T("ID_VALUECURVE_Plasma_Speed"));
	FlexGridSizer3->Add(BitmapButton_Plasma_SpeedVC, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer139->Add(FlexGridSizer3, 1, wxALL|wxEXPAND, 2);
	TextCtrl87 = new BulkEditTextCtrl(this, IDD_TEXTCTRL_Plasma_Speed, _("10"), wxDefaultPosition, wxDLG_UNIT(this,wxSize(20,-1)), 0, wxDefaultValidator, _T("IDD_TEXTCTRL_Plasma_Speed"));
	TextCtrl87->SetMaxLength(3);
	FlexGridSizer139->Add(TextCtrl87, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BitmapButton_PlasmaSpeed = new xlLockButton(this, ID_BITMAPBUTTON_SLIDER_Plasma_Speed, wxNullBitmap, wxDefaultPosition, wxSize(14,14), wxBU_AUTODRAW|wxBORDER_NONE, wxDefaultValidator, _T("ID_BITMAPBUTTON_SLIDER_Plasma_Speed"));
	BitmapButton_PlasmaSpeed->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNHIGHLIGHT));
	FlexGridSizer139->Add(BitmapButton_PlasmaSpeed, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	SetSizer(FlexGridSizer139);

	Connect(ID_BITMAPBUTTON_CHOICE_Plasma_Color,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PlasmaPanel::OnLockButtonClick);
	Connect(ID_BITMAPBUTTON_SLIDER_Plasma_Style,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PlasmaPanel::OnLockButtonClick);
	Connect(ID_BITMAPBUTTON_SLIDER_Plasma_Line_Density,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PlasmaPanel::OnLockButtonClick);
	Connect(ID_VALUECURVE_Plasma_Speed,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PlasmaPanel::OnVCButtonClick);
	Connect(ID_BITMAPBUTTON_SLIDER_Plasma_Speed,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PlasmaPanel::OnLockButtonClick);
	//*)

    Connect(wxID_ANY, EVT_VC_CHANGED, (wxObjectEventFunction)&PlasmaPanel::OnVCChanged, 0, this);
    Connect(wxID_ANY, EVT_VALIDATEWINDOW, (wxObjectEventFunction)&PlasmaPanel::OnValidateWindow, 0, this);

    BitmapButton_Plasma_SpeedVC->GetValue()->SetLimits(PLASMA_SPEED_MIN, PLASMA_SPEED_MAX);

    SetName("ID_PANEL_PLASMA");

    ValidateWindow();
}

PlasmaPanel::~PlasmaPanel()
{
	//(*Destroy(PlasmaPanel)
	//*)
}

void PlasmaPanel::ValidateWindow()
{
}
