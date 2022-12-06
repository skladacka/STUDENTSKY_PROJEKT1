/***************************************************************
 * This source files comes from the xLights project
 * https://www.xlights.org
 * https://github.com/smeighan/xLights
 * See the github commit history for a record of contributing
 * developers.
 * Copyright claimed based on commit dates recorded in Github
 * License: https://github.com/smeighan/xLights/blob/master/License.txt
 **************************************************************/

#include "SubBufferPanel.h"

#include <wx/dcbuffer.h>
#include <wx/arrstr.h>
#include "BufferSizeDialog.h"
#include "xLightsApp.h"
#include "xLightsMain.h"
#include "sequencer/MainSequencer.h"
#include "UtilFunctions.h"

wxDEFINE_EVENT(SUBBUFFER_RANGE_CHANGED, wxCommandEvent);

BEGIN_EVENT_TABLE(SubBufferPanel, wxWindow)
EVT_MOTION(SubBufferPanel::mouseMoved)
EVT_LEFT_DOWN(SubBufferPanel::mouseLeftDown)
EVT_LEFT_UP(SubBufferPanel::mouseLeftUp)
EVT_LEFT_DCLICK(SubBufferPanel::mouseDblClick)
EVT_PAINT(SubBufferPanel::Paint)
EVT_CONTEXT_MENU(SubBufferPanel::ContextMenu)
END_EVENT_TABLE()

SubBufferPanel::SubBufferPanel(wxPanel* parent, bool usevc, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
    : wxWindow(parent, id, pos, size, style, "ID_CUSTOM_SubBuffer"), xlCustomControl()
{
    _usevc = usevc;
    x1 = y1 = 0.0;
    x2 = y2 = 100.0;
    x = 0.0;
    y = 0.0;
    draggingHandle = -1;
    SetBackgroundStyle(wxBG_STYLE_SYSTEM);
}

SubBufferPanel::~SubBufferPanel()
{
}

void SubBufferPanel::SendChangeEvent() {
    wxCommandEvent ev(SUBBUFFER_RANGE_CHANGED, GetId());
    ev.SetString(GetValue());
    GetEventHandler()->ProcessEvent(ev);
}

bool SubBufferPanel::HasVC() const
{
    return x1vc.find("Active=TRUE") != std::string::npos ||
        x2vc.find("Active=TRUE") != std::string::npos ||
        y1vc.find("Active=TRUE") != std::string::npos ||
        y2vc.find("Active=TRUE") != std::string::npos ||
        xvc.find("Active=TRUE") != std::string::npos ||
        yvc.find("Active=TRUE") != std::string::npos;
}

void SubBufferPanel::SetDefaults()
{
    x1 = y1 = 0.0;
    x2 = y2 = 100.0;
    x = 0.0;
    y = 0.0;
    x1vc = "";
    x2vc = "";
    y1vc = "";
    y2vc = "";
    xvc = "";
    yvc = "";
    SendChangeEvent();
    Refresh();
}

void SubBufferPanel::SetValue(const std::string& val)
{

    wxString sb = val;
    sb.Replace("Max", "yyz");

    wxArrayString v = wxSplit(sb, 'x');

    if (v.size() < 1) {
        x1 = 0;
    }
    else if (v[0].Contains("Active=TRUE")) {
        v[0].Replace("yyz", "Max");
        x1vc = v[0];
        x1 = 0;
    }
    else {
        x1 = v.size() > 0 ? wxAtof(v[0]) : 0.0;
    }

    if (v.size() < 2) {
        y1 = 0;
    }
    else if (v[1].Contains("Active=TRUE")) {
        v[1].Replace("yyz", "Max");
        y1vc = v[1];
        y1 = 0;
    }
    else {
        y1 = v.size() > 1 ? wxAtof(v[1]) : 0.0;
    }

    if (v.size() < 3) {
        x2 = 100;
    }
    else if (v[2].Contains("Active=TRUE")) {
        v[2].Replace("yyz", "Max");
        x2vc = v[2];
        x2 = 100.0;
    }
    else {
        x2 = v.size() > 2 ? wxAtof(v[2]) : 100.0;
    }

    if (v.size() < 4) {
        y2 = 100;
    }
    else if (v[3].Contains("Active=TRUE")) {
        v[3].Replace("yyz", "Max");
        y2vc = v[3];
        y2 = 100.0;
    }
    else {
        y2 = v.size() > 3 ? wxAtof(v[3]) : 100.0;
    }

    if (v.size() < 5) {
        x = 0;
    }
    else if (v[4].Contains("Active=TRUE")) {
        v[4].Replace("yyz", "Max");
        xvc = v[4];
        x = 0.0;
    }
    else {
        x = v.size() > 4 ? wxAtof(v[4]) : 0.0;
    }

    if (v.size() < 6) {
        y = 0;
    }
    else if (v[5].Contains("Active=TRUE")) {
        v[5].Replace("yyz", "Max");
        yvc = v[5];
        y = 0.0;
    }
    else {
        y = v.size() > 5 ? wxAtof(v[5]) : 0.0;
    }

    if (x1 > x2) std::swap(x1, x2);
    if (y1 > y2) std::swap(y1, y2);

    Refresh();
}

std::string SubBufferPanel::GetValue() const
{

    bool fx1vc = wxString(x1vc).Contains("Active=TRUE");
    bool fx2vc = wxString(x2vc).Contains("Active=TRUE");
    bool fy1vc = wxString(y1vc).Contains("Active=TRUE");
    bool fy2vc = wxString(y2vc).Contains("Active=TRUE");
    bool fxvc = wxString(xvc).Contains("Active=TRUE");
    bool fyvc = wxString(yvc).Contains("Active=TRUE");

    if (std::abs(x1) > 0.001
        || std::abs(y1) > 0.001
        || std::abs(100.0 - x2) > 0.001
        || std::abs(100.0 - y2) > 0.001
        || fx1vc
        || fx2vc
        || fy1vc
        || fy2vc
        || fxvc
        || fyvc
        ) {

        std::string x1s = wxString::Format("%.2f", x1).ToStdString();
        std::string x2s = wxString::Format("%.2f", x2).ToStdString();
        std::string y1s = wxString::Format("%.2f", y1).ToStdString();
        std::string y2s = wxString::Format("%.2f", y2).ToStdString();
        std::string xs = wxString::Format("%.2f", x).ToStdString();
        std::string ys = wxString::Format("%.2f", y).ToStdString();

        return wxString::Format("%sx%sx%sx%sx%sx%s", fx1vc ? x1vc : x1s, fy1vc ? y1vc : y1s, fx2vc ? x2vc : x2s, fy2vc ? y2vc : y2s, fxvc ? xvc : xs, fyvc ? yvc : ys).ToStdString();
    }
    return "";
}

inline bool IsWithin(int mx, int my, int x, int y)
{
    return mx >= (x - 2) && mx <= (x + 2)
        && my >= (y - 2) && my <= (y + 2);
}

void SubBufferPanel::ContextMenu(wxContextMenuEvent& event)
{
    wxMenu menu;
    menu.Append(wxNewId(), "Full Buffer");
    menu.AppendSeparator();
    wxMenu* halves = new wxMenu();
    halves->Append(wxNewId(), "Left Half");
    halves->Append(wxNewId(), "Right Half");
    halves->AppendSeparator();
    halves->Append(wxNewId(), "Top Half");
    halves->Append(wxNewId(), "Bottom Half");
    menu.AppendSubMenu(halves, "Halves");
    wxMenu* thirds = new wxMenu();
    thirds->Append(wxNewId(), "Left Third");
    thirds->Append(wxNewId(), "Middle Third");
    thirds->Append(wxNewId(), "Right Third");
    thirds->AppendSeparator();
    thirds->Append(wxNewId(), "Top Third");
    thirds->Append(wxNewId(), "Center Third");
    thirds->Append(wxNewId(), "Bottom Third");
    menu.AppendSubMenu(thirds, "Thirds");
    wxMenu* quarters = new wxMenu();
    quarters->Append(wxNewId(), "Top Left");
    quarters->Append(wxNewId(), "Top Right");
    quarters->Append(wxNewId(), "Bottom Left");
    quarters->Append(wxNewId(), "Bottom Right");
    menu.AppendSubMenu(quarters, "Quarters");
    if (_usevc) // we only do this in the sequencer. In the submodel dialog we dont want this menu option
    {
        menu.AppendSeparator();
        menu.Append(wxNewId(), "Oversize");
    }
    menu.AppendSeparator();
    menu.Append(wxNewId(), "Edit");
    menu.AppendSeparator();
    if (_usevc) // we only do this in the sequencer. In the submodel dialog we dont want this menu option
        menu.Append(wxNewId(), "Apply to selected effects");
    menu.Connect(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&SubBufferPanel::MenuItemSelected, nullptr, this);
    quarters->Connect(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&SubBufferPanel::MenuItemSelected, nullptr, this);
    thirds->Connect(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&SubBufferPanel::MenuItemSelected, nullptr, this);
    halves->Connect(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&SubBufferPanel::MenuItemSelected, nullptr, this);

    PopupMenu(&menu);
}

void SubBufferPanel::MenuItemSelected(wxCommandEvent &event) {
    wxMenu *m = (wxMenu*)event.GetEventObject();
    wxMenuItem *item = m->FindItem(event.GetId());
    if (item != nullptr) {
        wxString nm = item->GetItemLabel();
        if (nm == "Full Buffer") {
            x1 = y1 = 0.0;
            x2 = y2 = 100.0;
            x1vc = "";
            x2vc = "";
            y1vc = "";
            y2vc = "";
            x = 0.0;
            y = 0.0;
            xvc = "";
            yvc = "";
        }
        else if (nm == "Top Half") {
            x1 = 0.0;
            y1 = 50.0;
            x2 = y2 = 100.0;
            x1vc = "";
            x2vc = "";
            y1vc = "";
            y2vc = "";
            x = 0.0;
            y = 0.0;
            xvc = "";
            yvc = "";
        }
        else if (nm == "Bottom Half") {
            x1 = y1 = 0.0;
            y2 = 50.0;
            x2 = 100.0;
            x1vc = "";
            x2vc = "";
            y1vc = "";
            y2vc = "";
            x = 0.0;
            y = 0.0;
            xvc = "";
            yvc = "";
        }
        else if (nm == "Left Half") {
            x1 = y1 = 0.0;
            y2 = 100.0;
            x2 = 50.0;
            x1vc = "";
            x2vc = "";
            y1vc = "";
            y2vc = "";
            x = 0.0;
            y = 0.0;
            xvc = "";
            yvc = "";
        }
        else if (nm == "Right Half") {
            x1 = 50.0;
            y1 = 0.0;
            x2 = y2 = 100.0;
            x1vc = "";
            x2vc = "";
            y1vc = "";
            y2vc = "";
            x = 0.0;
            y = 0.0;
            xvc = "";
            yvc = "";
        }
        else if (nm == "Left Third") {
            x1 = y1 = 0.0;
            y2 = 100.0;
            x2 = 33.33f;
            x1vc = "";
            x2vc = "";
            y1vc = "";
            y2vc = "";
            x = 0.0;
            y = 0.0;
            xvc = "";
            yvc = "";
        }
        else if (nm == "Top Third") {
            x1 = 0.0f;
            y1 = 66.66f;
            x2 = 100.0f;
            y2 = 100.0f;
            x1vc = "";
            x2vc = "";
            y1vc = "";
            y2vc = "";
            x = 0.0;
            y = 0.0;
            xvc = "";
            yvc = "";
        }
        else if (nm == "Middle Third") {
            x1 = 33.33f;
            y1 = 0.0;
            y2 = 100.0;
            x2 = 66.66f;
            x1vc = "";
            x2vc = "";
            y1vc = "";
            y2vc = "";
            x = 0.0;
            y = 0.0;
            xvc = "";
            yvc = "";
        }
        else if (nm == "Center Third") {
            y1 = 33.33f;
            x1 = 0.0;
            x2 = 100.0;
            y2 = 66.66f;
            x1vc = "";
            x2vc = "";
            y1vc = "";
            y2vc = "";
            x = 0.0;
            y = 0.0;
            xvc = "";
            yvc = "";
        }
        else if (nm == "Right Third") {
            x1 = 66.66f;
            y1 = 0.0;
            x2 = y2 = 100.0;
            x1vc = "";
            x2vc = "";
            y1vc = "";
            y2vc = "";
            x = 0.0;
            y = 0.0;
            xvc = "";
            yvc = "";
        }
        else if (nm == "Bottom Third") {
            y2 = 33.33f;
            x1 = y1 = 0.0;
            x2 = 100.0;
            x1vc = "";
            x2vc = "";
            y1vc = "";
            y2vc = "";
            x = 0.0;
            y = 0.0;
            xvc = "";
            yvc = "";
        }
        else if (nm == "Top Left") {
            x1 = 0.0;
            y1 = 50.0;
            x2 = 50.0;
            y2 = 100.0;
            x1vc = "";
            x2vc = "";
            y1vc = "";
            y2vc = "";
            x = 0.0;
            y = 0.0;
            xvc = "";
            yvc = "";
        }
        else if (nm == "Bottom Left") {
            x1 = 0.0;
            y1 = 0.0;
            y2 = 50.0;
            x2 = 50.0;
            x1vc = "";
            x2vc = "";
            y1vc = "";
            y2vc = "";
            x = 0.0;
            y = 0.0;
            xvc = "";
            yvc = "";
        }
        else if (nm == "Top Right") {
            x1 = 50.0;
            y1 = 50.0;
            x2 = 100.0;
            y2 = 100.0;
            x1vc = "";
            x2vc = "";
            y1vc = "";
            y2vc = "";
            x = 0.0;
            y = 0.0;
            xvc = "";
            yvc = "";
        }
        else if (nm == "Bottom Right") {
            x1 = 50.0;
            y1 = 0.0;
            y2 = 50.0;
            x2 = 100.0;
            x1vc = "";
            x2vc = "";
            y1vc = "";
            y2vc = "";
            x = 0.0;
            y = 0.0;
            xvc = "";
            yvc = "";
        }
        else if (nm == "Oversize") {
            x1 = -100.0;
            y1 = -100.0;
            x2 = y2 = 200.0;
            x1vc = "";
            x2vc = "";
            y1vc = "";
            y2vc = "";
            x = 0.0;
            y = 0.0;
            xvc = "";
            yvc = "";
        }
        else if (nm == "Edit")
        {
            BufferSizeDialog bsd(this, _usevc);
            bsd.SetSizes(y2, x1, y1, x2, y2vc, x1vc, y1vc, x2vc, x, y, xvc, yvc);
            OptimiseDialogPosition(&bsd);

            if (bsd.ShowModal() == wxID_OK)
            {
                x1 = bsd.SpinCtrl_Left->GetValue();
                x2 = bsd.SpinCtrl_Right->GetValue();
                y2 = bsd.SpinCtrl_Top->GetValue();
                y1 = bsd.SpinCtrl_Bottom->GetValue();
                x = bsd.SpinCtrl_XC->GetValue();
                y = bsd.SpinCtrl_YC->GetValue();
                x1vc = bsd.ValueCurve_Left->GetValue()->Serialise();
                x2vc = bsd.ValueCurve_Right->GetValue()->Serialise();
                y2vc = bsd.ValueCurve_Top->GetValue()->Serialise();
                y1vc = bsd.ValueCurve_Bottom->GetValue()->Serialise();
                xvc = bsd.ValueCurve_XC->GetValue()->Serialise();
                yvc = bsd.ValueCurve_YC->GetValue()->Serialise();
            }
            else
            {
                return;
            }
        }
        else if (nm == "Apply to selected effects")
        {
            xLightsApp::GetFrame()->GetMainSequencer()->ApplyEffectSettingToSelected("", "B_CUSTOM_SubBuffer", GetValue(), nullptr, "");
        }

        SendChangeEvent();
        Refresh();
    }
}

int SubBufferPanel::OverMouseHandle(wxMouseEvent& event)
{
    wxSize size = GetSize();
    float startX = size.GetWidth() / 10.0;
    float startY = size.GetHeight() / 10.0;
    float bw = size.GetWidth() * 0.8;
    float bh = size.GetHeight() * 0.8;

    float x1b = ((x1 + x) * bw) / 100.0 + startX;
    float x2b = ((x2 + x) * bw) / 100.0 + startX;
    float y1b = ((100.0 - y1 - y) * bh) / 100.0 + startY;
    float y2b = ((100.0 - y2 - y) * bh) / 100.0 + startY;

    if (IsWithin(event.GetX(), event.GetY(), x1b, y1b)) {
        return 0;
    }
    else if (IsWithin(event.GetX(), event.GetY(), x1b, y2b)) {
        return 1;
    }
    else if (IsWithin(event.GetX(), event.GetY(), x2b, y2b)) {
        return 2;
    }
    else if (IsWithin(event.GetX(), event.GetY(), x2b, y1b)) {
        return 3;
    }

    if (event.GetX() > x1b && event.GetX() < x2b && event.GetY() > y2b && event.GetY() < y1b) {
        return 4;
    }

    return -1;
}

void SubBufferPanel::mouseDblClick(wxMouseEvent& event)
{
    SetDefaults();
}

void SubBufferPanel::mouseLeftDown(wxMouseEvent& event)
{
    if (!IsEnabled() || HasVC()) {
        return;
    }
    if (draggingHandle == -1) {
        draggingHandle = OverMouseHandle(event);
    }
    if (draggingHandle != -1) {
        CaptureMouse();
    }
    _startMovePos = event.GetPosition();
    _start1 = wxPoint(x1, y1);
    _start2 = wxPoint(x2, y2);
}

void SubBufferPanel::mouseLeftUp(wxMouseEvent& event)
{
    if (draggingHandle >= 0) {
        mouseMoved(event);
        ReleaseMouse();
        draggingHandle = -1;
        Refresh();
    }
    if (!IsEnabled()) {
        return;
    }
}

void SubBufferPanel::Convert(float& xx, float& yy, wxMouseEvent& event)
{
    wxSize size = GetSize();
    float startX = size.GetWidth() / 10.0;
    float startY = size.GetHeight() / 10.0;
    float bw = size.GetWidth() * 0.8;
    float bh = size.GetHeight() * 0.8;

    xx = (event.GetX() - startX) * 100.0 / bw;
    yy = 100.0 - (event.GetY() - startY) * 100.0 / bh;
}

void SubBufferPanel::mouseMoved(wxMouseEvent& event)
{
    if (!IsEnabled()) {
        return;
    }

    float dummy;
    switch (draggingHandle) {
    case 0:
        if (event.ControlDown() || event.CmdDown()) {
            if (event.ShiftDown()) {
                // y only
                Convert(dummy, y1, event);
                y1 -= y;
            }
            else {
                // x only
                Convert(x1, dummy, event);
                x1 -= x;
            }
        }
        else {
            Convert(x1, y1, event);
            x1 -= x;
            y1 -= y;
        }
        break;
    case 1:
        if (event.ControlDown() || event.CmdDown()) {
            if (event.ShiftDown()) {
                // y only
                Convert(dummy, y2, event);
                y2 -= y;
            }
            else {
                // x only
                Convert(x1, dummy, event);
                x1 -= x;
            }
        }
        else {
            Convert(x1, y2, event);
            x1 -= x;
            y2 -= y;
        }
        break;
    case 2:
        if (event.ControlDown() || event.CmdDown()) {
            if (event.ShiftDown()) {
                // y only
                Convert(dummy, y2, event);
                y2 -= y;
            }
            else {
                // x only
                Convert(x2, dummy, event);
                x2 -= x;
            }
        }
        else {
            Convert(x2, y2, event);
            x2 -= x;
            y2 -= y;
        }
        break;
    case 3:
        if (event.ControlDown() || event.CmdDown()) {
            if (event.ShiftDown()) {
                // y only
                Convert(dummy, y1, event);
                y1 -= y;
            }
            else {
                // x only
                Convert(x2, dummy, event);
                x2 -= x;
            }
        }
        else {
            Convert(x2, y1, event);
            x2 -= x;
            y1 -= y;
        }
        break;
    case 4:
    {
        wxSize size = GetSize();
        float bw = size.GetWidth() * 0.8;
        float bh = size.GetHeight() * 0.8;
        int xx = (event.GetX() - _startMovePos.x) * 100 / bw;
        int yy = (event.GetY() - _startMovePos.y) * 100 / bh;

        if (event.ControlDown() && !event.ShiftDown()) {
            x1 = _start1.x + xx;
            x2 = _start2.x + xx;
        }
        else if (event.ControlDown() && event.ShiftDown()) {
            y1 = _start1.y - yy;
            y2 = _start2.y - yy;
        }
        else {
            x1 = _start1.x + xx;
            x2 = _start2.x + xx;
            y1 = _start1.y - yy;
            y2 = _start2.y - yy;
        }
    }
    break;
    default: {
        int i = OverMouseHandle(event);
        if (i > -1 && !HasVC()) {
            switch (i) {
            case 0:
            case 2:
                SetCursor(wxCURSOR_SIZENESW);
                break;
            case 1:
            case 3:
                SetCursor(wxCURSOR_SIZENWSE);
                break;
            case 4:
                SetCursor(wxCURSOR_SIZING);
                break;
            }
        }
        else {
            SetCursor(wxCURSOR_DEFAULT);
        }
    }
    }
    if (draggingHandle >= 0) {

        // any dragging removes the value curves
        x1vc = "";
        x2vc = "";
        y1vc = "";
        y2vc = "";
        xvc = "";
        yvc = "";

        if (x2 < x1) std::swap(x1, x2);
        if (y2 < y1) std::swap(y1, y2);
        SendChangeEvent();
        Refresh();
    }
}

void SubBufferPanel::Paint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
#ifdef __LINUX__
    if (!IsShownOnScreen()) return;
#endif

    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.SetBrush(*wxGREY_BRUSH);
    wxSize size = GetSize();
    dc.DrawRectangle(0, 0, size.GetWidth(), size.GetHeight());
    if (IsEnabled()) {
        dc.SetBrush(*wxBLACK_BRUSH);
        dc.SetPen(*wxBLACK_PEN);
    }
    else {
        dc.SetBrush(*wxGREY_BRUSH);
        dc.SetPen(*wxGREY_PEN);
    }
    float startX = size.GetWidth() / 10.0;
    float startY = size.GetHeight() / 10.0;
    float bw = size.GetWidth() * 0.8;
    float bh = size.GetHeight() * 0.8;
    dc.DrawRectangle(startX, startY, bw, bh);

    if (!IsEnabled()) {
        return;
    }
    if (HasVC()) {
        dc.SetBrush(*wxRED_BRUSH);
        dc.SetPen(*wxRED_PEN);
    }
    else {
        dc.SetBrush(*wxYELLOW_BRUSH);
        dc.SetPen(*wxYELLOW_PEN);
    }

    float x1b = ((x1 + x) * bw) / 100.0 + startX;
    float x2b = ((x2 + x) * bw) / 100.0 + startX;

    float y1b = ((100.0 - y1 - y) * bh) / 100.0 + startY;
    float y2b = ((100.0 - y2 - y) * bh) / 100.0 + startY;

    dc.DrawCircle(x1b, y1b, 2);
    dc.DrawCircle(x1b, y2b, 2);
    dc.DrawCircle(x2b, y2b, 2);
    dc.DrawCircle(x2b, y1b, 2);

    dc.SetBrush(*wxTRANSPARENT_BRUSH);

    if (HasVC()) {
        dc.SetPen(wxPen(*wxRED, 1, wxPENSTYLE_DOT));
    }
    else {
        dc.SetPen(wxPen(*wxYELLOW, 1, wxPENSTYLE_DOT));
    }
    dc.DrawRectangle(x1b, y1b, x2b - x1b, y2b - y1b);

    dc.SetTextForeground(*wxYELLOW);
    dc.SetFont(wxFont(wxSize(0, 8), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    wxString str = wxString::Format("%.2fx%.2f", x2 + x, y2 + y);
    wxSize sz = dc.GetTextExtent(str);
    dc.DrawText(wxString::Format("%.2fx%.2f", x1 + x, y1 + y), x1b + 2, y1b - sz.GetHeight() - 1);
    dc.DrawText(wxString::Format("%.2fx%.2f", x1 + x, y2 + y), x1b + 2, y2b);

    dc.DrawText(str, x2b - 3 - sz.GetWidth(), y2b);
    str = wxString::Format("%.2fx%.2f", x2 + x, y1 + y);
    sz = dc.GetTextExtent(str);
    dc.DrawText(str, x2b - 3 - sz.GetWidth(), y1b - sz.GetHeight() - 1);
}
