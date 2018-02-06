#include "../xLightsMain.h"
#include "MainSequencer.h"
#include "TimeLine.h"
#include "../UtilFunctions.h"
#include <wx/event.h>
#include <wx/clipbrd.h>
#include "../xLightsVersion.h"

//(*InternalHeaders(MainSequencer)
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include <wx/dcbuffer.h>

//(*IdInit(MainSequencer)
const long MainSequencer::ID_CHOICE_VIEW_CHOICE = wxNewId();
const long MainSequencer::ID_PANEL1 = wxNewId();
const long MainSequencer::ID_PANEL3 = wxNewId();
const long MainSequencer::ID_PANEL6 = wxNewId();
const long MainSequencer::ID_PANEL2 = wxNewId();
const long MainSequencer::ID_SCROLLBAR_EFFECTS_VERTICAL = wxNewId();
const long MainSequencer::ID_SCROLLBAR_EFFECT_GRID_HORZ = wxNewId();
//*)


wxDEFINE_EVENT(EVT_HORIZ_SCROLL, wxCommandEvent);
wxDEFINE_EVENT(EVT_SCROLL_RIGHT, wxCommandEvent);

BEGIN_EVENT_TABLE(MainSequencer,wxPanel)
    EVT_MOUSEWHEEL(MainSequencer::mouseWheelMoved)

    EVT_COMMAND(wxID_ANY, EVT_HORIZ_SCROLL, MainSequencer::HorizontalScrollChanged)
    EVT_COMMAND(wxID_ANY, EVT_SCROLL_RIGHT, MainSequencer::ScrollRight)
    EVT_COMMAND(wxID_ANY, EVT_TIME_LINE_CHANGED, MainSequencer::TimelineChanged)
    EVT_COMMAND(wxID_ANY, EVT_SEQUENCE_CHANGED, MainSequencer::SequenceChanged)
    EVT_COMMAND(wxID_ANY, EVT_WAVE_FORM_HIGHLIGHT, MainSequencer::TimeLineSelectionChanged)

END_EVENT_TABLE()

void MainSequencer::SetHandlers(wxWindow *window)
{
    if (window) {
        window->Connect(wxID_ANY,
                        wxEVT_CHAR,
                        wxKeyEventHandler(MainSequencer::OnChar),
                        (wxObject*) nullptr,
                        this);
        window->Connect(wxID_ANY,
                        wxEVT_CHAR_HOOK,
                        wxKeyEventHandler(MainSequencer::OnCharHook),
                        (wxObject*) nullptr,
                        this);
        window->Connect(wxID_ANY,
                        wxEVT_KEY_DOWN,
                        wxKeyEventHandler(MainSequencer::OnKeyDown),
                        (wxObject*) nullptr,
                        this);

        wxWindowList &list = window->GetChildren();
        for (wxWindowList::iterator it = list.begin(); it != list.end(); ++it) {
            wxWindow* pclChild = *it;
            SetHandlers(pclChild);
        }

        window->Connect(wxID_CUT, wxEVT_MENU, (wxObjectEventFunction)&MainSequencer::DoCut,0,this);
        window->Connect(wxID_COPY, wxEVT_MENU, (wxObjectEventFunction)&MainSequencer::DoCopy,0,this);
        window->Connect(wxID_PASTE, wxEVT_MENU, (wxObjectEventFunction)&MainSequencer::DoPaste,0,this);
        window->Connect(wxID_UNDO, wxEVT_MENU, (wxObjectEventFunction)&MainSequencer::DoUndo,0,this);
        window->Connect(wxID_REDO, wxEVT_MENU, (wxObjectEventFunction)&MainSequencer::DoRedo,0,this);
    }
}

class TimeDisplayControl : public xlGLCanvas
{
public:
    TimeDisplayControl(wxPanel* parent, wxWindowID id, const wxPoint &pos=wxDefaultPosition,
                       const wxSize &size=wxDefaultSize, long style=0)
    : xlGLCanvas(parent, id, pos, size, style, "TimeDisplay", true) {
    // ReSharper disable once CppVirtualFunctionCallInsideCtor
        SetBackgroundStyle(wxBG_STYLE_PAINT);
        _time = "Time: 00:00:00";
        _selected = "";
        _fps = "";
        _fontSize = 14;
    }
    virtual ~TimeDisplayControl(){};

    virtual void SetLabels(const wxString &time, const wxString &fps) {
        _fps = fps;
        _time = time;
        renderGL();
    }

    void SetGLSize(int w, int h)
    {
        SetMinSize(wxSize(w, h));

        wxSize size = GetSize();
        if (w == -1) w = size.GetWidth();
        if (h == -1) h = size.GetHeight();

        SetSize(w, h);
        mWindowHeight = h;
        mWindowWidth = w;
        mWindowResized = true;
        if (h > 50)
        {
            _fontSize = 14;
        }
        else
        {
            _fontSize = 10;
        }

        Refresh();
        renderGL();
    }

    void SetSelected(const wxString &sel)
    {
        _selected = sel;
        renderGL();
    }

    protected:
    DECLARE_EVENT_TABLE()
    void Paint( wxPaintEvent& event ) {
        renderGL();
    }

    virtual bool UsesVertexTextureAccumulator() override {return true;}
    virtual bool UsesVertexColorAccumulator() override {return false;}
    virtual bool UsesVertexAccumulator() override {return false;}
    virtual bool UsesAddVertex() override {return false;}
    void InitializeGLCanvas() override
    {
        if(!IsShownOnScreen()) return;
        SetCurrentGLContext();
        xlColor c(wxSystemSettings::GetColour(wxSYS_COLOUR_FRAMEBK));
        LOG_GL_ERRORV(glClearColor(((float)c.Red())/255.0f,
                                   ((float)c.Green())/255.0f,
                                   ((float)c.Blue())/255.0f, 1.0f));
        LOG_GL_ERRORV(glDisable(GL_BLEND));
        LOG_GL_ERRORV(glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA));
        LOG_GL_ERRORV(glClear(GL_COLOR_BUFFER_BIT));
        prepare2DViewport(0,0,mWindowWidth, mWindowHeight);
        mIsInitialized = true;
    }

    void renderGL()
    {
        if(!mIsInitialized) { InitializeGLCanvas(); }
        if(!IsShownOnScreen()) return;

        SetCurrentGLContext();
        glClear(GL_COLOR_BUFFER_BIT);
        prepare2DViewport(0,0,mWindowWidth, mWindowHeight);
        DrawGLUtils::xlVertexTextAccumulator va;
#define LINEGAP 1.2
        int y = _fontSize * LINEGAP;
        va.AddVertex(5, y, _time);
        y += _fontSize * LINEGAP;
        // only display FPS if we have room
        if (y + _fontSize * LINEGAP <= mWindowHeight)
        {
            va.AddVertex(5, y, _fps);
            y += _fontSize * LINEGAP;
        }
        if (y <= mWindowHeight)
        {
            va.AddVertex(5, y, _selected);
        }
        DrawGLUtils::Draw(va, _fontSize, GetContentScaleFactor());
        SwapBuffers();
    }

private:
    std::string _time;
    std::string _fps;
    std::string _selected;
    int _fontSize;
};

BEGIN_EVENT_TABLE(TimeDisplayControl, xlGLCanvas)
EVT_PAINT(TimeDisplayControl::Paint)
END_EVENT_TABLE()

MainSequencer::MainSequencer(wxWindow* parent, bool smallWaveform, wxWindowID id,const wxPoint& pos,const wxSize& size)
    : touchBarSupport(), effectGridTouchbar(nullptr)
{
    log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));
    logger_base.debug("                Creating main sequencer");

    //(*Initialize(MainSequencer)
    wxFlexGridSizer* FlexGridSizer4;
    wxFlexGridSizer* FlexGridSizer2;
    wxStaticText* StaticText1;
    wxFlexGridSizer* FlexGridSizer1;

    Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxWANTS_CHARS, _T("wxID_ANY"));
    FlexGridSizer1 = new wxFlexGridSizer(3, 3, 0, 0);
    FlexGridSizer1->AddGrowableCol(1);
    FlexGridSizer1->AddGrowableRow(1);
    FlexGridSizer2 = new wxFlexGridSizer(0, 1, 0, 0);
    FlexGridSizer2->AddGrowableCol(0);
    StaticText1 = new wxStaticText(this, wxID_ANY, _("View:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
    FlexGridSizer2->Add(StaticText1, 1, wxTOP|wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    ViewChoice = new wxChoice(this, ID_CHOICE_VIEW_CHOICE, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE_VIEW_CHOICE"));
    FlexGridSizer2->Add(ViewChoice, 1, wxBOTTOM|wxLEFT|wxRIGHT, 5);
    FlexGridSizer2->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(FlexGridSizer2, 0, wxEXPAND, 0);
    FlexGridSizer4 = new wxFlexGridSizer(2, 0, 0, 0);
    FlexGridSizer4->AddGrowableCol(0);
    FlexGridSizer4->AddGrowableRow(1);
    PanelTimeLine = new TimeLine(this, ID_PANEL1, wxDefaultPosition, wxDLG_UNIT(this,wxSize(-1,15)), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    PanelTimeLine->SetMinSize(wxDLG_UNIT(this,wxSize(-1,15)));
    PanelTimeLine->SetMaxSize(wxDLG_UNIT(this,wxSize(-1,15)));
    FlexGridSizer4->Add(PanelTimeLine, 1, wxALL|wxEXPAND, 0);
    PanelWaveForm = new Waveform(this, ID_PANEL3, wxDefaultPosition, wxDLG_UNIT(this,wxSize(-1,40)), wxTAB_TRAVERSAL, _T("ID_PANEL3"));
    PanelWaveForm->SetMinSize(wxDLG_UNIT(this,wxSize(-1,40)));
    PanelWaveForm->SetMaxSize(wxDLG_UNIT(this,wxSize(-1,40)));
    FlexGridSizer4->Add(PanelWaveForm, 1, wxALL|wxEXPAND, 0);
    FlexGridSizer1->Add(FlexGridSizer4, 1, wxALL|wxEXPAND, 0);
    FlexGridSizer1->Add(-1,-1,1, wxALL|wxEXPAND, 5);
    PanelRowHeadings = new RowHeading(this, ID_PANEL6, wxDefaultPosition, wxDLG_UNIT(this,wxSize(90,-1)), wxTAB_TRAVERSAL, _T("ID_PANEL6"));
    PanelRowHeadings->SetMinSize(wxDLG_UNIT(this,wxSize(90,-1)));
    PanelRowHeadings->SetMaxSize(wxDLG_UNIT(this,wxSize(90,-1)));
    FlexGridSizer1->Add(PanelRowHeadings, 1, wxALL|wxEXPAND, 0);
    PanelEffectGrid = new EffectsGrid(this, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxFULL_REPAINT_ON_RESIZE, _T("ID_PANEL2"));
    FlexGridSizer1->Add(PanelEffectGrid, 1, wxALL|wxEXPAND, 0);
    ScrollBarEffectsVertical = new wxScrollBar(this, ID_SCROLLBAR_EFFECTS_VERTICAL, wxDefaultPosition, wxDefaultSize, wxSB_VERTICAL|wxALWAYS_SHOW_SB, wxDefaultValidator, _T("ID_SCROLLBAR_EFFECTS_VERTICAL"));
    ScrollBarEffectsVertical->SetScrollbar(0, 1, 10, 1);
    FlexGridSizer1->Add(ScrollBarEffectsVertical, 1, wxALL|wxEXPAND, 0);
    FlexGridSizer1->Add(-1,-1,1, wxALL|wxEXPAND, 5);
    ScrollBarEffectsHorizontal = new wxScrollBar(this, ID_SCROLLBAR_EFFECT_GRID_HORZ, wxDefaultPosition, wxDefaultSize, wxSB_HORIZONTAL|wxALWAYS_SHOW_SB, wxDefaultValidator, _T("ID_SCROLLBAR_EFFECT_GRID_HORZ"));
    ScrollBarEffectsHorizontal->SetScrollbar(0, 1, 100, 1);
    FlexGridSizer1->Add(ScrollBarEffectsHorizontal, 1, wxALL|wxEXPAND, 0);
    FlexGridSizer1->Add(-1,-1,1, wxALL|wxEXPAND, 5);
    SetSizer(FlexGridSizer1);
    FlexGridSizer1->Fit(this);
    FlexGridSizer1->SetSizeHints(this);

    Connect(ID_SCROLLBAR_EFFECTS_VERTICAL,wxEVT_SCROLL_TOP|wxEVT_SCROLL_BOTTOM|wxEVT_SCROLL_LINEUP|wxEVT_SCROLL_LINEDOWN|wxEVT_SCROLL_PAGEUP|wxEVT_SCROLL_PAGEDOWN|wxEVT_SCROLL_THUMBTRACK|wxEVT_SCROLL_THUMBRELEASE|wxEVT_SCROLL_CHANGED,(wxObjectEventFunction)&MainSequencer::OnScrollBarEffectsVerticalScrollChanged);
    Connect(ID_SCROLLBAR_EFFECTS_VERTICAL,wxEVT_SCROLL_TOP,(wxObjectEventFunction)&MainSequencer::OnScrollBarEffectsVerticalScrollChanged);
    Connect(ID_SCROLLBAR_EFFECTS_VERTICAL,wxEVT_SCROLL_BOTTOM,(wxObjectEventFunction)&MainSequencer::OnScrollBarEffectsVerticalScrollChanged);
    Connect(ID_SCROLLBAR_EFFECTS_VERTICAL,wxEVT_SCROLL_LINEUP,(wxObjectEventFunction)&MainSequencer::OnScrollBarEffectsVerticalScrollChanged);
    Connect(ID_SCROLLBAR_EFFECTS_VERTICAL,wxEVT_SCROLL_LINEDOWN,(wxObjectEventFunction)&MainSequencer::OnScrollBarEffectsVerticalScrollChanged);
    Connect(ID_SCROLLBAR_EFFECTS_VERTICAL,wxEVT_SCROLL_PAGEUP,(wxObjectEventFunction)&MainSequencer::OnScrollBarEffectsVerticalScrollChanged);
    Connect(ID_SCROLLBAR_EFFECTS_VERTICAL,wxEVT_SCROLL_PAGEDOWN,(wxObjectEventFunction)&MainSequencer::OnScrollBarEffectsVerticalScrollChanged);
    Connect(ID_SCROLLBAR_EFFECTS_VERTICAL,wxEVT_SCROLL_THUMBTRACK,(wxObjectEventFunction)&MainSequencer::OnScrollBarEffectsVerticalScrollChanged);
    Connect(ID_SCROLLBAR_EFFECTS_VERTICAL,wxEVT_SCROLL_CHANGED,(wxObjectEventFunction)&MainSequencer::OnScrollBarEffectsVerticalScrollChanged);
    Connect(ID_SCROLLBAR_EFFECT_GRID_HORZ,wxEVT_SCROLL_TOP|wxEVT_SCROLL_BOTTOM|wxEVT_SCROLL_LINEUP|wxEVT_SCROLL_LINEDOWN|wxEVT_SCROLL_PAGEUP|wxEVT_SCROLL_PAGEDOWN|wxEVT_SCROLL_THUMBTRACK|wxEVT_SCROLL_THUMBRELEASE|wxEVT_SCROLL_CHANGED,(wxObjectEventFunction)&MainSequencer::OnScrollBarEffectGridHorzScroll);
    Connect(ID_SCROLLBAR_EFFECT_GRID_HORZ,wxEVT_SCROLL_TOP,(wxObjectEventFunction)&MainSequencer::OnScrollBarEffectGridHorzScroll);
    Connect(ID_SCROLLBAR_EFFECT_GRID_HORZ,wxEVT_SCROLL_BOTTOM,(wxObjectEventFunction)&MainSequencer::OnScrollBarEffectGridHorzScroll);
    Connect(ID_SCROLLBAR_EFFECT_GRID_HORZ,wxEVT_SCROLL_LINEUP,(wxObjectEventFunction)&MainSequencer::OnScrollBarEffectsHorizontalScrollLineUp);
    Connect(ID_SCROLLBAR_EFFECT_GRID_HORZ,wxEVT_SCROLL_LINEDOWN,(wxObjectEventFunction)&MainSequencer::OnScrollBarEffectsHorizontalScrollLineDown);
    Connect(ID_SCROLLBAR_EFFECT_GRID_HORZ,wxEVT_SCROLL_PAGEUP,(wxObjectEventFunction)&MainSequencer::OnScrollBarEffectGridHorzScroll);
    Connect(ID_SCROLLBAR_EFFECT_GRID_HORZ,wxEVT_SCROLL_PAGEDOWN,(wxObjectEventFunction)&MainSequencer::OnScrollBarEffectGridHorzScroll);
    Connect(ID_SCROLLBAR_EFFECT_GRID_HORZ,wxEVT_SCROLL_THUMBTRACK,(wxObjectEventFunction)&MainSequencer::OnScrollBarEffectGridHorzScroll);
    Connect(ID_SCROLLBAR_EFFECT_GRID_HORZ,wxEVT_SCROLL_CHANGED,(wxObjectEventFunction)&MainSequencer::OnScrollBarEffectGridHorzScroll);
    //*)

    logger_base.debug("                Create time display control");
    timeDisplay = new TimeDisplayControl(this, wxID_ANY);
    FlexGridSizer2->Add(timeDisplay, 1, wxALL |wxEXPAND, 0);
    //FlexGridSizer2->AddGrowableRow(3);

    FlexGridSizer2->Fit(this);
    FlexGridSizer2->SetSizeHints(this);

    if (smallWaveform)
    {
        SetSmallWaveform();
    }
    else
    {
        SetLargeWaveform();
    }

    _savedTopModel = "";
    mParent = parent;
    mPlayType = 0;

    logger_base.debug("                Set handlers");
    SetHandlers(this);

    logger_base.debug("                Load key bindings");
    keyBindings.LoadDefaults();
    mCanUndo = false;
    mPasteByCell = false;
    // ReSharper disable CppVirtualFunctionCallInsideCtor
    SetName("MainSequencer");
    // ReSharper restore CppVirtualFunctionCallInsideCtor

    logger_base.debug("                Initialise touch bar");
    touchBarSupport.Init(this);
}

MainSequencer::~MainSequencer()
{
    if (effectGridTouchbar) delete effectGridTouchbar;

	//(*Destroy(MainSequencer)
	//*)
}

void MainSequencer::SetSequenceElements(SequenceElements* elements)
{
    mSequenceElements = elements;
}

void MainSequencer::UpdateEffectGridVerticalScrollBar()
{
    int position = mSequenceElements->GetFirstVisibleModelRow();
    int range = mSequenceElements->GetTotalNumberOfModelRows();
    int pageSize = mSequenceElements->GetMaxModelsDisplayed();
    int thumbSize = mSequenceElements->GetMaxModelsDisplayed();
    ScrollBarEffectsVertical->SetScrollbar(position,thumbSize,range,pageSize);
    ScrollBarEffectsVertical->Refresh();
    PanelEffectGrid->Refresh();
    PanelRowHeadings->Refresh();
}

void MainSequencer::UpdateTimeDisplay(int time_ms, float fps)
{
    int time = time_ms >= 0 ? time_ms : 0;
    int msec=time % 1000;
    int seconds=time / 1000;
    int minutes=seconds / 60;
    seconds=seconds % 60;
    wxString play_time = wxString::Format("Time: %d:%02d.%02d", minutes, seconds, msec);
    wxString fpsStr;
    if (fps >= 0)
    {
        fpsStr = wxString::Format("FPS: %5.1f",fps);
    }
    timeDisplay->SetLabels(play_time, fpsStr);
}

void MainSequencer::UpdateSelectedDisplay(int selected)
{
    if (selected == 0)
    {
        timeDisplay->SetSelected("");
    }
    else
    {
        timeDisplay->SetSelected(wxString::Format("Selected: %s", FORMATTIME(selected)));
    }
}

void MainSequencer::SetPlayStatus(int play_type)
{
    mPlayType = play_type;
}

void MainSequencer::OnScrollBarEffectGridHorzScroll(wxScrollEvent& event)
{
    int position = ScrollBarEffectsHorizontal->GetThumbPosition();
    int timeLength = PanelTimeLine->GetTimeLength();

    int startTime = (int)(((double)position/(double)timeLength) * (double)timeLength);
    PanelTimeLine->SetStartTimeMS(startTime);
    UpdateEffectGridHorizontalScrollBar();
}

void MainSequencer::OnScrollBarEffectsVerticalScrollChanged(wxScrollEvent& event)
{
    int position = ScrollBarEffectsVertical->GetThumbPosition();
    mSequenceElements->SetFirstVisibleModelRow(position);
    UpdateEffectGridVerticalScrollBar();
}

void MainSequencer::mouseWheelMoved(wxMouseEvent& event)
{
    int i = event.GetWheelRotation();
    if (event.GetWheelAxis() == wxMOUSE_WHEEL_HORIZONTAL) {
        int position = ScrollBarEffectsHorizontal->GetThumbPosition();
        int ts = ScrollBarEffectsHorizontal->GetThumbSize() / 10;
        if (ts ==0) {
            ts = 1;
        }
        if (i < 0) {
            if (position > 0) {
                position -= ts;
            }
        } else if (i > 0) {
            position += ts;
            if (position >= ScrollBarEffectsHorizontal->GetRange()) {
                position = ScrollBarEffectsHorizontal->GetRange() - 1;
            }
        }
        ScrollBarEffectsHorizontal->SetThumbPosition(position);
        wxCommandEvent eventScroll(EVT_HORIZ_SCROLL);
        HorizontalScrollChanged(eventScroll);
    } else {
        int position = ScrollBarEffectsVertical->GetThumbPosition();
        if(i<0)
        {
            if(position < ScrollBarEffectsVertical->GetRange()-1)
            {
                position++;
                ScrollBarEffectsVertical->SetThumbPosition(position);
                mSequenceElements->SetFirstVisibleModelRow(position);
            }
        } else if (i > 0) {
            if(position > 0)
            {
                position--;
                ScrollBarEffectsVertical->SetThumbPosition(position);
                mSequenceElements->SetFirstVisibleModelRow(position);
            }
        }
        mSequenceElements->PopulateVisibleRowInformation();
        PanelEffectGrid->ForceRefresh();  // call this so we can check if we need to update which effects are selected
        PanelRowHeadings->Refresh();
    }
}

void MainSequencer::OnCharHook(wxKeyEvent& event)
{
    wxChar uc = event.GetKeyCode();

    if (mSequenceElements != nullptr && mSequenceElements->GetXLightsFrame() != nullptr && mSequenceElements->GetXLightsFrame()->IsACActive())
    {
        if (PanelEffectGrid->HandleACKey(uc, event.ShiftDown()))
        {
            event.StopPropagation();
            return;
        }
    }

    //printf("OnCharHook %d   %c\n", uc, uc);
    switch(uc)
    {
        case WXK_BACK:
			PanelEffectGrid->DeleteSelectedEffects();
			event.StopPropagation();
			break;
		case WXK_INSERT:
		case WXK_NUMPAD_INSERT:
#ifdef __WXMSW__
			if (event.ControlDown())
			{
                if (mSequenceElements != nullptr) {
                    Copy();
                }
				event.StopPropagation();
			}
			else if (event.ShiftDown())
			{
                if (mSequenceElements != nullptr) {
                    Paste();
                }
				event.StopPropagation();
			}
#endif
			break;
		case WXK_DELETE:
#ifdef __WXMSW__
			if (!event.ShiftDown())
#endif
			{
				// Delete
                if (mSequenceElements != nullptr) {
                    PanelEffectGrid->DeleteSelectedEffects();
                }
				event.StopPropagation();
			}
#ifdef __WXMSW__
			else
			{
				// Cut - windows only
                if (mSequenceElements != nullptr) {
                    Cut();
                }
				event.StopPropagation();
			}
#endif
			break;
        case WXK_PAUSE:
            {
                wxCommandEvent playEvent(EVT_PAUSE_SEQUENCE);
                wxPostEvent(mParent, playEvent);
            }
            event.StopPropagation();
            break;
        case WXK_HOME:
            {
                wxCommandEvent playEvent(EVT_SEQUENCE_FIRST_FRAME);
                wxPostEvent(mParent, playEvent);
            }
            event.StopPropagation();
            break;
        case WXK_END:
            {
                wxCommandEvent playEvent(EVT_SEQUENCE_LAST_FRAME);
                wxPostEvent(mParent, playEvent);
            }
            event.StopPropagation();
            break;
        case WXK_SPACE:
            {
                wxCommandEvent playEvent(EVT_TOGGLE_PLAY);
                wxPostEvent(mParent, playEvent);
            }
            event.StopPropagation();
            break;
        case WXK_UP:
            PanelEffectGrid->MoveSelectedEffectUp(event.ShiftDown());
            event.StopPropagation();
            break;
        case WXK_DOWN:
            PanelEffectGrid->MoveSelectedEffectDown(event.ShiftDown());
            event.StopPropagation();
            break;
        case WXK_LEFT:
            PanelEffectGrid->MoveSelectedEffectLeft(event.ShiftDown(), event.ControlDown(), event.AltDown());
            event.StopPropagation();
            break;
        case WXK_RIGHT:
            PanelEffectGrid->MoveSelectedEffectRight(event.ShiftDown(), event.ControlDown(), event.AltDown());
            event.StopPropagation();
            break;
        case '.':
            if (event.ControlDown())
            {
                // save current effects grid position
                SavePosition();
            }
            break;
        case '/':
            if (event.ControlDown())
            {
                //restore saved effects grid position
                RestorePosition();
            }
            break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case WXK_NUMPAD0:
        case WXK_NUMPAD1:
        case WXK_NUMPAD2:
        case WXK_NUMPAD3:
        case WXK_NUMPAD4:
        case WXK_NUMPAD5:
        case WXK_NUMPAD6:
        case WXK_NUMPAD7:
        case WXK_NUMPAD8:
        case WXK_NUMPAD9:
        {
            int number = wxAtoi(uc);

            if (number > 9) number -= WXK_NUMPAD0;

            if (event.ControlDown())
            {
                if (event.ShiftDown())
                {
                    PanelTimeLine->SetStartTimeMS(number * 10 * PanelTimeLine->GetTimeLength() / 100);
                    UpdateEffectGridHorizontalScrollBar();
                }
                else
                {
                    PanelTimeLine->GoToTag(number);
                }
            }
        }
            break;
        case WXK_PAGEUP:
            if (event.ControlDown())
            {
                ScrollToRow(0);
            }
            else
            {
                ScrollToRow(std::max(0, mSequenceElements->GetFirstVisibleModelRow() - mSequenceElements->GetMaxModelsDisplayed()));
            }
            break;
        case WXK_PAGEDOWN:
            if (event.ControlDown())
            {
                ScrollToRow(mSequenceElements->GetTotalNumberOfModelRows());
            }
            else
            {
                ScrollToRow(std::min(mSequenceElements->GetTotalNumberOfModelRows() - mSequenceElements->GetMaxModelsDisplayed(), mSequenceElements->GetFirstVisibleModelRow() + mSequenceElements->GetMaxModelsDisplayed()));
            }
            break;
        default:
            event.Skip();
            break;
    }
}

void MainSequencer::OnKeyDown(wxKeyEvent& event)
{
    //wxChar uc = event.GetUnicodeKey();
    //printf("OnKeyDown %d   %c\n", uc, uc);
    event.Skip();
}
void MainSequencer::OnChar(wxKeyEvent& event)
{
    wxChar uc = event.GetUnicodeKey();

    if (mSequenceElements != nullptr && mSequenceElements->GetXLightsFrame() != nullptr && mSequenceElements->GetXLightsFrame()->IsACActive())
    {
        if (PanelEffectGrid->HandleACKey(uc))
        {
            event.StopPropagation();
            return;
        }
    }

    KeyBinding *binding = keyBindings.Find(uc);
    if (binding != nullptr) {
        event.StopPropagation();
        if (mSequenceElements == nullptr) {
            return;
        }
        switch (binding->GetType()) {
            case TIMING_ADD:
                InsertTimingMarkFromRange();
                break;
            case TIMING_SPLIT:
                SplitTimingMark();
                break;
            case KEY_ZOOM_IN:
                PanelTimeLine->ZoomIn();
                break;
            case KEY_ZOOM_OUT:
                PanelTimeLine->ZoomOut();
                break;
            case RANDOM_EFFECT:
                PanelEffectGrid->Paste("Random\t\t\n", xlights_version_string);
                break;
            case EFFECT_STRING:
                PanelEffectGrid->Paste(binding->GetEffectName() + "\t" + binding->GetEffectString() + "\t\n", binding->GetEffectDataVersion());
                break;
        }
    }
    //printf("OnChar %d   %c\n", uc, uc);
    switch(uc)
    {
        case 'c':
        case 'C':
        case WXK_CONTROL_C:
            if (event.CmdDown() || event.ControlDown()) {
                if (mSequenceElements != nullptr) {
                    Copy();
                    event.StopPropagation();
                }
            }
            break;
        case 'x':
        case 'X':
        case WXK_CONTROL_X:
            if (event.CmdDown() || event.ControlDown()) {
                if (mSequenceElements != nullptr) {
                    Cut();
                    event.StopPropagation();
                }
            }
            break;
        case 'v':
        case 'V':
        case WXK_CONTROL_V:
            if (event.CmdDown() || event.ControlDown()) {
                Paste();
                event.StopPropagation();
            }
            break;
        case 'z':
        case 'Z':
        case WXK_CONTROL_Z:
            if (event.CmdDown() || event.ControlDown()) {
                if( mSequenceElements != nullptr &&
                   mSequenceElements->get_undo_mgr().CanUndo() ) {
                    mSequenceElements->get_undo_mgr().UndoLastStep();
                    PanelEffectGrid->Refresh();
                    PanelEffectGrid->sendRenderDirtyEvent();
                }
                event.StopPropagation();
            }
            break;
        case 'a':
        case 'A':
        case WXK_CONTROL_A:
            if (event.CmdDown() || event.ControlDown()) {
                if (mSequenceElements != nullptr) {
                    if (event.ShiftDown())
                        mSequenceElements->SelectAllEffects();
                    else
                        mSequenceElements->SelectAllEffectsNoTiming();
                    PanelEffectGrid->Refresh();
                }
                event.StopPropagation();
            }
            break;
    }
}
void MainSequencer::ToggleHousePreview() {
    if (mSequenceElements != nullptr && mSequenceElements->GetXLightsFrame() != nullptr) {
        wxCommandEvent event;
        mSequenceElements->GetXLightsFrame()->ShowHideHousePreview(event);
    }
}
void MainSequencer::ToggleModelPreview() {
    if (mSequenceElements != nullptr && mSequenceElements->GetXLightsFrame() != nullptr) {
        wxCommandEvent event;
        mSequenceElements->GetXLightsFrame()->ShowHideModelPreview(event);
    }
}
void MainSequencer::TouchPlayControl(const std::string &evt) {
    wxCommandEvent e;
    if (evt == "Play") {
        mSequenceElements->GetXLightsFrame()->OnAuiToolBarItemPlayButtonClick(e);
    } else if (evt == "Pause") {
        mSequenceElements->GetXLightsFrame()->OnAuiToolBarItemPauseButtonClick(e);
    } else if (evt == "Stop") {
        mSequenceElements->GetXLightsFrame()->OnAuiToolBarItemStopClick(e);
    } else if (evt == "Back") {
        mSequenceElements->GetXLightsFrame()->OnAuiToolBarFirstFrameClick(e);
    } else if (evt == "Forward") {
        mSequenceElements->GetXLightsFrame()->OnAuiToolBarLastFrameClick(e);
    }
}


void MainSequencer::TouchButtonEvent(wxCommandEvent &event) {
    if (mSequenceElements != nullptr) {
        wxString effect = ((wxWindow*)event.GetEventObject())->GetName();
        PanelEffectGrid->Paste(effect + "\t\t\n", xlights_version_string);
    }
}
void MainSequencer::SetupTouchBar(EffectManager &effectManager, ColorPanelTouchBar *colorBar) {
    if (effectGridTouchbar == nullptr && touchBarSupport.HasTouchBar()) {
        effectGridTouchbar = new EffectGridTouchBar(touchBarSupport, effectManager, this, colorBar);
        effectGridTouchbar->SetActive();
    }
}

void MainSequencer::DoCopy(wxCommandEvent& event) {
    if (mSequenceElements != nullptr) {
        Copy();
    }
}
void MainSequencer::DoCut(wxCommandEvent& event) {
    if (mSequenceElements != nullptr) {
        Cut();
    }
}
void MainSequencer::DoPaste(wxCommandEvent& event) {
    if (mSequenceElements != nullptr) {
        Paste();
    }
}

void MainSequencer::DoUndo(wxCommandEvent& event) {

    if (PanelEffectGrid == nullptr) return;

    if (mSequenceElements != nullptr && mSequenceElements->get_undo_mgr().CanUndo() ) {
        mSequenceElements->get_undo_mgr().UndoLastStep();
        PanelEffectGrid->Refresh();
        PanelEffectGrid->sendRenderDirtyEvent();
    }
}

void MainSequencer::DoRedo(wxCommandEvent& event) {
}

void MainSequencer::SetLargeWaveform()
{
    PanelWaveForm->SetGLSize(-1, Waveform::GetLargeSize());
    timeDisplay->SetGLSize(-1, Waveform::GetLargeSize() - 22);
    Layout();
    PanelWaveForm->Refresh();
    timeDisplay->Refresh();
}

void MainSequencer::SetSmallWaveform()
{
    PanelWaveForm->SetGLSize(-1, Waveform::GetSmallSize());
    timeDisplay->SetGLSize(-1, Waveform::GetSmallSize() - 22);
    Layout();
    PanelWaveForm->Refresh();
    timeDisplay->Refresh();
}

void MainSequencer::GetPresetData(wxString& copy_data)
{
    if (PanelEffectGrid->IsACActive()) {
        GetACEffectsData(copy_data);
    }
    else
    {
        GetSelectedEffectsData(copy_data);
    }
}

void MainSequencer::GetSelectedEffectsData(wxString& copy_data) {
    log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));

    int start_column = PanelEffectGrid->GetStartColumn();
    int column_start_time = -1000;
    int number_of_timings = 0;
    int number_of_effects = 0;
    int number_of_timing_rows = mSequenceElements->GetNumberOfTimingRows();
    int first_timing_row = -1;
    int last_timing_row = 0;
    wxString effect_data;
    EffectLayer* tel = mSequenceElements->GetVisibleEffectLayer(mSequenceElements->GetSelectedTimingRow());
    if( tel != nullptr && start_column != -1) {
        Effect* eff = tel->GetEffect(start_column);
        if( eff != nullptr ) {
            column_start_time = eff->GetStartTimeMS();
        }
    }

    for(int i=0;i<mSequenceElements->GetRowInformationSize();i++)
    {
        int row_number;
        if( i < number_of_timing_rows )
        {
            row_number = mSequenceElements->GetRowInformation(i)->Index;
        }
        else
        {
            row_number = mSequenceElements->GetRowInformation(i)->Index-mSequenceElements->GetFirstVisibleModelRow();
        }
        EffectLayer* el = mSequenceElements->GetEffectLayer(i);
        for (int x = 0; x < el->GetEffectCount(); x++) {
            Effect *ef = el->GetEffect(x);
            if( ef == nullptr ) break;
            if (ef->GetSelected() != EFFECT_NOT_SELECTED && !ef->GetTagged()) {
                ef->SetTagged(true);
                wxString start_time = wxString::Format("%d",ef->GetStartTimeMS());
                wxString end_time = wxString::Format("%d",ef->GetEndTimeMS());
                wxString row = wxString::Format("%d",row_number);
                wxString column_start = wxString::Format("%d",column_start_time);
                effect_data += ef->GetEffectName() + "\t" + ef->GetSettingsAsString() + "\t" + ef->GetPaletteAsString() +
                               "\t" + start_time + "\t" + end_time + "\t" + row + "\t" + column_start;
                if( i < number_of_timing_rows )
                {
                    number_of_timings++;
                    last_timing_row = row_number;
                    if( first_timing_row < 0 )
                    {
                        first_timing_row = row_number;
                    }
                    effect_data += "\tTIMING_EFFECT\n";
                }
                else
                {
                    number_of_effects++;
                    if( column_start_time == -1000 && mSequenceElements->GetSelectedTimingRow() >= 0 )
                    {
                        if (tel == nullptr)
                        {
                            logger_base.crit("MainSequencer::GetSelectedEffectsData tel is nullptr ... this is going to crash.");
                        }

                        if( tel->HitTestEffectByTime(ef->GetStartTimeMS()+1,start_column) )
                        {
                            column_start_time = tel->GetEffect(start_column)->GetStartTimeMS();
                        }
                    }
                    if( column_start_time != -1000 )  // add paste by cell info
                    {
                        if( mSequenceElements->GetSelectedTimingRow() >= 0 )
                        {
                            Effect* te_start = tel->GetEffectByTime(ef->GetStartTimeMS() + 1); // if we don't add 1ms, it picks up the end of the previous timing instead of the start of this one
                            Effect* te_end = tel->GetEffectByTime(ef->GetEndTimeMS());
                            if( te_start != nullptr && te_end != nullptr )
                            {
                                if (tel == nullptr)
                                {
                                    logger_base.crit("MainSequencer::GetSelectedEffectsData tel is nullptr ... this is going to crash.");
                                }

                                int start_pct = ((ef->GetStartTimeMS() - te_start->GetStartTimeMS()) * 100) / (te_start->GetEndTimeMS() - te_start->GetStartTimeMS());
                                int end_pct = ((ef->GetEndTimeMS() - te_end->GetStartTimeMS()) * 100) / (te_end->GetEndTimeMS() - te_end->GetStartTimeMS());
                                int start_index;
                                int end_index;
                                tel->HitTestEffectByTime(te_start->GetStartTimeMS()+1,start_index);
                                tel->HitTestEffectByTime(te_end->GetStartTimeMS()+1,end_index);
                                wxString start_pct_str = wxString::Format("%d",start_pct);
                                wxString end_pct_str = wxString::Format("%d",end_pct);
                                wxString start_index_str = wxString::Format("%d",start_index);
                                wxString end_index_str = wxString::Format("%d",end_index);
                                effect_data += "\t" + start_index_str + "\t" + end_index_str + "\t" + start_pct_str + "\t" + end_pct_str + "\n";
                            } else {effect_data += "\tNO_PASTE_BY_CELL\n";}
                        } else {effect_data += "\tNO_PASTE_BY_CELL\n";}
                    } else {effect_data += "\tNO_PASTE_BY_CELL\n";}
                }
            }
        }
    }
    if( first_timing_row >= 0 )
    {
        last_timing_row -= first_timing_row;  // calculate the total number of timing rows
    }

    wxString num_timings = wxString::Format("%d",number_of_timings);
    wxString num_effects = wxString::Format("%d",number_of_effects);
    wxString num_timing_rows = wxString::Format("%d",number_of_timing_rows);
    wxString last_row = wxString::Format("%d",last_timing_row);
    wxString starting_column = wxString::Format("%d",start_column);
    copy_data = "CopyFormat1\t" + num_timings + "\t" + num_effects + "\t" + num_timing_rows + "\t" + last_row + "\t" + starting_column;
    if( column_start_time != -1000 ) {
        copy_data += "\tPASTE_BY_CELL\n" + effect_data;
    }
    else {
        copy_data += "\tNO_PASTE_BY_CELL\n" + effect_data;
    }
    UnTagAllEffects();
}

void MainSequencer::GetACEffectsData(wxString& copy_data) {
    log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));

    int start_column = PanelEffectGrid->GetStartColumn();
    int end_column = PanelEffectGrid->GetEndColumn();
    int start_row = PanelEffectGrid->GetStartRow();
    int end_row = PanelEffectGrid->GetEndRow();
    int column_start_time = -1000;
    int column_end_time = -1000;
    int number_of_timings = 0;
    int number_of_effects = 0;
    int number_of_timing_rows = mSequenceElements->GetNumberOfTimingRows();
    int last_timing_row = 0;
    wxString effect_data;
    EffectLayer* tel = mSequenceElements->GetVisibleEffectLayer(mSequenceElements->GetSelectedTimingRow());
    if( tel != nullptr && start_column != -1) {
        Effect* eff1 = tel->GetEffect(start_column);
        Effect* eff2 = tel->GetEffect(end_column);
        if( eff1 != nullptr && eff2 != nullptr ) {
            column_start_time = eff1->GetStartTimeMS();
            column_end_time = eff2->GetEndTimeMS();
        } else {
            return;
        }
    } else {
        return;  // there should always be a range selection in AC copy mode
    }

    for(int i=0;i<mSequenceElements->GetRowInformationSize();i++)
    {
        int row_number;
        if( i < number_of_timing_rows )
        {
            row_number = mSequenceElements->GetRowInformation(i)->Index;
        }
        else
        {
            row_number = mSequenceElements->GetRowInformation(i)->Index;// -mSequenceElements->GetFirstVisibleModelRow();
        }
        if( row_number >= start_row && row_number <= end_row )
        {
            EffectLayer* el = mSequenceElements->GetEffectLayer(i);
            for (int x = 0; x < el->GetEffectCount(); x++) {
                Effect *ef = el->GetEffect(x);
                if( ef == nullptr ) break;
                if(!ef->GetTagged()) {
                    ef->SetTagged(true);
                    if( ef->GetStartTimeMS() < column_end_time && ef->GetEndTimeMS() > column_start_time ) {
                        int start_offset = 0;
                        int end_offset = 0;
                        if( ef->GetStartTimeMS() < column_start_time ) {
                            start_offset = column_start_time - ef->GetStartTimeMS();
                        }
                        if( ef->GetEndTimeMS() > column_end_time ) {
                            end_offset = ef->GetEndTimeMS() - column_end_time;
                        }
                        int adj_start_time = ef->GetStartTimeMS() + start_offset;
                        int adj_end_time = ef->GetEndTimeMS() - end_offset;
                        wxString start_time = wxString::Format("%d",adj_start_time);
                        wxString end_time = wxString::Format("%d",adj_end_time);
                        wxString row = wxString::Format("%d",row_number);
                        wxString column_start = wxString::Format("%d",column_start_time);
                        std::string settings = PanelEffectGrid->TruncateEffectSettings(ef->GetSettings(), ef->GetEffectName(), ef->GetStartTimeMS(), ef->GetEndTimeMS(), adj_start_time, adj_end_time);

                        effect_data += ef->GetEffectName() + "\t" + settings + "\t" + ef->GetPaletteAsString() +
                                       "\t" + start_time + "\t" + end_time + "\t" + row + "\t" + column_start;
                        number_of_effects++;
                        Effect* te_start = tel->GetEffectByTime(adj_start_time + 1); // if we don't add 1ms, it picks up the end of the previous timing instead of the start of this one
                        Effect* te_end = tel->GetEffectByTime(adj_end_time);
                        if( te_start != nullptr && te_end != nullptr )
                        {
                            if (tel == nullptr)
                            {
                                logger_base.crit("MainSequencer::GetSelectedEffectsData tel is nullptr ... this is going to crash.");
                            }

                            int start_pct = ((adj_start_time - te_start->GetStartTimeMS()) * 100) / (te_start->GetEndTimeMS() - te_start->GetStartTimeMS());
                            int end_pct = ((adj_end_time - te_end->GetStartTimeMS()) * 100) / (te_end->GetEndTimeMS() - te_end->GetStartTimeMS());
                            int start_index;
                            int end_index;
                            tel->HitTestEffectByTime(te_start->GetStartTimeMS()+1,start_index);
                            tel->HitTestEffectByTime(te_end->GetStartTimeMS()+1,end_index);
                            wxString start_pct_str = wxString::Format("%d",start_pct);
                            wxString end_pct_str = wxString::Format("%d",end_pct);
                            wxString start_index_str = wxString::Format("%d",start_index);
                            wxString end_index_str = wxString::Format("%d",end_index);
                            effect_data += "\t" + start_index_str + "\t" + end_index_str + "\t" + start_pct_str + "\t" + end_pct_str + "\n";
                        }
                    }
                }
            }
        }
    }
    wxString num_timings = wxString::Format("%d",number_of_timings);
    wxString num_effects = wxString::Format("%d",number_of_effects);
    wxString num_timing_rows = wxString::Format("%d",number_of_timing_rows);
    wxString last_row = wxString::Format("%d",last_timing_row);
    wxString starting_column = wxString::Format("%d",start_column);
    wxString ending_column = wxString::Format("%d",end_column);
    wxString starting_row = wxString::Format("%d",start_row);
    wxString ending_row = wxString::Format("%d",end_row);
    wxString starting_time = wxString::Format("%d",column_start_time);
    wxString ending_time = wxString::Format("%d",column_end_time);
    copy_data = "CopyFormatAC\t" + num_timings + "\t" + num_effects + "\t" + num_timing_rows + "\t" + last_row + "\t" + starting_column + "\t" + ending_column + "\t" + starting_row + "\t" + ending_row + "\t" + starting_time + "\t" + ending_time;
    copy_data += "\tPASTE_BY_CELL\n" + effect_data;
    UnTagAllEffects();
}

bool MainSequencer::CopySelectedEffects() {
    wxString copy_data;
    if( PanelEffectGrid->IsACActive() ) {
        GetACEffectsData(copy_data);
    } else {
        GetSelectedEffectsData(copy_data);
    }
    if (!copy_data.IsEmpty() && wxTheClipboard->Open()) {
        if (!wxTheClipboard->SetData(new wxTextDataObject(copy_data))) {
            wxMessageBox(_("Unable to copy data to clipboard."), _("Error"));
        }
        wxTheClipboard->Close();
        return true;
    }
    return false;
}

void MainSequencer::Copy() {
    if (CopySelectedEffects()) {
        PanelEffectGrid->SetCanPaste();
    }
}

void MainSequencer::Cut() {
    if (CopySelectedEffects()) {
        if (PanelEffectGrid->IsACActive())
        {
            PanelEffectGrid->DoACDraw(false, ACTYPE::OFF, ACSTYLE::INTENSITY, ACTOOL::TOOLNIL, ACMODE::MODENIL);
        }
        else
        {
            PanelEffectGrid->DeleteSelectedEffects();
        }
    }
}

Effect* MainSequencer::GetSelectedEffect()
{
    return PanelEffectGrid->GetSelectedEffect();
}

int MainSequencer::GetSelectedEffectCount(const std::string effectName) const
{
    return PanelEffectGrid->GetSelectedEffectCount(effectName);
}

void MainSequencer::ApplyEffectSettingToSelected(const std::string effectName, const std::string id, const std::string value, ValueCurve* vc, const std::string& vcid)
{
    return PanelEffectGrid->ApplyEffectSettingToSelected(effectName, id, value, vc, vcid);
}

void MainSequencer::Paste(bool row_paste) {
    wxTextDataObject data;
    wxClipboard *cbd = wxClipboard::Get();
    if (cbd && cbd->Open()) {
        if ((cbd->IsSupported(wxDF_TEXT) || cbd->IsSupported(wxDF_UNICODETEXT))
            && cbd->GetData(data)) {
            //assume clipboard always has data from same version of xLights
            PanelEffectGrid->Paste(data.GetText(), xlights_version_string, row_paste);
        }
        cbd->Close();
    }
}

void MainSequencer::InsertTimingMarkFromRange()
{
    log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));

    bool is_range = true;
    int x1;
    int x2;
    if( mPlayType == PLAY_TYPE_MODEL )
    {
        x1 = PanelTimeLine->GetPlayMarker();
        x2 = x1;
    }
    else
    {
        x1 = PanelTimeLine->GetSelectedPositionStart();
        x2 = PanelTimeLine->GetSelectedPositionEnd();
    }
    if( x2 == -1 ) x2 = x1;
    if( x1 == x2) is_range = false;
    int selectedTiming = mSequenceElements->GetSelectedTimingRow();
    if(selectedTiming >= 0)
    {
        int t1 = PanelTimeLine->GetAbsoluteTimeMSfromPosition(x1);
        int t2 = PanelTimeLine->GetAbsoluteTimeMSfromPosition(x2);
        if (t2 > PanelTimeLine->GetTimeLength())
        {
            t2 = PanelTimeLine->GetTimeLength();
        }
        if(is_range)
        {
            Element* e = mSequenceElements->GetVisibleRowInformation(selectedTiming)->element;
            EffectLayer* el = e->GetEffectLayer(mSequenceElements->GetVisibleRowInformation(selectedTiming)->layerIndex);

            if (el == nullptr)
            {
                logger_base.crit("MainSequencer::InsertTimingMarkFromRange el is nullptr ... this is going to crash.");
            }

            int i1 = -1;
            int i2 = -1;

            el->HitTestEffectByTime(t1, i1);
            el->HitTestEffectByTime(t2, i2);

            if ((!el->HitTestEffectByTime(t1,i1) && !el->HitTestEffectByTime(t2,i2) && !el->HitTestEffectBetweenTime(t1,t2)) ||
                (!el->HitTestEffectBetweenTime(t1,t2) && i1 != i2))
            {
                std::string name,settings;
                el->AddEffect(0,name,settings,"",t1,t2,false,false);
                PanelEffectGrid->ForceRefresh();
            }
            else
            {
                wxMessageBox("Timing exist already in the selected region","Timing placement error");
            }
        }
        else
        {
            // x1 and x2 are the same. Insert from end time of timing to the left to x2
            Element* e = mSequenceElements->GetVisibleRowInformation(selectedTiming)->element;
            EffectLayer* el = e->GetEffectLayer(mSequenceElements->GetVisibleRowInformation(selectedTiming)->layerIndex);

            if (el == nullptr)
            {
                logger_base.crit("MainSequencer::InsertTimingMarkFromRange [2] el is nullptr ... this is going to crash.");
            }

            int index;
            if(!el->HitTestEffectByTime(t2,index))
            {
                // get effect to left and right
                Effect * lefteffect = nullptr;
                Effect * righteffect = nullptr;

                for (int x = 0; x < el->GetEffectCount(); x++) {
                    Effect * eff = el->GetEffect(x);
                    if (eff->GetEndTimeMS() < t2 && (lefteffect == nullptr || lefteffect->GetEndTimeMS() < eff->GetEndTimeMS()))
                    {
                        lefteffect = eff;
                    }
                    if (righteffect == nullptr && eff->GetStartTimeMS() > t2) {
                        righteffect = eff;
                        break;
                    }
                }

                std::string name;
                std::string settings;
                if (lefteffect != nullptr && righteffect != nullptr)
                {
                    // fill to left and right
                    el->AddEffect(0, name, settings, "", lefteffect->GetEndTimeMS(), t2, false, false);
                    el->AddEffect(0, name, settings, "", t2, righteffect->GetStartTimeMS(), false, false);
                }
                else if (lefteffect != nullptr)
                {
                    el->AddEffect(0, name, settings, "", lefteffect->GetEndTimeMS(), t2, false, false);
                }
                else if (righteffect != nullptr)
                {
                    el->AddEffect(0, name, settings, "", t2, righteffect->GetStartTimeMS(), false, false);
                }
                else
                {
                    el->AddEffect(0, name, settings, "", 0, t2, false, false);
                }

                PanelEffectGrid->ForceRefresh();
            }
            else
            {
                SplitTimingMark();  // inserting a timing mark inside a timing mark same as a split
            }
        }
    }
}

void MainSequencer::SplitTimingMark()
{
    log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));

    int x1;
    int x2;
    if( mPlayType == PLAY_TYPE_MODEL )
    {
        x1 = PanelTimeLine->GetPlayMarker();
        x2 = x1;
    }
    else
    {
        x1 = PanelTimeLine->GetSelectedPositionStart();
        x2 = PanelTimeLine->GetSelectedPositionEnd();
    }
    if( x2 == -1 ) x2 = x1;
    int selectedTiming = mSequenceElements->GetSelectedTimingRow();
    if(selectedTiming >= 0)
    {
        Element* e = mSequenceElements->GetVisibleRowInformation(selectedTiming)->element;
        EffectLayer* el = e->GetEffectLayer(mSequenceElements->GetVisibleRowInformation(selectedTiming)->layerIndex);

        if (el == nullptr)
        {
            logger_base.crit("MainSequencer::SplitTimingMark el is nullptr ... this is going to crash.");
        }

        int index1,index2;
        int t1 = PanelTimeLine->GetAbsoluteTimeMSfromPosition(x1);
        int t2 = PanelTimeLine->GetAbsoluteTimeMSfromPosition(x2);
        if(el->HitTestEffectByTime(t1,index1) && el->HitTestEffectByTime(t2,index2))
        {
            if( index1 == index2 )
            {
                Effect* eff1 = el->GetEffect(index1);
                int old_end_time = eff1->GetEndTimeMS();
                if( t1 != t2 || ((t1 == t2) && t1 != eff1->GetStartTimeMS() && t1 != eff1->GetEndTimeMS()) )
                {
                    eff1->SetEndTimeMS(t1);
                    std::string name,settings;
                    el->AddEffect(0,name,settings,"",t2,old_end_time,false,false);
                    PanelEffectGrid->ForceRefresh();
                }
            }
            else
            {
                wxMessageBox("Timing cannot be split across timing marks.","Timing placement error");
            }
        }
    }
}

void MainSequencer::OnScrollBarEffectsHorizontalScrollLineUp(wxScrollEvent& event)
{
    int position = ScrollBarEffectsHorizontal->GetThumbPosition();
    if( position > 0 )
    {
        int ts = ScrollBarEffectsHorizontal->GetThumbSize() / 10;
        if (ts == 0) {
            ts = 1;
        }
        position -= ts;
        if (position < 0) {
            position = 0;
        }
        ScrollBarEffectsHorizontal->SetThumbPosition(position);
    }
}

void MainSequencer::OnScrollBarEffectsHorizontalScrollLineDown(wxScrollEvent& event)
{
    int position = ScrollBarEffectsHorizontal->GetThumbPosition();
    int limit = ScrollBarEffectsHorizontal->GetRange();
    if( position < limit-1 )
    {
        int ts = ScrollBarEffectsHorizontal->GetThumbSize() / 10;
        if (ts == 0) {
            ts = 1;
        }
        position += ts;
        if (position >= limit) {
            position = limit - 1;
        }
        ScrollBarEffectsHorizontal->SetThumbPosition(position);
    }
}


void MainSequencer::HorizontalScrollChanged( wxCommandEvent& event)
{
    int position = ScrollBarEffectsHorizontal->GetThumbPosition();
    int timeLength = PanelTimeLine->GetTimeLength();
    int startTime = (int)(((double)position/(double)timeLength) * (double)timeLength);
    PanelTimeLine->SetStartTimeMS(startTime);
    UpdateEffectGridHorizontalScrollBar();
}

void MainSequencer::ScrollRight(wxCommandEvent& event)
{
    int position = ScrollBarEffectsHorizontal->GetThumbPosition();
    int limit = ScrollBarEffectsHorizontal->GetRange();
    if( position < limit-1 )
    {
        int ts = ScrollBarEffectsHorizontal->GetThumbSize();
        if (ts == 0) {
            ts = 1;
        }
        position += ts;
        if (position >= limit) {
            position = limit - 1;
        }
        ScrollBarEffectsHorizontal->SetThumbPosition(position);
        wxCommandEvent eventScroll(EVT_HORIZ_SCROLL);
        HorizontalScrollChanged(eventScroll);
    }
}

void MainSequencer::TimeLineSelectionChanged(wxCommandEvent& event)
{
    UpdateSelectedDisplay(event.GetInt());
}

void MainSequencer::SequenceChanged(wxCommandEvent& event)
{
    mSequenceElements->IncrementChangeCount(nullptr);
}

void MainSequencer::TimelineChanged( wxCommandEvent& event)
{
    TimelineChangeArguments *tla = (TimelineChangeArguments*)(event.GetClientData());
    PanelWaveForm->SetZoomLevel(tla->ZoomLevel);
    PanelWaveForm->SetStartPixelOffset(tla->StartPixelOffset);
    UpdateTimeDisplay(tla->CurrentTimeMS, -1);
    PanelTimeLine->Update();
    PanelWaveForm->Refresh();
    PanelWaveForm->Update();
    PanelEffectGrid->SetStartPixelOffset(tla->StartPixelOffset);
    PanelEffectGrid->Refresh();
    PanelEffectGrid->Update();
    UpdateEffectGridHorizontalScrollBar();
    delete tla;
}

void MainSequencer::UpdateEffectGridHorizontalScrollBar()
{
    PanelWaveForm->SetZoomLevel(PanelTimeLine->GetZoomLevel());
    PanelWaveForm->SetStartPixelOffset(PanelTimeLine->GetStartPixelOffset());
    UpdateTimeDisplay(PanelTimeLine->GetCurrentPlayMarkerMS(), -1);

    //printf("%d\n", PanelTimeLine->GetStartPixelOffset());
    PanelTimeLine->Refresh();
    PanelTimeLine->Update();
    PanelWaveForm->Refresh();
    PanelWaveForm->Update();
    PanelEffectGrid->SetStartPixelOffset(PanelTimeLine->GetStartPixelOffset());
    PanelEffectGrid->Refresh();
    PanelEffectGrid->Update();


    int zoomLevel = PanelTimeLine->GetZoomLevel();
    int maxZoomLevel = PanelTimeLine->GetMaxZoomLevel();
    if(zoomLevel == maxZoomLevel)
    {
        // Max Zoom so scrollbar is same size as window.
        int range = PanelTimeLine->GetSize().x;
        int pageSize =range;
        int thumbSize = range;
        ScrollBarEffectsHorizontal->SetScrollbar(0,thumbSize,range,pageSize);
    }
    else
    {
        int startTime;
        int endTime;
        int range = PanelTimeLine->GetTimeLength();
        PanelTimeLine->GetViewableTimeRange(startTime,endTime);

        int diff = endTime - startTime;
        int thumbSize = diff;
        int pageSize = thumbSize;
        int position = startTime;
        ScrollBarEffectsHorizontal->SetScrollbar(position,thumbSize,range,pageSize);
    }

    ScrollBarEffectsHorizontal->Refresh();
}

void MainSequencer::TagAllSelectedEffects()
{
    for(int row=0;row<mSequenceElements->GetRowInformationSize();row++)
    {
        EffectLayer* el = mSequenceElements->GetEffectLayer(row);
        el->TagAllSelectedEffects();
    }
}

void MainSequencer::UnTagAllEffects()
{
    for(int i=0;i<mSequenceElements->GetRowInformationSize();i++) {
        EffectLayer* el = mSequenceElements->GetEffectLayer(i);
        if( el != nullptr ) {
            el->UnTagAllEffects();
        }
    }
}

void MainSequencer::RestorePosition()
{
    if (mSequenceElements == nullptr) return;

    if (_savedTopModel != "")
    {
        PanelTimeLine->RestorePosition();
        Element* elem = mSequenceElements->GetElement(_savedTopModel);
        if (elem != nullptr)
        {
            for (int row = 0; row < mSequenceElements->GetRowInformationSize(); row++)
            {
                EffectLayer* el = mSequenceElements->GetEffectLayer(row);
                if (el->GetParentElement()->GetModelName() == elem->GetName()) {
                    ScrollToRow(row - mSequenceElements->GetNumberOfTimingRows());
                }
            }
        }
    }
}

void MainSequencer::SavePosition()
{
    if (mSequenceElements == nullptr || mSequenceElements->GetVisibleEffectLayer(mSequenceElements->GetNumberOfTimingRows()) == nullptr)
    {
        _savedTopModel = "";
    }
    else
    {
        PanelTimeLine->SavePosition();
        Element* elem = mSequenceElements->GetVisibleEffectLayer(mSequenceElements->GetNumberOfTimingRows())->GetParentElement();
        _savedTopModel = elem->GetName();
    }
}

void MainSequencer::ScrollToRow(int row)
{
    if (mSequenceElements == nullptr) return;

    mSequenceElements->SetFirstVisibleModelRow(row);
    UpdateEffectGridVerticalScrollBar();
}
