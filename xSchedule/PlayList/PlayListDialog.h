#pragma once

/***************************************************************
 * This source files comes from the xLights project
 * https://www.xlights.org
 * https://github.com/xLightsSequencer/xLights
 * See the github commit history for a record of contributing
 * developers.
 * Copyright claimed based on commit dates recorded in Github
 * License: https://github.com/xLightsSequencer/xLights/blob/master/License.txt
 **************************************************************/

//(*Headers(PlayListDialog)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/filedlg.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/splitter.h>
#include <wx/stattext.h>
#include <wx/treectrl.h>
//*)

wxDECLARE_EVENT(EVT_UPDATEITEMNAME, wxCommandEvent);

class OutputManager;
class PlayList;
class wxXmlNode;
class PlayListStep;
class PlayListItem;
class ScheduleOptions;

class PlayListDialog: public wxDialog
{
    PlayList* _playlist = nullptr;
    PlayList* _savedState = nullptr;
    OutputManager* _outputManager = nullptr;
    ScheduleOptions* _options = nullptr;
    bool _dragging = false;

    void ValidateWindow();
    void PopulateTree(PlayList* playlist, PlayListStep* step, PlayListItem* item);
    int GetPos(const wxTreeItemId& item);
    void HighlightDropItem(wxTreeItemId* id);
    void DeleteSelectedItem();
    void SwapPage(wxNotebookPage* newpage, const std::string& text = "");
    wxTreeItemId FindStepTreeItem(PlayListStep* step);
    void AddItem(PlayList* playlist, PlayListStep* step, PlayListItem* newitem);
    void Clone();

    public:

		PlayListDialog(wxWindow* parent, OutputManager* outputManager, PlayList* playlist, ScheduleOptions* options, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~PlayListDialog();
        void UpdateTree();
        void OnTreeDragEnd(wxMouseEvent& event);
        void OnTreeDragQuit(wxMouseEvent& event);
        void OnTreeMouseMove(wxMouseEvent& event);

		//(*Declarations(PlayListDialog)
		wxButton* Button_AddAudio;
		wxButton* Button_AddFSEQ;
		wxButton* Button_Cancel;
		wxButton* Button_Clone;
		wxButton* Button_Delete;
		wxButton* Button_FSEQVideo;
		wxButton* Button_Ok;
		wxFileDialog* FileDialog1;
		wxNotebook* Notebook1;
		wxPanel* Panel1;
		wxPanel* Panel2;
		wxSplitterWindow* SplitterWindow1;
		wxStaticText* StaticText2;
		wxTreeCtrl* TreeCtrl_PlayList;
		//*)

	protected:

		//(*Identifiers(PlayListDialog)
		static const long ID_TREECTRL1;
		static const long ID_STATICTEXT2;
		static const long ID_BUTTON3;
		static const long ID_BUTTON4;
		static const long ID_BUTTON5;
		static const long ID_BUTTON7;
		static const long ID_BUTTON6;
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		static const long ID_PANEL1;
		static const long ID_NOTEBOOK1;
		static const long ID_PANEL2;
		static const long ID_SPLITTERWINDOW1;
		//*)

        static const long ID_MNU_CLONE;
        static const long ID_MNU_ADDSTEP;
        static const long ID_MNU_ADDESEQ;
        static const long ID_MNU_ADDFADE;
        static const long ID_MNU_ADDFSEQ;
        static const long ID_MNU_ADDSCREENMAP;
        static const long ID_MNU_ADDTEXT;
        static const long ID_MNU_ADDFILE;
        static const long ID_MNU_ADDFSEQVIDEO;
        static const long ID_MNU_ADDTEST;
        static const long ID_MNU_ADDMICROPHONE;
        static const long ID_MNU_ADDCOLOURORGAN;
        static const long ID_MNU_ADDRDS;
        static const long ID_MNU_ADDPROJECTOR;
        static const long ID_MNU_ADDALLOFF;
        static const long ID_MNU_ADDSETCOLOUR;
        static const long ID_MNU_ADDCOMMAND;
        static const long ID_MNU_ADDMQTT;
        static const long ID_MNU_ADDOSC;
        static const long ID_MNU_ADDPROCESS;
        static const long ID_MNU_ADDCURL;
        static const long ID_MNU_ADDSERIAL;
        static const long ID_MNU_ADDMIDI;
        static const long ID_MNU_ADDFPPEVENT;
        static const long ID_MNU_ADDVIDEO;
        static const long ID_MNU_ADDAUDIO;
        static const long ID_MNU_ADDARTNETTRIGGER;
        static const long ID_MNU_ADDIMAGE;
        static const long ID_MNU_ADDJUKEBOX;
        static const long ID_MNU_ADDDELAY;
        static const long ID_MNU_ADDDIM;
        static const long ID_MNU_DELETE;
        static const long ID_MNU_REMOVEEMPTYSTEPS;

        bool IsPlayList(wxTreeItemId id);
        bool IsPlayListStep(wxTreeItemId id);
        void OnTreeCtrlMenu(wxCommandEvent &event);
        void OnDropFiles(wxDropFilesEvent& event);

	private:

		//(*Handlers(PlayListDialog)
		void OnTextCtrl_PlayListNameText(wxCommandEvent& event);
		void OnTreeCtrl_PlayListSelectionChanged(wxTreeEvent& event);
		void OnTreeCtrl_PlayListBeginDrag(wxTreeEvent& event);
		void OnTreeCtrl_PlayListEndDrag(wxTreeEvent& event);
		void OnTreeCtrl_PlayListItemMenu(wxTreeEvent& event);
		void OnButton_CancelClick(wxCommandEvent& event);
		void OnButton_OkClick(wxCommandEvent& event);
		void OnTreeCtrl_PlayListKeyDown(wxTreeEvent& event);
		void OnNotebook1PageChanged(wxNotebookEvent& event);
		void OnButton_AddFSEQClick(wxCommandEvent& event);
		void OnButton_FSEQVideoClick(wxCommandEvent& event);
		void OnButton_AddAudioClick(wxCommandEvent& event);
		void OnButton_DeleteClick(wxCommandEvent& event);
		void OnButton_CloneClick(wxCommandEvent& event);
		//*)

        void UpdateItemName(wxCommandEvent& event){
            UpdateTree();
        }

		DECLARE_EVENT_TABLE()
};

