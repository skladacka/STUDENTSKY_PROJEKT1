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

 //(*Headers(FirePanel)
 #include <wx/panel.h>
 class wxBitmapButton;
 class wxCheckBox;
 class wxChoice;
 class wxFlexGridSizer;
 class wxSlider;
 class wxStaticText;
 class wxTextCtrl;
 //*)

#include "../BulkEditControls.h"
#include "EffectPanelUtils.h"

class FirePanel: public xlEffectPanel
{
	public:

		FirePanel(wxWindow* parent);
		virtual ~FirePanel();
		virtual void ValidateWindow() override;

		//(*Declarations(FirePanel)
		BulkEditCheckBox* CheckBox_Fire_GrowWithMusic;
		BulkEditChoice* Choice_Fire_Location;
		BulkEditSlider* Slider_Fire_Height;
		BulkEditSlider* Slider_Fire_HueShift;
		BulkEditSliderF1* Slider_Fire_GrowthCycles;
		BulkEditTextCtrl* TextCtrl_Fire_Height;
		BulkEditTextCtrlF1* TextCtrl_Fire_GrowthCycles;
		BulkEditValueCurveButton* BitmapButton_Fire_GrowthCyclesVC;
		BulkEditValueCurveButton* BitmapButton_Fire_HeightVC;
		BulkEditValueCurveButton* BitmapButton_Fire_HueShiftVC;
		wxStaticText* StaticText132;
		wxStaticText* StaticText193;
		wxStaticText* StaticText30;
		wxStaticText* StaticText83;
		xlLockButton* BitmapButton_FireGrow;
		xlLockButton* BitmapButton_FireHeight;
		xlLockButton* BitmapButton_FireWidth;
		xlLockButton* BitmapButton_Fire_GrowWithMusic;
		//*)

	protected:

		//(*Identifiers(FirePanel)
		static const long ID_STATICTEXT_Fire_Height;
		static const long ID_SLIDER_Fire_Height;
		static const long ID_VALUECURVE_Fire_Height;
		static const long IDD_TEXTCTRL_Fire_Height;
		static const long ID_BITMAPBUTTON_SLIDER_Fire_Height;
		static const long ID_STATICTEXT_Fire_HueShift;
		static const long ID_SLIDER_Fire_HueShift;
		static const long ID_VALUECURVE_Fire_HueShift;
		static const long IDD_TEXTCTRL_Fire_HueShift;
		static const long ID_BITMAPBUTTON_SLIDER_Fire_HueShift;
		static const long ID_STATICTEXT_Fire_GrowthCycles;
		static const long IDD_SLIDER_Fire_GrowthCycles;
		static const long ID_VALUECURVE_Fire_GrowthCycles;
		static const long ID_TEXTCTRL_Fire_GrowthCycles;
		static const long ID_BITMAPBUTTON_SLIDER_Fire_GrowthCycles;
		static const long ID_CHECKBOX_Fire_GrowWithMusic;
		static const long ID_BITMAPBUTTON_CHECKBOX_Fire_GrowWithMusic;
		static const long ID_STATICTEXT_Fire_Location;
		static const long ID_CHOICE_Fire_Location;
		//*)

	public:

		//(*Handlers(FirePanel)
		void OnCheckBox_Fire_GrowWithMusicClick(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};
