/***************************************************************
 * This source files comes from the xLights project
 * https://www.xlights.org
 * https://github.com/xLightsSequencer/xLights
 * See the github commit history for a record of contributing
 * developers.
 * Copyright claimed based on commit dates recorded in Github
 * License: https://github.com/xLightsSequencer/xLights/blob/master/License.txt
 **************************************************************/

#include "LifePanel.h"
#include "EffectPanelUtils.h"

//(*InternalHeaders(LifePanel)
#include <wx/bmpbuttn.h>
#include <wx/intl.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/slider.h>
#include <wx/stattext.h>
#include <wx/string.h>
#include <wx/textctrl.h>
//*)

//(*IdInit(LifePanel)
const long LifePanel::ID_STATICTEXT_Life_Count = wxNewId();
const long LifePanel::ID_SLIDER_Life_Count = wxNewId();
const long LifePanel::IDD_TEXTCTRL_Life_Count = wxNewId();
const long LifePanel::ID_BITMAPBUTTON_SLIDER_Life_Count = wxNewId();
const long LifePanel::ID_STATICTEXT_Life_Seed = wxNewId();
const long LifePanel::ID_SLIDER_Life_Seed = wxNewId();
const long LifePanel::IDD_TEXTCTRL_Life_Seed = wxNewId();
const long LifePanel::ID_BITMAPBUTTON_SLIDER_Life_Seed = wxNewId();
const long LifePanel::ID_STATICTEXT_Life_Speed = wxNewId();
const long LifePanel::ID_SLIDER_Life_Speed = wxNewId();
const long LifePanel::IDD_TEXTCTRL_Life_Speed = wxNewId();
//*)

BEGIN_EVENT_TABLE(LifePanel,wxPanel)
	//(*EventTable(LifePanel)
	//*)
END_EVENT_TABLE()

LifePanel::LifePanel(wxWindow* parent) : xlEffectPanel(parent)
{
	//(*Initialize(LifePanel)
	BulkEditTextCtrl* TextCtrl30;
	BulkEditTextCtrl* TextCtrl31;
	BulkEditTextCtrl* TextCtrl32;
	wxFlexGridSizer* FlexGridSizer40;

	Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	FlexGridSizer40 = new wxFlexGridSizer(0, 4, 0, 0);
	FlexGridSizer40->AddGrowableCol(1);
	StaticText35 = new wxStaticText(this, ID_STATICTEXT_Life_Count, _("Cells to start"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_Life_Count"));
	FlexGridSizer40->Add(StaticText35, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	Slider_Life_Count = new BulkEditSlider(this, ID_SLIDER_Life_Count, 50, 0, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_Life_Count"));
	FlexGridSizer40->Add(Slider_Life_Count, 1, wxALL|wxEXPAND, 2);
	TextCtrl32 = new BulkEditTextCtrl(this, IDD_TEXTCTRL_Life_Count, _("50"), wxDefaultPosition, wxDLG_UNIT(this,wxSize(20,-1)), 0, wxDefaultValidator, _T("IDD_TEXTCTRL_Life_Count"));
	TextCtrl32->SetMaxLength(3);
	FlexGridSizer40->Add(TextCtrl32, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BitmapButton_LifeCount = new xlLockButton(this, ID_BITMAPBUTTON_SLIDER_Life_Count, wxNullBitmap, wxDefaultPosition, wxSize(14,14), wxBU_AUTODRAW|wxBORDER_NONE, wxDefaultValidator, _T("ID_BITMAPBUTTON_SLIDER_Life_Count"));
	BitmapButton_LifeCount->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNHIGHLIGHT));
	FlexGridSizer40->Add(BitmapButton_LifeCount, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	StaticText37 = new wxStaticText(this, ID_STATICTEXT_Life_Seed, _("Type"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_Life_Seed"));
	FlexGridSizer40->Add(StaticText37, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	Slider_Life_Seed = new BulkEditSlider(this, ID_SLIDER_Life_Seed, 0, 0, 4, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_Life_Seed"));
	FlexGridSizer40->Add(Slider_Life_Seed, 1, wxALL|wxEXPAND, 2);
	TextCtrl31 = new BulkEditTextCtrl(this, IDD_TEXTCTRL_Life_Seed, _("0"), wxDefaultPosition, wxDLG_UNIT(this,wxSize(20,-1)), 0, wxDefaultValidator, _T("IDD_TEXTCTRL_Life_Seed"));
	TextCtrl31->SetMaxLength(2);
	FlexGridSizer40->Add(TextCtrl31, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BitmapButton_LifeSpeed = new xlLockButton(this, ID_BITMAPBUTTON_SLIDER_Life_Seed, wxNullBitmap, wxDefaultPosition, wxSize(14,14), wxBU_AUTODRAW|wxBORDER_NONE, wxDefaultValidator, _T("ID_BITMAPBUTTON_SLIDER_Life_Seed"));
	BitmapButton_LifeSpeed->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNHIGHLIGHT));
	FlexGridSizer40->Add(BitmapButton_LifeSpeed, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	StaticText134 = new wxStaticText(this, ID_STATICTEXT_Life_Speed, _("Speed"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_Life_Speed"));
	FlexGridSizer40->Add(StaticText134, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	Slider_Life_Speed = new BulkEditSlider(this, ID_SLIDER_Life_Speed, 10, 1, 30, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_Life_Speed"));
	FlexGridSizer40->Add(Slider_Life_Speed, 1, wxALL|wxEXPAND, 2);
	TextCtrl30 = new BulkEditTextCtrl(this, IDD_TEXTCTRL_Life_Speed, _("10"), wxDefaultPosition, wxDLG_UNIT(this,wxSize(20,-1)), 0, wxDefaultValidator, _T("IDD_TEXTCTRL_Life_Speed"));
	TextCtrl30->SetMaxLength(2);
	FlexGridSizer40->Add(TextCtrl30, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	SetSizer(FlexGridSizer40);

	Connect(ID_BITMAPBUTTON_SLIDER_Life_Count,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LifePanel::OnLockButtonClick);
	Connect(ID_BITMAPBUTTON_SLIDER_Life_Seed,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LifePanel::OnLockButtonClick);
	//*)

    Connect(wxID_ANY, EVT_VC_CHANGED, (wxObjectEventFunction)&LifePanel::OnVCChanged, 0, this);
    Connect(wxID_ANY, EVT_VALIDATEWINDOW, (wxObjectEventFunction)&LifePanel::OnValidateWindow, 0, this);

    SetName("ID_PANEL_LIFE");

	ValidateWindow();
}

LifePanel::~LifePanel()
{
	//(*Destroy(LifePanel)
	//*)
}

void LifePanel::ValidateWindow()
{
}
