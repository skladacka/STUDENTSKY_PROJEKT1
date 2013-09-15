#include "ModelDialog.h"
#include "ModelClass.h"
#include <wx/msgdlg.h>

//(*InternalHeaders(ModelDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(ModelDialog)
const long ModelDialog::ID_STATICTEXT1 = wxNewId();
const long ModelDialog::ID_TEXTCTRL1 = wxNewId();
const long ModelDialog::ID_STATICTEXT5 = wxNewId();
const long ModelDialog::ID_CHOICE1 = wxNewId();
const long ModelDialog::ID_STATICTEXT12 = wxNewId();
const long ModelDialog::ID_CHOICE_STRING_TYPE = wxNewId();
const long ModelDialog::ID_STATICTEXT2 = wxNewId();
const long ModelDialog::ID_SPINCTRL1 = wxNewId();
const long ModelDialog::ID_STATICTEXT3 = wxNewId();
const long ModelDialog::ID_SPINCTRL2 = wxNewId();
const long ModelDialog::ID_STATICTEXT4 = wxNewId();
const long ModelDialog::ID_SPINCTRL3 = wxNewId();
const long ModelDialog::ID_STATICTEXT6 = wxNewId();
const long ModelDialog::ID_SPINCTRL4 = wxNewId();
const long ModelDialog::ID_STATICTEXT8 = wxNewId();
const long ModelDialog::ID_RADIOBUTTON1 = wxNewId();
const long ModelDialog::ID_RADIOBUTTON2 = wxNewId();
const long ModelDialog::ID_STATICTEXT11 = wxNewId();
const long ModelDialog::ID_RADIOBUTTON4 = wxNewId();
const long ModelDialog::ID_RADIOBUTTON3 = wxNewId();
const long ModelDialog::ID_STATICTEXT9 = wxNewId();
const long ModelDialog::ID_CHOICE3 = wxNewId();
const long ModelDialog::ID_STATICTEXT10 = wxNewId();
const long ModelDialog::ID_CHECKBOX1 = wxNewId();
const long ModelDialog::ID_STATICTEXT13 = wxNewId();
const long ModelDialog::ID_CHECKBOX2 = wxNewId();
const long ModelDialog::ID_GRID_START_CHANNELS = wxNewId();
const long ModelDialog::ID_SCROLLEDWINDOW1 = wxNewId();
const long ModelDialog::ID_STATICTEXT14 = wxNewId();
const long ModelDialog::ID_BUTTON_CUSTOM_MODEL_HELP = wxNewId();
const long ModelDialog::ID_GRID_Custom = wxNewId();
const long ModelDialog::ID_SCROLLEDWINDOW2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(ModelDialog,wxDialog)
    //(*EventTable(ModelDialog)
    //*)
END_EVENT_TABLE()

ModelDialog::ModelDialog(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(ModelDialog)
    wxFlexGridSizer* FlexGridSizer4;
    wxFlexGridSizer* FlexGridSizer3;
    wxFlexGridSizer* FlexGridSizer5;
    wxFlexGridSizer* FlexGridSizer2;
    wxBoxSizer* BoxSizer2;
    wxBoxSizer* BoxSizer1;
    wxFlexGridSizer* FlexGridSizer1;
    wxStdDialogButtonSizer* StdDialogButtonSizer1;

    Create(parent, wxID_ANY, _("Model"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
    SetHelpText(_("Pixels Start in the upper left and go right or down depending on Vertical or Horizontal orientation.  Trees are always Vertical."));
    FlexGridSizer1 = new wxFlexGridSizer(0, 3, 0, 0);
    FlexGridSizer2 = new wxFlexGridSizer(0, 2, 0, 0);
    StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Model Name"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    FlexGridSizer2->Add(StaticText1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl_Name = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    FlexGridSizer2->Add(TextCtrl_Name, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Display As"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
    FlexGridSizer2->Add(StaticText5, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    Choice_DisplayAs = new wxChoice(this, ID_CHOICE1, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE1"));
    Choice_DisplayAs->SetSelection( Choice_DisplayAs->Append(_("Tree 360")) );
    Choice_DisplayAs->Append(_("Tree 270"));
    Choice_DisplayAs->Append(_("Tree 180"));
    Choice_DisplayAs->Append(_("Tree 90"));
    Choice_DisplayAs->Append(_("Vert Matrix"));
    Choice_DisplayAs->Append(_("Horiz Matrix"));
    Choice_DisplayAs->Append(_("Single Line"));
    Choice_DisplayAs->Append(_("Arches"));
    Choice_DisplayAs->Append(_("Window Frame"));
    Choice_DisplayAs->Append(_("Star"));
    Choice_DisplayAs->Append(_("Wreath"));
    Choice_DisplayAs->Append(_("Custom"));
    FlexGridSizer2->Add(Choice_DisplayAs, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    StaticText9 = new wxStaticText(this, ID_STATICTEXT12, _("Type of String"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT12"));
    FlexGridSizer2->Add(StaticText9, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    Choice_StringType = new wxChoice(this, ID_CHOICE_STRING_TYPE, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE_STRING_TYPE"));
    Choice_StringType->SetSelection( Choice_StringType->Append(_("RGB Nodes")) );
    Choice_StringType->Append(_("RBG Nodes"));
    Choice_StringType->Append(_("GBR Nodes"));
    Choice_StringType->Append(_("GRB Nodes"));
    Choice_StringType->Append(_("BRG Nodes"));
    Choice_StringType->Append(_("BGR Nodes"));
    Choice_StringType->Append(_("3 Channel RGB"));
    Choice_StringType->Append(_("Single Color Red"));
    Choice_StringType->Append(_("Single Color Green"));
    Choice_StringType->Append(_("Single Color Blue"));
    Choice_StringType->Append(_("Single Color White"));
    Choice_StringType->Append(_("Strobes White 3fps"));
    FlexGridSizer2->Add(Choice_StringType, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    StaticText_Strings = new wxStaticText(this, ID_STATICTEXT2, _("Actual # of Strings"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    FlexGridSizer2->Add(StaticText_Strings, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    SpinCtrl_parm1 = new wxSpinCtrl(this, ID_SPINCTRL1, _T("1"), wxDefaultPosition, wxSize(60,21), 0, 1, 100, 1, _T("ID_SPINCTRL1"));
    SpinCtrl_parm1->SetValue(_T("1"));
    FlexGridSizer2->Add(SpinCtrl_parm1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    StaticText_Nodes = new wxStaticText(this, ID_STATICTEXT3, _("# of RGB Nodes per String"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    FlexGridSizer2->Add(StaticText_Nodes, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    SpinCtrl_parm2 = new wxSpinCtrl(this, ID_SPINCTRL2, _T("50"), wxDefaultPosition, wxSize(60,21), 0, 1, 300, 50, _T("ID_SPINCTRL2"));
    SpinCtrl_parm2->SetValue(_T("50"));
    FlexGridSizer2->Add(SpinCtrl_parm2, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    StaticText_Strands = new wxStaticText(this, ID_STATICTEXT4, _("# of Strands per String"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
    StaticText_Strands->SetHelpText(_("How many times is a string folded\?"));
    FlexGridSizer2->Add(StaticText_Strands, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    SpinCtrl_parm3 = new wxSpinCtrl(this, ID_SPINCTRL3, _T("1"), wxDefaultPosition, wxSize(60,21), 0, 1, 100, 1, _T("ID_SPINCTRL3"));
    SpinCtrl_parm3->SetValue(_T("1"));
    FlexGridSizer2->Add(SpinCtrl_parm3, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _("Start Channel"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
    FlexGridSizer2->Add(StaticText6, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    SpinCtrl_StartChannel = new wxSpinCtrl(this, ID_SPINCTRL4, _T("1"), wxDefaultPosition, wxSize(90,21), 0, 1, 99999, 1, _T("ID_SPINCTRL4"));
    SpinCtrl_StartChannel->SetValue(_T("1"));
    FlexGridSizer2->Add(SpinCtrl_StartChannel, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    StaticText3 = new wxStaticText(this, ID_STATICTEXT8, _("Starting Corner"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
    StaticText3->SetHelpText(_("The point at which pixels in your model start."));
    FlexGridSizer2->Add(StaticText3, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    RadioButton_TopLeft = new wxRadioButton(this, ID_RADIOBUTTON1, _("Top Left"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP, wxDefaultValidator, _T("ID_RADIOBUTTON1"));
    RadioButton_TopLeft->SetValue(true);
    BoxSizer1->Add(RadioButton_TopLeft, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    RadioButton_TopRight = new wxRadioButton(this, ID_RADIOBUTTON2, _("Top Right"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON2"));
    BoxSizer1->Add(RadioButton_TopRight, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer2->Add(BoxSizer1, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    StaticText8 = new wxStaticText(this, ID_STATICTEXT11, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT11"));
    FlexGridSizer2->Add(StaticText8, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    RadioButton_BotLeft = new wxRadioButton(this, ID_RADIOBUTTON4, _("Bottom Left"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON4"));
    BoxSizer2->Add(RadioButton_BotLeft, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    RadioButton_BotRight = new wxRadioButton(this, ID_RADIOBUTTON3, _("Bottom Right"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON3"));
    BoxSizer2->Add(RadioButton_BotRight, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer2->Add(BoxSizer2, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    StaticText4 = new wxStaticText(this, ID_STATICTEXT9, _("Smooth Edges - antialias\n(future capability)"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT9"));
    FlexGridSizer2->Add(StaticText4, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    Choice_Antialias = new wxChoice(this, ID_CHOICE3, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE3"));
    Choice_Antialias->SetSelection( Choice_Antialias->Append(_("None")) );
    Choice_Antialias->Append(_("2x"));
    Choice_Antialias->Disable();
    FlexGridSizer2->Add(Choice_Antialias, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    StaticText7 = new wxStaticText(this, ID_STATICTEXT10, _("Part of my display"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT10"));
    FlexGridSizer2->Add(StaticText7, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    CheckBox_MyDisplay = new wxCheckBox(this, ID_CHECKBOX1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
    CheckBox_MyDisplay->SetValue(true);
    FlexGridSizer2->Add(CheckBox_MyDisplay, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    StaticText10 = new wxStaticText(this, ID_STATICTEXT13, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT13"));
    FlexGridSizer2->Add(StaticText10, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(FlexGridSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer3 = new wxFlexGridSizer(2, 1, 0, 0);
    FlexGridSizer3->AddGrowableRow(1);
    cbIndividualStartNumbers = new wxCheckBox(this, ID_CHECKBOX2, _("Individual Start Chans"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
    cbIndividualStartNumbers->SetValue(false);
    FlexGridSizer3->Add(cbIndividualStartNumbers, 1, wxTOP|wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 10);
    ScrolledWindow1 = new wxScrolledWindow(this, ID_SCROLLEDWINDOW1, wxDefaultPosition, wxDefaultSize, wxVSCROLL, _T("ID_SCROLLEDWINDOW1"));
    gridStartChannels = new wxGrid(ScrolledWindow1, ID_GRID_START_CHANNELS, wxPoint(-1,-3), wxSize(232,323), wxVSCROLL|wxHSCROLL|wxFULL_REPAINT_ON_RESIZE, _T("ID_GRID_START_CHANNELS"));
    gridStartChannels->CreateGrid(0,2);
    gridStartChannels->EnableEditing(true);
    gridStartChannels->EnableGridLines(true);
    gridStartChannels->SetColLabelSize(20);
    gridStartChannels->SetRowLabelSize(25);
    gridStartChannels->SetDefaultColSize(90, true);
    gridStartChannels->SetColLabelValue(0, _("Start Ch."));
    gridStartChannels->SetColLabelValue(1, _("End Ch."));
    gridStartChannels->SetDefaultCellFont( gridStartChannels->GetFont() );
    gridStartChannels->SetDefaultCellTextColour( gridStartChannels->GetForegroundColour() );
    FlexGridSizer3->Add(ScrolledWindow1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(FlexGridSizer3, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer4 = new wxFlexGridSizer(2, 1, 0, 0);
    FlexGridSizer4->AddGrowableRow(1);
    FlexGridSizer5 = new wxFlexGridSizer(0, 3, 0, 0);
    StaticTextCustomModel = new wxStaticText(this, ID_STATICTEXT14, _("Custom Model"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT14"));
    FlexGridSizer5->Add(StaticTextCustomModel, 1, wxTOP|wxBOTTOM|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ButtonCustomModelHelp = new wxButton(this, ID_BUTTON_CUSTOM_MODEL_HELP, _("Custom Model Help"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON_CUSTOM_MODEL_HELP"));
    FlexGridSizer5->Add(ButtonCustomModelHelp, 1, wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer4->Add(FlexGridSizer5, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    ScrolledWindow2 = new wxScrolledWindow(this, ID_SCROLLEDWINDOW2, wxDefaultPosition, wxSize(430,323), wxVSCROLL|wxHSCROLL, _T("ID_SCROLLEDWINDOW2"));
    ScrolledWindow2->SetMinSize(wxSize(504,-1));
    GridCustom = new wxGrid(ScrolledWindow2, ID_GRID_Custom, wxPoint(6,5), wxSize(484,318), 0, _T("ID_GRID_Custom"));
    GridCustom->CreateGrid(1,1);
    GridCustom->EnableEditing(true);
    GridCustom->EnableGridLines(true);
    GridCustom->SetColLabelSize(20);
    GridCustom->SetRowLabelSize(30);
    GridCustom->SetDefaultColSize(30, true);
    GridCustom->SetDefaultCellFont( GridCustom->GetFont() );
    GridCustom->SetDefaultCellTextColour( GridCustom->GetForegroundColour() );
    FlexGridSizer4->Add(ScrolledWindow2, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(FlexGridSizer4, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StdDialogButtonSizer1 = new wxStdDialogButtonSizer();
    StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_OK, wxEmptyString));
    StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_CANCEL, wxEmptyString));
    StdDialogButtonSizer1->Realize();
    FlexGridSizer1->Add(StdDialogButtonSizer1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SetSizer(FlexGridSizer1);
    FlexGridSizer1->Fit(this);
    FlexGridSizer1->SetSizeHints(this);
    Center();

    Connect(ID_CHOICE1,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&ModelDialog::OnChoice_DisplayAsSelect);
    Connect(ID_CHOICE_STRING_TYPE,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&ModelDialog::OnChoice_StringTypeSelect);
    Connect(ID_SPINCTRL1,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&ModelDialog::OnSpinCtrl_parm1Change);
    Connect(ID_SPINCTRL2,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&ModelDialog::OnSpinCtrl_parm2Change);
    Connect(ID_SPINCTRL4,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&ModelDialog::OnSpinCtrl_StartChannelChange);
    Connect(ID_CHECKBOX2,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&ModelDialog::OncbIndividualStartNumbersClick);
    Connect(ID_GRID_START_CHANNELS,wxEVT_GRID_CELL_CHANGE,(wxObjectEventFunction)&ModelDialog::OngridStartChannelsCellChange);
    Connect(ID_BUTTON_CUSTOM_MODEL_HELP,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ModelDialog::OnButtonCustomModelHelpClick);
    Connect(ID_GRID_Custom,wxEVT_GRID_CELL_CHANGE,(wxObjectEventFunction)&ModelDialog::OnGridCustomCellChange);
    //*)

    gridStartChannels->SetDefaultEditor(new wxGridCellNumberEditor());
    GridCustom->SetDefaultEditor(new wxGridCellNumberEditor());
    HasCustomData = false;
}

ModelDialog::~ModelDialog()
{
    //(*Destroy(ModelDialog)
    //*)
}


// make grid the size specified by the spin controls
void ModelDialog::ResizeCustomGrid()
{
    int numCols=SpinCtrl_parm1->GetValue();
    int numRows=SpinCtrl_parm2->GetValue();
    int deltaCols=numCols - GridCustom->GetNumberCols();
    int deltaRows=numRows - GridCustom->GetNumberRows();
    if (deltaCols > 0) GridCustom->AppendCols(deltaCols);
    if (deltaRows > 0) GridCustom->AppendRows(deltaRows);
    if (deltaCols < 0) GridCustom->DeleteCols(numCols,-deltaCols);
    if (deltaRows < 0) GridCustom->DeleteRows(numRows,-deltaRows);
}

bool ModelDialog::IsCustom()
{
    return Choice_DisplayAs->GetStringSelection() == wxT("Custom");
}

// initialize grid with saved values
wxString ModelDialog::GetCustomGridData()
{
    wxString customChannelData, value;
    if (IsCustom())
    {
        int numCols=GridCustom->GetNumberCols();
        int numRows=GridCustom->GetNumberRows();
        for(int row=0; row < numRows; row++)
        {
            if (row > 0) customChannelData+=wxT(";");
            for(int col=0; col<numCols; col++)
            {
                if (col > 0) customChannelData+=wxT(",");
                value = GridCustom->GetCellValue(row,col);
                if (value == wxT("0") || value.StartsWith(wxT("-"))) value.clear();
                customChannelData += value;
            }
        }
    }
    return customChannelData;
}

// Initialize custom grid with saved values
// Must be called AFTER Choice_DisplayAs & SpinCtrl_parm1 & SpinCtrl_parm2 have been set
void ModelDialog::SetCustomGridData(const wxString& customChannelData)
{
    wxString value;
    wxArrayString cols;

    if (!IsCustom()) return;

    HasCustomData = true;
    if (customChannelData.IsEmpty())
    {
        ResizeCustomGrid();
        return;
    }

    wxArrayString rows=wxSplit(customChannelData,';');
    for(size_t row=0; row < rows.size(); row++)
    {
        if (row >= GridCustom->GetNumberRows()) GridCustom->AppendRows();
        cols=wxSplit(rows[row],',');
        for(size_t col=0; col < cols.size(); col++)
        {
            if (col >= GridCustom->GetNumberCols()) GridCustom->AppendCols();
            value=cols[col];
            if (!value.IsEmpty() && value != wxT("0"))
            {
                GridCustom->SetCellValue(row,col,value);
            }
        }
    }
    SpinCtrl_parm1->SetValue(GridCustom->GetNumberCols());
    SpinCtrl_parm2->SetValue(GridCustom->GetNumberRows());
}


int ModelDialog::GetChannelsPerString()
{
    wxString StringType=Choice_StringType->GetStringSelection();
    if (ModelClass::HasSingleChannel(StringType)) return 1;
    if (ModelClass::HasSingleNode(StringType)) return 3;
    if (IsCustom())
    {
        // find max node number
        int numCols=GridCustom->GetNumberCols();
        int numRows=GridCustom->GetNumberRows();
        long val,maxval=0;
        wxString valstr;
        for(int row=0; row < numRows; row++)
        {
            for(int col=0; col<numCols; col++)
            {
                valstr = GridCustom->GetCellValue(row,col);
                if (valstr.ToLong(&val)) {
                    maxval=std::max(val,maxval);
                }
            }
        }
        return maxval*3;
    }
    else
    {
        return SpinCtrl_parm2->GetValue()*3;
    }
}


int ModelDialog::GetNumberOfStrings()
{
    wxString DisplayAs=Choice_DisplayAs->GetStringSelection();
    return (ModelClass::HasOneString(DisplayAs)) ? 1 : SpinCtrl_parm1->GetValue();
}


void ModelDialog::UpdateLabels()
{
    wxString DisplayAs=Choice_DisplayAs->GetStringSelection();
    wxString StringType=Choice_StringType->GetStringSelection();
    wxString NodeLabel = ModelClass::HasSingleChannel(StringType) ? _("lights") : _("RGB Nodes");
    wxString s;

    if (DisplayAs == wxT("Arches"))
    {
        StaticText_Strings->SetLabelText(_("# of Arches"));
        s=_("# of ") + NodeLabel + _(" per Arch");
        StaticText_Nodes->SetLabelText(s);
        StaticText_Strands->SetLabelText(_("n/a"));
        SpinCtrl_parm3->SetValue(1);
        SpinCtrl_parm3->Enable(false);
    }
    else if (DisplayAs == wxT("Window Frame"))
    {
        s=_("# of ") + NodeLabel + _(" Top");
        StaticText_Strings->SetLabelText(s);
        s=_("# of ") + NodeLabel + _(" Left/Right");
        StaticText_Nodes->SetLabelText(s);
        s=_("# of ") + NodeLabel + _(" Bottom");
        StaticText_Strands->SetLabelText(s);
        SpinCtrl_parm3->Enable(true);
    }
    else if (DisplayAs == wxT("Star")
            )
    {
        StaticText_Strings->SetLabelText(_("Actual # of Strings"));
        s=_("# of ") + NodeLabel + _(" per String");
        StaticText_Nodes->SetLabelText(s);
        StaticText_Strands->SetLabelText(_("# of points"));
        SpinCtrl_parm3->Enable(true);
    }
    else if (DisplayAs == wxT("Single Line") || DisplayAs == wxT("Wreath"))
    {
        StaticText_Strings->SetLabelText(_("Actual # of Strings"));
        s=_("# of ") + NodeLabel + _(" per String");
        StaticText_Nodes->SetLabelText(s);
        StaticText_Strands->SetLabelText(_("n/a"));
        SpinCtrl_parm3->SetValue(1);
        SpinCtrl_parm3->Enable(false);
    }
    else if (DisplayAs == wxT("Custom"))
    {
        StaticText_Strings->SetLabelText(_("Model Width"));
        StaticText_Nodes->SetLabelText(_("Model Height"));
        StaticText_Strands->SetLabelText(_("n/a"));
        SpinCtrl_parm3->SetValue(1);
        SpinCtrl_parm3->Enable(false);
    }
    else
    {
        // matrix or tree or wreath
        StaticText_Strings->SetLabelText(_("Actual # of Strings"));
        s=_("# of ") + NodeLabel + _(" per String");
        StaticText_Nodes->SetLabelText(s);
        StaticText_Strands->SetLabelText(_("# of Strands per String"));
        SpinCtrl_parm3->Enable(true);
    }
    UpdateStartChannels();
    bool CustomFlag = IsCustom();
    GridCustom->Show(CustomFlag);

    // set start corner text
    if (DisplayAs == wxT("Wreath") || DisplayAs == wxT("Star"))
    {
        RadioButton_BotRight->SetLabelText(_("Btm ctr, CCW"));
        RadioButton_TopRight->SetLabelText(_("Top ctr, CW"));
        RadioButton_BotLeft->SetLabelText (_("Btm ctr, CW"));
        RadioButton_TopLeft->SetLabelText (_("Top ctr, CCW"));
    }
    else if (CustomFlag)
    {
        RadioButton_BotRight->SetLabelText(_("n/a"));
        RadioButton_TopRight->SetLabelText(_("n/a"));
        RadioButton_BotLeft->SetLabelText (_("n/a"));
        RadioButton_TopLeft->SetLabelText (_("n/a"));
    }
    else
    {
        RadioButton_BotRight->SetLabelText(_("Bottom Right"));
        RadioButton_TopRight->SetLabelText(_("Top Right"));
        RadioButton_BotLeft->SetLabelText (_("Bottom Left"));
        RadioButton_TopLeft->SetLabelText (_("Top Left"));
    }
    RadioButton_BotRight->Enable(!CustomFlag);
    RadioButton_TopRight->Enable(!CustomFlag);
    RadioButton_BotLeft->Enable(!CustomFlag);
    RadioButton_TopLeft->Enable(!CustomFlag);
    if (CustomFlag && !HasCustomData) ResizeCustomGrid();
}


void ModelDialog::OnChoice_DisplayAsSelect(wxCommandEvent& event)
{
    UpdateLabels();
}

void ModelDialog::OncbIndividualStartNumbersClick(wxCommandEvent& event)
{
    UpdateStartChannels();
}

void ModelDialog::UpdateStartChannels()
{
    int StringStartChan,StringEndChan;
    long StringStartChanLong;
    wxString tmpStr;
    int StringCnt = GetNumberOfStrings();
    bool OneString = StringCnt == 1;
    int ChannelsPerString = GetChannelsPerString();

    // Update number of grid rows
    int curRowCnt = gridStartChannels->GetNumberRows();
    if (StringCnt > curRowCnt )
    {
        gridStartChannels->AppendRows(StringCnt - curRowCnt);
    }
    else if ( StringCnt < curRowCnt )
    {
        gridStartChannels->DeleteRows(StringCnt, curRowCnt - StringCnt);
    }

    // if only 1 string, then disable individual string start channels
    if (OneString) cbIndividualStartNumbers->SetValue(false);
    cbIndividualStartNumbers->Enable(!OneString);

    // update grid
    if(cbIndividualStartNumbers->IsChecked())
    {
        // update end channel numbers only
        for (int stringnum=0; stringnum<StringCnt; stringnum++)
        {
            tmpStr = gridStartChannels->GetCellValue(stringnum,0);
            if (tmpStr.ToLong(&StringStartChanLong) && StringStartChanLong > 0) {
                StringEndChan=StringStartChanLong + ChannelsPerString - 1;
                gridStartChannels->SetCellValue(stringnum,1, wxString::Format(wxT("%i"),StringEndChan));
            }
        }
        SetReadOnly(false);
    }
    else
    {
        // update start and end channel numbers
        int startchan = SpinCtrl_StartChannel->GetValue();
        for (int stringnum=0; stringnum<StringCnt; stringnum++)
        {
            StringStartChan=startchan + (stringnum*ChannelsPerString);
            StringEndChan=StringStartChan + ChannelsPerString - 1;
            gridStartChannels->SetCellValue(stringnum,0, wxString::Format(wxT("%i"),StringStartChan));
            gridStartChannels->SetCellValue(stringnum,1, wxString::Format(wxT("%i"),StringEndChan));
        }
        SetReadOnly(true);
    }

}

void ModelDialog::SetReadOnly(bool readonly)
{
    int ii;

    for(ii=0; ii < gridStartChannels->GetNumberRows(); ii++)
    {
        gridStartChannels->SetReadOnly(ii,0,readonly);
        //End channel is always read only.
        gridStartChannels->SetReadOnly(ii,1,true);
    }
}
void ModelDialog::OnSpinCtrl_parm1Change(wxSpinEvent& event)
{
    if (IsCustom()) {
        ResizeCustomGrid();
    } else {
        UpdateStartChannels();
    }
}

void ModelDialog::OnSpinCtrl_parm2Change(wxSpinEvent& event)
{
    if (IsCustom()) {
        ResizeCustomGrid();
    } else {
        UpdateStartChannels();
    }
}

void ModelDialog::OnSpinCtrl_StartChannelChange(wxSpinEvent& event)
{
    UpdateStartChannels();
}

void ModelDialog::OngridStartChannelsCellChange(wxGridEvent& event)
{
    int row = event.GetRow(),
        col = event.GetCol();
    wxString tmpStr;
    long val;

    if (col==0)
    {
        tmpStr = gridStartChannels->GetCellValue(row,col);
        if ( (!tmpStr.ToLong(&val) || val <= 0) )
        {
            wxMessageBox(_("Cell value must be a positive numeric value"));
            gridStartChannels->SetCellValue(row,col,wxT("1"));
        }
        else
        {
            UpdateStartChannels();
        }
    }
    event.Skip();
}

// does not update name
void ModelDialog::UpdateXml(wxXmlNode* e)
{
    int ii;
    long numStrings;
    wxString tempStr;
    if(e->HasAttribute(wxT("Advanced")))
    {
        e->DeleteAttribute(wxT("Advanced"));
        tempStr = e->GetAttribute(wxT("parm1"));
        tempStr.ToLong(&numStrings);
        for(ii=0; ii < numStrings; ii++)
        {
            e->DeleteAttribute(ModelClass::StartChanAttrName(ii));
        }
    }
    if (e->HasAttribute(wxT("CustomModel")))
    {
        e->DeleteAttribute(wxT("CustomModel"));
    }
    if (cbIndividualStartNumbers->IsChecked())
    {
        e->AddAttribute(wxT("Advanced"), wxT("1"));
        for(ii=0; ii < gridStartChannels->GetNumberRows(); ii++)
        {
            e->AddAttribute(ModelClass::StartChanAttrName(ii),gridStartChannels->GetCellValue(ii,0));
        }
    }
    if (e->HasAttribute(wxT("StartSide"))) e->DeleteAttribute(wxT("StartSide"));
    e->DeleteAttribute(wxT("DisplayAs"));
    e->DeleteAttribute(wxT("StringType"));
    e->DeleteAttribute(wxT("parm1"));
    e->DeleteAttribute(wxT("parm2"));
    e->DeleteAttribute(wxT("parm3"));
    e->DeleteAttribute(wxT("StartChannel"));
    e->DeleteAttribute(wxT("Order"));
    e->DeleteAttribute(wxT("Dir"));
    e->DeleteAttribute(wxT("Antialias"));
    e->AddAttribute(wxT("DisplayAs"), Choice_DisplayAs->GetStringSelection());
    e->AddAttribute(wxT("StringType"), Choice_StringType->GetStringSelection());
    e->AddAttribute(wxT("parm1"), wxString::Format(wxT("%d"),SpinCtrl_parm1->GetValue()));
    e->AddAttribute(wxT("parm2"), wxString::Format(wxT("%d"),SpinCtrl_parm2->GetValue()));
    e->AddAttribute(wxT("parm3"), wxString::Format(wxT("%d"),SpinCtrl_parm3->GetValue()));
    e->AddAttribute(wxT("StartChannel"), wxString::Format(wxT("%d"),SpinCtrl_StartChannel->GetValue()));
    //e->AddAttribute(wxT("Order"), Choice_Order->GetStringSelection());
    if (RadioButton_TopLeft->GetValue() || RadioButton_TopRight->GetValue() )
        e->AddAttribute(wxT("StartSide"),wxT("T"));
    else
        e->AddAttribute(wxT("StartSide"),wxT("B"));
    if (RadioButton_TopLeft->GetValue() || RadioButton_BotLeft->GetValue() )
        e->AddAttribute(wxT("Dir"),wxT("L"));
    else
        e->AddAttribute(wxT("Dir"),wxT("R"));

    e->AddAttribute(wxT("Antialias"), wxString::Format(wxT("%d"),Choice_Antialias->GetSelection()));
    if (Choice_DisplayAs->GetStringSelection() == wxT("Custom"))
    {
        e->AddAttribute(wxT("CustomModel"),GetCustomGridData());
    }
    ModelClass::SetMyDisplay(e,CheckBox_MyDisplay->GetValue());
}

void ModelDialog::SetFromXml(wxXmlNode* e, const wxString& NameSuffix)
{
    long n;
    wxString name, direction, startSide, tempStr;
    name=e->GetAttribute(wxT("name")) + NameSuffix;
    TextCtrl_Name->SetValue(name);
    Choice_DisplayAs->SetStringSelection(e->GetAttribute(wxT("DisplayAs")));
    Choice_StringType->SetStringSelection(e->GetAttribute(wxT("StringType"),wxT("RGB Nodes")));
    SpinCtrl_parm1->SetValue(e->GetAttribute(wxT("parm1")));
    SpinCtrl_parm2->SetValue(e->GetAttribute(wxT("parm2")));
    SpinCtrl_parm3->SetValue(e->GetAttribute(wxT("parm3")));
    SpinCtrl_StartChannel->SetValue(e->GetAttribute(wxT("StartChannel")));
    //Choice_Order->SetStringSelection(e->GetAttribute(wxT("Order")));
    tempStr=e->GetAttribute(wxT("Antialias"),wxT("0"));
    tempStr.ToLong(&n);
    Choice_Antialias->SetSelection(n);
    direction=e->GetAttribute(wxT("Dir"));
    if(e->HasAttribute(wxT("StartSide")))
    {
        startSide=e->GetAttribute(wxT("StartSide"));
    }
    else
    {
        startSide = wxT("B");
    }
    if(e->HasAttribute(wxT("Advanced")))
    {
        cbIndividualStartNumbers->SetValue(true);
        tempStr = e->GetAttribute(wxT("parm1"));
        tempStr.ToLong(&n);  // number of strings
        for(int ii=0; ii < n; ii++)
        {
            gridStartChannels->AppendRows();
            gridStartChannels->SetCellValue(ii,0,e->GetAttribute(ModelClass::StartChanAttrName(ii)));
        }
    }

    if (direction == wxT("R") )
    {
        if(startSide == wxT("B"))
            RadioButton_BotRight->SetValue(true);
        else
            RadioButton_TopRight->SetValue(true);
    }
    else
    {
        if(startSide == wxT("B"))
            RadioButton_BotLeft->SetValue(true);
        else
            RadioButton_TopLeft->SetValue(true);
    }
    if (e->HasAttribute(wxT("CustomModel")))
    {
        e->GetAttribute(wxT("CustomModel"),&tempStr);
        SetCustomGridData(tempStr);
    }
    CheckBox_MyDisplay->SetValue(ModelClass::IsMyDisplay(e));
    UpdateLabels();
    Fit();
}

void ModelDialog::OnButtonCustomModelHelpClick(wxCommandEvent& event)
{
    wxMessageBox(_("To create a custom model, set 'Display As' to 'Custom';\nthen set the model width and model height values.\n\nA custom model allows you to represent almost any display element \ncreated from RGB lights. If your element has 12 RGB nodes in it, then \nyou will need to place the numbers 1 through 12 in the grid. \nPlace the numbers so that they are in the shape of your display \nelement. If you enter a number and then need to erase it, enter a 0.\n\nNode 1 will be assigned:\n  StartChannel\n  StartChannel+1\n  StartChannel+2\nNode 2 will be assigned:\n  StartChannel+3\n  StartChannel+4\n  StartChannel+5\netc.\n\nFor example, to model a candy cane with 12 nodes, you could \nstart with a grid 4 columns wide and 10 rows high. You would place the \nnumbers 1-9 up the right-hand side, 10 and 11 would go in the middle \ncells in the top row, and then 12 would go in column A, row 2.\n\n     11 10\n12           9\n               8\n               7\n               6\n               5\n               4\n               3\n               2\n               1"));
}

void ModelDialog::OnChoice_StringTypeSelect(wxCommandEvent& event)
{
    UpdateLabels();
}

void ModelDialog::OnGridCustomCellChange(wxGridEvent& event)
{
    UpdateStartChannels();
}
