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

 //(*Headers(ServoConfigDialog)
#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
//*)

class ServoConfigDialog : public wxDialog
{
	public:

		ServoConfigDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~ServoConfigDialog();

		//(*Declarations(ServoConfigDialog)
		wxCheckBox* CheckBox_16bits;
		wxSpinCtrl* SpinCtrl_NumMotion;
		wxSpinCtrl* SpinCtrl_NumServos;
		wxSpinCtrl* SpinCtrl_NumStatic;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		//*)

	protected:

		//(*Identifiers(ServoConfigDialog)
		static const long ID_STATICTEXT1;
		static const long ID_SPINCTRL_NumServos;
		static const long ID_STATICTEXT2;
		static const long ID_SPINCTRL_NumStatic;
		static const long ID_STATICTEXT3;
		static const long ID_SPINCTRL_NumMotion;
		static const long ID_CHECKBOX_16bits;
		//*)

	private:

		//(*Handlers(ServoConfigDialog)
		void OnSpinCtrl_NumServosChange(wxSpinEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

