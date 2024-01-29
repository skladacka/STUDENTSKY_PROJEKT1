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

//(*Headers(LyricUserDictDialog)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/grid.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

class PhonemeDictionary;

class LyricUserDictDialog: public wxDialog
{
    void ValidateWindow();

	public:

		LyricUserDictDialog(PhonemeDictionary* dictionary, const wxString &showDirectory, wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~LyricUserDictDialog();

		//(*Declarations(LyricUserDictDialog)
		wxButton* ButtonAddLyric;
		wxButton* ButtonDeleteRow;
		wxButton* ButtonLyricCancel;
		wxButton* ButtonLyricOK;
		wxGrid* GridUserLyricDict;
		wxStaticText* StaticText1;
		wxTextCtrl* TextCtrlNewLyric;
		wxTextCtrl* TextCtrlOldLyric;
		wxTextCtrl* TextCtrlPhonems;
		//*)

	protected:

		//(*Identifiers(LyricUserDictDialog)
		static const long ID_TEXTCTRL_NEW_LYRIC;
		static const long ID_STATICTEXT1;
		static const long ID_TEXTCTRL_OLD_LYRIC;
		static const long ID_TEXTCTRL_PHEMONES;
		static const long ID_BUTTON_ADD_LYRIC;
		static const long ID_GRID_USER_LYRIC_DICT;
		static const long ID_BUTTON_DELETE_ROW;
		static const long ID_BUTTON_LYRIC_OK;
		static const long ID_BUTTON_LYRIC_CANCEL;
		//*)

	private:

        PhonemeDictionary* m_dictionary;
        wxString m_showDirectory;
        wxArrayString m_removeList;

        void ReadUserDictionary() const;
        void WriteUserDictionary() const;
        void InsertRow(const wxString& text, std::vector<wxArrayString> phonemeList) const;
        bool DoesGridContain(const wxString& text) const;
        bool IsValidPhoneme(const wxString& text) const;

		//(*Handlers(LyricUserDictDialog)
		void OnButtonAddLyricClick(wxCommandEvent& event);
		void OnButtonDeleteRowClick(wxCommandEvent& event);
		void OnButtonLyricOKClick(wxCommandEvent& event);
		void OnButtonLyricCancelClick(wxCommandEvent& event);
		void OnTextCtrlOldLyricText(wxCommandEvent& event);
		void OnGridUserLyricDictCellSelect(wxGridEvent& event);
		void OnGridUserLyricDictCellChanged(wxGridEvent& event);
		void OnTextCtrlNewLyricText(wxCommandEvent& event);
		void OnGridUserLyricDictLabelLeftClick(wxGridEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};
