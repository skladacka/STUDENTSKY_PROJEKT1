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

#include "graphics/opengl/xlGLCanvas.h"
#include "BulkEditControls.h"
#include "EffectPanelUtils.h"


//(*Headers(WarpPanel)
#include <wx/bmpbuttn.h>
#include <wx/choice.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/slider.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

class WarpPanel: public xlEffectPanel
{
	public:

		WarpPanel(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~WarpPanel();
		virtual void ValidateWindow() override;

		//(*Declarations(WarpPanel)
		BulkEditChoice* Choice_Warp_Treatment;
		BulkEditChoice* Choice_Warp_Type;
		BulkEditSlider* Slider_Warp_Cycle_Count;
		BulkEditSlider* Slider_Warp_Frequency;
		BulkEditSlider* Slider_Warp_Speed;
		BulkEditSlider* Slider_Warp_X;
		BulkEditSlider* Slider_Warp_Y;
		BulkEditTextCtrl* TextCtrl_Warp_Cycle_Count;
		BulkEditTextCtrl* TextCtrl_Warp_Frequency;
		BulkEditTextCtrl* TextCtrl_Warp_Speed;
		BulkEditTextCtrl* TextCtrl_Warp_X;
		BulkEditTextCtrl* TextCtrl_Warp_Y;
		BulkEditValueCurveButton* BitmapButton_Warp_X;
		BulkEditValueCurveButton* BitmapButton_Warp_Y;
		wxStaticText* StaticText10;
		wxStaticText* StaticText11;
		wxStaticText* StaticText12;
		wxStaticText* StaticText13;
		wxStaticText* StaticText14;
		wxStaticText* StaticText8;
		wxStaticText* StaticText9;
		wxTextCtrl* TextCtrl1;
		//*)

        void CheckTypeTreatment();

	protected:

		//(*Identifiers(WarpPanel)
		static const long ID_TEXTCTRL1;
		static const long ID_STATICTEXT8;
		static const long ID_CHOICE_Warp_Type;
		static const long ID_STATICTEXT9;
		static const long ID_CHOICE_Warp_Treatment_APPLYLAST;
		static const long ID_STATICTEXT10;
		static const long ID_SLIDER_Warp_X;
		static const long ID_VALUECURVE_Warp_X;
		static const long ID_TEXTCTRL_Warp_X;
		static const long ID_STATICTEXT11;
		static const long ID_SLIDER_Warp_Y;
		static const long ID_VALUECURVE_Warp_Y;
		static const long ID_TEXTCTRL_Warp_Y;
		static const long ID_STATICTEXT12;
		static const long ID_SLIDER_Warp_Cycle_Count;
		static const long ID_TEXTCTRL_Warp_Cycle_Count;
		static const long ID_STATICTEXT13;
		static const long ID_SLIDER_Warp_Speed;
		static const long ID_TEXTCTRL_Warp_Speed;
		static const long ID_STATICTEXT14;
		static const long ID_SLIDER_Warp_Frequency;
		static const long ID_TEXTCTRL_Warp_Frequency;
		//*)

	private:

		//(*Handlers(WarpPanel)
		void OnChoice_Warp_TypeSelect(wxCommandEvent& event);
		void OnChoice_Warp_TreatmentSelect(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};
