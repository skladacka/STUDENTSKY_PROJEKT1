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

//(*Headers(ControllerConnectionDialog)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
//*)

enum class controller_connection_bulkedit
{
    CEBE_CONTROLLERCONNECTION,
    CEBE_CONTROLLERCONNECTIONINCREMENT,
    CEBE_CONTROLLERGAMMA,
    CEBE_CONTROLLERCOLOURORDER,
    CEBE_CONTROLLERGROUPCOUNT,
    CEBE_CONTROLLERDIRECTION,
	CEBE_CONTROLLERSTARTNULLNODES,
	CEBE_CONTROLLERENDNULLNODES,
	CEBE_CONTROLLERBRIGHTNESS,
    CEBE_SMARTREMOTE,
    CEBE_CONTROLLERPROTOCOL,
    CEBE_SMARTREMOTETYPE
};

class wxXmlNode;
class Model;

class ControllerConnectionDialog: public wxDialog
{
    controller_connection_bulkedit _type;
    wxString _protocol;

	public:

		ControllerConnectionDialog(wxWindow* parent, controller_connection_bulkedit type, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~ControllerConnectionDialog();

		//(*Declarations(ControllerConnectionDialog)
		wxButton* Button_Cancel;
		wxButton* Button_Ok;
		wxCheckBox* CheckBox_Brightness;
		wxCheckBox* CheckBox_ColorOrder;
		wxCheckBox* CheckBox_EndNullNodes;
		wxCheckBox* CheckBox_Gamma;
		wxCheckBox* CheckBox_GroupCount;
		wxCheckBox* CheckBox_PixelDirection;
		wxCheckBox* CheckBox_StartNullNodes;
		wxChoice* Choice_Protocol;
		wxChoice* Choice_SmartRemote;
		wxChoice* ColorOrder;
		wxChoice* PixelDirection;
		wxSpinCtrl* Brightness;
		wxSpinCtrl* EndNullNodes;
		wxSpinCtrl* GroupCount;
		wxSpinCtrl* SpinCtrl_Port;
		wxSpinCtrl* StartNullNodes;
		wxStaticText* SmartRemote;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		//*)

        wxSpinCtrlDouble *Gamma;
    
        void Set(Model *m);
        void Get(wxXmlNode* controllerConnection, int strings);

	protected:

		//(*Identifiers(ControllerConnectionDialog)
		static const long ID_STATICTEXT1;
		static const long ID_CHOICE1;
		static const long ID_STATICTEXT2;
		static const long ID_SPINCTRL1;
		static const long ID_STATICTEXT3;
		static const long ID_CHOICE2;
		static const long ID_CHECKBOX1;
		static const long ID_PIXEL_reverse;
		static const long ID_CHECKBOX5;
		static const long ID_PIXEL_colorOrder;
		static const long ID_CHECKBOX2;
		static const long ID_PIXEL_startNullNodes;
		static const long ID_CHECKBOX7;
		static const long ID_SPINCTRL_EndNullNodes;
		static const long ID_CHECKBOX3;
		static const long ID_PIXEL_brightness;
		static const long ID_CHECKBOX4;
		static const long ID_PIXEL_gamma;
		static const long ID_CHECKBOX6;
		static const long ID_SPINCTRL2;
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		//*)

	private:

		//(*Handlers(ControllerConnectionDialog)
		void OnButton_OkClick(wxCommandEvent& event);
		void OnButton_CancelClick(wxCommandEvent& event);
		void OnPixelDirectionClick(wxCommandEvent& event);
		void OnCheckBox_NullNodesClick(wxCommandEvent& event);
		void OnCheckBox_GammaClick(wxCommandEvent& event);
		void OnCheckBox_BrightnessClick(wxCommandEvent& event);
		void OnCheckBox_ColorOrderClick(wxCommandEvent& event);
		void OnChoice_ProtocolSelect(wxCommandEvent& event);
		void OnCheckBox_GroupCountClick(wxCommandEvent& event);
		void OnCheckBox_EndNullNodesClick(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};
