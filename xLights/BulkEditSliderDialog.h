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

 //(*Headers(BulkEditSliderDialog)
 #include <wx/bmpbuttn.h>
 #include <wx/button.h>
 #include <wx/dialog.h>
 #include <wx/sizer.h>
 #include <wx/slider.h>
 #include <wx/stattext.h>
 #include <wx/textctrl.h>
 //*)

#include "BulkEditControls.h"
#include "ValueCurveButton.h"

class BulkEditSliderDialog: public wxDialog
{
    ValueCurveButton* _vcb;
    BESLIDERTYPE _type;

	public:

		BulkEditSliderDialog(wxWindow* parent, const std::string& label, int value, int min, int max, int pageSize, BESLIDERTYPE type, ValueCurveButton* vcb, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~BulkEditSliderDialog();

		//(*Declarations(BulkEditSliderDialog)
		ValueCurveButton* BitmapButton_VC;
		wxButton* Button_Cancel;
		wxButton* Button_Ok;
		wxSlider* Slider_BulkEdit;
		wxStaticText* StaticText_Label;
		wxTextCtrl* TextCtrl_BulkEdit;
		//*)

	protected:

		//(*Identifiers(BulkEditSliderDialog)
		static const long ID_STATICTEXT_BulkEdit;
		static const long ID_SLIDER_BulkEdit;
		static const long ID_VALUECURVE_BulkEdit;
		static const long ID_TEXTCTRL_BulkEdit;
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		//*)

	private:

		//(*Handlers(BulkEditSliderDialog)
		void OnButton_OkClick(wxCommandEvent& event);
		void OnButton_CancelClick(wxCommandEvent& event);
		void OnSlider_BulkEditCmdSliderUpdated(wxScrollEvent& event);
		void OnTextCtrl_BulkEditText(wxCommandEvent& event);
		void OnBitmapButton_VCClick(wxCommandEvent& event);
		//*)

        void OnVCChanged(wxCommandEvent& event);

		DECLARE_EVENT_TABLE()
};
