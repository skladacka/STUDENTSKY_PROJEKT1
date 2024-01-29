/***************************************************************
 * This source files comes from the xLights project
 * https://www.xlights.org
 * https://github.com/xLightsSequencer/xLights
 * See the github commit history for a record of contributing
 * developers.
 * Copyright claimed based on commit dates recorded in Github
 * License: https://github.com/xLightsSequencer/xLights/blob/master/License.txt
 **************************************************************/

#include "SnowstormPanel.h"
#include "EffectPanelUtils.h"

//(*InternalHeaders(SnowstormPanel)
#include <wx/bmpbuttn.h>
#include <wx/intl.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/slider.h>
#include <wx/stattext.h>
#include <wx/string.h>
#include <wx/textctrl.h>
//*)

//(*IdInit(SnowstormPanel)
const long SnowstormPanel::ID_STATICTEXT_Snowstorm_Count = wxNewId();
const long SnowstormPanel::ID_SLIDER_Snowstorm_Count = wxNewId();
const long SnowstormPanel::IDD_TEXTCTRL_Snowstorm_Count = wxNewId();
const long SnowstormPanel::ID_BITMAPBUTTON_SLIDER_Snowstorm_Count = wxNewId();
const long SnowstormPanel::ID_STATICTEXT_Snowstorm_Length = wxNewId();
const long SnowstormPanel::ID_SLIDER_Snowstorm_Length = wxNewId();
const long SnowstormPanel::IDD_TEXTCTRL_Snowstorm_Length = wxNewId();
const long SnowstormPanel::ID_BITMAPBUTTON_SLIDER_Snowstorm_Length = wxNewId();
const long SnowstormPanel::ID_STATICTEXT_Snowstorm_Speed = wxNewId();
const long SnowstormPanel::ID_SLIDER_Snowstorm_Speed = wxNewId();
const long SnowstormPanel::IDD_TEXTCTRL_Snowstorm_Speed = wxNewId();
const long SnowstormPanel::ID_BITMAPBUTTON_SLIDER_Snowstorm_Speed = wxNewId();
//*)

BEGIN_EVENT_TABLE(SnowstormPanel,wxPanel)
	//(*EventTable(SnowstormPanel)
	//*)
END_EVENT_TABLE()

SnowstormPanel::SnowstormPanel(wxWindow* parent) : xlEffectPanel(parent)
{
	//(*Initialize(SnowstormPanel)
	BulkEditTextCtrl* TextCtrl53;
	BulkEditTextCtrl* TextCtrl54;
	BulkEditTextCtrl* TextCtrl55;
	wxFlexGridSizer* FlexGridSizer44;

	Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	FlexGridSizer44 = new wxFlexGridSizer(0, 4, 0, 0);
	FlexGridSizer44->AddGrowableCol(1);
	StaticText45 = new wxStaticText(this, ID_STATICTEXT_Snowstorm_Count, _("Max flakes"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_Snowstorm_Count"));
	FlexGridSizer44->Add(StaticText45, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	Slider_Snowstorm_Count = new BulkEditSlider(this, ID_SLIDER_Snowstorm_Count, 50, 0, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_Snowstorm_Count"));
	FlexGridSizer44->Add(Slider_Snowstorm_Count, 1, wxALL|wxEXPAND, 2);
	TextCtrl53 = new BulkEditTextCtrl(this, IDD_TEXTCTRL_Snowstorm_Count, _("50"), wxDefaultPosition, wxDLG_UNIT(this,wxSize(20,-1)), 0, wxDefaultValidator, _T("IDD_TEXTCTRL_Snowstorm_Count"));
	TextCtrl53->SetMaxLength(3);
	FlexGridSizer44->Add(TextCtrl53, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BitmapButton_SnowstormCount = new xlLockButton(this, ID_BITMAPBUTTON_SLIDER_Snowstorm_Count, wxNullBitmap, wxDefaultPosition, wxSize(14,14), wxBU_AUTODRAW|wxBORDER_NONE, wxDefaultValidator, _T("ID_BITMAPBUTTON_SLIDER_Snowstorm_Count"));
	BitmapButton_SnowstormCount->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNHIGHLIGHT));
	FlexGridSizer44->Add(BitmapButton_SnowstormCount, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	StaticText51 = new wxStaticText(this, ID_STATICTEXT_Snowstorm_Length, _("Trail Length"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_Snowstorm_Length"));
	FlexGridSizer44->Add(StaticText51, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	Slider_Snowstorm_Length = new BulkEditSlider(this, ID_SLIDER_Snowstorm_Length, 50, 0, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_Snowstorm_Length"));
	FlexGridSizer44->Add(Slider_Snowstorm_Length, 1, wxALL|wxEXPAND, 2);
	TextCtrl54 = new BulkEditTextCtrl(this, IDD_TEXTCTRL_Snowstorm_Length, _("50"), wxDefaultPosition, wxDLG_UNIT(this,wxSize(20,-1)), 0, wxDefaultValidator, _T("IDD_TEXTCTRL_Snowstorm_Length"));
	TextCtrl54->SetMaxLength(3);
	FlexGridSizer44->Add(TextCtrl54, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BitmapButton_SnowstormLength = new xlLockButton(this, ID_BITMAPBUTTON_SLIDER_Snowstorm_Length, wxNullBitmap, wxDefaultPosition, wxSize(14,14), wxBU_AUTODRAW|wxBORDER_NONE, wxDefaultValidator, _T("ID_BITMAPBUTTON_SLIDER_Snowstorm_Length"));
	BitmapButton_SnowstormLength->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNHIGHLIGHT));
	FlexGridSizer44->Add(BitmapButton_SnowstormLength, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	StaticText180 = new wxStaticText(this, ID_STATICTEXT_Snowstorm_Speed, _("Speed"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_Snowstorm_Speed"));
	FlexGridSizer44->Add(StaticText180, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	Slider_Snowstorm_Speed = new BulkEditSlider(this, ID_SLIDER_Snowstorm_Speed, 10, 1, 50, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_Snowstorm_Speed"));
	FlexGridSizer44->Add(Slider_Snowstorm_Speed, 1, wxALL|wxEXPAND, 5);
	TextCtrl55 = new BulkEditTextCtrl(this, IDD_TEXTCTRL_Snowstorm_Speed, _("10"), wxDefaultPosition, wxDLG_UNIT(this,wxSize(20,-1)), 0, wxDefaultValidator, _T("IDD_TEXTCTRL_Snowstorm_Speed"));
	TextCtrl55->SetMaxLength(3);
	FlexGridSizer44->Add(TextCtrl55, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BitmapButton_SnowstormSpeed = new xlLockButton(this, ID_BITMAPBUTTON_SLIDER_Snowstorm_Speed, wxNullBitmap, wxDefaultPosition, wxSize(14,14), wxBU_AUTODRAW|wxBORDER_NONE, wxDefaultValidator, _T("ID_BITMAPBUTTON_SLIDER_Snowstorm_Speed"));
	BitmapButton_SnowstormSpeed->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNHIGHLIGHT));
	FlexGridSizer44->Add(BitmapButton_SnowstormSpeed, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(FlexGridSizer44);

	Connect(ID_BITMAPBUTTON_SLIDER_Snowstorm_Count,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SnowstormPanel::OnLockButtonClick);
	Connect(ID_BITMAPBUTTON_SLIDER_Snowstorm_Length,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SnowstormPanel::OnLockButtonClick);
	Connect(ID_BITMAPBUTTON_SLIDER_Snowstorm_Speed,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SnowstormPanel::OnLockButtonClick);
	//*)

    Connect(wxID_ANY, EVT_VC_CHANGED, (wxObjectEventFunction)&SnowstormPanel::OnVCChanged, 0, this);
    Connect(wxID_ANY, EVT_VALIDATEWINDOW, (wxObjectEventFunction)&SnowstormPanel::OnValidateWindow, 0, this);

    SetName("ID_PANEL_SNOWSTORM");

    ValidateWindow();
}

SnowstormPanel::~SnowstormPanel()
{
	//(*Destroy(SnowstormPanel)
	//*)
}

void SnowstormPanel::ValidateWindow()
{
}
