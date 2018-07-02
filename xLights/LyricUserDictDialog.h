#ifndef LYRICUSERDICTDIALOG_H
#define LYRICUSERDICTDIALOG_H

//(*Headers(LyricUserDictDialog)
#include <wx/button.h>
#include <wx/combobox.h>
#include <wx/dialog.h>
#include <wx/grid.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

class PhonemeDictionary;

class LyricUserDictDialog: public wxDialog
{
	public:

		LyricUserDictDialog(PhonemeDictionary* dictionary, wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~LyricUserDictDialog();

		//(*Declarations(LyricUserDictDialog)
		wxButton* ButtonAddLyric;
		wxButton* ButtonDeleteRow;
		wxButton* ButtonLyricCancel;
		wxButton* ButtonLyricOK;
		wxComboBox* ComboBoxOldLyric;
		wxGrid* GridUserLyricDict;
		wxStaticText* StaticText1;
		wxTextCtrl* TextCtrlNewLyric;
		//*)

	protected:

		//(*Identifiers(LyricUserDictDialog)
		static const long ID_TEXTCTRL_NEW_LYRIC;
		static const long ID_STATICTEXT1;
		static const long ID_COMBOBOX_OLD_LYRIC;
		static const long ID_BUTTON_ADD_LYRIC;
		static const long ID_GRID_USER_LYRIC_DICT;
		static const long ID_BUTTON_DELETE_ROW;
		static const long ID_BUTTON_LYRIC_OK;
		static const long ID_BUTTON_LYRIC_CANCEL;
		//*)

	private:

        PhonemeDictionary * m_dictionary;

        void ReadUserDictionary();
        void WriteUserDictionary();
        void InsertRow(const wxString& text, wxArrayString phonemeList);
        bool DoesGridContain(const wxString& text);
        bool IsValidPhoneme(const wxString& text);

		//(*Handlers(LyricUserDictDialog)
		void OnButtonAddLyricClick(wxCommandEvent& event);
		void OnButtonDeleteRowClick(wxCommandEvent& event);
		void OnButtonLyricOKClick(wxCommandEvent& event);
		void OnButtonLyricCancelClick(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
