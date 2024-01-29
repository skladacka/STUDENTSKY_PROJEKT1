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

 //(*Headers(CandlePanel)
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

class CandlePanel: public xlEffectPanel
{
	virtual void ValidateWindow() override;

	public:

		CandlePanel(wxWindow* parent);
		virtual ~CandlePanel();

		//(*Declarations(CandlePanel)
		BulkEditCheckBox* CheckBox_PerNode;
		BulkEditSlider* Slider_Candle_FlameAgility;
		BulkEditSlider* Slider_Candle_WindBaseline;
		BulkEditSlider* Slider_Candle_WindCalmness;
		BulkEditSlider* Slider_Candle_WindVariability;
		BulkEditTextCtrl* TextCtrl_Candle_FlameAgility;
		BulkEditTextCtrl* TextCtrl_Candle_WindBaseline;
		BulkEditTextCtrl* TextCtrl_Candle_WindCalmness;
		BulkEditTextCtrl* TextCtrl_Candle_WindVariability;
		BulkEditValueCurveButton* BitmapButton_Candle_FlameAgilityVC;
		BulkEditValueCurveButton* BitmapButton_Candle_WindBaselineVC;
		BulkEditValueCurveButton* BitmapButton_Candle_WindCalmnessVC;
		BulkEditValueCurveButton* BitmapButton_Candle_WindVariabilityVC;
		wxStaticText* StaticText132;
		wxStaticText* StaticText1;
		wxStaticText* StaticText30;
		wxStaticText* StaticText83;
		xlLockButton* BitmapButton_Cande_WindBaseline;
		xlLockButton* BitmapButton_Cande_WinfVariability;
		xlLockButton* BitmapButton_Candle_FlameAgility;
		xlLockButton* BitmapButton_Candle_WindCalmness;
		//*)

	protected:

		//(*Identifiers(CandlePanel)
		static const long ID_STATICTEXT_Candle_FlameAgility;
		static const long IDD_SLIDER_Candle_FlameAgility;
		static const long ID_VALUECURVE_Candle_FlameAgility;
		static const long ID_TEXTCTRL_Candle_FlameAgility;
		static const long ID_BITMAPBUTTON_SLIDER_Candle_FlameAgility;
		static const long ID_STATICTEXT_Candle_WindBaseline;
		static const long IDD_SLIDER_Candle_WindBaseline;
		static const long ID_VALUECURVE_Candle_WindBaseline;
		static const long ID_TEXTCTRL_Candle_WindBaseline;
		static const long ID_BITMAPBUTTON_SLIDER_Candle_WindBaseline;
		static const long ID_STATICTEXT_Candle_WindVariability;
		static const long IDD_SLIDER_Candle_WindVariability;
		static const long ID_VALUECURVE_Candle_WindVariability;
		static const long ID_TEXTCTRL_Candle_WindVariability;
		static const long ID_BITMAPBUTTON_SLIDER_Candle_WindVariability;
		static const long ID_STATICTEXT_Candle_WindCalmness;
		static const long IDD_SLIDER_Candle_WindCalmness;
		static const long ID_VALUECURVE_Candle_WindCalmness;
		static const long ID_TEXTCTRL_Candle_WindCalmness;
		static const long ID_BITMAPBUTTON_SLIDER_Candle_WindCalmness;
		static const long ID_CHECKBOX_PerNode;
		//*)

	public:

		//(*Handlers(CandlePanel)
		void OnCheckBox_Candle_GrowWithMusicClick(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};
