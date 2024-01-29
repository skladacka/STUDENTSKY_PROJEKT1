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

//(*Headers(ColorWashPanel)
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

class ColorWashPanel: public xlEffectPanel
{
public:

	ColorWashPanel(wxWindow* parent);
	virtual ~ColorWashPanel();
	virtual void ValidateWindow() override;

    //(*Declarations(ColorWashPanel)
    BulkEditCheckBox* CircularPaletteCheckBox;
	BulkEditCheckBox* HFadeCheckBox;
	BulkEditCheckBox* ShimmerCheckBox;
	BulkEditCheckBox* VFadeCheckBox;
	BulkEditSliderF1* SliderCycles;
	BulkEditTextCtrlF1* CyclesTextCtrl;
	BulkEditValueCurveButton* BitmapButton_ColorWash_CyclesVC;
	wxStaticText* StaticText5;
	xlLockButton* BitmapButton_ColorWashCount;
	xlLockButton* BitmapButton_ColorWashHFade;
	xlLockButton* BitmapButton_ColorWashVFade;
    BulkEditCheckBox* ReverseFadesCheckBox;
    //*)

    //(*Identifiers(ColorWashPanel)
	static const long ID_STATICTEXT_ColorWash_Cycles;
	static const long IDD_SLIDER_ColorWash_Cycles;
	static const long ID_VALUECURVE_ColorWash_Cycles;
	static const long ID_TEXTCTRL_ColorWash_Cycles;
	static const long ID_BITMAPBUTTON_SLIDER_ColorWash_Cycles;
	static const long ID_CHECKBOX_ColorWash_VFade;
	static const long ID_BITMAPBUTTON_CHECKBOX_ColorWash_VFade;
	static const long ID_CHECKBOX_ColorWash_HFade;
	static const long ID_BITMAPBUTTON_CHECKBOX_ColorWash_HFade;
	static const long ID_CHECKBOX_ColorWash_ReverseFades;
	static const long ID_CHECKBOX_ColorWash_Shimmer;
	static const long ID_CHECKBOX_ColorWash_CircularPalette;
    //*)

    //(*Handlers(ColorWashPanel)
    //*)


	DECLARE_EVENT_TABLE()
};
