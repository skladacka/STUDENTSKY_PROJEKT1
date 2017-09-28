#include "MorphPanel.h"
#include "../../include/padlock16x16-blue.xpm"
#include "EffectPanelUtils.h"
#include "MorphEffect.h"

//(*InternalHeaders(MorphPanel)
#include <wx/notebook.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/bitmap.h>
#include <wx/slider.h>
#include <wx/settings.h>
#include <wx/choice.h>
#include <wx/bmpbuttn.h>
#include <wx/intl.h>
#include <wx/button.h>
#include <wx/image.h>
#include <wx/string.h>
//*)

//(*IdInit(MorphPanel)
const long MorphPanel::ID_SLIDER_Morph_Start_X1 = wxNewId();
const long MorphPanel::ID_VALUECURVE_Morph_Start_X1 = wxNewId();
const long MorphPanel::IDD_TEXTCTRL_Morph_Start_X1 = wxNewId();
const long MorphPanel::ID_BITMAPBUTTON_SLIDER_Morph_Start_X1 = wxNewId();
const long MorphPanel::ID_STATICTEXT3 = wxNewId();
const long MorphPanel::ID_SLIDER_Morph_Start_Y1 = wxNewId();
const long MorphPanel::ID_VALUECURVE_Morph_Start_Y1 = wxNewId();
const long MorphPanel::IDD_TEXTCTRL_Morph_Start_Y1 = wxNewId();
const long MorphPanel::ID_BITMAPBUTTON_SLIDER_Morph_Start_Y1 = wxNewId();
const long MorphPanel::ID_STATICTEXT61 = wxNewId();
const long MorphPanel::ID_SLIDER_Morph_Start_X2 = wxNewId();
const long MorphPanel::ID_VALUECURVE_Morph_Start_X2 = wxNewId();
const long MorphPanel::IDD_TEXTCTRL_Morph_Start_X2 = wxNewId();
const long MorphPanel::ID_BITMAPBUTTON_SLIDER_Morph_Start_X2 = wxNewId();
const long MorphPanel::ID_STATICTEXT102 = wxNewId();
const long MorphPanel::ID_SLIDER_Morph_Start_Y2 = wxNewId();
const long MorphPanel::ID_VALUECURVE_Morph_Start_Y2 = wxNewId();
const long MorphPanel::IDD_TEXTCTRL_Morph_Start_Y2 = wxNewId();
const long MorphPanel::ID_BITMAPBUTTON_SLIDER_Morph_Start_Y2 = wxNewId();
const long MorphPanel::ID_STATICTEXT115 = wxNewId();
const long MorphPanel::ID_SLIDER_MorphStartLength = wxNewId();
const long MorphPanel::ID_VALUECURVE_MorphStartLength = wxNewId();
const long MorphPanel::IDD_TEXTCTRL_MorphStartLength = wxNewId();
const long MorphPanel::ID_BITMAPBUTTON_SLIDER_MorphStartLength = wxNewId();
const long MorphPanel::ID_CHECKBOX_Morph_Start_Link = wxNewId();
const long MorphPanel::IDD_CHOICE_Morph_QuickSet = wxNewId();
const long MorphPanel::ID_PANEL4 = wxNewId();
const long MorphPanel::ID_STATICTEXT116 = wxNewId();
const long MorphPanel::ID_SLIDER_Morph_End_X1 = wxNewId();
const long MorphPanel::ID_VALUECURVE_Morph_End_X1 = wxNewId();
const long MorphPanel::IDD_TEXTCTRL_Morph_End_X1 = wxNewId();
const long MorphPanel::ID_BITMAPBUTTON_SLIDER_Morph_End_X1 = wxNewId();
const long MorphPanel::ID_STATICTEXT117 = wxNewId();
const long MorphPanel::ID_SLIDER_Morph_End_Y1 = wxNewId();
const long MorphPanel::ID_VALUECURVE_Morph_End_Y1 = wxNewId();
const long MorphPanel::IDD_TEXTCTRL_Morph_End_Y1 = wxNewId();
const long MorphPanel::ID_BITMAPBUTTON_SLIDER_Morph_End_Y1 = wxNewId();
const long MorphPanel::ID_STATICTEXT119 = wxNewId();
const long MorphPanel::ID_SLIDER_Morph_End_X2 = wxNewId();
const long MorphPanel::ID_VALUECURVE_Morph_End_X2 = wxNewId();
const long MorphPanel::IDD_TEXTCTRL_Morph_End_X2 = wxNewId();
const long MorphPanel::ID_BITMAPBUTTON_SLIDER_Morph_End_X2 = wxNewId();
const long MorphPanel::ID_STATICTEXT121 = wxNewId();
const long MorphPanel::ID_SLIDER_Morph_End_Y2 = wxNewId();
const long MorphPanel::ID_VALUECURVE_Morph_End_Y2 = wxNewId();
const long MorphPanel::IDD_TEXTCTRL_Morph_End_Y2 = wxNewId();
const long MorphPanel::ID_BITMAPBUTTON_SLIDER_Morph_End_Y2 = wxNewId();
const long MorphPanel::ID_STATICTEXT122 = wxNewId();
const long MorphPanel::ID_SLIDER_MorphEndLength = wxNewId();
const long MorphPanel::ID_VALUECURVE_MorphEndLength = wxNewId();
const long MorphPanel::IDD_TEXTCTRL_MorphEndLength = wxNewId();
const long MorphPanel::ID_BITMAPBUTTON_SLIDER_MorphEndLength = wxNewId();
const long MorphPanel::ID_CHECKBOX_Morph_End_Link = wxNewId();
const long MorphPanel::ID_PANEL27 = wxNewId();
const long MorphPanel::ID_STATICTEXT123 = wxNewId();
const long MorphPanel::ID_SLIDER_MorphDuration = wxNewId();
const long MorphPanel::ID_VALUECURVE_MorphDuration = wxNewId();
const long MorphPanel::IDD_TEXTCTRL_MorphDuration = wxNewId();
const long MorphPanel::ID_BITMAPBUTTON_SLIDER_MorphDuration = wxNewId();
const long MorphPanel::ID_STATICTEXT124 = wxNewId();
const long MorphPanel::ID_SLIDER_MorphAccel = wxNewId();
const long MorphPanel::ID_VALUECURVE_MorphAccel = wxNewId();
const long MorphPanel::IDD_TEXTCTRL_MorphAccel = wxNewId();
const long MorphPanel::ID_BITMAPBUTTON_SLIDER_MorphAccel = wxNewId();
const long MorphPanel::ID_STATICTEXT97 = wxNewId();
const long MorphPanel::ID_SLIDER_Morph_Repeat_Count = wxNewId();
const long MorphPanel::ID_VALUECURVE_Morph_Repeat_Count = wxNewId();
const long MorphPanel::IDD_TEXTCTRL_Morph_Repeat_Count = wxNewId();
const long MorphPanel::ID_BITMAPBUTTON_SLIDER_Morph_Repeat_Count = wxNewId();
const long MorphPanel::ID_STATICTEXT156 = wxNewId();
const long MorphPanel::ID_SLIDER_Morph_Repeat_Skip = wxNewId();
const long MorphPanel::ID_VALUECURVE_Morph_Repeat_Skip = wxNewId();
const long MorphPanel::IDD_TEXTCTRL_Morph_Repeat_Skip = wxNewId();
const long MorphPanel::ID_BITMAPBUTTON_SLIDER_Morph_Repeat_Skip = wxNewId();
const long MorphPanel::ID_STATICTEXT157 = wxNewId();
const long MorphPanel::ID_SLIDER_Morph_Stagger = wxNewId();
const long MorphPanel::ID_VALUECURVE_Morph_Stagger = wxNewId();
const long MorphPanel::IDD_TEXTCTRL_Morph_Stagger = wxNewId();
const long MorphPanel::ID_BITMAPBUTTON_SLIDER_Morph_Stagger = wxNewId();
const long MorphPanel::ID_CHECKBOX_ShowHeadAtStart = wxNewId();
const long MorphPanel::ID_BITMAPBUTTON_CHECKBOX_ShowHeadAtStart = wxNewId();
const long MorphPanel::ID_BUTTONSWAP = wxNewId();
const long MorphPanel::ID_PANEL30 = wxNewId();
const long MorphPanel::ID_NOTEBOOK_Morph = wxNewId();
//*)

BEGIN_EVENT_TABLE(MorphPanel,wxPanel)
	//(*EventTable(MorphPanel)
	//*)
END_EVENT_TABLE()

MorphPanel::MorphPanel(wxWindow* parent)
{
	//(*Initialize(MorphPanel)
	wxPanel* MorphPanelStart;
	wxFlexGridSizer* FlexGridSizer105;
	wxTextCtrl* TextCtrl_Morph_Stagger;
	wxStaticText* StaticText2;
	wxTextCtrl* TextCtrl_Morph_Repeat_Count;
	wxTextCtrl* TextCtrl_MorphAccel;
	wxPanel* MorphPanelOptions;
	wxPanel* MorphPanelEnd;
	wxFlexGridSizer* FlexGridSizer86;
	wxTextCtrl* TextCtrl_Morph_Repeat_Skip;
	wxTextCtrl* TextCtrl_MorphStartLength;
	wxFlexGridSizer* FlexGridSizer99;
	wxFlexGridSizer* FlexGridSizer78;
	wxTextCtrl* TextCtrl_MorphDuration;
	wxTextCtrl* TextCtrl_MorphEndLength;
	wxFlexGridSizer* FlexGridSizer104;
	wxFlexGridSizer* FlexGridSizer94;

	Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	FlexGridSizer78 = new wxFlexGridSizer(0, 1, 0, 0);
	FlexGridSizer78->AddGrowableCol(0);
	Notebook_Morph = new wxNotebook(this, ID_NOTEBOOK_Morph, wxDefaultPosition, wxDefaultSize, 0, _T("ID_NOTEBOOK_Morph"));
	MorphPanelStart = new wxPanel(Notebook_Morph, ID_PANEL4, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL4"));
	FlexGridSizer86 = new wxFlexGridSizer(0, 5, 0, 0);
	FlexGridSizer86->AddGrowableCol(1);
	StaticText2 = new wxStaticText(MorphPanelStart, wxID_ANY, _("X1a:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	FlexGridSizer86->Add(StaticText2, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	Slider_Morph_Start_X1 = new wxSlider(MorphPanelStart, ID_SLIDER_Morph_Start_X1, 0, 0, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_Morph_Start_X1"));
	FlexGridSizer86->Add(Slider_Morph_Start_X1, 1, wxALL|wxEXPAND, 5);
	BitmapButton_Morph_Start_X1 = new ValueCurveButton(MorphPanelStart, ID_VALUECURVE_Morph_Start_X1, valuecurvenotselected_24, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_VALUECURVE_Morph_Start_X1"));
	FlexGridSizer86->Add(BitmapButton_Morph_Start_X1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	TextCtrl_Morph_Start_X1 = new wxTextCtrl(MorphPanelStart, IDD_TEXTCTRL_Morph_Start_X1, _("0"), wxDefaultPosition, wxDLG_UNIT(MorphPanelStart,wxSize(20,-1)), wxTE_CENTRE, wxDefaultValidator, _T("IDD_TEXTCTRL_Morph_Start_X1"));
	TextCtrl_Morph_Start_X1->SetMaxLength(3);
	FlexGridSizer86->Add(TextCtrl_Morph_Start_X1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BitmapButton_Morph_StartX1 = new wxBitmapButton(MorphPanelStart, ID_BITMAPBUTTON_SLIDER_Morph_Start_X1, padlock16x16_blue_xpm, wxDefaultPosition, wxSize(13,13), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON_SLIDER_Morph_Start_X1"));
	BitmapButton_Morph_StartX1->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	FlexGridSizer86->Add(BitmapButton_Morph_StartX1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText3 = new wxStaticText(MorphPanelStart, ID_STATICTEXT3, _("Y1a:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	FlexGridSizer86->Add(StaticText3, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	Slider_Morph_Start_Y1 = new wxSlider(MorphPanelStart, ID_SLIDER_Morph_Start_Y1, 0, 0, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_Morph_Start_Y1"));
	FlexGridSizer86->Add(Slider_Morph_Start_Y1, 1, wxALL|wxEXPAND, 5);
	BitmapButton_Morph_Start_Y1 = new ValueCurveButton(MorphPanelStart, ID_VALUECURVE_Morph_Start_Y1, valuecurvenotselected_24, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_VALUECURVE_Morph_Start_Y1"));
	FlexGridSizer86->Add(BitmapButton_Morph_Start_Y1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	TextCtrl_Morph_Start_Y1 = new wxTextCtrl(MorphPanelStart, IDD_TEXTCTRL_Morph_Start_Y1, _("0"), wxDefaultPosition, wxDLG_UNIT(MorphPanelStart,wxSize(20,-1)), wxTE_CENTRE, wxDefaultValidator, _T("IDD_TEXTCTRL_Morph_Start_Y1"));
	TextCtrl_Morph_Start_Y1->SetMaxLength(3);
	FlexGridSizer86->Add(TextCtrl_Morph_Start_Y1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BitmapButton_Morph_StartY1 = new wxBitmapButton(MorphPanelStart, ID_BITMAPBUTTON_SLIDER_Morph_Start_Y1, padlock16x16_blue_xpm, wxDefaultPosition, wxSize(13,13), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON_SLIDER_Morph_Start_Y1"));
	BitmapButton_Morph_StartY1->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	FlexGridSizer86->Add(BitmapButton_Morph_StartY1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText59 = new wxStaticText(MorphPanelStart, ID_STATICTEXT61, _("X1b:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT61"));
	FlexGridSizer86->Add(StaticText59, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	Slider_Morph_Start_X2 = new wxSlider(MorphPanelStart, ID_SLIDER_Morph_Start_X2, 100, 0, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_Morph_Start_X2"));
	FlexGridSizer86->Add(Slider_Morph_Start_X2, 1, wxALL|wxEXPAND, 5);
	BitmapButton_Morph_Start_X2 = new ValueCurveButton(MorphPanelStart, ID_VALUECURVE_Morph_Start_X2, valuecurvenotselected_24, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_VALUECURVE_Morph_Start_X2"));
	FlexGridSizer86->Add(BitmapButton_Morph_Start_X2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	TextCtrl_Morph_Start_X2 = new wxTextCtrl(MorphPanelStart, IDD_TEXTCTRL_Morph_Start_X2, _("100"), wxDefaultPosition, wxDLG_UNIT(MorphPanelStart,wxSize(20,-1)), wxTE_CENTRE, wxDefaultValidator, _T("IDD_TEXTCTRL_Morph_Start_X2"));
	TextCtrl_Morph_Start_X2->SetMaxLength(3);
	FlexGridSizer86->Add(TextCtrl_Morph_Start_X2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BitmapButton_Morph_StartX2 = new wxBitmapButton(MorphPanelStart, ID_BITMAPBUTTON_SLIDER_Morph_Start_X2, padlock16x16_blue_xpm, wxDefaultPosition, wxSize(13,13), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON_SLIDER_Morph_Start_X2"));
	BitmapButton_Morph_StartX2->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	FlexGridSizer86->Add(BitmapButton_Morph_StartX2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText99 = new wxStaticText(MorphPanelStart, ID_STATICTEXT102, _("Y1b:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT102"));
	FlexGridSizer86->Add(StaticText99, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	Slider_Morph_Start_Y2 = new wxSlider(MorphPanelStart, ID_SLIDER_Morph_Start_Y2, 0, 0, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_Morph_Start_Y2"));
	FlexGridSizer86->Add(Slider_Morph_Start_Y2, 1, wxALL|wxEXPAND, 5);
	BitmapButton_Morph_Start_Y2 = new ValueCurveButton(MorphPanelStart, ID_VALUECURVE_Morph_Start_Y2, valuecurvenotselected_24, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_VALUECURVE_Morph_Start_Y2"));
	FlexGridSizer86->Add(BitmapButton_Morph_Start_Y2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	TextCtrl_Morph_Start_Y2 = new wxTextCtrl(MorphPanelStart, IDD_TEXTCTRL_Morph_Start_Y2, _("0"), wxDefaultPosition, wxDLG_UNIT(MorphPanelStart,wxSize(20,-1)), wxTE_CENTRE, wxDefaultValidator, _T("IDD_TEXTCTRL_Morph_Start_Y2"));
	TextCtrl_Morph_Start_Y2->SetMaxLength(3);
	FlexGridSizer86->Add(TextCtrl_Morph_Start_Y2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BitmapButton_Morph_StartY2 = new wxBitmapButton(MorphPanelStart, ID_BITMAPBUTTON_SLIDER_Morph_Start_Y2, padlock16x16_blue_xpm, wxDefaultPosition, wxSize(13,13), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON_SLIDER_Morph_Start_Y2"));
	BitmapButton_Morph_StartY2->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	FlexGridSizer86->Add(BitmapButton_Morph_StartY2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText115 = new wxStaticText(MorphPanelStart, ID_STATICTEXT115, _("Head Length:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT115"));
	FlexGridSizer86->Add(StaticText115, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	Slider_MorphStartLength = new wxSlider(MorphPanelStart, ID_SLIDER_MorphStartLength, 1, 0, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_MorphStartLength"));
	FlexGridSizer86->Add(Slider_MorphStartLength, 1, wxALL|wxEXPAND, 5);
	BitmapButton_MorphStartLength = new ValueCurveButton(MorphPanelStart, ID_VALUECURVE_MorphStartLength, valuecurvenotselected_24, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_VALUECURVE_MorphStartLength"));
	FlexGridSizer86->Add(BitmapButton_MorphStartLength, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	TextCtrl_MorphStartLength = new wxTextCtrl(MorphPanelStart, IDD_TEXTCTRL_MorphStartLength, _("1"), wxDefaultPosition, wxDLG_UNIT(MorphPanelStart,wxSize(20,-1)), wxTE_CENTRE, wxDefaultValidator, _T("IDD_TEXTCTRL_MorphStartLength"));
	TextCtrl_MorphStartLength->SetMaxLength(3);
	FlexGridSizer86->Add(TextCtrl_MorphStartLength, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BitmapButton_Morph_StartLength = new wxBitmapButton(MorphPanelStart, ID_BITMAPBUTTON_SLIDER_MorphStartLength, padlock16x16_blue_xpm, wxDefaultPosition, wxSize(13,13), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON_SLIDER_MorphStartLength"));
	BitmapButton_Morph_StartLength->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	FlexGridSizer86->Add(BitmapButton_Morph_StartLength, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer86->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	CheckBox_Morph_Start_Link = new wxCheckBox(MorphPanelStart, ID_CHECKBOX_Morph_Start_Link, _("Link Points"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX_Morph_Start_Link"));
	CheckBox_Morph_Start_Link->SetValue(false);
	FlexGridSizer86->Add(CheckBox_Morph_Start_Link, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer86->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer86->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	FlexGridSizer86->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer86->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	Choice_Morph_QuickSet = new wxChoice(MorphPanelStart, IDD_CHOICE_Morph_QuickSet, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("IDD_CHOICE_Morph_QuickSet"));
	Choice_Morph_QuickSet->SetSelection( Choice_Morph_QuickSet->Append(_("Morph Quickset Options")) );
	Choice_Morph_QuickSet->Append(_("Full Sweep Up"));
	Choice_Morph_QuickSet->Append(_("Full Sweep Down"));
	Choice_Morph_QuickSet->Append(_("Full Sweep Right"));
	Choice_Morph_QuickSet->Append(_("Full Sweep Left"));
	Choice_Morph_QuickSet->Append(_("Single Sweep Up"));
	Choice_Morph_QuickSet->Append(_("Single Sweep Down"));
	Choice_Morph_QuickSet->Append(_("Single Sweep Right"));
	Choice_Morph_QuickSet->Append(_("Single Sweep Left"));
	FlexGridSizer86->Add(Choice_Morph_QuickSet, 1, wxALL|wxEXPAND, 5);
	MorphPanelStart->SetSizer(FlexGridSizer86);
	FlexGridSizer86->Fit(MorphPanelStart);
	FlexGridSizer86->SetSizeHints(MorphPanelStart);
	MorphPanelEnd = new wxPanel(Notebook_Morph, ID_PANEL27, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL27"));
	FlexGridSizer94 = new wxFlexGridSizer(0, 5, 0, 0);
	FlexGridSizer94->AddGrowableCol(1);
	StaticText117 = new wxStaticText(MorphPanelEnd, ID_STATICTEXT116, _("X2a:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT116"));
	FlexGridSizer94->Add(StaticText117, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	Slider_Morph_End_X1 = new wxSlider(MorphPanelEnd, ID_SLIDER_Morph_End_X1, 0, 0, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_Morph_End_X1"));
	FlexGridSizer94->Add(Slider_Morph_End_X1, 1, wxALL|wxEXPAND, 5);
	BitmapButton_Morph_End_X1 = new ValueCurveButton(MorphPanelEnd, ID_VALUECURVE_Morph_End_X1, valuecurvenotselected_24, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_VALUECURVE_Morph_End_X1"));
	FlexGridSizer94->Add(BitmapButton_Morph_End_X1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	TextCtrl_Morph_End_X1 = new wxTextCtrl(MorphPanelEnd, IDD_TEXTCTRL_Morph_End_X1, _("0"), wxDefaultPosition, wxDLG_UNIT(MorphPanelEnd,wxSize(20,-1)), wxTE_CENTRE, wxDefaultValidator, _T("IDD_TEXTCTRL_Morph_End_X1"));
	TextCtrl_Morph_End_X1->SetMaxLength(3);
	FlexGridSizer94->Add(TextCtrl_Morph_End_X1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BitmapButton_Morph_EndX1 = new wxBitmapButton(MorphPanelEnd, ID_BITMAPBUTTON_SLIDER_Morph_End_X1, padlock16x16_blue_xpm, wxDefaultPosition, wxSize(13,13), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON_SLIDER_Morph_End_X1"));
	BitmapButton_Morph_EndX1->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	FlexGridSizer94->Add(BitmapButton_Morph_EndX1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText119 = new wxStaticText(MorphPanelEnd, ID_STATICTEXT117, _("Y2a:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT117"));
	FlexGridSizer94->Add(StaticText119, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	Slider_Morph_End_Y1 = new wxSlider(MorphPanelEnd, ID_SLIDER_Morph_End_Y1, 100, 0, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_Morph_End_Y1"));
	FlexGridSizer94->Add(Slider_Morph_End_Y1, 1, wxALL|wxEXPAND, 5);
	BitmapButton_Morph_End_Y1 = new ValueCurveButton(MorphPanelEnd, ID_VALUECURVE_Morph_End_Y1, valuecurvenotselected_24, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_VALUECURVE_Morph_End_Y1"));
	FlexGridSizer94->Add(BitmapButton_Morph_End_Y1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	TextCtrl_Morph_End_Y1 = new wxTextCtrl(MorphPanelEnd, IDD_TEXTCTRL_Morph_End_Y1, _("100"), wxDefaultPosition, wxDLG_UNIT(MorphPanelEnd,wxSize(20,-1)), wxTE_CENTRE, wxDefaultValidator, _T("IDD_TEXTCTRL_Morph_End_Y1"));
	TextCtrl_Morph_End_Y1->SetMaxLength(3);
	FlexGridSizer94->Add(TextCtrl_Morph_End_Y1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BitmapButton_Morph_EndY1 = new wxBitmapButton(MorphPanelEnd, ID_BITMAPBUTTON_SLIDER_Morph_End_Y1, padlock16x16_blue_xpm, wxDefaultPosition, wxSize(13,13), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON_SLIDER_Morph_End_Y1"));
	BitmapButton_Morph_EndY1->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	FlexGridSizer94->Add(BitmapButton_Morph_EndY1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText120 = new wxStaticText(MorphPanelEnd, ID_STATICTEXT119, _("X2b:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT119"));
	FlexGridSizer94->Add(StaticText120, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	Slider_Morph_End_X2 = new wxSlider(MorphPanelEnd, ID_SLIDER_Morph_End_X2, 100, 0, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_Morph_End_X2"));
	FlexGridSizer94->Add(Slider_Morph_End_X2, 1, wxALL|wxEXPAND, 5);
	BitmapButton_Morph_End_X2 = new ValueCurveButton(MorphPanelEnd, ID_VALUECURVE_Morph_End_X2, valuecurvenotselected_24, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_VALUECURVE_Morph_End_X2"));
	FlexGridSizer94->Add(BitmapButton_Morph_End_X2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	TextCtrl_Morph_End_X2 = new wxTextCtrl(MorphPanelEnd, IDD_TEXTCTRL_Morph_End_X2, _("100"), wxDefaultPosition, wxDLG_UNIT(MorphPanelEnd,wxSize(20,-1)), wxTE_CENTRE, wxDefaultValidator, _T("IDD_TEXTCTRL_Morph_End_X2"));
	TextCtrl_Morph_End_X2->SetMaxLength(3);
	FlexGridSizer94->Add(TextCtrl_Morph_End_X2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BitmapButton_Morph_EndX2 = new wxBitmapButton(MorphPanelEnd, ID_BITMAPBUTTON_SLIDER_Morph_End_X2, padlock16x16_blue_xpm, wxDefaultPosition, wxSize(13,13), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON_SLIDER_Morph_End_X2"));
	BitmapButton_Morph_EndX2->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	FlexGridSizer94->Add(BitmapButton_Morph_EndX2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText121 = new wxStaticText(MorphPanelEnd, ID_STATICTEXT121, _("Y2b:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT121"));
	FlexGridSizer94->Add(StaticText121, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	Slider_Morph_End_Y2 = new wxSlider(MorphPanelEnd, ID_SLIDER_Morph_End_Y2, 100, 0, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_Morph_End_Y2"));
	FlexGridSizer94->Add(Slider_Morph_End_Y2, 1, wxALL|wxEXPAND, 5);
	BitmapButton_Morph_End_Y2 = new ValueCurveButton(MorphPanelEnd, ID_VALUECURVE_Morph_End_Y2, valuecurvenotselected_24, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_VALUECURVE_Morph_End_Y2"));
	FlexGridSizer94->Add(BitmapButton_Morph_End_Y2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	TextCtrl_Morph_End_Y2 = new wxTextCtrl(MorphPanelEnd, IDD_TEXTCTRL_Morph_End_Y2, _("100"), wxDefaultPosition, wxDLG_UNIT(MorphPanelEnd,wxSize(20,-1)), wxTE_CENTRE, wxDefaultValidator, _T("IDD_TEXTCTRL_Morph_End_Y2"));
	TextCtrl_Morph_End_Y2->SetMaxLength(3);
	FlexGridSizer94->Add(TextCtrl_Morph_End_Y2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BitmapButton_Morph_EndY2 = new wxBitmapButton(MorphPanelEnd, ID_BITMAPBUTTON_SLIDER_Morph_End_Y2, padlock16x16_blue_xpm, wxDefaultPosition, wxSize(13,13), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON_SLIDER_Morph_End_Y2"));
	BitmapButton_Morph_EndY2->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	FlexGridSizer94->Add(BitmapButton_Morph_EndY2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText122 = new wxStaticText(MorphPanelEnd, ID_STATICTEXT122, _("Head Length:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT122"));
	FlexGridSizer94->Add(StaticText122, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	Slider_MorphEndLength = new wxSlider(MorphPanelEnd, ID_SLIDER_MorphEndLength, 1, 0, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_MorphEndLength"));
	FlexGridSizer94->Add(Slider_MorphEndLength, 1, wxALL|wxEXPAND, 5);
	BitmapButton_MorphEndLength = new ValueCurveButton(MorphPanelEnd, ID_VALUECURVE_MorphEndLength, valuecurvenotselected_24, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_VALUECURVE_MorphEndLength"));
	FlexGridSizer94->Add(BitmapButton_MorphEndLength, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	TextCtrl_MorphEndLength = new wxTextCtrl(MorphPanelEnd, IDD_TEXTCTRL_MorphEndLength, _("1"), wxDefaultPosition, wxDLG_UNIT(MorphPanelEnd,wxSize(20,-1)), wxTE_CENTRE, wxDefaultValidator, _T("IDD_TEXTCTRL_MorphEndLength"));
	TextCtrl_MorphEndLength->SetMaxLength(3);
	FlexGridSizer94->Add(TextCtrl_MorphEndLength, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BitmapButton_Morph_EndLength = new wxBitmapButton(MorphPanelEnd, ID_BITMAPBUTTON_SLIDER_MorphEndLength, padlock16x16_blue_xpm, wxDefaultPosition, wxSize(13,13), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON_SLIDER_MorphEndLength"));
	BitmapButton_Morph_EndLength->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	FlexGridSizer94->Add(BitmapButton_Morph_EndLength, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer94->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	CheckBox_Morph_End_Link = new wxCheckBox(MorphPanelEnd, ID_CHECKBOX_Morph_End_Link, _("Link Points"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX_Morph_End_Link"));
	CheckBox_Morph_End_Link->SetValue(false);
	FlexGridSizer94->Add(CheckBox_Morph_End_Link, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	MorphPanelEnd->SetSizer(FlexGridSizer94);
	FlexGridSizer94->Fit(MorphPanelEnd);
	FlexGridSizer94->SetSizeHints(MorphPanelEnd);
	MorphPanelOptions = new wxPanel(Notebook_Morph, ID_PANEL30, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL30"));
	FlexGridSizer104 = new wxFlexGridSizer(0, 1, 0, 0);
	FlexGridSizer104->AddGrowableCol(0);
	FlexGridSizer99 = new wxFlexGridSizer(0, 5, 0, 0);
	FlexGridSizer99->AddGrowableCol(1);
	StaticText123 = new wxStaticText(MorphPanelOptions, ID_STATICTEXT123, _("Head Duration:"), wxDefaultPosition, wxSize(-1,-1), 0, _T("ID_STATICTEXT123"));
	FlexGridSizer99->Add(StaticText123, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	Slider_MorphDuration = new wxSlider(MorphPanelOptions, ID_SLIDER_MorphDuration, 20, 0, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_MorphDuration"));
	FlexGridSizer99->Add(Slider_MorphDuration, 1, wxALL|wxEXPAND, 5);
	BitmapButton_MorphDuration = new ValueCurveButton(MorphPanelOptions, ID_VALUECURVE_MorphDuration, valuecurvenotselected_24, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_VALUECURVE_MorphDuration"));
	FlexGridSizer99->Add(BitmapButton_MorphDuration, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	TextCtrl_MorphDuration = new wxTextCtrl(MorphPanelOptions, IDD_TEXTCTRL_MorphDuration, _("20"), wxDefaultPosition, wxDLG_UNIT(MorphPanelOptions,wxSize(20,-1)), wxTE_CENTRE, wxDefaultValidator, _T("IDD_TEXTCTRL_MorphDuration"));
	TextCtrl_MorphDuration->SetMaxLength(3);
	FlexGridSizer99->Add(TextCtrl_MorphDuration, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BitmapButton_Morph_Duration = new wxBitmapButton(MorphPanelOptions, ID_BITMAPBUTTON_SLIDER_MorphDuration, padlock16x16_blue_xpm, wxDefaultPosition, wxSize(13,13), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON_SLIDER_MorphDuration"));
	BitmapButton_Morph_Duration->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	FlexGridSizer99->Add(BitmapButton_Morph_Duration, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText124 = new wxStaticText(MorphPanelOptions, ID_STATICTEXT124, _("Acceleration:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT124"));
	FlexGridSizer99->Add(StaticText124, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	Slider_MorphAccel = new wxSlider(MorphPanelOptions, ID_SLIDER_MorphAccel, 0, -10, 10, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_MorphAccel"));
	FlexGridSizer99->Add(Slider_MorphAccel, 1, wxALL|wxEXPAND, 5);
	BitmapButton_MorphAccel = new ValueCurveButton(MorphPanelOptions, ID_VALUECURVE_MorphAccel, valuecurvenotselected_24, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_VALUECURVE_MorphAccel"));
	FlexGridSizer99->Add(BitmapButton_MorphAccel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	TextCtrl_MorphAccel = new wxTextCtrl(MorphPanelOptions, IDD_TEXTCTRL_MorphAccel, _("0"), wxDefaultPosition, wxDLG_UNIT(MorphPanelOptions,wxSize(20,-1)), wxTE_CENTRE, wxDefaultValidator, _T("IDD_TEXTCTRL_MorphAccel"));
	TextCtrl_MorphAccel->SetMaxLength(3);
	FlexGridSizer99->Add(TextCtrl_MorphAccel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BitmapButton_Morph_Accel = new wxBitmapButton(MorphPanelOptions, ID_BITMAPBUTTON_SLIDER_MorphAccel, padlock16x16_blue_xpm, wxDefaultPosition, wxSize(13,13), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON_SLIDER_MorphAccel"));
	BitmapButton_Morph_Accel->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	FlexGridSizer99->Add(BitmapButton_Morph_Accel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText171 = new wxStaticText(MorphPanelOptions, ID_STATICTEXT97, _("Repeat Count:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT97"));
	FlexGridSizer99->Add(StaticText171, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	Slider_Morph_Repeat_Count = new wxSlider(MorphPanelOptions, ID_SLIDER_Morph_Repeat_Count, 0, 0, 250, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_Morph_Repeat_Count"));
	FlexGridSizer99->Add(Slider_Morph_Repeat_Count, 1, wxALL|wxEXPAND, 5);
	BitmapButton_Morph_Repeat_Count = new ValueCurveButton(MorphPanelOptions, ID_VALUECURVE_Morph_Repeat_Count, valuecurvenotselected_24, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_VALUECURVE_Morph_Repeat_Count"));
	FlexGridSizer99->Add(BitmapButton_Morph_Repeat_Count, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	TextCtrl_Morph_Repeat_Count = new wxTextCtrl(MorphPanelOptions, IDD_TEXTCTRL_Morph_Repeat_Count, _("0"), wxDefaultPosition, wxDLG_UNIT(MorphPanelOptions,wxSize(20,-1)), wxTE_CENTRE, wxDefaultValidator, _T("IDD_TEXTCTRL_Morph_Repeat_Count"));
	TextCtrl_Morph_Repeat_Count->SetMaxLength(3);
	FlexGridSizer99->Add(TextCtrl_Morph_Repeat_Count, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BitmapButton_Morph_RepeatCount = new wxBitmapButton(MorphPanelOptions, ID_BITMAPBUTTON_SLIDER_Morph_Repeat_Count, padlock16x16_blue_xpm, wxDefaultPosition, wxSize(13,13), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON_SLIDER_Morph_Repeat_Count"));
	BitmapButton_Morph_RepeatCount->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	FlexGridSizer99->Add(BitmapButton_Morph_RepeatCount, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText172 = new wxStaticText(MorphPanelOptions, ID_STATICTEXT156, _("Repeat Skip:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT156"));
	FlexGridSizer99->Add(StaticText172, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	Slider_Morph_Repeat_Skip = new wxSlider(MorphPanelOptions, ID_SLIDER_Morph_Repeat_Skip, 1, 1, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_Morph_Repeat_Skip"));
	FlexGridSizer99->Add(Slider_Morph_Repeat_Skip, 1, wxALL|wxEXPAND, 5);
	BitmapButton_Morph_Repeat_Skip = new ValueCurveButton(MorphPanelOptions, ID_VALUECURVE_Morph_Repeat_Skip, valuecurvenotselected_24, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_VALUECURVE_Morph_Repeat_Skip"));
	FlexGridSizer99->Add(BitmapButton_Morph_Repeat_Skip, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	TextCtrl_Morph_Repeat_Skip = new wxTextCtrl(MorphPanelOptions, IDD_TEXTCTRL_Morph_Repeat_Skip, _("1"), wxDefaultPosition, wxDLG_UNIT(MorphPanelOptions,wxSize(20,-1)), wxTE_CENTRE, wxDefaultValidator, _T("IDD_TEXTCTRL_Morph_Repeat_Skip"));
	TextCtrl_Morph_Repeat_Skip->SetMaxLength(3);
	FlexGridSizer99->Add(TextCtrl_Morph_Repeat_Skip, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BitmapButton_Morph_RepeatSkip = new wxBitmapButton(MorphPanelOptions, ID_BITMAPBUTTON_SLIDER_Morph_Repeat_Skip, padlock16x16_blue_xpm, wxDefaultPosition, wxSize(13,13), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON_SLIDER_Morph_Repeat_Skip"));
	BitmapButton_Morph_RepeatSkip->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	FlexGridSizer99->Add(BitmapButton_Morph_RepeatSkip, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText173 = new wxStaticText(MorphPanelOptions, ID_STATICTEXT157, _("Stagger:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT157"));
	FlexGridSizer99->Add(StaticText173, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	Slider_Morph_Stagger = new wxSlider(MorphPanelOptions, ID_SLIDER_Morph_Stagger, 0, -100, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_Morph_Stagger"));
	FlexGridSizer99->Add(Slider_Morph_Stagger, 1, wxALL|wxEXPAND, 5);
	BitmapButton_Morph_Stagger = new ValueCurveButton(MorphPanelOptions, ID_VALUECURVE_Morph_Stagger, valuecurvenotselected_24, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_VALUECURVE_Morph_Stagger"));
	FlexGridSizer99->Add(BitmapButton_Morph_Stagger, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	TextCtrl_Morph_Stagger = new wxTextCtrl(MorphPanelOptions, IDD_TEXTCTRL_Morph_Stagger, _("0"), wxDefaultPosition, wxDLG_UNIT(MorphPanelOptions,wxSize(20,-1)), wxTE_CENTRE, wxDefaultValidator, _T("IDD_TEXTCTRL_Morph_Stagger"));
	TextCtrl_Morph_Stagger->SetMaxLength(3);
	FlexGridSizer99->Add(TextCtrl_Morph_Stagger, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BitmapButton_MorphStagger = new wxBitmapButton(MorphPanelOptions, ID_BITMAPBUTTON_SLIDER_Morph_Stagger, padlock16x16_blue_xpm, wxDefaultPosition, wxSize(13,13), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON_SLIDER_Morph_Stagger"));
	BitmapButton_MorphStagger->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	FlexGridSizer99->Add(BitmapButton_MorphStagger, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer104->Add(FlexGridSizer99, 1, wxALL|wxEXPAND, 1);
	FlexGridSizer105 = new wxFlexGridSizer(0, 2, 0, 0);
	CheckBox_ShowHeadAtStart = new wxCheckBox(MorphPanelOptions, ID_CHECKBOX_ShowHeadAtStart, _("Show Head at Start"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX_ShowHeadAtStart"));
	CheckBox_ShowHeadAtStart->SetValue(false);
	FlexGridSizer105->Add(CheckBox_ShowHeadAtStart, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	BitmapButton_ShowHeadAtStart = new wxBitmapButton(MorphPanelOptions, ID_BITMAPBUTTON_CHECKBOX_ShowHeadAtStart, padlock16x16_blue_xpm, wxDefaultPosition, wxSize(13,13), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON_CHECKBOX_ShowHeadAtStart"));
	BitmapButton_ShowHeadAtStart->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	FlexGridSizer105->Add(BitmapButton_ShowHeadAtStart, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer104->Add(FlexGridSizer105, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	ButtonSwap = new wxButton(MorphPanelOptions, ID_BUTTONSWAP, _("Swap Start and End Points"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONSWAP"));
	FlexGridSizer104->Add(ButtonSwap, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	MorphPanelOptions->SetSizer(FlexGridSizer104);
	FlexGridSizer104->Fit(MorphPanelOptions);
	FlexGridSizer104->SetSizeHints(MorphPanelOptions);
	Notebook_Morph->AddPage(MorphPanelStart, _("Start"), false);
	Notebook_Morph->AddPage(MorphPanelEnd, _("End"), false);
	Notebook_Morph->AddPage(MorphPanelOptions, _("Options"), false);
	FlexGridSizer78->Add(Notebook_Morph, 1, wxALL|wxEXPAND, 5);
	SetSizer(FlexGridSizer78);
	FlexGridSizer78->Fit(this);
	FlexGridSizer78->SetSizeHints(this);

	Connect(ID_SLIDER_Morph_Start_X1,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&MorphPanel::UpdateLinkedTextCtrlVC);
	Connect(ID_VALUECURVE_Morph_Start_X1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MorphPanel::OnVCButtonClick);
	Connect(IDD_TEXTCTRL_Morph_Start_X1,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&MorphPanel::UpdateLinkedSlider);
	Connect(ID_BITMAPBUTTON_SLIDER_Morph_Start_X1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MorphPanel::OnLockButtonClick);
	Connect(ID_SLIDER_Morph_Start_Y1,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&MorphPanel::UpdateLinkedTextCtrlVC);
	Connect(ID_VALUECURVE_Morph_Start_Y1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MorphPanel::OnVCButtonClick);
	Connect(IDD_TEXTCTRL_Morph_Start_Y1,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&MorphPanel::UpdateLinkedSlider);
	Connect(ID_BITMAPBUTTON_SLIDER_Morph_Start_Y1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MorphPanel::OnLockButtonClick);
	Connect(ID_SLIDER_Morph_Start_X2,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&MorphPanel::UpdateLinkedTextCtrlVC);
	Connect(ID_VALUECURVE_Morph_Start_X2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MorphPanel::OnVCButtonClick);
	Connect(IDD_TEXTCTRL_Morph_Start_X2,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&MorphPanel::UpdateLinkedSlider);
	Connect(ID_BITMAPBUTTON_SLIDER_Morph_Start_X2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MorphPanel::OnLockButtonClick);
	Connect(ID_SLIDER_Morph_Start_Y2,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&MorphPanel::UpdateLinkedTextCtrlVC);
	Connect(ID_VALUECURVE_Morph_Start_Y2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MorphPanel::OnVCButtonClick);
	Connect(IDD_TEXTCTRL_Morph_Start_Y2,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&MorphPanel::UpdateLinkedSlider);
	Connect(ID_BITMAPBUTTON_SLIDER_Morph_Start_Y2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MorphPanel::OnLockButtonClick);
	Connect(ID_SLIDER_MorphStartLength,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&MorphPanel::UpdateLinkedTextCtrlVC);
	Connect(ID_VALUECURVE_MorphStartLength,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MorphPanel::OnVCButtonClick);
	Connect(IDD_TEXTCTRL_MorphStartLength,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&MorphPanel::UpdateLinkedSlider);
	Connect(ID_BITMAPBUTTON_SLIDER_MorphStartLength,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MorphPanel::OnLockButtonClick);
	Connect(ID_CHECKBOX_Morph_Start_Link,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MorphPanel::OnCheckBox_Morph_Start_LinkClick);
	Connect(IDD_CHOICE_Morph_QuickSet,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&MorphPanel::OnChoice_Morph_QuickSetSelect);
	Connect(ID_SLIDER_Morph_End_X1,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&MorphPanel::UpdateLinkedTextCtrlVC);
	Connect(ID_VALUECURVE_Morph_End_X1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MorphPanel::OnVCButtonClick);
	Connect(IDD_TEXTCTRL_Morph_End_X1,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&MorphPanel::UpdateLinkedSlider);
	Connect(ID_BITMAPBUTTON_SLIDER_Morph_End_X1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MorphPanel::OnLockButtonClick);
	Connect(ID_SLIDER_Morph_End_Y1,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&MorphPanel::UpdateLinkedTextCtrlVC);
	Connect(ID_VALUECURVE_Morph_End_Y1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MorphPanel::OnVCButtonClick);
	Connect(IDD_TEXTCTRL_Morph_End_Y1,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&MorphPanel::UpdateLinkedSlider);
	Connect(ID_BITMAPBUTTON_SLIDER_Morph_End_Y1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MorphPanel::OnLockButtonClick);
	Connect(ID_SLIDER_Morph_End_X2,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&MorphPanel::UpdateLinkedTextCtrlVC);
	Connect(ID_VALUECURVE_Morph_End_X2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MorphPanel::OnVCButtonClick);
	Connect(IDD_TEXTCTRL_Morph_End_X2,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&MorphPanel::UpdateLinkedSlider);
	Connect(ID_BITMAPBUTTON_SLIDER_Morph_End_X2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MorphPanel::OnLockButtonClick);
	Connect(ID_SLIDER_Morph_End_Y2,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&MorphPanel::UpdateLinkedTextCtrlVC);
	Connect(ID_VALUECURVE_Morph_End_Y2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MorphPanel::OnVCButtonClick);
	Connect(IDD_TEXTCTRL_Morph_End_Y2,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&MorphPanel::UpdateLinkedSlider);
	Connect(ID_BITMAPBUTTON_SLIDER_Morph_End_Y2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MorphPanel::OnLockButtonClick);
	Connect(ID_SLIDER_MorphEndLength,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&MorphPanel::UpdateLinkedTextCtrlVC);
	Connect(ID_VALUECURVE_MorphEndLength,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MorphPanel::OnVCButtonClick);
	Connect(IDD_TEXTCTRL_MorphEndLength,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&MorphPanel::UpdateLinkedSlider);
	Connect(ID_BITMAPBUTTON_SLIDER_MorphEndLength,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MorphPanel::OnLockButtonClick);
	Connect(ID_CHECKBOX_Morph_End_Link,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MorphPanel::OnCheckBox_Morph_End_LinkClick);
	Connect(ID_SLIDER_MorphDuration,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&MorphPanel::UpdateLinkedTextCtrlVC);
	Connect(ID_VALUECURVE_MorphDuration,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MorphPanel::OnVCButtonClick);
	Connect(IDD_TEXTCTRL_MorphDuration,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&MorphPanel::UpdateLinkedSlider);
	Connect(ID_BITMAPBUTTON_SLIDER_MorphDuration,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MorphPanel::OnLockButtonClick);
	Connect(ID_SLIDER_MorphAccel,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&MorphPanel::UpdateLinkedTextCtrlVC);
	Connect(ID_VALUECURVE_MorphAccel,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MorphPanel::OnVCButtonClick);
	Connect(IDD_TEXTCTRL_MorphAccel,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&MorphPanel::UpdateLinkedSlider);
	Connect(ID_BITMAPBUTTON_SLIDER_MorphAccel,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MorphPanel::OnLockButtonClick);
	Connect(ID_SLIDER_Morph_Repeat_Count,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&MorphPanel::UpdateLinkedTextCtrlVC);
	Connect(ID_VALUECURVE_Morph_Repeat_Count,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MorphPanel::OnVCButtonClick);
	Connect(IDD_TEXTCTRL_Morph_Repeat_Count,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&MorphPanel::UpdateLinkedSlider);
	Connect(ID_BITMAPBUTTON_SLIDER_Morph_Repeat_Count,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MorphPanel::OnLockButtonClick);
	Connect(ID_SLIDER_Morph_Repeat_Skip,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&MorphPanel::UpdateLinkedTextCtrlVC);
	Connect(ID_VALUECURVE_Morph_Repeat_Skip,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MorphPanel::OnVCButtonClick);
	Connect(IDD_TEXTCTRL_Morph_Repeat_Skip,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&MorphPanel::UpdateLinkedSlider);
	Connect(ID_BITMAPBUTTON_SLIDER_Morph_Repeat_Skip,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MorphPanel::OnLockButtonClick);
	Connect(ID_SLIDER_Morph_Stagger,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&MorphPanel::UpdateLinkedTextCtrlVC);
	Connect(ID_VALUECURVE_Morph_Stagger,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MorphPanel::OnVCButtonClick);
	Connect(IDD_TEXTCTRL_Morph_Stagger,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&MorphPanel::UpdateLinkedSlider);
	Connect(ID_BITMAPBUTTON_SLIDER_Morph_Stagger,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MorphPanel::OnLockButtonClick);
	Connect(ID_BITMAPBUTTON_CHECKBOX_ShowHeadAtStart,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MorphPanel::OnLockButtonClick);
	Connect(ID_BUTTONSWAP,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MorphPanel::OnButtonSwapClick);
	//*)
    SetName("ID_PANEL_MORPH");

    Connect(wxID_ANY, EVT_VC_CHANGED, (wxObjectEventFunction)&MorphPanel::OnVCChanged, nullptr, this);

    BitmapButton_Morph_Start_X1->GetValue()->SetLimits(MORPH_X_MIN, MORPH_X_MAX);
    BitmapButton_Morph_End_X1->GetValue()->SetLimits(MORPH_X_MIN, MORPH_X_MAX);
    BitmapButton_Morph_Start_Y1->GetValue()->SetLimits(MORPH_Y_MIN, MORPH_Y_MAX);
    BitmapButton_Morph_End_Y1->GetValue()->SetLimits(MORPH_Y_MIN, MORPH_Y_MAX);
    BitmapButton_Morph_Start_X2->GetValue()->SetLimits(MORPH_X_MIN, MORPH_X_MAX);
    BitmapButton_Morph_End_X2->GetValue()->SetLimits(MORPH_X_MIN, MORPH_X_MAX);
    BitmapButton_Morph_Start_Y2->GetValue()->SetLimits(MORPH_Y_MIN, MORPH_Y_MAX);
    BitmapButton_Morph_End_Y2->GetValue()->SetLimits(MORPH_Y_MIN, MORPH_Y_MAX);
    BitmapButton_MorphStartLength->GetValue()->SetLimits(MORPH_STARTLENGTH_MIN, MORPH_STARTLENGTH_MAX);
    BitmapButton_MorphEndLength->GetValue()->SetLimits(MORPH_ENDLENGTH_MIN, MORPH_ENDLENGTH_MAX);
    BitmapButton_MorphDuration->GetValue()->SetLimits(MORPH_DURATION_MIN, MORPH_DURATION_MAX);
    BitmapButton_MorphAccel->GetValue()->SetLimits(MORPH_ACCEL_MIN, MORPH_ACCEL_MAX);
    BitmapButton_Morph_Repeat_Count->GetValue()->SetLimits(MORPH_REPEAT_MIN, MORPH_REPEAT_MAX);
    BitmapButton_Morph_Repeat_Skip->GetValue()->SetLimits(MORPH_REPEATSKIP_MIN, MORPH_REPEATSKIP_MAX);
    BitmapButton_Morph_Stagger->GetValue()->SetLimits(MORPH_STAGGER_MIN, MORPH_STAGGER_MAX);
}

MorphPanel::~MorphPanel()
{
	//(*Destroy(MorphPanel)
	//*)
}

PANEL_EVENT_HANDLERS(MorphPanel)


void MorphPanel::OnCheckBox_Morph_Start_LinkClick(wxCommandEvent& event)
{
    wxCheckBox* control=(wxCheckBox*)event.GetEventObject();
    EffectPanelUtils::enableControlsByName(this, "ID_SLIDER_Morph_Start_X2", !control->IsChecked());
    EffectPanelUtils::enableControlsByName(this, "ID_SLIDER_Morph_Start_Y2", !control->IsChecked());
    EffectPanelUtils::enableControlsByName(this, "ID_VALUECURVE_Morph_Start_X2", !control->IsChecked());
    EffectPanelUtils::enableControlsByName(this, "ID_VALUECURVE_Morph_Start_Y2", !control->IsChecked());
}

void MorphPanel::OnCheckBox_Morph_End_LinkClick(wxCommandEvent& event)
{
    wxCheckBox* control=(wxCheckBox*)event.GetEventObject();
    EffectPanelUtils::enableControlsByName(this, "ID_SLIDER_Morph_End_X2", !control->IsChecked());
    EffectPanelUtils::enableControlsByName(this, "ID_SLIDER_Morph_End_Y2", !control->IsChecked());
    EffectPanelUtils::enableControlsByName(this, "ID_VALUECURVE_Morph_End_X2", !control->IsChecked());
    EffectPanelUtils::enableControlsByName(this, "ID_VALUECURVE_Morph_End_Y2", !control->IsChecked());
}


void MorphPanel::OnChoice_Morph_QuickSetSelect(wxCommandEvent& event)
{
    wxString quickset_choice = Choice_Morph_QuickSet->GetStringSelection();
    if( quickset_choice == "Full Sweep Up" )
    {
        TextCtrl_Morph_Start_X1->SetValue("0");
        TextCtrl_Morph_Start_X2->SetValue("100");
        TextCtrl_Morph_End_X1->SetValue("0");
        TextCtrl_Morph_End_X2->SetValue("100");
        TextCtrl_Morph_Start_Y1->SetValue("0");
        TextCtrl_Morph_Start_Y2->SetValue("0");
        TextCtrl_Morph_End_Y1->SetValue("100");
        TextCtrl_Morph_End_Y2->SetValue("100");
    }
    else if( quickset_choice == "Full Sweep Down" )
    {
        TextCtrl_Morph_Start_X1->SetValue("0");
        TextCtrl_Morph_Start_X2->SetValue("100");
        TextCtrl_Morph_End_X1->SetValue("0");
        TextCtrl_Morph_End_X2->SetValue("100");
        TextCtrl_Morph_Start_Y1->SetValue("100");
        TextCtrl_Morph_Start_Y2->SetValue("100");
        TextCtrl_Morph_End_Y1->SetValue("0");
        TextCtrl_Morph_End_Y2->SetValue("0");
    }
    else if( quickset_choice == "Full Sweep Left" )
    {
        TextCtrl_Morph_Start_X1->SetValue("100");
        TextCtrl_Morph_Start_X2->SetValue("100");
        TextCtrl_Morph_End_X1->SetValue("0");
        TextCtrl_Morph_End_X2->SetValue("0");
        TextCtrl_Morph_Start_Y1->SetValue("0");
        TextCtrl_Morph_Start_Y2->SetValue("100");
        TextCtrl_Morph_End_Y1->SetValue("0");
        TextCtrl_Morph_End_Y2->SetValue("100");
    }
    else if( quickset_choice == "Full Sweep Right" )
    {
        TextCtrl_Morph_Start_X1->SetValue("0");
        TextCtrl_Morph_Start_X2->SetValue("0");
        TextCtrl_Morph_End_X1->SetValue("100");
        TextCtrl_Morph_End_X2->SetValue("100");
        TextCtrl_Morph_Start_Y1->SetValue("0");
        TextCtrl_Morph_Start_Y2->SetValue("100");
        TextCtrl_Morph_End_Y1->SetValue("0");
        TextCtrl_Morph_End_Y2->SetValue("100");
    }
    else if( quickset_choice == "Single Sweep Up" )
    {
        TextCtrl_Morph_Start_X1->SetValue("0");
        TextCtrl_Morph_Start_X2->SetValue("0");
        TextCtrl_Morph_End_X1->SetValue("0");
        TextCtrl_Morph_End_X2->SetValue("0");
        TextCtrl_Morph_Start_Y1->SetValue("0");
        TextCtrl_Morph_Start_Y2->SetValue("0");
        TextCtrl_Morph_End_Y1->SetValue("100");
        TextCtrl_Morph_End_Y2->SetValue("100");
    }
    else if( quickset_choice == "Single Sweep Down" )
    {
        TextCtrl_Morph_Start_X1->SetValue("0");
        TextCtrl_Morph_Start_X2->SetValue("0");
        TextCtrl_Morph_End_X1->SetValue("0");
        TextCtrl_Morph_End_X2->SetValue("0");
        TextCtrl_Morph_Start_Y1->SetValue("100");
        TextCtrl_Morph_Start_Y2->SetValue("100");
        TextCtrl_Morph_End_Y1->SetValue("0");
        TextCtrl_Morph_End_Y2->SetValue("0");
    }
    else if( quickset_choice == "Single Sweep Left" )
    {
        TextCtrl_Morph_Start_X1->SetValue("100");
        TextCtrl_Morph_Start_X2->SetValue("100");
        TextCtrl_Morph_End_X1->SetValue("0");
        TextCtrl_Morph_End_X2->SetValue("0");
        TextCtrl_Morph_Start_Y1->SetValue("0");
        TextCtrl_Morph_Start_Y2->SetValue("0");
        TextCtrl_Morph_End_Y1->SetValue("0");
        TextCtrl_Morph_End_Y2->SetValue("0");
    }
    else if( quickset_choice == "Single Sweep Right" )
    {
        TextCtrl_Morph_Start_X1->SetValue("0");
        TextCtrl_Morph_Start_X2->SetValue("0");
        TextCtrl_Morph_End_X1->SetValue("100");
        TextCtrl_Morph_End_X2->SetValue("100");
        TextCtrl_Morph_Start_Y1->SetValue("0");
        TextCtrl_Morph_Start_Y2->SetValue("0");
        TextCtrl_Morph_End_Y1->SetValue("0");
        TextCtrl_Morph_End_Y2->SetValue("0");
    }
    Choice_Morph_QuickSet->SetSelection(0);
}

void MorphPanel::OnButtonSwapClick(wxCommandEvent& event)
{
	auto tempValue = TextCtrl_Morph_Start_X1->GetValue();
	TextCtrl_Morph_Start_X1->SetValue(TextCtrl_Morph_End_X1->GetValue());
	TextCtrl_Morph_End_X1->SetValue(tempValue);

	tempValue = TextCtrl_Morph_Start_X2->GetValue();
	TextCtrl_Morph_Start_X2->SetValue(TextCtrl_Morph_End_X2->GetValue());
	TextCtrl_Morph_End_X2->SetValue(tempValue);

	tempValue = TextCtrl_Morph_Start_Y1->GetValue();
	TextCtrl_Morph_Start_Y1->SetValue(TextCtrl_Morph_End_Y1->GetValue());
	TextCtrl_Morph_End_Y1->SetValue(tempValue);

	tempValue = TextCtrl_Morph_Start_Y2->GetValue();
	TextCtrl_Morph_Start_Y2->SetValue(TextCtrl_Morph_End_Y2->GetValue());
	TextCtrl_Morph_End_Y2->SetValue(tempValue);
}
