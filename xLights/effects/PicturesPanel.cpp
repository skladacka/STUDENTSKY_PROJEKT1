#include "PicturesPanel.h"
#include "../../include/padlock16x16-blue.xpm"
#include "EffectPanelUtils.h"

//(*InternalHeaders(PicturesPanel)
#include <wx/bmpbuttn.h>
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/notebook.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/slider.h>
#include <wx/intl.h>
#include <wx/filepicker.h>
#include <wx/gbsizer.h>
#include <wx/stattext.h>
#include <wx/bitmap.h>
#include <wx/textctrl.h>
#include <wx/choice.h>
#include <wx/image.h>
//*)

//(*IdInit(PicturesPanel)
const long PicturesPanel::ID_FILEPICKER_Pictures_Filename = wxNewId();
const long PicturesPanel::ID_STATICTEXT_Pictures_Direction = wxNewId();
const long PicturesPanel::ID_CHOICE_Pictures_Direction = wxNewId();
const long PicturesPanel::ID_BITMAPBUTTON_CHOICE_Pictures_Direction = wxNewId();
const long PicturesPanel::ID_STATICTEXT_Pictures_Speed = wxNewId();
const long PicturesPanel::IDD_SLIDER_Pictures_Speed = wxNewId();
const long PicturesPanel::ID_TEXTCTRL_Pictures_Speed = wxNewId();
const long PicturesPanel::ID_BITMAPBUTTON_SLIDER_Pictures_Speed = wxNewId();
const long PicturesPanel::ID_STATICTEXT_Pictures_FrameRateAdj = wxNewId();
const long PicturesPanel::IDD_SLIDER_Pictures_FrameRateAdj = wxNewId();
const long PicturesPanel::ID_TEXTCTRL_Pictures_FrameRateAdj = wxNewId();
const long PicturesPanel::ID_BITMAPBUTTON_SLIDER_Pictures_FrameRateAdj = wxNewId();
const long PicturesPanel::ID_CHECKBOX_Pictures_PixelOffsets = wxNewId();
const long PicturesPanel::ID_CHOICE_Scaling = wxNewId();
const long PicturesPanel::ID_CHECKBOX_Pictures_Shimmer = wxNewId();
const long PicturesPanel::ID_CHECKBOX_LoopGIF = wxNewId();
const long PicturesPanel::ID_STATICTEXT_PicturesXC = wxNewId();
const long PicturesPanel::ID_SLIDER_PicturesXC = wxNewId();
const long PicturesPanel::ID_CHECKBOX_Pictures_WrapX = wxNewId();
const long PicturesPanel::IDD_TEXTCTRL_PicturesXC = wxNewId();
const long PicturesPanel::ID_STATICTEXT_PicturesYC = wxNewId();
const long PicturesPanel::IDD_TEXTCTRL_PicturesYC = wxNewId();
const long PicturesPanel::ID_SLIDER_PicturesYC = wxNewId();
const long PicturesPanel::ID_PANEL43 = wxNewId();
const long PicturesPanel::ID_STATICTEXT_PicturesEndXC = wxNewId();
const long PicturesPanel::ID_SLIDER_PicturesEndXC = wxNewId();
const long PicturesPanel::IDD_TEXTCTRL_PicturesEndXC = wxNewId();
const long PicturesPanel::ID_STATICTEXT_PicturesEndYC = wxNewId();
const long PicturesPanel::IDD_TEXTCTRL_PicturesEndYC = wxNewId();
const long PicturesPanel::ID_SLIDER_PicturesEndYC = wxNewId();
const long PicturesPanel::ID_PANEL45 = wxNewId();
const long PicturesPanel::ID_STATICTEXT_Pictures_StartScale = wxNewId();
const long PicturesPanel::ID_SLIDER_Pictures_StartScale = wxNewId();
const long PicturesPanel::IDD_TEXTCTRL_Pictures_StartScale = wxNewId();
const long PicturesPanel::ID_PANEL1 = wxNewId();
const long PicturesPanel::ID_STATICTEXT_Pictures_EndScale = wxNewId();
const long PicturesPanel::ID_SLIDER_Pictures_EndScale = wxNewId();
const long PicturesPanel::IDD_TEXTCTRL_Pictures_EndScale = wxNewId();
const long PicturesPanel::ID_PANEL2 = wxNewId();
const long PicturesPanel::IDD_NOTEBOOK_Pictures_Positions = wxNewId();
//*)

BEGIN_EVENT_TABLE(PicturesPanel,wxPanel)
	//(*EventTable(PicturesPanel)
	//*)
END_EVENT_TABLE()

PicturesPanel::PicturesPanel(wxWindow* parent)
{
	//(*Initialize(PicturesPanel)
	wxFlexGridSizer* FlexGridSizer102;
	wxFlexGridSizer* FlexGridSizer1;
	wxFlexGridSizer* FlexGridSizer2;
	wxPanel* Panel1;
	wxFlexGridSizer* FlexGridSizer111;
	wxFlexGridSizer* FlexGridSizer19;
	BulkEditTextCtrl* TextCtrl2;
	wxFlexGridSizer* FlexGridSizer112;
	wxPanel* PictureStartPositionPanel;
	BulkEditTextCtrl* TextCtrl1;
	BulkEditTextCtrlF1* TextCtrl48;
	wxPanel* Panel2;
	BulkEditTextCtrl* TextCtrl_PicturesXC;
	wxFlexGridSizer* FlexGridSizer42;
	wxNotebook* Notebook4;
	wxFlexGridSizer* FlexGridSizer3;
	BulkEditTextCtrl* TextCtrl_PicturesYC;
	wxFlexGridSizer* FlexGridSizer31;
	BulkEditTextCtrlF1* TextCtrl_Pictures_FR;
	BulkEditTextCtrl* TextCtrl_Pictures_EndScale;
	BulkEditTextCtrl* TextCtrl_Pictures_StartScale;
	wxFlexGridSizer* FlexGridSizer106;
	wxGridBagSizer* GridBagSizer2;
	wxFlexGridSizer* FlexGridSizer113;
	wxGridBagSizer* GridBagSizer3;

	Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	FlexGridSizer42 = new wxFlexGridSizer(0, 1, 0, 0);
	FlexGridSizer42->AddGrowableCol(0);
	FlexGridSizer19 = new wxFlexGridSizer(0, 2, 0, 0);
	FlexGridSizer19->AddGrowableCol(0);
	FilePickerCtrl1 = new xlPictureFilePickerCtrl(this, ID_FILEPICKER_Pictures_Filename, wxEmptyString, wxEmptyString, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxFLP_FILE_MUST_EXIST|wxFLP_OPEN, wxDefaultValidator, _T("ID_FILEPICKER_Pictures_Filename"));
	FlexGridSizer19->Add(FilePickerCtrl1, 1, wxALL|wxEXPAND, 0);
	FlexGridSizer42->Add(FlexGridSizer19, 1, wxALL|wxEXPAND, 0);
	FlexGridSizer31 = new wxFlexGridSizer(0, 4, 0, 0);
	FlexGridSizer31->AddGrowableCol(1);
	StaticText46 = new wxStaticText(this, ID_STATICTEXT_Pictures_Direction, _("Movement"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_Pictures_Direction"));
	FlexGridSizer31->Add(StaticText46, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	Choice_Pictures_Direction = new BulkEditChoice(this, ID_CHOICE_Pictures_Direction, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE_Pictures_Direction"));
	Choice_Pictures_Direction->SetSelection( Choice_Pictures_Direction->Append(_("none")) );
	Choice_Pictures_Direction->Append(_("left"));
	Choice_Pictures_Direction->Append(_("right"));
	Choice_Pictures_Direction->Append(_("up"));
	Choice_Pictures_Direction->Append(_("down"));
	Choice_Pictures_Direction->Append(_("up-left"));
	Choice_Pictures_Direction->Append(_("down-left"));
	Choice_Pictures_Direction->Append(_("up-right"));
	Choice_Pictures_Direction->Append(_("down-right"));
	Choice_Pictures_Direction->Append(_("peekaboo"));
	Choice_Pictures_Direction->Append(_("wiggle"));
	Choice_Pictures_Direction->Append(_("zoom in"));
	Choice_Pictures_Direction->Append(_("peekaboo 90"));
	Choice_Pictures_Direction->Append(_("peekaboo 180"));
	Choice_Pictures_Direction->Append(_("peekaboo 270"));
	Choice_Pictures_Direction->Append(_("vix 2 routine"));
	Choice_Pictures_Direction->Append(_("flag wave"));
	Choice_Pictures_Direction->Append(_("up once"));
	Choice_Pictures_Direction->Append(_("down once"));
	Choice_Pictures_Direction->Append(_("vector"));
	Choice_Pictures_Direction->Append(_("tile-left"));
	Choice_Pictures_Direction->Append(_("tile-right"));
	Choice_Pictures_Direction->Append(_("tile-down"));
	Choice_Pictures_Direction->Append(_("tile-up"));
	FlexGridSizer31->Add(Choice_Pictures_Direction, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	FlexGridSizer31->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BitmapButton_PicturesDirection = new wxBitmapButton(this, ID_BITMAPBUTTON_CHOICE_Pictures_Direction, padlock16x16_blue_xpm, wxDefaultPosition, wxSize(13,13), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON_CHOICE_Pictures_Direction"));
	BitmapButton_PicturesDirection->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	FlexGridSizer31->Add(BitmapButton_PicturesDirection, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	StaticText68 = new wxStaticText(this, ID_STATICTEXT_Pictures_Speed, _("Movement Speed"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_Pictures_Speed"));
	FlexGridSizer31->Add(StaticText68, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	Slider_Pictures_Speed = new BulkEditSliderF1(this, IDD_SLIDER_Pictures_Speed, 10, 0, 200, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("IDD_SLIDER_Pictures_Speed"));
	FlexGridSizer31->Add(Slider_Pictures_Speed, 1, wxALL|wxEXPAND, 2);
	TextCtrl48 = new BulkEditTextCtrlF1(this, ID_TEXTCTRL_Pictures_Speed, _("1.0"), wxDefaultPosition, wxDLG_UNIT(this,wxSize(20,-1)), 0, wxDefaultValidator, _T("ID_TEXTCTRL_Pictures_Speed"));
	TextCtrl48->SetMaxLength(4);
	FlexGridSizer31->Add(TextCtrl48, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BitmapButton_PicturesSpeed = new wxBitmapButton(this, ID_BITMAPBUTTON_SLIDER_Pictures_Speed, padlock16x16_blue_xpm, wxDefaultPosition, wxSize(13,13), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON_SLIDER_Pictures_Speed"));
	BitmapButton_PicturesSpeed->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	FlexGridSizer31->Add(BitmapButton_PicturesSpeed, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText96 = new wxStaticText(this, ID_STATICTEXT_Pictures_FrameRateAdj, _("Frame Rate Adj"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_Pictures_FrameRateAdj"));
	FlexGridSizer31->Add(StaticText96, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	Slider_Pictures_FR = new BulkEditSliderF1(this, IDD_SLIDER_Pictures_FrameRateAdj, 10, 0, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("IDD_SLIDER_Pictures_FrameRateAdj"));
	FlexGridSizer31->Add(Slider_Pictures_FR, 1, wxALL|wxEXPAND, 2);
	TextCtrl_Pictures_FR = new BulkEditTextCtrlF1(this, ID_TEXTCTRL_Pictures_FrameRateAdj, _("1.0"), wxDefaultPosition, wxDLG_UNIT(this,wxSize(20,-1)), wxTE_CENTRE, wxDefaultValidator, _T("ID_TEXTCTRL_Pictures_FrameRateAdj"));
	TextCtrl_Pictures_FR->SetMaxLength(4);
	FlexGridSizer31->Add(TextCtrl_Pictures_FR, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BitmapButton_PicturesFrameRateAdj = new wxBitmapButton(this, ID_BITMAPBUTTON_SLIDER_Pictures_FrameRateAdj, padlock16x16_blue_xpm, wxDefaultPosition, wxSize(13,13), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON_SLIDER_Pictures_FrameRateAdj"));
	BitmapButton_PicturesFrameRateAdj->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	FlexGridSizer31->Add(BitmapButton_PicturesFrameRateAdj, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	FlexGridSizer42->Add(FlexGridSizer31, 1, wxEXPAND, 2);
	FlexGridSizer102 = new wxFlexGridSizer(0, 1, 0, 0);
	FlexGridSizer102->AddGrowableCol(0);
	FlexGridSizer1 = new wxFlexGridSizer(0, 2, 0, 0);
	CheckBox_Pictures_PixelOffsets = new BulkEditCheckBox(this, ID_CHECKBOX_Pictures_PixelOffsets, _("Offsets In Pixels"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX_Pictures_PixelOffsets"));
	CheckBox_Pictures_PixelOffsets->SetValue(false);
	FlexGridSizer1->Add(CheckBox_Pictures_PixelOffsets, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	Choice_Scaling = new BulkEditChoice(this, ID_CHOICE_Scaling, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE_Scaling"));
	Choice_Scaling->SetSelection( Choice_Scaling->Append(_("No Scaling")) );
	Choice_Scaling->Append(_("Scale To Fit"));
	Choice_Scaling->Append(_("Scale Keep Aspect Ratio"));
	FlexGridSizer1->Add(Choice_Scaling, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	CheckBox_Pictures_Shimmer = new BulkEditCheckBox(this, ID_CHECKBOX_Pictures_Shimmer, _("Shimmer"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX_Pictures_Shimmer"));
	CheckBox_Pictures_Shimmer->SetValue(false);
	FlexGridSizer1->Add(CheckBox_Pictures_Shimmer, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	CheckBox_LoopGIF = new BulkEditCheckBox(this, ID_CHECKBOX_LoopGIF, _("Loop Animated GIF"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX_LoopGIF"));
	CheckBox_LoopGIF->SetValue(false);
	FlexGridSizer1->Add(CheckBox_LoopGIF, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer102->Add(FlexGridSizer1, 1, wxALL|wxEXPAND, 5);
	Notebook4 = new wxNotebook(this, IDD_NOTEBOOK_Pictures_Positions, wxDefaultPosition, wxDefaultSize, 0, _T("IDD_NOTEBOOK_Pictures_Positions"));
	PictureStartPositionPanel = new wxPanel(Notebook4, ID_PANEL43, wxPoint(15,49), wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL43"));
	FlexGridSizer112 = new wxFlexGridSizer(0, 2, 0, 0);
	FlexGridSizer112->AddGrowableCol(0);
	FlexGridSizer106 = new wxFlexGridSizer(0, 2, 0, 0);
	FlexGridSizer106->AddGrowableCol(1);
	StaticText_Pictures_XC = new wxStaticText(PictureStartPositionPanel, ID_STATICTEXT_PicturesXC, _("X-axis Center"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_PicturesXC"));
	FlexGridSizer106->Add(StaticText_Pictures_XC, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	Slider_PicturesXC = new BulkEditSlider(PictureStartPositionPanel, ID_SLIDER_PicturesXC, 0, -100, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_PicturesXC"));
	FlexGridSizer106->Add(Slider_PicturesXC, 1, wxALL|wxEXPAND, 5);
	CheckBox_Pictures_WrapX = new BulkEditCheckBox(PictureStartPositionPanel, ID_CHECKBOX_Pictures_WrapX, _("Wrap X"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX_Pictures_WrapX"));
	CheckBox_Pictures_WrapX->SetValue(false);
	FlexGridSizer106->Add(CheckBox_Pictures_WrapX, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	TextCtrl_PicturesXC = new BulkEditTextCtrl(PictureStartPositionPanel, IDD_TEXTCTRL_PicturesXC, _("0"), wxDefaultPosition, wxDLG_UNIT(PictureStartPositionPanel,wxSize(20,-1)), wxTE_CENTRE, wxDefaultValidator, _T("IDD_TEXTCTRL_PicturesXC"));
	TextCtrl_PicturesXC->SetMaxLength(4);
	FlexGridSizer106->Add(TextCtrl_PicturesXC, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer112->Add(FlexGridSizer106, 1, wxALL|wxEXPAND, 5);
	GridBagSizer2 = new wxGridBagSizer(0, 0);
	StaticText_Pictures_YC = new wxStaticText(PictureStartPositionPanel, ID_STATICTEXT_PicturesYC, _("Y-axis Center"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_PicturesYC"));
	GridBagSizer2->Add(StaticText_Pictures_YC, wxGBPosition(1, 0), wxDefaultSpan, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	TextCtrl_PicturesYC = new BulkEditTextCtrl(PictureStartPositionPanel, IDD_TEXTCTRL_PicturesYC, _("0"), wxDefaultPosition, wxDLG_UNIT(PictureStartPositionPanel,wxSize(20,-1)), wxTE_CENTRE, wxDefaultValidator, _T("IDD_TEXTCTRL_PicturesYC"));
	TextCtrl_PicturesYC->SetMaxLength(4);
	GridBagSizer2->Add(TextCtrl_PicturesYC, wxGBPosition(2, 0), wxDefaultSpan, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	Slider_PicturesYC = new BulkEditSlider(PictureStartPositionPanel, ID_SLIDER_PicturesYC, 0, -100, 100, wxDefaultPosition, wxDefaultSize, wxSL_VERTICAL|wxSL_INVERSE, wxDefaultValidator, _T("ID_SLIDER_PicturesYC"));
	GridBagSizer2->Add(Slider_PicturesYC, wxGBPosition(0, 1), wxGBSpan(4, 1), wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer112->Add(GridBagSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	PictureStartPositionPanel->SetSizer(FlexGridSizer112);
	FlexGridSizer112->Fit(PictureStartPositionPanel);
	FlexGridSizer112->SetSizeHints(PictureStartPositionPanel);
	PictureEndPositionPanel = new wxPanel(Notebook4, ID_PANEL45, wxPoint(104,13), wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL45"));
	FlexGridSizer111 = new wxFlexGridSizer(0, 2, 0, 0);
	FlexGridSizer111->AddGrowableCol(0);
	FlexGridSizer113 = new wxFlexGridSizer(0, 2, 0, 0);
	FlexGridSizer113->AddGrowableCol(1);
	StaticText160 = new wxStaticText(PictureEndPositionPanel, ID_STATICTEXT_PicturesEndXC, _("X-axis Center"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_PicturesEndXC"));
	FlexGridSizer113->Add(StaticText160, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	Slider_PicturesEndXC = new BulkEditSlider(PictureEndPositionPanel, ID_SLIDER_PicturesEndXC, 0, -100, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_PicturesEndXC"));
	FlexGridSizer113->Add(Slider_PicturesEndXC, 1, wxALL|wxEXPAND, 5);
	FlexGridSizer113->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	TextCtrl1 = new BulkEditTextCtrl(PictureEndPositionPanel, IDD_TEXTCTRL_PicturesEndXC, _("0"), wxDefaultPosition, wxDLG_UNIT(PictureEndPositionPanel,wxSize(20,-1)), wxTE_CENTRE, wxDefaultValidator, _T("IDD_TEXTCTRL_PicturesEndXC"));
	TextCtrl1->SetMaxLength(4);
	FlexGridSizer113->Add(TextCtrl1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer111->Add(FlexGridSizer113, 1, wxALL|wxEXPAND, 5);
	GridBagSizer3 = new wxGridBagSizer(0, 0);
	StaticText161 = new wxStaticText(PictureEndPositionPanel, ID_STATICTEXT_PicturesEndYC, _("Y-axis Center"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_PicturesEndYC"));
	GridBagSizer3->Add(StaticText161, wxGBPosition(1, 0), wxDefaultSpan, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	TextCtrl2 = new BulkEditTextCtrl(PictureEndPositionPanel, IDD_TEXTCTRL_PicturesEndYC, _("0"), wxDefaultPosition, wxDLG_UNIT(PictureEndPositionPanel,wxSize(20,-1)), wxTE_CENTRE, wxDefaultValidator, _T("IDD_TEXTCTRL_PicturesEndYC"));
	TextCtrl2->SetMaxLength(4);
	GridBagSizer3->Add(TextCtrl2, wxGBPosition(2, 0), wxDefaultSpan, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	Slider_PicturesEndYC = new BulkEditSlider(PictureEndPositionPanel, ID_SLIDER_PicturesEndYC, 0, -100, 100, wxDefaultPosition, wxDefaultSize, wxSL_VERTICAL|wxSL_INVERSE, wxDefaultValidator, _T("ID_SLIDER_PicturesEndYC"));
	GridBagSizer3->Add(Slider_PicturesEndYC, wxGBPosition(0, 1), wxGBSpan(4, 1), wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer111->Add(GridBagSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	PictureEndPositionPanel->SetSizer(FlexGridSizer111);
	FlexGridSizer111->Fit(PictureEndPositionPanel);
	FlexGridSizer111->SetSizeHints(PictureEndPositionPanel);
	Panel1 = new wxPanel(Notebook4, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	FlexGridSizer2 = new wxFlexGridSizer(0, 3, 0, 0);
	FlexGridSizer2->AddGrowableCol(1);
	StaticText1 = new wxStaticText(Panel1, ID_STATICTEXT_Pictures_StartScale, _("Scaling"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_Pictures_StartScale"));
	FlexGridSizer2->Add(StaticText1, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	Slider_Pictures_StartScale = new BulkEditSlider(Panel1, ID_SLIDER_Pictures_StartScale, 100, 0, 1000, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_Pictures_StartScale"));
	FlexGridSizer2->Add(Slider_Pictures_StartScale, 1, wxALL|wxEXPAND, 5);
	TextCtrl_Pictures_StartScale = new BulkEditTextCtrl(Panel1, IDD_TEXTCTRL_Pictures_StartScale, _("100"), wxDefaultPosition, wxDLG_UNIT(Panel1,wxSize(25,-1)), wxTE_CENTRE, wxDefaultValidator, _T("IDD_TEXTCTRL_Pictures_StartScale"));
	TextCtrl_Pictures_StartScale->SetMaxLength(4);
	FlexGridSizer2->Add(TextCtrl_Pictures_StartScale, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	Panel1->SetSizer(FlexGridSizer2);
	FlexGridSizer2->Fit(Panel1);
	FlexGridSizer2->SetSizeHints(Panel1);
	Panel2 = new wxPanel(Notebook4, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
	FlexGridSizer3 = new wxFlexGridSizer(0, 3, 0, 0);
	FlexGridSizer3->AddGrowableCol(1);
	StaticText2 = new wxStaticText(Panel2, ID_STATICTEXT_Pictures_EndScale, _("Scaling"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_Pictures_EndScale"));
	FlexGridSizer3->Add(StaticText2, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	Slider_Pictures_EndScale = new BulkEditSlider(Panel2, ID_SLIDER_Pictures_EndScale, 100, 0, 1000, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_Pictures_EndScale"));
	FlexGridSizer3->Add(Slider_Pictures_EndScale, 1, wxALL|wxEXPAND, 5);
	TextCtrl_Pictures_EndScale = new BulkEditTextCtrl(Panel2, IDD_TEXTCTRL_Pictures_EndScale, _("100"), wxDefaultPosition, wxDLG_UNIT(Panel2,wxSize(25,-1)), wxTE_CENTRE, wxDefaultValidator, _T("IDD_TEXTCTRL_Pictures_EndScale"));
	TextCtrl_Pictures_EndScale->SetMaxLength(4);
	FlexGridSizer3->Add(TextCtrl_Pictures_EndScale, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	Panel2->SetSizer(FlexGridSizer3);
	FlexGridSizer3->Fit(Panel2);
	FlexGridSizer3->SetSizeHints(Panel2);
	Notebook4->AddPage(PictureStartPositionPanel, _("Start Position"), false);
	Notebook4->AddPage(PictureEndPositionPanel, _("End Position"), false);
	Notebook4->AddPage(Panel1, _("Start Scale"), false);
	Notebook4->AddPage(Panel2, _("End Scale"), false);
	FlexGridSizer102->Add(Notebook4, 1, wxALL|wxEXPAND, 5);
	FlexGridSizer42->Add(FlexGridSizer102, 1, wxALL|wxEXPAND, 5);
	SetSizer(FlexGridSizer42);
	FlexGridSizer42->Fit(this);
	FlexGridSizer42->SetSizeHints(this);

	Connect(ID_CHOICE_Pictures_Direction,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&PicturesPanel::OnChoicePicturesDirectionSelect);
	Connect(ID_BITMAPBUTTON_CHOICE_Pictures_Direction,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PicturesPanel::OnLockButtonClick);
	Connect(ID_BITMAPBUTTON_SLIDER_Pictures_Speed,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PicturesPanel::OnLockButtonClick);
	Connect(ID_BITMAPBUTTON_SLIDER_Pictures_FrameRateAdj,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PicturesPanel::OnLockButtonClick);
	//*)
    SetName("ID_PANEL_PICTURES");
}

PicturesPanel::~PicturesPanel()
{
	//(*Destroy(PicturesPanel)
	//*)
}

PANEL_EVENT_HANDLERS(PicturesPanel)

static inline void EnableControl(wxWindow *w, int id, bool e) {
    wxWindow *c = w->FindWindowById(id);
    if (c) c->Enable(e);
}

void PicturesPanel::OnChoicePicturesDirectionSelect(wxCommandEvent& event)
{
    bool enable = "vector" == Choice_Pictures_Direction->GetStringSelection();
    EnableControl(Choice_Pictures_Direction->GetParent(), IDD_TEXTCTRL_PicturesEndXC, enable);
    EnableControl(Choice_Pictures_Direction->GetParent(), IDD_TEXTCTRL_PicturesEndYC, enable);
    EnableControl(Choice_Pictures_Direction->GetParent(), ID_SLIDER_PicturesEndXC, enable);
    EnableControl(Choice_Pictures_Direction->GetParent(), ID_SLIDER_PicturesEndYC, enable);
    PictureEndPositionPanel->Enable(enable);
}

void PicturesPanel::OnFilePickerCtrl1FileChanged(wxFileDirPickerEvent& event)
{
}
