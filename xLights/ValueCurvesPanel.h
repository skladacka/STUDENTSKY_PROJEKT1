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

//(*Headers(ValueCurvesPanel)
#include <wx/panel.h>
#include <wx/scrolwin.h>
#include <wx/sizer.h>
//*)

#include <wx/dir.h>
#include "DragValueCurveBitmapButton.h"

class ValueCurvesPanel: public wxPanel
{
    int ProcessPresetDir(wxDir& directory, bool subdirs);

	public:

		ValueCurvesPanel(
                        wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~ValueCurvesPanel();
		void UpdateValueCurveButtons(bool reload);

		//(*Declarations(ValueCurvesPanel)
		wxFlexGridSizer* FlexGridSizer1;
		wxFlexGridSizer* FlexGridSizer2;
		wxGridSizer* GridSizer1;
		wxPanel* Panel_Sizer;
		wxScrolledWindow* ScrolledWindow1;
		//*)

	protected:

		//(*Identifiers(ValueCurvesPanel)
		static const long ID_SCROLLEDWINDOW1;
		static const long ID_PANEL1;
		//*)

	private:
		//(*Handlers(ValueCurvesPanel)
		void OnResize(wxSizeEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

