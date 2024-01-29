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
#include "../BulkEditControls.h"
#include "EffectPanelUtils.h"

class ShaderConfig;
class SequenceEelements;

//(*Headers(ShaderPanel)
#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include <wx/filepicker.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/slider.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

class ShaderPanel: public xlEffectPanel
{
    ShaderConfig* _shaderConfig = nullptr;

	public:

		ShaderPanel(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~ShaderPanel();
		virtual void ValidateWindow() override;

		//(*Declarations(ShaderPanel)
		BulkEditFilePickerCtrl* FilePickerCtrl1;
		BulkEditSlider* Slider_Shader_LeadIn;
		BulkEditSlider* Slider_Shader_Offset_X;
		BulkEditSlider* Slider_Shader_Offset_Y;
		BulkEditSlider* Slider_Shader_Zoom;
		BulkEditSliderF2* Slider_Shader_Speed;
		BulkEditTextCtrl* TextCtrl_Shader_LeadIn;
		BulkEditTextCtrl* TextCtrl_Shader_Offset_X;
		BulkEditTextCtrl* TextCtrl_Shader_Offset_Y;
		BulkEditTextCtrl* TextCtrl_Shader_Zoom;
		BulkEditTextCtrlF2* TextCtrl_Shader_Speed;
		BulkEditValueCurveButton* BitmapButton_Shader_Offset_X;
		BulkEditValueCurveButton* BitmapButton_Shader_Offset_Y;
		BulkEditValueCurveButton* BitmapButton_Shader_Speed;
		BulkEditValueCurveButton* BitmapButton_Shader_Zoom;
		wxButton* Button_Download;
		wxFlexGridSizer* FlexGridSizer1;
		wxFlexGridSizer* FlexGridSizer_Dynamic;
		wxStaticText* StaticText1;
		wxStaticText* StaticText_Shader_LeadIn;
		wxStaticText* StaticText_Shader_Offset_X;
		wxStaticText* StaticText_Shader_Offset_Y;
		wxStaticText* StaticText_Shader_Speed;
		wxStaticText* StaticText_Shader_Zoom;
		//*)

	protected:

		//(*Identifiers(ShaderPanel)
		static const long ID_STATICTEXT1;
		static const long ID_0FILEPICKERCTRL_IFS;
		static const long ID_BUTTON1;
		static const long ID_STATICTEXT2;
		static const long IDD_SLIDER_Shader_LeadIn;
		static const long ID_TEXTCTRL_Shader_LeadIn;
		static const long ID_STATICTEXT3;
		static const long ID_SLIDER_Shader_Speed;
		static const long ID_VALUECURVE_Shader_Speed;
		static const long IDD_TEXTCTRL_Shader_Speed;
		static const long ID_STATICTEXT4;
		static const long IDD_SLIDER_Shader_Offset_X;
		static const long ID_VALUECURVE_Shader_Offset_X;
		static const long ID_TEXTCTRL_Shader_Offset_X;
		static const long ID_STATICTEXT5;
		static const long IDD_SLIDER_Shader_Offset_Y;
		static const long ID_VALUECURVE_Shader_Offset_Y;
		static const long ID_TEXTCTRL_Shader_Offset_Y;
		static const long ID_STATICTEXT6;
		static const long IDD_SLIDER_Shader_Zoom;
		static const long ID_VALUECURVE_Shader_Zoom;
		static const long ID_TEXTCTRL_Shader_Zoom;
		//*)

		static const long ID_CANVAS;

#ifndef __WXOSX__
        xlGLCanvas *_preview;
#endif
        friend class ShaderEffect;

	private:

		//(*Handlers(ShaderPanel)
		void OnFilePickerCtrl1FileChanged(wxFileDirPickerEvent& event);
		void OnButton_DownloadClick(wxCommandEvent& event);
		//*)

        bool BuildUI(const wxString& filename, SequenceElements* sequenceElements);

		DECLARE_EVENT_TABLE()
};
