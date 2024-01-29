/***************************************************************
 * This source files comes from the xLights project
 * https://www.xlights.org
 * https://github.com/xLightsSequencer/xLights
 * See the github commit history for a record of contributing
 * developers.
 * Copyright claimed based on commit dates recorded in Github
 * License: https://github.com/xLightsSequencer/xLights/blob/master/License.txt
 **************************************************************/

#include "PreviewPane.h"
#include "LayoutGroup.h"
#include <wx/wx.h>

//(*InternalHeaders(PreviewPane)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(PreviewPane)
const long PreviewPane::ID_PANEL_PREVIEW = wxNewId();
//*)

BEGIN_EVENT_TABLE(PreviewPane,wxDialog)
	//(*EventTable(PreviewPane)
	//*)
    EVT_MOVE(PreviewPane::OnMoved)
END_EVENT_TABLE()

PreviewPane::PreviewPane(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(PreviewPane)
	wxFlexGridSizer* Panel_Sizer;

	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	SetMinSize(wxDLG_UNIT(parent,wxSize(100,100)));
	Panel_Sizer = new wxFlexGridSizer(0, 1, 0, 0);
	Panel_Sizer->AddGrowableCol(0);
	Panel_Sizer->AddGrowableRow(0);
	PreviewPanel = new wxPanel(this, ID_PANEL_PREVIEW, wxDefaultPosition, wxDefaultSize, 0, _T("ID_PANEL_PREVIEW"));
	PreviewPanelSizer = new wxFlexGridSizer(0, 1, 0, 0);
	PreviewPanelSizer->AddGrowableCol(0);
	PreviewPanelSizer->AddGrowableRow(0);
	PreviewPanel->SetSizer(PreviewPanelSizer);
	PreviewPanelSizer->Fit(PreviewPanel);
	PreviewPanelSizer->SetSizeHints(PreviewPanel);
	Panel_Sizer->Add(PreviewPanel, 1, wxALL|wxEXPAND, 2);
	SetSizer(Panel_Sizer);
	Panel_Sizer->Fit(this);
	Panel_Sizer->SetSizeHints(this);

	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&PreviewPane::OnClose);
	Connect(wxEVT_SIZE,(wxObjectEventFunction)&PreviewPane::OnResize);
	//*)
}

PreviewPane::~PreviewPane()
{
	//(*Destroy(PreviewPane)
	//*)
}


void PreviewPane::OnClose(wxCloseEvent& event)
{
    layout_grp->SetPreviewActive(false);
    layout_grp->GetMenuItem()->Check(false);
}

void PreviewPane::SetLayoutGroup( LayoutGroup* grp )
{
    layout_grp = grp;
    SetTitle(grp->GetName());
}

bool PreviewPane::GetActive()
{
    return !layout_grp->GetPreviewHidden();
}

void PreviewPane::OnResize(wxSizeEvent& event)
{
    layout_grp->SetPreviewSize(event.GetSize());
    event.Skip();
}

void PreviewPane::OnMoved(wxMoveEvent& event)
{
    layout_grp->SetPreviewPosition(GetPosition());
    event.Skip();
}
