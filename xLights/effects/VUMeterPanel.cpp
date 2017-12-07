#include "VUMeterPanel.h"
#include "../../include/padlock16x16-blue.xpm"
#include "EffectPanelUtils.h"
#include "../xLightsMain.h"
#include "VUMeterEffect.h"

//(*InternalHeaders(VUMeterPanel)
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
#include <wx/image.h>
#include <wx/string.h>
//*)

//(*IdInit(VUMeterPanel)
const long VUMeterPanel::ID_STATICTEXT1 = wxNewId();
const long VUMeterPanel::ID_SLIDER_VUMeter_Bars = wxNewId();
const long VUMeterPanel::IDD_TEXTCTRL_VUMeter_Bars = wxNewId();
const long VUMeterPanel::ID_BITMAPBUTTON_CHOICE_VUMeter_Bars = wxNewId();
const long VUMeterPanel::ID_STATICTEXT2 = wxNewId();
const long VUMeterPanel::ID_CHOICE_VUMeter_Type = wxNewId();
const long VUMeterPanel::ID_STATICTEXT5 = wxNewId();
const long VUMeterPanel::ID_BITMAPBUTTON_CHOICE_VUMeter_Type = wxNewId();
const long VUMeterPanel::ID_STATICTEXT3 = wxNewId();
const long VUMeterPanel::ID_CHOICE_VUMeter_TimingTrack = wxNewId();
const long VUMeterPanel::ID_STATICTEXT4 = wxNewId();
const long VUMeterPanel::ID_BITMAPBUTTON_CHOICE_VUMeter_TimingTrack = wxNewId();
const long VUMeterPanel::ID_STATICTEXT6 = wxNewId();
const long VUMeterPanel::ID_SLIDER_VUMeter_Sensitivity = wxNewId();
const long VUMeterPanel::IDD_TEXTCTRL_VUMeter_Sensitivity = wxNewId();
const long VUMeterPanel::ID_BITMAPBUTTON_SLIDER_VUMeter_Sensitivity = wxNewId();
const long VUMeterPanel::ID_STATICTEXT7 = wxNewId();
const long VUMeterPanel::ID_CHOICE_VUMeter_Shape = wxNewId();
const long VUMeterPanel::ID_STATICTEXT8 = wxNewId();
const long VUMeterPanel::ID_BITMAPBUTTON_CHOICE_VUMeter_Shape = wxNewId();
const long VUMeterPanel::ID_STATICTEXT9 = wxNewId();
const long VUMeterPanel::ID_CHECKBOX_VUMeter_SlowDownFalls = wxNewId();
const long VUMeterPanel::ID_STATICTEXT10 = wxNewId();
const long VUMeterPanel::ID_BITMAPBUTTON_CHECKBOX_VUMeter_SlowDownFalls = wxNewId();
const long VUMeterPanel::ID_STATICTEXT12 = wxNewId();
const long VUMeterPanel::ID_SLIDER_VUMeter_StartNote = wxNewId();
const long VUMeterPanel::IDD_TEXTCTRL_VUMeter_StartNote = wxNewId();
const long VUMeterPanel::ID_BITMAPBUTTON_VUMeter_StartNote = wxNewId();
const long VUMeterPanel::ID_STATICTEXT11 = wxNewId();
const long VUMeterPanel::ID_SLIDER_VUMeter_EndNote = wxNewId();
const long VUMeterPanel::IDD_TEXTCTRL_VUMeter_EndNote = wxNewId();
const long VUMeterPanel::ID_BITMAPBUTTON_VUMeter_EndNote = wxNewId();
const long VUMeterPanel::ID_STATICTEXT13 = wxNewId();
const long VUMeterPanel::ID_SLIDER_VUMeter_XOffset = wxNewId();
const long VUMeterPanel::IDD_TEXTCTRL_VUMeter_XOffset = wxNewId();
const long VUMeterPanel::ID_BITMAPBUTTON_VUMeter_XOffset = wxNewId();
const long VUMeterPanel::ID_STATICTEXT14 = wxNewId();
const long VUMeterPanel::ID_SLIDER_VUMeter_YOffset = wxNewId();
const long VUMeterPanel::ID_VALUECURVE_VUMeter_YOffset = wxNewId();
const long VUMeterPanel::IDD_TEXTCTRL_VUMeter_YOffset = wxNewId();
const long VUMeterPanel::ID_BITMAPBUTTON_VUMeter_YOffset = wxNewId();
//*)

BEGIN_EVENT_TABLE(VUMeterPanel,wxPanel)
	//(*EventTable(VUMeterPanel)
	//*)
END_EVENT_TABLE()

VUMeterPanel::VUMeterPanel(wxWindow* parent)
{
	//(*Initialize(VUMeterPanel)
	wxFlexGridSizer* FlexGridSizer4;
	wxFlexGridSizer* FlexGridSizer3;
	wxFlexGridSizer* FlexGridSizer2;
	wxFlexGridSizer* FlexGridSizer1;
	wxFlexGridSizer* FlexGridSizer42;
	wxFlexGridSizer* FlexGridSizer31;

	Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	FlexGridSizer42 = new wxFlexGridSizer(0, 1, 0, 0);
	FlexGridSizer42->AddGrowableCol(0);
	FlexGridSizer31 = new wxFlexGridSizer(0, 4, 0, 0);
	FlexGridSizer31->AddGrowableCol(1);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Bars"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	FlexGridSizer31->Add(StaticText1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	FlexGridSizer4 = new wxFlexGridSizer(0, 2, 0, 0);
	FlexGridSizer4->AddGrowableCol(0);
	Slider_VUMeter_Bars = new wxSlider(this, ID_SLIDER_VUMeter_Bars, 20, 1, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_VUMeter_Bars"));
	FlexGridSizer4->Add(Slider_VUMeter_Bars, 1, wxALL|wxEXPAND, 2);
	FlexGridSizer31->Add(FlexGridSizer4, 1, wxALL|wxEXPAND, 2);
	TextCtrl_VUMeter_Bars = new wxTextCtrl(this, IDD_TEXTCTRL_VUMeter_Bars, _("20"), wxDefaultPosition, wxDLG_UNIT(this,wxSize(20,-1)), 0, wxDefaultValidator, _T("IDD_TEXTCTRL_VUMeter_Bars"));
	FlexGridSizer31->Add(TextCtrl_VUMeter_Bars, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BitmapButton_VUMeter_Bars = new wxBitmapButton(this, ID_BITMAPBUTTON_CHOICE_VUMeter_Bars, wxNullBitmap, wxDefaultPosition, wxSize(13,13), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON_CHOICE_VUMeter_Bars"));
	BitmapButton_VUMeter_Bars->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	FlexGridSizer31->Add(BitmapButton_VUMeter_Bars, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Type"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	FlexGridSizer31->Add(StaticText2, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	Choice_VUMeter_Type = new wxChoice(this, ID_CHOICE_VUMeter_Type, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE_VUMeter_Type"));
	Choice_VUMeter_Type->Append(_("Spectrogram"));
	Choice_VUMeter_Type->Append(_("Volume Bars"));
	Choice_VUMeter_Type->SetSelection( Choice_VUMeter_Type->Append(_("Waveform")) );
	Choice_VUMeter_Type->Append(_("On"));
	Choice_VUMeter_Type->Append(_("Color On"));
	Choice_VUMeter_Type->Append(_("Intensity Wave"));
	Choice_VUMeter_Type->Append(_("Pulse"));
	Choice_VUMeter_Type->Append(_("Level Bar"));
	Choice_VUMeter_Type->Append(_("Level Pulse"));
	Choice_VUMeter_Type->Append(_("Level Pulse Color"));
	Choice_VUMeter_Type->Append(_("Level Shape"));
	Choice_VUMeter_Type->Append(_("Timing Event Bar"));
	Choice_VUMeter_Type->Append(_("Timing Event Bars"));
	Choice_VUMeter_Type->Append(_("Timing Event Spike"));
	Choice_VUMeter_Type->Append(_("Timing Event Sweep"));
	Choice_VUMeter_Type->Append(_("Timing Event Color"));
	Choice_VUMeter_Type->Append(_("Timing Event Jump"));
	Choice_VUMeter_Type->Append(_("Timing Event Jump 100"));
	Choice_VUMeter_Type->Append(_("Timing Event Pulse"));
	Choice_VUMeter_Type->Append(_("Note On"));
	Choice_VUMeter_Type->Append(_("Note Level Pulse"));
	Choice_VUMeter_Type->Append(_("Note Level Bar"));
	FlexGridSizer31->Add(Choice_VUMeter_Type, 1, wxALL|wxEXPAND, 2);
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	FlexGridSizer31->Add(StaticText5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BitmapButton_VUMeter_Type = new wxBitmapButton(this, ID_BITMAPBUTTON_CHOICE_VUMeter_Type, wxNullBitmap, wxDefaultPosition, wxSize(13,13), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON_CHOICE_VUMeter_Type"));
	BitmapButton_VUMeter_Type->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	FlexGridSizer31->Add(BitmapButton_VUMeter_Type, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Timing Track"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	FlexGridSizer31->Add(StaticText3, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	Choice_VUMeter_TimingTrack = new wxChoice(this, ID_CHOICE_VUMeter_TimingTrack, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE_VUMeter_TimingTrack"));
	Choice_VUMeter_TimingTrack->Disable();
	FlexGridSizer31->Add(Choice_VUMeter_TimingTrack, 1, wxALL|wxEXPAND, 2);
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	FlexGridSizer31->Add(StaticText4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BitmapButton_VUMeter_TimingTrack = new wxBitmapButton(this, ID_BITMAPBUTTON_CHOICE_VUMeter_TimingTrack, wxNullBitmap, wxDefaultPosition, wxSize(13,13), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON_CHOICE_VUMeter_TimingTrack"));
	BitmapButton_VUMeter_TimingTrack->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	FlexGridSizer31->Add(BitmapButton_VUMeter_TimingTrack, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _("Sensitivity"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	FlexGridSizer31->Add(StaticText6, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	FlexGridSizer3 = new wxFlexGridSizer(2, 3, 0, 0);
	FlexGridSizer3->AddGrowableCol(0);
	Slider_VUMeter_Sensitivity = new wxSlider(this, ID_SLIDER_VUMeter_Sensitivity, 70, 0, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_VUMeter_Sensitivity"));
	FlexGridSizer3->Add(Slider_VUMeter_Sensitivity, 1, wxALL|wxEXPAND, 2);
	FlexGridSizer31->Add(FlexGridSizer3, 1, wxALL|wxEXPAND, 2);
	TextCtrl_VUMeter_Sensitivity = new wxTextCtrl(this, IDD_TEXTCTRL_VUMeter_Sensitivity, _("70"), wxDefaultPosition, wxDLG_UNIT(this,wxSize(20,-1)), 0, wxDefaultValidator, _T("IDD_TEXTCTRL_VUMeter_Sensitivity"));
	FlexGridSizer31->Add(TextCtrl_VUMeter_Sensitivity, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BitmapButton_VUMeter_Sensitivity = new wxBitmapButton(this, ID_BITMAPBUTTON_SLIDER_VUMeter_Sensitivity, wxNullBitmap, wxDefaultPosition, wxSize(13,13), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON_SLIDER_VUMeter_Sensitivity"));
	BitmapButton_VUMeter_Sensitivity->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	FlexGridSizer31->Add(BitmapButton_VUMeter_Sensitivity, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _("Shape"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	FlexGridSizer31->Add(StaticText7, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	Choice_VUMeter_Shape = new wxChoice(this, ID_CHOICE_VUMeter_Shape, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE_VUMeter_Shape"));
	Choice_VUMeter_Shape->SetSelection( Choice_VUMeter_Shape->Append(_("Circle")) );
	Choice_VUMeter_Shape->Append(_("Filled Circle"));
	Choice_VUMeter_Shape->Append(_("Square"));
	Choice_VUMeter_Shape->Append(_("Filled Square"));
	Choice_VUMeter_Shape->Append(_("Diamond"));
	Choice_VUMeter_Shape->Append(_("Filled Diamond"));
	Choice_VUMeter_Shape->Append(_("Star"));
	Choice_VUMeter_Shape->Append(_("Filled Star"));
	Choice_VUMeter_Shape->Append(_("Tree"));
	Choice_VUMeter_Shape->Append(_("Filled Tree"));
	Choice_VUMeter_Shape->Append(_("Crucifix"));
	Choice_VUMeter_Shape->Append(_("Filled Crucifix"));
	Choice_VUMeter_Shape->Append(_("Present"));
	Choice_VUMeter_Shape->Append(_("Filled Present"));
	Choice_VUMeter_Shape->Append(_("Candy Cane"));
	Choice_VUMeter_Shape->Append(_("Snowflake"));
	Choice_VUMeter_Shape->Append(_("Heart"));
	Choice_VUMeter_Shape->Append(_("Filled Heart"));
	FlexGridSizer31->Add(Choice_VUMeter_Shape, 1, wxALL|wxEXPAND, 2);
	StaticText8 = new wxStaticText(this, ID_STATICTEXT8, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	FlexGridSizer31->Add(StaticText8, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BitmapButton_VUMeter_Shape = new wxBitmapButton(this, ID_BITMAPBUTTON_CHOICE_VUMeter_Shape, wxNullBitmap, wxDefaultPosition, wxSize(13,13), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON_CHOICE_VUMeter_Shape"));
	BitmapButton_VUMeter_Shape->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	FlexGridSizer31->Add(BitmapButton_VUMeter_Shape, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticText9 = new wxStaticText(this, ID_STATICTEXT9, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT9"));
	FlexGridSizer31->Add(StaticText9, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	CheckBox_VUMeter_SlowDownFalls = new wxCheckBox(this, ID_CHECKBOX_VUMeter_SlowDownFalls, _("Slow Down Falls"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX_VUMeter_SlowDownFalls"));
	CheckBox_VUMeter_SlowDownFalls->SetValue(true);
	FlexGridSizer31->Add(CheckBox_VUMeter_SlowDownFalls, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	StaticText10 = new wxStaticText(this, ID_STATICTEXT10, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT10"));
	FlexGridSizer31->Add(StaticText10, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BitmapButton_VUMeter_SlowDownFalls = new wxBitmapButton(this, ID_BITMAPBUTTON_CHECKBOX_VUMeter_SlowDownFalls, wxNullBitmap, wxDefaultPosition, wxSize(13,13), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON_CHECKBOX_VUMeter_SlowDownFalls"));
	BitmapButton_VUMeter_SlowDownFalls->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	FlexGridSizer31->Add(BitmapButton_VUMeter_SlowDownFalls, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText12 = new wxStaticText(this, ID_STATICTEXT12, _("Start Note"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT12"));
	FlexGridSizer31->Add(StaticText12, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	Slider_VUMeter_StartNote = new wxSlider(this, ID_SLIDER_VUMeter_StartNote, 36, 0, 127, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_VUMeter_StartNote"));
	FlexGridSizer31->Add(Slider_VUMeter_StartNote, 1, wxALL|wxEXPAND, 2);
	TextCtrl_VUMeter_StartNote = new wxTextCtrl(this, IDD_TEXTCTRL_VUMeter_StartNote, _("36"), wxDefaultPosition, wxDLG_UNIT(this,wxSize(20,-1)), 0, wxDefaultValidator, _T("IDD_TEXTCTRL_VUMeter_StartNote"));
	FlexGridSizer31->Add(TextCtrl_VUMeter_StartNote, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BitmapButton_VUMeter_StartNote = new wxBitmapButton(this, ID_BITMAPBUTTON_VUMeter_StartNote, wxNullBitmap, wxDefaultPosition, wxSize(13,13), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON_VUMeter_StartNote"));
	BitmapButton_VUMeter_StartNote->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	FlexGridSizer31->Add(BitmapButton_VUMeter_StartNote, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText11 = new wxStaticText(this, ID_STATICTEXT11, _("End Note"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT11"));
	FlexGridSizer31->Add(StaticText11, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	Slider_VUMeter_EndNote = new wxSlider(this, ID_SLIDER_VUMeter_EndNote, 84, 0, 127, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_VUMeter_EndNote"));
	FlexGridSizer31->Add(Slider_VUMeter_EndNote, 1, wxALL|wxEXPAND, 2);
	TextCtrl_VUMeter_EndNote = new wxTextCtrl(this, IDD_TEXTCTRL_VUMeter_EndNote, _("84"), wxDefaultPosition, wxDLG_UNIT(this,wxSize(20,-1)), 0, wxDefaultValidator, _T("IDD_TEXTCTRL_VUMeter_EndNote"));
	FlexGridSizer31->Add(TextCtrl_VUMeter_EndNote, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BitmapButton_VUMeter_EndNote = new wxBitmapButton(this, ID_BITMAPBUTTON_VUMeter_EndNote, wxNullBitmap, wxDefaultPosition, wxSize(13,13), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON_VUMeter_EndNote"));
	BitmapButton_VUMeter_EndNote->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	FlexGridSizer31->Add(BitmapButton_VUMeter_EndNote, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText13 = new wxStaticText(this, ID_STATICTEXT13, _("Horizontal Offset"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT13"));
	FlexGridSizer31->Add(StaticText13, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	FlexGridSizer2 = new wxFlexGridSizer(0, 2, 0, 0);
	FlexGridSizer2->AddGrowableCol(0);
	Slider_VUMeter_XOffset = new wxSlider(this, ID_SLIDER_VUMeter_XOffset, 0, -100, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_VUMeter_XOffset"));
	FlexGridSizer2->Add(Slider_VUMeter_XOffset, 1, wxALL|wxEXPAND, 2);
	FlexGridSizer31->Add(FlexGridSizer2, 1, wxALL|wxEXPAND, 2);
	TextCtrl_VUMeter_XOffset = new wxTextCtrl(this, IDD_TEXTCTRL_VUMeter_XOffset, _("0"), wxDefaultPosition, wxDLG_UNIT(this,wxSize(20,-1)), 0, wxDefaultValidator, _T("IDD_TEXTCTRL_VUMeter_XOffset"));
	FlexGridSizer31->Add(TextCtrl_VUMeter_XOffset, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BitmapButton_VUMeter_XOffset = new wxBitmapButton(this, ID_BITMAPBUTTON_VUMeter_XOffset, wxNullBitmap, wxDefaultPosition, wxSize(13,13), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON_VUMeter_XOffset"));
	BitmapButton_VUMeter_XOffset->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	FlexGridSizer31->Add(BitmapButton_VUMeter_XOffset, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticText14 = new wxStaticText(this, ID_STATICTEXT14, _("Vertical Offset"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT14"));
	FlexGridSizer31->Add(StaticText14, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	FlexGridSizer1 = new wxFlexGridSizer(0, 2, 0, 0);
	FlexGridSizer1->AddGrowableCol(0);
	Slider_VUMeter_YOffset = new wxSlider(this, ID_SLIDER_VUMeter_YOffset, 0, -100, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_VUMeter_YOffset"));
	FlexGridSizer1->Add(Slider_VUMeter_YOffset, 1, wxALL|wxEXPAND, 2);
	BitmapButton_VUMeter_YOffsetVC = new ValueCurveButton(this, ID_VALUECURVE_VUMeter_YOffset, valuecurvenotselected_24, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_VALUECURVE_VUMeter_YOffset"));
	FlexGridSizer1->Add(BitmapButton_VUMeter_YOffsetVC, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	FlexGridSizer31->Add(FlexGridSizer1, 1, wxALL|wxEXPAND, 2);
	TextCtrl_VUMeter_YOffset = new wxTextCtrl(this, IDD_TEXTCTRL_VUMeter_YOffset, _("0"), wxDefaultPosition, wxDLG_UNIT(this,wxSize(20,-1)), 0, wxDefaultValidator, _T("IDD_TEXTCTRL_VUMeter_YOffset"));
	FlexGridSizer31->Add(TextCtrl_VUMeter_YOffset, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BitmapButton_VUMeter_YOffset = new wxBitmapButton(this, ID_BITMAPBUTTON_VUMeter_YOffset, wxNullBitmap, wxDefaultPosition, wxSize(13,13), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON_VUMeter_YOffset"));
	BitmapButton_VUMeter_YOffset->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	FlexGridSizer31->Add(BitmapButton_VUMeter_YOffset, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	FlexGridSizer42->Add(FlexGridSizer31, 1, wxEXPAND, 2);
	SetSizer(FlexGridSizer42);
	FlexGridSizer42->Fit(this);
	FlexGridSizer42->SetSizeHints(this);

	Connect(ID_SLIDER_VUMeter_Bars,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&VUMeterPanel::UpdateLinkedTextCtrl);
	Connect(IDD_TEXTCTRL_VUMeter_Bars,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&VUMeterPanel::UpdateLinkedSlider);
	Connect(ID_BITMAPBUTTON_CHOICE_VUMeter_Bars,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&VUMeterPanel::OnLockButtonClick);
	Connect(ID_CHOICE_VUMeter_Type,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&VUMeterPanel::OnChoice_VUMeter_TypeSelect);
	Connect(ID_BITMAPBUTTON_CHOICE_VUMeter_Type,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&VUMeterPanel::OnLockButtonClick);
	Connect(ID_BITMAPBUTTON_CHOICE_VUMeter_TimingTrack,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&VUMeterPanel::OnLockButtonClick);
	Connect(ID_SLIDER_VUMeter_Sensitivity,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&VUMeterPanel::UpdateLinkedTextCtrl);
	Connect(IDD_TEXTCTRL_VUMeter_Sensitivity,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&VUMeterPanel::UpdateLinkedSlider);
	Connect(ID_BITMAPBUTTON_SLIDER_VUMeter_Sensitivity,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&VUMeterPanel::OnLockButtonClick);
	Connect(ID_CHOICE_VUMeter_Shape,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&VUMeterPanel::OnChoice_VUMeter_TypeSelect);
	Connect(ID_BITMAPBUTTON_CHOICE_VUMeter_Shape,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&VUMeterPanel::OnLockButtonClick);
	Connect(ID_BITMAPBUTTON_CHECKBOX_VUMeter_SlowDownFalls,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&VUMeterPanel::OnLockButtonClick);
	Connect(ID_SLIDER_VUMeter_StartNote,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&VUMeterPanel::OnSlider_VUMeter_StartNoteCmdSliderUpdated);
	Connect(IDD_TEXTCTRL_VUMeter_StartNote,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&VUMeterPanel::OnTextCtrl_VUMeter_StartNoteText);
	Connect(ID_BITMAPBUTTON_VUMeter_StartNote,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&VUMeterPanel::OnLockButtonClick);
	Connect(ID_SLIDER_VUMeter_EndNote,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&VUMeterPanel::OnSlider_VUMeter_StartNoteCmdSliderUpdated);
	Connect(IDD_TEXTCTRL_VUMeter_EndNote,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&VUMeterPanel::OnTextCtrl_VUMeter_StartNoteText);
	Connect(ID_BITMAPBUTTON_VUMeter_EndNote,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&VUMeterPanel::OnLockButtonClick);
	Connect(ID_SLIDER_VUMeter_XOffset,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&VUMeterPanel::UpdateLinkedTextCtrl);
	Connect(IDD_TEXTCTRL_VUMeter_XOffset,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&VUMeterPanel::UpdateLinkedSlider);
	Connect(ID_BITMAPBUTTON_VUMeter_XOffset,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&VUMeterPanel::OnLockButtonClick);
	Connect(ID_SLIDER_VUMeter_YOffset,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&VUMeterPanel::UpdateLinkedTextCtrlVC);
	Connect(ID_VALUECURVE_VUMeter_YOffset,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&VUMeterPanel::OnVCButtonClick);
	Connect(IDD_TEXTCTRL_VUMeter_YOffset,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&VUMeterPanel::UpdateLinkedSlider);
	Connect(ID_BITMAPBUTTON_VUMeter_YOffset,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&VUMeterPanel::OnLockButtonClick);
	//*)
    SetName("ID_PANEL_VUMeter");

    Connect(wxID_ANY, EVT_VC_CHANGED, (wxObjectEventFunction)&VUMeterPanel::OnVCChanged, 0, this);

    BitmapButton_VUMeter_YOffsetVC->GetValue()->SetLimits(VUMETER_OFFSET_MIN, VUMETER_OFFSET_MAX);

	ValidateWindow();
}

VUMeterPanel::~VUMeterPanel()
{
	//(*Destroy(VUMeterPanel)
	//*)
}

PANEL_EVENT_HANDLERS(VUMeterPanel)

void VUMeterPanel::ValidateWindow()
{
    if (Choice_VUMeter_Type->GetStringSelection() == "Timing Event Spike" ||
        Choice_VUMeter_Type->GetStringSelection() == "Timing Event Sweep" ||
        Choice_VUMeter_Type->GetStringSelection() == "Pulse" ||
        Choice_VUMeter_Type->GetStringSelection() == "Timing Event Color" ||
        Choice_VUMeter_Type->GetStringSelection() == "Timing Event Pulse" ||
        Choice_VUMeter_Type->GetStringSelection() == "Timing Event Bar" ||
        Choice_VUMeter_Type->GetStringSelection() == "Timing Event Bars" ||
        Choice_VUMeter_Type->GetStringSelection() == "Timing Event Jump 100" ||
        Choice_VUMeter_Type->GetStringSelection() == "Timing Event Jump")
    {
        Choice_VUMeter_TimingTrack->Enable();
    }
    else
    {
        Choice_VUMeter_TimingTrack->Disable();
    }

    if (Choice_VUMeter_Type->GetStringSelection() == "Level Pulse" ||
        Choice_VUMeter_Type->GetStringSelection() == "Level Pulse Color" ||
        Choice_VUMeter_Type->GetStringSelection() == "Level Shape" ||
        Choice_VUMeter_Type->GetStringSelection() == "Level Bar" ||
        Choice_VUMeter_Type->GetStringSelection() == "Note Level Bar" ||
        Choice_VUMeter_Type->GetStringSelection() == "Note Level Pulse")
    {
        Slider_VUMeter_Sensitivity->Enable();
        TextCtrl_VUMeter_Sensitivity->Enable();
    }
    else
    {
        Slider_VUMeter_Sensitivity->Disable();
        TextCtrl_VUMeter_Sensitivity->Disable();
    }

    if (Choice_VUMeter_Type->GetStringSelection() == "Level Shape" ||
        Choice_VUMeter_Type->GetStringSelection() == "Spectrogram")
    {
        CheckBox_VUMeter_SlowDownFalls->Enable();
    }
    else
    {
        CheckBox_VUMeter_SlowDownFalls->Disable();
    }

    if (Choice_VUMeter_Type->GetStringSelection() == "Level Shape")
    {
        Choice_VUMeter_Shape->Enable();
        if (Choice_VUMeter_Shape->GetStringSelection() == "Star" || Choice_VUMeter_Shape->GetStringSelection() == "Filled Star" || 
			Choice_VUMeter_Shape->GetStringSelection() == "Snowflake")
        {
            // we use bars to set number of points
            Slider_VUMeter_Bars->Enable();
            TextCtrl_VUMeter_Bars->Enable();
        }
        else
        {
            Slider_VUMeter_Bars->Disable();
            TextCtrl_VUMeter_Bars->Disable();
        }
    }
    else
    {
        Choice_VUMeter_Shape->Disable();

        if (Choice_VUMeter_Type->GetStringSelection() == "On" ||
            Choice_VUMeter_Type->GetStringSelection() == "Color On" ||
            Choice_VUMeter_Type->GetStringSelection() == "Timing Event Color" ||
            Choice_VUMeter_Type->GetStringSelection() == "Note On")
        {
            Slider_VUMeter_Bars->Disable();
            TextCtrl_VUMeter_Bars->Disable();
        }
        else
        {
            Slider_VUMeter_Bars->Enable();
            TextCtrl_VUMeter_Bars->Enable();
        }
    }

    if (Choice_VUMeter_Type->GetStringSelection() == "Spectrogram" ||
        Choice_VUMeter_Type->GetStringSelection() == "Note On" ||
        Choice_VUMeter_Type->GetStringSelection() == "Note Level Bar" ||
        Choice_VUMeter_Type->GetStringSelection() == "Note Level Pulse")
    {
        Slider_VUMeter_EndNote->Enable();
        Slider_VUMeter_StartNote->Enable();
        TextCtrl_VUMeter_EndNote->Enable();
        TextCtrl_VUMeter_StartNote->Enable();
    }
    else
    {
        Slider_VUMeter_EndNote->Disable();
        Slider_VUMeter_StartNote->Disable();
        TextCtrl_VUMeter_EndNote->Disable();
        TextCtrl_VUMeter_StartNote->Disable();
    }
    if (Choice_VUMeter_Type->GetStringSelection() == "Spectrogram" || Choice_VUMeter_Type->GetStringSelection() == "Level Shape")
    {
        Slider_VUMeter_XOffset->Enable();
        TextCtrl_VUMeter_XOffset->Enable();
    }
    else
    {
        Slider_VUMeter_XOffset->Disable();
        TextCtrl_VUMeter_XOffset->Disable();
    }
    if (Choice_VUMeter_Type->GetStringSelection() == "Level Shape" ||
        Choice_VUMeter_Type->GetStringSelection() == "Waveform")
    {
        Slider_VUMeter_YOffset->Enable();
        TextCtrl_VUMeter_YOffset->Enable();
        BitmapButton_VUMeter_YOffsetVC->Enable();
    }
    else
    {
        Slider_VUMeter_YOffset->Disable();
        TextCtrl_VUMeter_YOffset->Disable();
        BitmapButton_VUMeter_YOffsetVC->Disable();
    }

    Slider_VUMeter_StartNote->SetToolTip(wxString(xLightsFrame::DecodeMidi(Slider_VUMeter_StartNote->GetValue()).c_str()));
    Slider_VUMeter_EndNote->SetToolTip(wxString(xLightsFrame::DecodeMidi(Slider_VUMeter_EndNote->GetValue()).c_str()));
}

void VUMeterPanel::OnChoice_VUMeter_TypeSelect(wxCommandEvent& event)
{
	ValidateWindow();
}

void VUMeterPanel::OnSlider_VUMeter_StartNoteCmdSliderUpdated(wxScrollEvent& event)
{
    int start = Slider_VUMeter_StartNote->GetValue();
    int end = Slider_VUMeter_EndNote->GetValue();
    if (event.GetEventObject() == Slider_VUMeter_StartNote)
    {
        if (end < start)
        {
            end = start;
        }
    }
    else
    {
        if (end < start)
        {
            start = end;
        }
    }

    if (end != Slider_VUMeter_EndNote->GetValue())
    {
        Slider_VUMeter_EndNote->SetValue(end);
    }
    wxString e = wxString::Format("%d", end);
    if (e != TextCtrl_VUMeter_EndNote->GetValue())
    {
        TextCtrl_VUMeter_EndNote->SetValue(e);
    }
    if (start != Slider_VUMeter_StartNote->GetValue())
    {
        Slider_VUMeter_StartNote->SetValue(start);
    }
    wxString s = wxString::Format("%d", start);
    if (s != TextCtrl_VUMeter_StartNote->GetValue())
    {
        TextCtrl_VUMeter_StartNote->SetValue(s);
    }
    ValidateWindow();
}

void VUMeterPanel::OnTextCtrl_VUMeter_StartNoteText(wxCommandEvent& event)
{
    int start = wxAtoi(TextCtrl_VUMeter_StartNote->GetValue());
    int end = wxAtoi(TextCtrl_VUMeter_EndNote->GetValue());
    if (event.GetEventObject() == TextCtrl_VUMeter_StartNote)
    {
        if (end < start)
        {
            end = start;
        }
    }
    else
    {
        if (end < start)
        {
            start = end;
        }
    }

    if (end != Slider_VUMeter_EndNote->GetValue())
    {
        Slider_VUMeter_EndNote->SetValue(end);
    }
    wxString e = wxString::Format("%d", end);
    if (e != TextCtrl_VUMeter_EndNote->GetValue())
    {
        TextCtrl_VUMeter_EndNote->SetValue(e);
    }
    if (start != Slider_VUMeter_StartNote->GetValue())
    {
        Slider_VUMeter_StartNote->SetValue(start);
    }
    wxString s = wxString::Format("%d", start);
    if (s != TextCtrl_VUMeter_StartNote->GetValue())
    {
        TextCtrl_VUMeter_StartNote->SetValue(s);
    }

    ValidateWindow();
}
