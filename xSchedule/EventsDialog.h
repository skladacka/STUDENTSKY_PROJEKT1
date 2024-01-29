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

//(*Headers(EventsDialog)
#include <wx/listctrl.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class OutputManager;
class ScheduleOptions;

class EventsDialog: public wxDialog
{
    ScheduleOptions* _scheduleOptions;
    OutputManager* _outputManager;
    void ValidateWindow();
    void LoadList();
    void EditSelected();

	public:

		EventsDialog(wxWindow* parent, OutputManager* outputManager, ScheduleOptions* scheduleOptions ,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~EventsDialog();

		//(*Declarations(EventsDialog)
		wxButton* Button_Delete;
		wxButton* Button_Edit;
		wxButton* Button_Add;
		wxButton* Button_Close;
		wxListView* ListView_Events;
		//*)

	protected:

		//(*Identifiers(EventsDialog)
		static const long ID_LISTVIEW1;
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		static const long ID_BUTTON3;
		static const long ID_BUTTON4;
		//*)

	private:

		//(*Handlers(EventsDialog)
		void OnButton_CloseClick(wxCommandEvent& event);
		void OnButton_DeleteClick(wxCommandEvent& event);
		void OnButton_EditClick(wxCommandEvent& event);
		void OnButton_AddClick(wxCommandEvent& event);
		void OnListView_EventsItemSelect(wxListEvent& event);
		void OnListView_EventsItemActivated(wxListEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

