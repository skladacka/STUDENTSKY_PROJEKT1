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

 //(*Headers(ButterflyPanel)
 #include <wx/panel.h>
 class wxBitmapButton;
 class wxChoice;
 class wxFlexGridSizer;
 class wxSlider;
 class wxStaticText;
 class wxTextCtrl;
 //*)

#include "../BulkEditControls.h"
#include "EffectPanelUtils.h"

class ButterflyPanel: public xlEffectPanel
{
	public:

		ButterflyPanel(wxWindow* parent);
		virtual ~ButterflyPanel();
		virtual void ValidateWindow() override;

		//(*Declarations(ButterflyPanel)
		BulkEditChoice* Choice_Butterfly_Colors;
		BulkEditChoice* Choice_Butterfly_Direction;
		BulkEditSlider* Slider_Butterfly_Chunks;
		BulkEditSlider* Slider_Butterfly_Skip;
		BulkEditSlider* Slider_Butterfly_Speed;
		BulkEditSlider* Slider_Butterfly_Style;
		BulkEditValueCurveButton* BitmapButton_Butterfly_Chunks;
		BulkEditValueCurveButton* BitmapButton_Butterfly_Skip;
		BulkEditValueCurveButton* BitmapButton_Butterfly_Speed;
		wxStaticText* StaticText175;
		wxStaticText* StaticText25;
		wxStaticText* StaticText26;
		wxStaticText* StaticText27;
		wxStaticText* StaticText28;
		wxStaticText* StaticText58;
		//*)

	protected:

		//(*Identifiers(ButterflyPanel)
		static const long ID_STATICTEXT_Butterfly_Colors;
		static const long ID_CHOICE_Butterfly_Colors;
		static const long ID_BITMAPBUTTON_CHOICE_Butterfly_Colors;
		static const long ID_STATICTEXT_Butterfly_Style;
		static const long ID_SLIDER_Butterfly_Style;
		static const long IDD_TEXTCTRL_Butterfly_Style;
		static const long ID_BITMAPBUTTON_SLIDER_Butterfly_Style;
		static const long ID_STATICTEXT_Butterfly_Chunks;
		static const long ID_SLIDER_Butterfly_Chunks;
		static const long ID_VALUECURVE_Butterfly_Chunks;
		static const long IDD_TEXTCTRL_Butterfly_Chunks;
		static const long ID_BITMAPBUTTON_SLIDER_Butterfly_Chunks;
		static const long ID_STATICTEXT_Butterfly_Skip;
		static const long ID_SLIDER_Butterfly_Skip;
		static const long ID_VALUECURVE_Butterfly_Skip;
		static const long IDD_TEXTCTRL_Butterfly_Skip;
		static const long ID_BITMAPBUTTON_SLIDER_Butterfly_Skip;
		static const long ID_STATICTEXT_Butterfly_Speed;
		static const long ID_SLIDER_Butterfly_Speed;
		static const long ID_VALUECURVE_Butterfly_Speed;
		static const long IDD_TEXTCTRL_Butterfly_Speed;
		static const long ID_BITMAPBUTTON_SLIDER_Butterfly_Speed;
		static const long ID_STATICTEXT_Butterfly_Direction;
		static const long ID_CHOICE_Butterfly_Direction;
		static const long ID_BITMAPBUTTON_CHOICE_Butterfly_Direction;
		//*)

	public:

		//(*Handlers(ButterflyPanel)
		//*)

    DECLARE_EVENT_TABLE()
};
