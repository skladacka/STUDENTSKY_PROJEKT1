#pragma once

/***************************************************************
 * This source files comes from the xLights project
 * https://www.xlights.org
 * https://github.com/smeighan/xLights
 * See the github commit history for a record of contributing
 * developers.
 * Copyright claimed based on commit dates recorded in Github
 * License: https://github.com/smeighan/xLights/blob/master/License.txt
 **************************************************************/

 //(*Headers(DMXPathPanel)
 #include <wx/panel.h>
 class wxBitmapButton;
 class wxButton;
 class wxChoice;
 class wxFlexGridSizer;
 class wxSlider;
 class wxStaticText;
 class wxTextCtrl;
 //*)

#include "../BulkEditControls.h"
#include "EffectPanelUtils.h"

class Model;

class DMXPathPanel: public xlEffectPanel
{
	public:

		DMXPathPanel(wxWindow* parent);
		virtual ~DMXPathPanel();
		virtual void ValidateWindow() override;

		//(*Declarations(DMXPathPanel)
		BulkEditChoice* Choice_DMXPath_Type;
		BulkEditSlider* Slider_DMXPath_Height;
		BulkEditSlider* Slider_DMXPath_Pan;
		BulkEditSlider* Slider_DMXPath_Rotation;
		BulkEditSlider* Slider_DMXPath_Tilt;
		BulkEditSlider* Slider_DMXPath_Width;
		BulkEditSlider* Slider_DMXPath_X;
		BulkEditSlider* Slider_DMXPath_Y;
		BulkEditTextCtrl* TextCtrl_DMXPath_Pan;
		BulkEditTextCtrl* TextCtrl_DMXPath_Rotation;
		BulkEditTextCtrl* TextCtrl_DMXPath_Tilt;
		BulkEditValueCurveButton* BitmapButton_DMXPath_PanVC;
		BulkEditValueCurveButton* BitmapButton_DMXPath_RotationVC;
		BulkEditValueCurveButton* BitmapButton_DMXPath_TiltVC;
		wxButton* Button_DMXPath_2D;
		wxFlexGridSizer* FlexGridSizer_Main;
		wxStaticText* StaticText10;
		wxStaticText* StaticText178;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxStaticText* StaticText8;
		wxStaticText* StaticText9;
		xlLockButton* BitmapButton_DMXPathHeight;
		xlLockButton* BitmapButton_DMXPathType;
		xlLockButton* BitmapButton_DMXPathWidth;
		xlLockButton* BitmapButton_DMXPathX;
		xlLockButton* BitmapButton_DMXPathY;
		xlLockButton* BitmapButton_DMXPath_Pan;
		xlLockButton* BitmapButton_DMXPath_Rotation;
		xlLockButton* BitmapButton_DMXPath_Tilt;
		//*)

	protected:

		//(*Identifiers(DMXPathPanel)
		static const long ID_STATICTEXT1;
		static const long ID_CHOICE_DMXPath_Type;
		static const long ID_BITMAPBUTTON_CHOICE_DMXPath_Type;
		static const long ID_STATICTEXT_DMXPath_Width;
		static const long ID_SLIDER_DMXPath_Width;
		static const long IDD_TEXTCTRL_DMXPath_Width;
		static const long ID_BITMAPBUTTON_SLIDER_DMXPath_Width;
		static const long ID_STATICTEXT_DMXPath_Height;
		static const long ID_SLIDER_DMXPath_Height;
		static const long IDD_TEXTCTRL_DMXPath_Height;
		static const long ID_BITMAPBUTTON_SLIDER_DMXPath_Height;
		static const long ID_STATICTEXT_DMXPath_X;
		static const long ID_SLIDER_DMXPath_X;
		static const long IDD_TEXTCTRL_DMXPath_X;
		static const long ID_BITMAPBUTTON_SLIDER_DMXPath_X;
		static const long ID_STATICTEXT_DMXPath_Y;
		static const long ID_SLIDER_DMXPath_Y;
		static const long IDD_TEXTCTRL_DMXPath_Y;
		static const long ID_BITMAPBUTTON_SLIDER_DMXPath_Y;
		static const long ID_STATICTEXT_DMXPath_Rotation;
		static const long ID_SLIDER_DMXPath_Rotation;
		static const long ID_VALUECURVE_DMXPath_Rotation;
		static const long IDD_TEXTCTRL_DMXPath_Rotation;
		static const long ID_BITMAPBUTTON_SLIDER_DMXPath_Rotation;
		static const long ID_STATICTEXT_DMXPath_Pan;
		static const long ID_SLIDER_DMXPath_Pan;
		static const long ID_VALUECURVE_DMXPath_Pan;
		static const long IDD_TEXTCTRL_DMXPath_Pan;
		static const long ID_BITMAPBUTTON_SLIDER_DMXPath_Pan;
		static const long ID_STATICTEXT_DMXPath_Tilt;
		static const long ID_SLIDER_DMXPath_Tilt;
		static const long ID_VALUECURVE_DMXPath_Tilt;
		static const long IDD_TEXTCTRL_DMXPath_Tilt;
		static const long ID_BITMAPBUTTON_SLIDER_DMXPath_Tilt;
		static const long ID_BUTTON_DMXPath_2D;
		//*)

	public:

		//(*Handlers(DMXPathPanel)
		void OnChoice_DMXPath_TypeSelect(wxCommandEvent& event);
		void OnButton_DMXPath_2DClick(wxCommandEvent& event);
		//*)


    DECLARE_EVENT_TABLE()
};
