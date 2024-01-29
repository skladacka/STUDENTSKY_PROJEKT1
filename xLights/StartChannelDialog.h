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

//(*Headers(StartChannelDialog)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/dialog.h>
#include <wx/radiobut.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
//*)

#include <string>
#include <map>

class Model;
class ModelManager;
class OutputManager;

class StartChannelDialog: public wxDialog
{
	public:

		StartChannelDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~StartChannelDialog();

		//(*Declarations(StartChannelDialog)
		wxButton* Button_Cancel;
		wxButton* Button_Ok;
		wxCheckBox* CheckBox_FromThisPreviewOnly;
		wxChoice* ChoiceController;
		wxChoice* ModelChoice;
		wxChoice* ipChoice;
		wxChoice* universeChoice;
		wxRadioButton* ControllerButton;
		wxRadioButton* ModelButton;
		wxRadioButton* NoneButton;
		wxRadioButton* StartModelButton;
		wxRadioButton* UniverseButton;
		wxSpinCtrl* StartChannel;
		//*)

        void Set(const wxString &start, const ModelManager &models, const std::string& preview, Model* model);
        void SetUniverseOptionsBasedOnIP(wxString ip);
        std::string Get();
        void UpdateModels(Model* model);
        OutputManager* _outputManager = nullptr;
        std::map<std::string, std::string> _modelsPreview;
        std::string _preview;
        Model* _model = nullptr;

	protected:

		//(*Identifiers(StartChannelDialog)
		static const long ID_SPINCTRL1;
		static const long ID_RADIOBUTTON1;
		static const long ID_RADIOBUTTON5;
		static const long ID_CHOICE3;
		static const long ID_CHOICE4;
		static const long ID_RADIOBUTTON3;
		static const long ID_CHOICE1;
		static const long ID_CHECKBOX1;
		static const long ID_RADIOBUTTON4;
		static const long ID_RADIOBUTTON6;
		static const long ID_CHOICE5;
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		//*)

	private:

		//(*Handlers(StartChannelDialog)
		void OnButtonSelect(wxCommandEvent& event);
		void OnipChoiceSelect(wxCommandEvent& event);
		void OnCheckBox_FromThisPreviewOnlyClick(wxCommandEvent& event);
		void OnButton_OkClick(wxCommandEvent& event);
		void OnButton_CancelClick(wxCommandEvent& event);
		void OnModelChoiceSelect(wxCommandEvent& event);
		void OnuniverseChoiceSelect(wxCommandEvent& event);
		void OnStartChannelChange(wxSpinEvent& event);
		void OnChoiceControllerSelect(wxCommandEvent& event);
		//*)

		void ValidateWindow();

		DECLARE_EVENT_TABLE()
};
