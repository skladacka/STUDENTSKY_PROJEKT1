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

//(*Headers(StrobePanel)
#include <wx/panel.h>
class wxBitmapButton;
class wxCheckBox;
class wxFlexGridSizer;
class wxSlider;
class wxStaticText;
class wxTextCtrl;
//*)

#include "../BulkEditControls.h"
#include "EffectPanelUtils.h"

class StrobePanel: public xlEffectPanel
{
	public:

		StrobePanel(wxWindow* parent);
		virtual ~StrobePanel();
		virtual void ValidateWindow() override;

		//(*Declarations(StrobePanel)
		BulkEditCheckBox* CheckBox_Strobe_Music;
		BulkEditSlider* Slider_Number_Strobes;
		BulkEditSlider* Slider_Strobe_Duration;
		BulkEditSlider* Slider_Strobe_Type;
		BulkEditTextCtrl* TextCtrl_Number_Strobes;
		BulkEditTextCtrl* TextCtrl_Strobe_Duration;
		BulkEditTextCtrl* TextCtrl_Strobe_Type;
		wxStaticText* StaticText110;
		wxStaticText* StaticText111;
		wxStaticText* StaticText112;
		xlLockButton* BitmapButton_NumberStrobes;
		xlLockButton* BitmapButton_StrobeDuration;
		xlLockButton* BitmapButton_Strobe_Music;
		xlLockButton* BitmapButton_Strobe_Type;
		//*)

	protected:

		//(*Identifiers(StrobePanel)
		static const long ID_STATICTEXT_Number_Strobes;
		static const long ID_SLIDER_Number_Strobes;
		static const long IDD_TEXTCTRL_Number_Strobes;
		static const long ID_BITMAPBUTTON_SLIDER_Number_Strobes;
		static const long ID_STATICTEXT_Strobe_Duration;
		static const long ID_SLIDER_Strobe_Duration;
		static const long IDD_TEXTCTRL_Strobe_Duration;
		static const long ID_BITMAPBUTTON_SLIDER_Strobe_Duration;
		static const long ID_STATICTEXT_Strobe_Type;
		static const long ID_SLIDER_Strobe_Type;
		static const long IDD_TEXTCTRL_Strobe_Type;
		static const long ID_BITMAPBUTTON_SLIDER_Strobe_Type;
		static const long ID_CHECKBOX_Strobe_Music;
		static const long ID_BITMAPBUTTON_CHECKBOX_Strobe_Music;
		//*)

	public:

		//(*Handlers(StrobePanel)
		void OnSlider_Strobe_TypeCmdScroll(wxScrollEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};
