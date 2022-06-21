#ifndef TIPOFTHEDAYDIALOG_H
#define TIPOFTHEDAYDIALOG_H

//(*Headers(TipOfTheDayDialog)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/html/htmlwin.h>
#include <wx/sizer.h>
//*)

#include <wx/xml/xml.h>

class TipOfDayThread;
class TODTracker;
class xlCachedHtmlWindow;

class TipOfTheDayDialog: public wxDialog
{
    TipOfDayThread* _thread = nullptr;
    
    const std::string &GetTODXMLFile() const { return todFileLocation; }
    bool GetTODAtLevel(wxXmlDocument& doc, TODTracker& tracker, const std::string& level);
    std::string todFileLocation;
    
	public:

		TipOfTheDayDialog(const std::string& url = "", wxWindow* parent = nullptr, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
		virtual ~TipOfTheDayDialog();
        void SetTODXMLFile(const std::string &f) { todFileLocation = f; }
        bool DoTipOfDay();
        void PrepTipOfDay(wxWindow* notify);
        void ClearVisited();

		//(*Declarations(TipOfTheDayDialog)
		wxButton* Button_Next;
		wxFlexGridSizer* FlexGridSizer1;
		xlCachedHtmlWindow* HtmlWindow1;
		//*)

	protected:

		//(*Identifiers(TipOfTheDayDialog)
		static const long ID_HTMLWINDOW1;
		static const long ID_BUTTON1;
		//*)

		bool IsLevelGreaterOrEqualTo(const std::string& act, const std::string& min);
        std::string BuildURL(const std::string& url) const;
        void LoadURL(const std::string& url);

	private:

		//(*Handlers(TipOfTheDayDialog)
		void OnButton_NextClick(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
