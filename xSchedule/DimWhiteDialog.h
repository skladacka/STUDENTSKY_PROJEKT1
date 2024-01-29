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

 //(*Headers(DimWhiteDialog)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/spinctrl.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class OutputManager;

class DimWhiteDialog: public wxDialog
{
    std::string& _startChannel;
    size_t& _nodes;
    size_t& _dim;
    std::string& _description;
    bool& _enabled;
    OutputManager* _outputManager;

    void ValidateWindow();

    public:

		DimWhiteDialog(wxWindow* parent, OutputManager* outputManager, std::string& startChannel, size_t& channels, size_t& dim, std::string& description, bool& enabled, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~DimWhiteDialog();

		//(*Declarations(DimWhiteDialog)
		wxStaticText* StaticText_StartChannel;
		wxCheckBox* CheckBox_Enabled;
		wxButton* Button_Ok;
		wxTextCtrl* TextCtrl_Description;
		wxStaticText* StaticText2;
		wxSpinCtrl* SpinCtrl_Nodes;
		wxStaticText* StaticText1;
		wxStaticText* StaticText3;
		wxButton* Button_Cancel;
		wxSpinCtrl* SpinCtrl_Brightness;
		wxTextCtrl* TextCtrl_StartChannel;
		wxStaticText* StaticText4;
		//*)

	protected:

		//(*Identifiers(DimWhiteDialog)
		static const long ID_STATICTEXT1;
		static const long ID_TEXTCTRL2;
		static const long ID_STATICTEXT5;
		static const long ID_STATICTEXT2;
		static const long ID_SPINCTRL2;
		static const long ID_STATICTEXT3;
		static const long ID_SPINCTRL3;
		static const long ID_STATICTEXT4;
		static const long ID_TEXTCTRL1;
		static const long ID_CHECKBOX1;
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		//*)

	private:

		//(*Handlers(DimWhiteDialog)
		void OnButton_OkClick(wxCommandEvent& event);
		void OnButton_CancelClick(wxCommandEvent& event);
		void OnTextCtrl_StartChannelText(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

