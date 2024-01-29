/***************************************************************
 * This source files comes from the xLights project
 * https://www.xlights.org
 * https://github.com/xLightsSequencer/xLights
 * See the github commit history for a record of contributing
 * developers.
 * Copyright claimed based on commit dates recorded in Github
 * License: https://github.com/xLightsSequencer/xLights/blob/master/License.txt
 **************************************************************/

#include "PlayListItemVideoPanel.h"
#include "PlayListItemVideo.h"
#include "VideoWindowPositionDialog.h"
#include "PlayListDialog.h"

//(*InternalHeaders(PlayListItemVideoPanel)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(PlayListItemVideoPanel)
const long PlayListItemVideoPanel::ID_STATICTEXT1 = wxNewId();
const long PlayListItemVideoPanel::ID_FILEPICKERCTRL1 = wxNewId();
const long PlayListItemVideoPanel::ID_BUTTON1 = wxNewId();
const long PlayListItemVideoPanel::ID_STATICTEXT5 = wxNewId();
const long PlayListItemVideoPanel::ID_CHECKBOX1 = wxNewId();
const long PlayListItemVideoPanel::ID_CHECKBOX2 = wxNewId();
const long PlayListItemVideoPanel::ID_CHECKBOX3 = wxNewId();
const long PlayListItemVideoPanel::ID_CHECKBOX4 = wxNewId();
const long PlayListItemVideoPanel::ID_CHECKBOX5 = wxNewId();
const long PlayListItemVideoPanel::ID_STATICTEXT3 = wxNewId();
const long PlayListItemVideoPanel::ID_SPINCTRL1 = wxNewId();
const long PlayListItemVideoPanel::ID_STATICTEXT4 = wxNewId();
const long PlayListItemVideoPanel::ID_SPINCTRL2 = wxNewId();
const long PlayListItemVideoPanel::ID_STATICTEXT2 = wxNewId();
const long PlayListItemVideoPanel::ID_TEXTCTRL1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(PlayListItemVideoPanel,wxPanel)
	//(*EventTable(PlayListItemVideoPanel)
	//*)
END_EVENT_TABLE()

class VideoFilePickerCtrl : public wxFilePickerCtrl
{
#define VIDEOFILES "Video Files|*.avi;*.mp4;*.mkv;*.mov;*.asf;*.flv;*.mpg;*.mpeg;*.m4v;*.wmv|All Files|*.*"

public:
    VideoFilePickerCtrl(wxWindow *parent,
        wxWindowID id,
        const wxString& path = wxEmptyString,
        const wxString& message = wxFileSelectorPromptStr,
        const wxString& wildcard = wxFileSelectorDefaultWildcardStr,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxFLP_DEFAULT_STYLE,
        const wxValidator& validator = wxDefaultValidator,
        const wxString& name = wxFilePickerCtrlNameStr) :
        wxFilePickerCtrl(parent, id, path, message, VIDEOFILES, pos, size, style, validator, name)
    {}
    virtual ~VideoFilePickerCtrl() {}
};

PlayListItemVideoPanel::PlayListItemVideoPanel(wxWindow* parent, PlayListItemVideo* video,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
    _video = video;

	//(*Initialize(PlayListItemVideoPanel)
	wxFlexGridSizer* FlexGridSizer1;

	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	FlexGridSizer1 = new wxFlexGridSizer(0, 2, 0, 0);
	FlexGridSizer1->AddGrowableCol(1);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Video File:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	FlexGridSizer1->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FilePickerCtrl_VideoFile = new VideoFilePickerCtrl(this, ID_FILEPICKERCTRL1, wxEmptyString, _("Video File"), wxEmptyString, wxDefaultPosition, wxDefaultSize, wxFLP_FILE_MUST_EXIST|wxFLP_OPEN|wxFLP_USE_TEXTCTRL, wxDefaultValidator, _T("ID_FILEPICKERCTRL1"));
	FlexGridSizer1->Add(FilePickerCtrl_VideoFile, 1, wxALL|wxEXPAND, 5);
	FlexGridSizer1->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	Button_PositionWindow = new wxButton(this, ID_BUTTON1, _("Position Window"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
	FlexGridSizer1->Add(Button_PositionWindow, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText_Position = new wxStaticText(this, ID_STATICTEXT5, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	FlexGridSizer1->Add(StaticText_Position, 1, wxALL|wxEXPAND, 5);
	FlexGridSizer1->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	CheckBox_Topmost = new wxCheckBox(this, ID_CHECKBOX1, _("Topmost Window"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
	CheckBox_Topmost->SetValue(true);
	FlexGridSizer1->Add(CheckBox_Topmost, 1, wxALL|wxEXPAND, 5);
	FlexGridSizer1->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	CheckBox_SuppressVirtualMatrix = new wxCheckBox(this, ID_CHECKBOX2, _("Suppress Virtual Matrix"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
	CheckBox_SuppressVirtualMatrix->SetValue(false);
	FlexGridSizer1->Add(CheckBox_SuppressVirtualMatrix, 1, wxALL|wxEXPAND, 5);
	FlexGridSizer1->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	CheckBox_CacheVideo = new wxCheckBox(this, ID_CHECKBOX3, _("Cache Video"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX3"));
	CheckBox_CacheVideo->SetValue(false);
	FlexGridSizer1->Add(CheckBox_CacheVideo, 1, wxALL|wxEXPAND, 5);
	FlexGridSizer1->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	CheckBox_LoopVideo = new wxCheckBox(this, ID_CHECKBOX4, _("Loop Video"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX4"));
	CheckBox_LoopVideo->SetValue(false);
	FlexGridSizer1->Add(CheckBox_LoopVideo, 1, wxALL|wxEXPAND, 5);
	FlexGridSizer1->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	CheckBox_UseMediaPlayer = new wxCheckBox(this, ID_CHECKBOX5, _("Use Media Player"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX5"));
	CheckBox_UseMediaPlayer->SetValue(false);
	FlexGridSizer1->Add(CheckBox_UseMediaPlayer, 1, wxALL|wxEXPAND, 5);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Fade In MS:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	FlexGridSizer1->Add(StaticText3, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	SpinCtrl_FadeIn = new wxSpinCtrl(this, ID_SPINCTRL1, _T("0"), wxDefaultPosition, wxDefaultSize, 0, 0, 10000, 0, _T("ID_SPINCTRL1"));
	SpinCtrl_FadeIn->SetValue(_T("0"));
	FlexGridSizer1->Add(SpinCtrl_FadeIn, 1, wxALL|wxEXPAND, 5);
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Fade Out MS:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	FlexGridSizer1->Add(StaticText4, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	SpinCtrl_FadeOut = new wxSpinCtrl(this, ID_SPINCTRL2, _T("0"), wxDefaultPosition, wxDefaultSize, 0, 0, 10000, 0, _T("ID_SPINCTRL2"));
	SpinCtrl_FadeOut->SetValue(_T("0"));
	FlexGridSizer1->Add(SpinCtrl_FadeOut, 1, wxALL|wxEXPAND, 5);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Delay:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	FlexGridSizer1->Add(StaticText2, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	TextCtrl_Delay = new wxTextCtrl(this, ID_TEXTCTRL1, _("0.000"), wxDefaultPosition, wxDefaultSize, wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	FlexGridSizer1->Add(TextCtrl_Delay, 1, wxALL|wxEXPAND, 5);
	SetSizer(FlexGridSizer1);
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);

	Connect(ID_FILEPICKERCTRL1,wxEVT_COMMAND_FILEPICKER_CHANGED,(wxObjectEventFunction)&PlayListItemVideoPanel::OnFilePickerCtrl_VideoFileFileChanged);
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PlayListItemVideoPanel::OnButton_PositionWindowClick);
	Connect(ID_CHECKBOX5,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&PlayListItemVideoPanel::OnCheckBox_UseMediaPlayerClick);
	Connect(ID_TEXTCTRL1,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&PlayListItemVideoPanel::OnTextCtrl_DelayText);
	//*)

    FilePickerCtrl_VideoFile->SetFileName(wxFileName(video->GetVideoFile()));
    TextCtrl_Delay->SetValue(wxString::Format(wxT("%.3f"), (float)video->GetDelay() / 1000.0));
    CheckBox_Topmost->SetValue(video->GetTopMost());
    CheckBox_CacheVideo->SetValue(video->GetCacheVideo());
    CheckBox_LoopVideo->SetValue(video->GetLoopVideo());
    CheckBox_SuppressVirtualMatrix->SetValue(video->GetSuppressVirtualMatrix());
    SpinCtrl_FadeIn->SetValue(video->GetFadeInMS());
    SpinCtrl_FadeOut->SetValue(video->GetFadeOutMS());
	CheckBox_UseMediaPlayer->SetValue(video->GetUseMediaPlayer());

    SetWindowPositionText();
	ValidateWindow();
}

void PlayListItemVideoPanel::SetWindowPositionText()
{
    StaticText_Position->SetLabel(wxString::Format("X: %d, Y: %d, W: %d, H: %d", _video->GetPosition().x, _video->GetPosition().y, _video->GetSize().GetWidth(), _video->GetSize().GetHeight()));
}

PlayListItemVideoPanel::~PlayListItemVideoPanel()
{
	//(*Destroy(PlayListItemVideoPanel)
	//*)
    _video->SetVideoFile(FilePickerCtrl_VideoFile->GetFileName().GetFullPath().ToStdString());
    _video->SetDelay(wxAtof(TextCtrl_Delay->GetValue()) * 1000);
    _video->SetTopmost(CheckBox_Topmost->GetValue());
    _video->SetCacheVideo(CheckBox_CacheVideo->GetValue());
    _video->SetLoopVideo(CheckBox_LoopVideo->GetValue());
    _video->SetSuppressVirtualMatrix(CheckBox_SuppressVirtualMatrix->GetValue());
    _video->SetFadeInMS(SpinCtrl_FadeIn->GetValue());
    _video->SetFadeOutMS(SpinCtrl_FadeOut->GetValue());
	_video->SetUseMediaPlayer(CheckBox_UseMediaPlayer->GetValue());
}

void PlayListItemVideoPanel::OnButton_PositionWindowClick(wxCommandEvent& event)
{
    VideoWindowPositionDialog dlg(this, _video->GetOptions(), wxID_ANY, _video->GetPosition(), _video->GetSize());

    if (dlg.ShowModal() == wxID_OK)
    {
        _video->SetLocation(dlg.GetDesiredPosition(), dlg.GetDesiredSize());
    }

    SetWindowPositionText();
}

void PlayListItemVideoPanel::OnFilePickerCtrl_VideoFileFileChanged(wxFileDirPickerEvent& event)
{
    _video->SetVideoFile(FilePickerCtrl_VideoFile->GetFileName().GetFullPath().ToStdString());
    wxCommandEvent e(EVT_UPDATEITEMNAME);
    wxPostEvent(GetParent()->GetParent()->GetParent()->GetParent(), e);
}

void PlayListItemVideoPanel::OnTextCtrl_DelayText(wxCommandEvent& event)
{
}

void PlayListItemVideoPanel::OnCheckBox_UseMediaPlayerClick(wxCommandEvent& event)
{
	ValidateWindow();
}

void PlayListItemVideoPanel::ValidateWindow()
{
	if (CheckBox_UseMediaPlayer->GetValue())
	{
		CheckBox_CacheVideo->Enable(false);
		SpinCtrl_FadeIn->Enable(false);
		SpinCtrl_FadeOut->Enable(false);
	}
	else
	{
		CheckBox_CacheVideo->Enable();
		SpinCtrl_FadeIn->Enable();
		SpinCtrl_FadeOut->Enable();
	}
}
