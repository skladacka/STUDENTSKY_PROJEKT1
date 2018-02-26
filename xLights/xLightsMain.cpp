/***************************************************************
 * Name:      xLightsMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Matt Brown (dowdybrown@yahoo.com)
 * Created:   2012-11-03
 * Copyright: Matt Brown ()
 * License:
 **************************************************************/

#include "xLightsMain.h"
#include "SplashDialog.h"
#include "UpdaterDialog.h"
#include <wx/msgdlg.h>
#include <wx/tokenzr.h>
#include <wx/dir.h>
#include <wx/textdlg.h>
#include <wx/numdlg.h>
#include <wx/persist.h>
#include <wx/persist/toplevel.h>
#include <wx/valnum.h>
#include <wx/clipbrd.h>
#include <wx/debugrpt.h>
#include <wx/protocol/http.h>
#include <wx/textctrl.h>
#include <wx/sstream.h>
#include <wx/regex.h>
#include "xLightsApp.h" //global app run-time flags
#include "heartbeat.h" //DJ
#include "SeqSettingsDialog.h"
#include <wx/grid.h>
#include <wx/mimetype.h>
#include "xLightsVersion.h"
#include "RenderCommandEvent.h"
#include "effects/RenderableEffect.h"
#include "LayoutPanel.h"
#include "models/ModelGroup.h"
#include "PixelTestDialog.h"
#include "ConvertDialog.h"
#include "GenerateCustomModelDialog.h"
#include "UtilFunctions.h"
#include "controllers/FPPConnectDialog.h"
#include "IPEntryDialog.h"
#include "ColorManagerDialog.h"
#include "HousePreviewPanel.h"
#include "BatchRenderDialog.h"
#include "VideoExporter.h"
#include "SequenceVideoPanel.h"

// image files
#include "../include/xLights.xpm"
#include "../include/xLights-16.xpm"
#include "../include/xLights-32.xpm"
#include "../include/xLights-64.xpm"
#include "../include/xLights-128.xpm"
#include "../include/control-pause-blue-icon.xpm"
#include "../include/control-play-blue-icon.xpm"

//(*InternalHeaders(xLightsFrame)
#include <wx/artprov.h>
#include <wx/bitmap.h>
#include <wx/intl.h>
#include <wx/image.h>
#include <wx/string.h>
//*)

#define TOOLBAR_SAVE_VERSION "0003:"
#define MAXBACKUPFILE_MB 30

#include "osxMacUtils.h"
#include <wx/zipstrm.h>
#include <wx/wfstream.h>
#include <cctype>
#include "outputs/IPOutput.h"
#include "GenerateLyricsDialog.h"
#include "VendorModelDialog.h"


//helper functions
enum wxbuildinfoformat
{
    short_f, long_f
};

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(xLightsFrame)
const long xLightsFrame::ID_AUITOOLBAR_OPENSHOW = wxNewId();
const long xLightsFrame::ID_AUITOOLBAR_NEWSEQUENCE = wxNewId();
const long xLightsFrame::ID_AUITOOLBAR_OPEN = wxNewId();
const long xLightsFrame::ID_AUITOOLBAR_SAVE = wxNewId();
const long xLightsFrame::ID_AUITOOLBAR_SAVEAS = wxNewId();
const long xLightsFrame::ID_AUITOOLBAR_RENDERALL = wxNewId();
const long xLightsFrame::ID_AUITOOLBAR_MAIN = wxNewId();
const long xLightsFrame::ID_AUITOOLBAR_PLAY_NOW = wxNewId();
const long xLightsFrame::ID_AUITOOLBAR_PAUSE = wxNewId();
const long xLightsFrame::ID_AUITOOLBAR_STOP = wxNewId();
const long xLightsFrame::ID_AUITOOLBAR_FIRST_FRAME = wxNewId();
const long xLightsFrame::ID_AUITOOLBAR_LAST_FRAME = wxNewId();
const long xLightsFrame::ID_AUITOOLBAR_REPLAY_SECTION = wxNewId();
const long xLightsFrame::ID_AUITOOLBAR_PLAY = wxNewId();
const long xLightsFrame::ID_AUITOOLBARITEM2 = wxNewId();
const long xLightsFrame::ID_AUITOOLBARITEM5 = wxNewId();
const long xLightsFrame::ID_AUITOOLBARITEM7 = wxNewId();
const long xLightsFrame::ID_AUITOOLBARITEM3 = wxNewId();
const long xLightsFrame::ID_AUITOOLBARITEM1 = wxNewId();
const long xLightsFrame::ID_AUITOOLBARITEM4 = wxNewId();
const long xLightsFrame::ID_AUITOOLBARITEM6 = wxNewId();
const long xLightsFrame::ID_AUITOOLBARITEM8 = wxNewId();
const long xLightsFrame::ID_AUIWINDOWTOOLBAR = wxNewId();
const long xLightsFrame::ID_PASTE_BY_TIME = wxNewId();
const long xLightsFrame::ID_PASTE_BY_CELL = wxNewId();
const long xLightsFrame::ID_AUITOOLBAR_EDIT = wxNewId();
const long xLightsFrame::ID_AUITOOLBARITEM_ACDISABLED = wxNewId();
const long xLightsFrame::ID_AUITOOLBARITEM_ACSELECT = wxNewId();
const long xLightsFrame::ID_AUITOOLBARITEM_ACOFF = wxNewId();
const long xLightsFrame::ID_AUITOOLBARITEM_ACON = wxNewId();
const long xLightsFrame::ID_AUITOOLBARITEM_ACSHIMMER = wxNewId();
const long xLightsFrame::ID_AUITOOLBARITEM_ACTWINKLE = wxNewId();
const long xLightsFrame::ID_AUITOOLBARITEM_ACINTENSITY = wxNewId();
const long xLightsFrame::ID_AUITOOLBARITEM_ACRAMPUP = wxNewId();
const long xLightsFrame::ID_AUITOOLBARITEM_ACRAMPDOWN = wxNewId();
const long xLightsFrame::ID_AUITOOLBARITEM_ACRAMPUPDOWN = wxNewId();
const long xLightsFrame::ID_CHOICE_PARM1 = wxNewId();
const long xLightsFrame::ID_CHOICE_PARM2 = wxNewId();
const long xLightsFrame::ID_AUITOOLBARITEM_ACFILL = wxNewId();
const long xLightsFrame::ID_AUITOOLBARITEM_ACCASCADE = wxNewId();
const long xLightsFrame::ID_AUITOOLBARITEM_ACFOREGROUND = wxNewId();
const long xLightsFrame::ID_AUITOOLBARITEM_ACBACKGROUND = wxNewId();
const long xLightsFrame::ID_AUITOOLBAR_AC = wxNewId();
const long xLightsFrame::ID_AUITOOLBARITEM14 = wxNewId();
const long xLightsFrame::ID_AUITOOLBAR_VIEW = wxNewId();
const long xLightsFrame::ID_BITMAPBUTTON_TAB_INFO = wxNewId();
const long xLightsFrame::ID_BUTTON_STOP_NOW = wxNewId();
const long xLightsFrame::ID_BUTTON_LIGHTS_OFF = wxNewId();
const long xLightsFrame::ID_CHECKBOX_LIGHT_OUTPUT = wxNewId();
const long xLightsFrame::ID_AUITOOLBAR_OUTPUT = wxNewId();
const long xLightsFrame::ID_AUIEFFECTSTOOLBAR = wxNewId();
const long xLightsFrame::ID_BUTTON3 = wxNewId();
const long xLightsFrame::ID_STATICTEXT4 = wxNewId();
const long xLightsFrame::ID_BUTTON_CHANGE_MEDIA_DIR = wxNewId();
const long xLightsFrame::ID_ANY = wxNewId();
const long xLightsFrame::ID_BITMAPBUTTON_Link_Dirs = wxNewId();
const long xLightsFrame::ID_BUTTON_SAVE_SETUP = wxNewId();
const long xLightsFrame::ID_BUTTON_ADD_DONGLE = wxNewId();
const long xLightsFrame::ID_BUTTON_ADD_E131 = wxNewId();
const long xLightsFrame::ID_BUTTON1 = wxNewId();
const long xLightsFrame::ID_BUTTON2 = wxNewId();
const long xLightsFrame::ID_BUTTON_ADD_LOR = wxNewId();
const long xLightsFrame::ID_BUTTON_ADD_DDP = wxNewId();
const long xLightsFrame::ID_BUTTON_NETWORK_CHANGE = wxNewId();
const long xLightsFrame::ID_BUTTON_NETWORK_DELETE = wxNewId();
const long xLightsFrame::ID_BUTTON_NETWORK_DELETE_ALL = wxNewId();
const long xLightsFrame::ID_STATICTEXT8 = wxNewId();
const long xLightsFrame::ID_SPINCTRL1 = wxNewId();
const long xLightsFrame::ID_BITMAPBUTTON1 = wxNewId();
const long xLightsFrame::ID_BITMAPBUTTON2 = wxNewId();
const long xLightsFrame::ID_LISTCTRL_NETWORKS = wxNewId();
const long xLightsFrame::ID_PANEL_SETUP = wxNewId();
const long xLightsFrame::ID_PANEL_PREVIEW = wxNewId();
const long xLightsFrame::ID_PANEL7 = wxNewId();
const long xLightsFrame::ID_NOTEBOOK1 = wxNewId();
const long xLightsFrame::ID_STATICTEXT6 = wxNewId();
const long xLightsFrame::ID_GAUGE1 = wxNewId();
const long xLightsFrame::ID_PANEL5 = wxNewId();
const long xLightsFrame::ID_STATICTEXT7 = wxNewId();
const long xLightsFrame::ID_PANEL1 = wxNewId();
const long xLightsFrame::ID_NEW_SEQUENCE = wxNewId();
const long xLightsFrame::ID_OPEN_SEQUENCE = wxNewId();
const long xLightsFrame::IS_SAVE_SEQ = wxNewId();
const long xLightsFrame::ID_SAVE_AS_SEQUENCE = wxNewId();
const long xLightsFrame::ID_CLOSE_SEQ = wxNewId();
const long xLightsFrame::ID_EXPORT_VIDEO = wxNewId();
const long xLightsFrame::ID_MENUITEM2 = wxNewId();
const long xLightsFrame::ID_FILE_BACKUP = wxNewId();
const long xLightsFrame::ID_FILE_ALTBACKUP = wxNewId();
const long xLightsFrame::ID_SHIFT_EFFECTS = wxNewId();
const long xLightsFrame::ID_MENUITEM13 = wxNewId();
const long xLightsFrame::ID_MENUITEM_CONVERT = wxNewId();
const long xLightsFrame::ID_MENUITEM_GenerateCustomModel = wxNewId();
const long xLightsFrame::ID_MNU_GENERATELYRICS = wxNewId();
const long xLightsFrame::ID_MNU_CHECKSEQ = wxNewId();
const long xLightsFrame::ID_MENU_VIEW_LOG = wxNewId();
const long xLightsFrame::ID_MENUITEM18 = wxNewId();
const long xLightsFrame::ID_EXPORT_MODELS = wxNewId();
const long xLightsFrame::ID_MNU_EXPORT_EFFECTS = wxNewId();
const long xLightsFrame::ID_MENU_FPP_CONNECT = wxNewId();
const long xLightsFrame::ID_MNU_PACKAGESEQUENCE = wxNewId();
const long xLightsFrame::ID_MENU_BATCH_RENDER = wxNewId();
const long xLightsFrame::ID_MNU_XSCHEDULE = wxNewId();
const long xLightsFrame::iD_MNU_VENDORCACHEPURGE = wxNewId();
const long xLightsFrame::ID_MNU_CRASH = wxNewId();
const long xLightsFrame::ID_MNU_DUMPRENDERSTATE = wxNewId();
const long xLightsFrame::ID_MENUITEM5 = wxNewId();
const long xLightsFrame::MNU_ID_ACLIGHTS = wxNewId();
const long xLightsFrame::ID_MNU_SHOWRAMPS = wxNewId();
const long xLightsFrame::ID_MENUITEM_SAVE_PERSPECTIVE = wxNewId();
const long xLightsFrame::ID_MENUITEM_SAVE_AS_PERSPECTIVE = wxNewId();
const long xLightsFrame::ID_MENUITEM_LOAD_PERSPECTIVE = wxNewId();
const long xLightsFrame::ID_MNU_PERSPECTIVES_AUTOSAVE = wxNewId();
const long xLightsFrame::ID_MENUITEM7 = wxNewId();
const long xLightsFrame::ID_MENUITEM_DISPLAY_ELEMENTS = wxNewId();
const long xLightsFrame::ID_MENUITEM12 = wxNewId();
const long xLightsFrame::ID_MENUITEM3 = wxNewId();
const long xLightsFrame::ID_MENUITEM14 = wxNewId();
const long xLightsFrame::ID_MENUITEM15 = wxNewId();
const long xLightsFrame::ID_MENUITEM16 = wxNewId();
const long xLightsFrame::ID_MENUITEM9 = wxNewId();
const long xLightsFrame::ID_MENUITEM17 = wxNewId();
const long xLightsFrame::ID_MENUITEM_EFFECT_ASSIST_WINDOW = wxNewId();
const long xLightsFrame::ID_MENUITEM_SELECT_EFFECT = wxNewId();
const long xLightsFrame::ID_MENUITEM_VIDEOPREVIEW = wxNewId();
const long xLightsFrame::ID_MENUITEM_WINDOWS_PERSPECTIVE = wxNewId();
const long xLightsFrame::ID_MENUITEM_WINDOWS_DOCKALL = wxNewId();
const long xLightsFrame::ID_MENUITEM11 = wxNewId();
const long xLightsFrame::ID_MENUITEM10 = wxNewId();
const long xLightsFrame::ID_PLAY_FULL = wxNewId();
const long xLightsFrame::ID_MNU_1POINT5SPEED = wxNewId();
const long xLightsFrame::ID_MN_2SPEED = wxNewId();
const long xLightsFrame::ID_MNU_3SPEED = wxNewId();
const long xLightsFrame::ID_MNU_4SPEED = wxNewId();
const long xLightsFrame::ID_PLAY_3_4 = wxNewId();
const long xLightsFrame::ID_PLAY_1_2 = wxNewId();
const long xLightsFrame::ID_PLAY_1_4 = wxNewId();
const long xLightsFrame::ID_MNU_LOUDVOLUME = wxNewId();
const long xLightsFrame::ID_MNU_MEDVOLUME = wxNewId();
const long xLightsFrame::ID_MNU_QUIET = wxNewId();
const long xLightsFrame::ID_MNU_SUPERQUIET = wxNewId();
const long xLightsFrame::ID_IMPORT_EFFECTS = wxNewId();
const long xLightsFrame::ID_SEQ_SETTINGS = wxNewId();
const long xLightsFrame::ID_RENDER_ON_SAVE = wxNewId();
const long xLightsFrame::ID_BACKUP_ON_SAVE = wxNewId();
const long xLightsFrame::ID_MENU_BACKUP_ON_LAUNCH = wxNewId();
const long xLightsFrame::ID_ALT_BACKUPLOCATION = wxNewId();
const long xLightsFrame::ID_MNU_BACKUP = wxNewId();
const long xLightsFrame::ID_MNU_EXCLUDEPRESETS = wxNewId();
const long xLightsFrame::ID_MNU_EXCLUDEAUDIOPKGSEQ = wxNewId();
const long xLightsFrame::ID_MENUITEM_ICON_SMALL = wxNewId();
const long xLightsFrame::ID_MENUITEM_ICON_MEDIUM = wxNewId();
const long xLightsFrame::ID_MENUITEM_ICON_LARGE = wxNewId();
const long xLightsFrame::ID_MENUITEM_ICON_XLARGE = wxNewId();
const long xLightsFrame::ID_MENUITEM4 = wxNewId();
const long xLightsFrame::ID_MNU_SMALLWAVEFORM = wxNewId();
const long xLightsFrame::ID_MENUITEM_GRID_ICON_XSMALL = wxNewId();
const long xLightsFrame::ID_MENUITEM_GRID_ICON_SMALL = wxNewId();
const long xLightsFrame::ID_MENUITEM_GRID_ICON_MEDIUM = wxNewId();
const long xLightsFrame::ID_MENUITEM_GRID_ICON_LARGE = wxNewId();
const long xLightsFrame::ID_MENUITEM_GRID_ICON_XLARGE = wxNewId();
const long xLightsFrame::ID_MENUITEM6 = wxNewId();
const long xLightsFrame::ID_MENUITEM_GRID_ICON_BACKGROUND_ON = wxNewId();
const long xLightsFrame::ID_MENUITEM_GRID_ICON_BACKGROUND_OFF = wxNewId();
const long xLightsFrame::ID_MENUITEM_Grid_Icon_Backgrounds = wxNewId();
const long xLightsFrame::ID_MENUITEM_GRID_NODE_VALUES_ON = wxNewId();
const long xLightsFrame::ID_MENUITEM_GRID_NODE_VALUES_OFF = wxNewId();
const long xLightsFrame::ID_MENUITEM8 = wxNewId();
const long xLightsFrame::ID_COLOR_MANAGER = wxNewId();
const long xLightsFrame::ID_MENU_CANVAS_ERASE_MODE = wxNewId();
const long xLightsFrame::ID_MENU_CANVAS_CANVAS_MODE = wxNewId();
const long xLightsFrame::ID_MENUITEM_RENDER_MODE = wxNewId();
const long xLightsFrame::ID_MENUITEM_EFFECT_ASSIST_ALWAYS_ON = wxNewId();
const long xLightsFrame::ID_MENUITEM_EFFECT_ASSIST_ALWAYS_OFF = wxNewId();
const long xLightsFrame::ID_MENUITEM_EFFECT_ASSIST_TOGGLE = wxNewId();
const long xLightsFrame::ID_MENUITEM_EFFECT_ASSIST = wxNewId();
const long xLightsFrame::ID_MENU_TIMING_EDIT_MODE = wxNewId();
const long xLightsFrame::ID_MENU_TIMING_PLAY_MODE = wxNewId();
const long xLightsFrame::ID_MENUITEM_Timing_DClick_Mode = wxNewId();
const long xLightsFrame::ID_MENU_OPENGL_AUTO = wxNewId();
const long xLightsFrame::ID_MENU_OPENGL_3 = wxNewId();
const long xLightsFrame::ID_MENU_OPENGL_2 = wxNewId();
const long xLightsFrame::ID_MENU_OPENGL_1 = wxNewId();
const long xLightsFrame::ID_MENUITEM19 = wxNewId();
const long xLightsFrame::ID_MNU_PLAYCONTROLSONPREVIEW = wxNewId();
const long xLightsFrame::ID_MNU_AUTOSHOWHOUSEPREVIEW = wxNewId();
const long xLightsFrame::ID_MENUITEM_AUTOSAVE_0 = wxNewId();
const long xLightsFrame::ID_MENUITEM_AUTOSAVE_3 = wxNewId();
const long xLightsFrame::ID_MENUITEM_AUTOSAVE_10 = wxNewId();
const long xLightsFrame::ID_MENUITEM_AUTOSAVE_15 = wxNewId();
const long xLightsFrame::ID_MENUITEM_AUTOSAVE_30 = wxNewId();
const long xLightsFrame::ID_MENUITEM20 = wxNewId();
const long xLightsFrame::ID_MNU_SD_None = wxNewId();
const long xLightsFrame::ID_MNU_SD_10 = wxNewId();
const long xLightsFrame::ID_MNU_SD_20 = wxNewId();
const long xLightsFrame::ID_MNU_SD_40 = wxNewId();
const long xLightsFrame::ID_MNU_SUPPRESSDUPLICATES = wxNewId();
const long xLightsFrame::ID_E131_Sync = wxNewId();
const long xLightsFrame::ID_MNU_FORCEIP = wxNewId();
const long xLightsFrame::ID_MNU_DEFAULTMODELBLENDOFF = wxNewId();
const long xLightsFrame::ID_MNU_SNAP_TO_TIMING = wxNewId();
const long xLightsFrame::idMenuHelpContent = wxNewId();
const long xLightsFrame::ID_MENU_HELP_FORMUM = wxNewId();
const long xLightsFrame::ID_MNU_VIDEOS = wxNewId();
const long xLightsFrame::ID_MENU_HELP_DOWNLOAD = wxNewId();
const long xLightsFrame::ID_MNU_HELP_RELEASE_NOTES = wxNewId();
const long xLightsFrame::ID_MENU_HELP_ISSUE = wxNewId();
const long xLightsFrame::ID_MENU_HELP_FACEBOOK = wxNewId();
const long xLightsFrame::ID_MNU_DONATE = wxNewId();
const long xLightsFrame::ID_MNU_UPDATE = wxNewId();
const long xLightsFrame::ID_TIMER1 = wxNewId();
const long xLightsFrame::ID_TIMER2 = wxNewId();
const long xLightsFrame::ID_TIMER_EFFECT_SETTINGS = wxNewId();
//*)

// For new sequencer
const long xLightsFrame::ID_PANEL_EFFECTS1 = wxNewId();
const long xLightsFrame::ID_PANEL_EFFECTS = wxNewId();
const long xLightsFrame::ID_NOTEBOOK_EFFECTS = wxNewId();
// End

const long xLightsFrame::ID_PLAYER_DIALOG = wxNewId();
const long xLightsFrame::ID_DELETE_EFFECT = wxNewId();
const long xLightsFrame::ID_IGNORE_CLICK = wxNewId();
const long xLightsFrame::ID_PROTECT_EFFECT = wxNewId();
const long xLightsFrame::ID_UNPROTECT_EFFECT = wxNewId();
const long xLightsFrame::ID_RANDOM_EFFECT = wxNewId();
const long xLightsFrame::ID_COPYROW_EFFECT = wxNewId(); //copy random effect across row -DJ
const long xLightsFrame::ID_CLEARROW_EFFECT = wxNewId(); //clear all effects on this row -DJ

const long xLightsFrame::ID_MENU_ITEM_PREVIEWS = wxNewId();
const long xLightsFrame::ID_MENU_ITEM_PREVIEWS_SHOW_ALL = wxNewId();

wxDEFINE_EVENT(EVT_ZOOM, wxCommandEvent);
wxDEFINE_EVENT(EVT_GSCROLL, wxCommandEvent);
wxDEFINE_EVENT(EVT_TIME_SELECTED, wxCommandEvent);
wxDEFINE_EVENT(EVT_MOUSE_POSITION, wxCommandEvent);
wxDEFINE_EVENT(EVT_ROW_HEADINGS_CHANGED, wxCommandEvent);
wxDEFINE_EVENT(EVT_WINDOW_RESIZED, wxCommandEvent);
wxDEFINE_EVENT(EVT_SELECTED_ROW_CHANGED, wxCommandEvent);
wxDEFINE_EVENT(EVT_EFFECT_CHANGED, wxCommandEvent);
wxDEFINE_EVENT(EVT_UNSELECTED_EFFECT, wxCommandEvent);
wxDEFINE_EVENT(EVT_PLAY_MODEL_EFFECT, wxCommandEvent);
wxDEFINE_EVENT(EVT_EFFECT_DROPPED, wxCommandEvent);
wxDEFINE_EVENT(EVT_EFFECT_UPDATED, wxCommandEvent);
wxDEFINE_EVENT(EVT_EFFECT_RANDOMIZE, wxCommandEvent);
wxDEFINE_EVENT(EVT_EFFECT_PALETTE_UPDATED, wxCommandEvent);
wxDEFINE_EVENT(EVT_FORCE_SEQUENCER_REFRESH, wxCommandEvent);
wxDEFINE_EVENT(EVT_LOAD_PERSPECTIVE, wxCommandEvent);
wxDEFINE_EVENT(EVT_PERSPECTIVES_CHANGED, wxCommandEvent);
wxDEFINE_EVENT(EVT_SAVE_PERSPECTIVES, wxCommandEvent);
wxDEFINE_EVENT(EVT_EXPORT_MODEL, wxCommandEvent);
wxDEFINE_EVENT(EVT_PLAY_MODEL, wxCommandEvent);
wxDEFINE_EVENT(EVT_COPY_MODEL_EFFECTS, wxCommandEvent);
wxDEFINE_EVENT(EVT_PASTE_MODEL_EFFECTS, wxCommandEvent);
wxDEFINE_EVENT(EVT_MODEL_SELECTED, wxCommandEvent);
wxDEFINE_EVENT(EVT_PLAY_SEQUENCE, wxCommandEvent);
wxDEFINE_EVENT(EVT_PAUSE_SEQUENCE, wxCommandEvent);
wxDEFINE_EVENT(EVT_TOGGLE_PLAY, wxCommandEvent);
wxDEFINE_EVENT(EVT_STOP_SEQUENCE, wxCommandEvent);
wxDEFINE_EVENT(EVT_SEQUENCE_FIRST_FRAME, wxCommandEvent);
wxDEFINE_EVENT(EVT_SEQUENCE_LAST_FRAME, wxCommandEvent);
wxDEFINE_EVENT(EVT_SEQUENCE_REWIND10, wxCommandEvent);
wxDEFINE_EVENT(EVT_SEQUENCE_FFORWARD10, wxCommandEvent);
wxDEFINE_EVENT(EVT_SEQUENCE_SEEKTO, wxCommandEvent);
wxDEFINE_EVENT(EVT_SEQUENCE_REPLAY_SECTION, wxCommandEvent);
wxDEFINE_EVENT(EVT_SHOW_DISPLAY_ELEMENTS, wxCommandEvent);
wxDEFINE_EVENT(EVT_IMPORT_TIMING, wxCommandEvent);
wxDEFINE_EVENT(EVT_IMPORT_NOTES, wxCommandEvent);
wxDEFINE_EVENT(EVT_CONVERT_DATA_TO_EFFECTS, wxCommandEvent);
wxDEFINE_EVENT(EVT_PROMOTE_EFFECTS, wxCommandEvent);
wxDEFINE_EVENT(EVT_RGBEFFECTS_CHANGED, wxCommandEvent);
wxDEFINE_EVENT(EVT_RENDER_RANGE, RenderCommandEvent);
wxDEFINE_EVENT(EVT_APPLYLAST, wxCommandEvent);
wxDEFINE_EVENT(EVT_SELECTED_EFFECT_CHANGED, SelectedEffectChangedEvent);

BEGIN_EVENT_TABLE(xLightsFrame,wxFrame)
    //(*EventTable(xLightsFrame)
    //*)
    EVT_COMMAND(wxID_ANY, EVT_ZOOM, xLightsFrame::Zoom)
    EVT_COMMAND(wxID_ANY, EVT_GSCROLL, xLightsFrame::Scroll)
    EVT_COMMAND(wxID_ANY, EVT_TIME_SELECTED, xLightsFrame::TimeSelected)
    EVT_COMMAND(wxID_ANY, EVT_MOUSE_POSITION, xLightsFrame::MousePositionUpdated)
    EVT_COMMAND(wxID_ANY, EVT_ROW_HEADINGS_CHANGED, xLightsFrame::RowHeadingsChanged)
    EVT_COMMAND(wxID_ANY, EVT_WINDOW_RESIZED, xLightsFrame::WindowResized)
    EVT_COMMAND(wxID_ANY, EVT_SELECTED_ROW_CHANGED, xLightsFrame::SelectedRowChanged)
    EVT_COMMAND(wxID_ANY, EVT_EFFECT_CHANGED, xLightsFrame::EffectChanged)
    EVT_COMMAND(wxID_ANY, EVT_UNSELECTED_EFFECT, xLightsFrame::UnselectedEffect)
    EVT_COMMAND(wxID_ANY, EVT_EFFECT_DROPPED, xLightsFrame::EffectDroppedOnGrid)
    EVT_COMMAND(wxID_ANY, EVT_PLAY_MODEL_EFFECT, xLightsFrame::PlayModelEffect)
    EVT_COMMAND(wxID_ANY, EVT_EFFECT_UPDATED, xLightsFrame::UpdateEffect)
    EVT_COMMAND(wxID_ANY, EVT_EFFECT_RANDOMIZE, xLightsFrame::RandomizeEffect)
    EVT_COMMAND(wxID_ANY, EVT_EFFECT_PALETTE_UPDATED, xLightsFrame::UpdateEffectPalette)
    EVT_COMMAND(wxID_ANY, EVT_FORCE_SEQUENCER_REFRESH, xLightsFrame::ForceSequencerRefresh)
    EVT_COMMAND(wxID_ANY, EVT_LOAD_PERSPECTIVE, xLightsFrame::LoadPerspective)
    EVT_COMMAND(wxID_ANY, EVT_SAVE_PERSPECTIVES, xLightsFrame::OnMenuItemViewSavePerspectiveSelected)
    EVT_COMMAND(wxID_ANY, EVT_PERSPECTIVES_CHANGED, xLightsFrame::PerspectivesChanged)
    EVT_COMMAND(wxID_ANY, EVT_EXPORT_MODEL, xLightsFrame::ExportModel)
    EVT_COMMAND(wxID_ANY, EVT_PLAY_MODEL, xLightsFrame::PlayModel)
    EVT_COMMAND(wxID_ANY, EVT_COPY_MODEL_EFFECTS, xLightsFrame::CopyModelEffects)
    EVT_COMMAND(wxID_ANY, EVT_PASTE_MODEL_EFFECTS, xLightsFrame::PasteModelEffects)
    EVT_COMMAND(wxID_ANY, EVT_MODEL_SELECTED, xLightsFrame::ModelSelected)
    EVT_COMMAND(wxID_ANY, EVT_PLAY_SEQUENCE, xLightsFrame::PlaySequence)
    EVT_COMMAND(wxID_ANY, EVT_PAUSE_SEQUENCE, xLightsFrame::PauseSequence)
    EVT_COMMAND(wxID_ANY, EVT_STOP_SEQUENCE, xLightsFrame::StopSequence)
    EVT_COMMAND(wxID_ANY, EVT_SEQUENCE_FIRST_FRAME, xLightsFrame::SequenceFirstFrame)
    EVT_COMMAND(wxID_ANY, EVT_SEQUENCE_LAST_FRAME, xLightsFrame::SequenceLastFrame)
    EVT_COMMAND(wxID_ANY, EVT_SEQUENCE_REWIND10, xLightsFrame::SequenceRewind10)
    EVT_COMMAND(wxID_ANY, EVT_SEQUENCE_FFORWARD10, xLightsFrame::SequenceFForward10)
    EVT_COMMAND(wxID_ANY, EVT_SEQUENCE_SEEKTO, xLightsFrame::SequenceSeekTo)
    EVT_COMMAND(wxID_ANY, EVT_SEQUENCE_REPLAY_SECTION, xLightsFrame::SequenceReplaySection)
    EVT_COMMAND(wxID_ANY, EVT_TOGGLE_PLAY, xLightsFrame::TogglePlay)
    EVT_COMMAND(wxID_ANY, EVT_SHOW_DISPLAY_ELEMENTS, xLightsFrame::ShowDisplayElements)
    EVT_COMMAND(wxID_ANY, EVT_IMPORT_TIMING, xLightsFrame::ExecuteImportTimingElement)
    EVT_COMMAND(wxID_ANY, EVT_IMPORT_NOTES, xLightsFrame::ExecuteImportNotes)
    EVT_COMMAND(wxID_ANY, EVT_CONVERT_DATA_TO_EFFECTS, xLightsFrame::ConvertDataRowToEffects)
    EVT_COMMAND(wxID_ANY, EVT_PROMOTE_EFFECTS, xLightsFrame::PromoteEffects)
    EVT_COMMAND(wxID_ANY, EVT_APPLYLAST, xLightsFrame::ApplyLast)
    EVT_COMMAND(wxID_ANY, EVT_RGBEFFECTS_CHANGED, xLightsFrame::PerspectivesChanged)
    wx__DECLARE_EVT1(EVT_RENDER_RANGE, wxID_ANY, &xLightsFrame::RenderRange)
    wx__DECLARE_EVT1(EVT_SELECTED_EFFECT_CHANGED, wxID_ANY, &xLightsFrame::SelectedEffectChanged)


END_EVENT_TABLE()



void AddEffectToolbarButtons(EffectManager &manager, xlAuiToolBar *EffectsToolBar) {

    for (int x = 0; x < manager.size(); x++) {
        DragEffectBitmapButton *BitmapButton34 = new DragEffectBitmapButton(EffectsToolBar, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize(16,16),
                                                    wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON38"));
        int size = 16;
        BitmapButton34->SetMinSize(wxSize(size,size));
        BitmapButton34->SetMaxSize(wxSize(size,size));
#ifndef LINUX
        BitmapButton34->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BACKGROUND));
#endif
        BitmapButton34->SetEffect(manager[x]);
        BitmapButton34->SetBitmapMargins(0,0);
        EffectsToolBar->AddControl(BitmapButton34, BitmapButton34->GetToolTipText());

        EffectsToolBar->FindToolByIndex(x)->SetMinSize(wxSize(size, size));
        EffectsToolBar->FindToolByIndex(x)->GetWindow()->SetSizeHints(size, size, size, size);
        EffectsToolBar->FindToolByIndex(x)->GetWindow()->SetMinSize(wxSize(size, size));
        EffectsToolBar->FindToolByIndex(x)->GetWindow()->SetMaxSize(wxSize(size, size));

    }
    EffectsToolBar->Realize();
}

xLightsFrame::xLightsFrame(wxWindow* parent, wxWindowID id) : mSequenceElements(this), AllModels(&_outputManager, this),
    layoutPanel(nullptr), color_mgr(this)
{
    static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));
    logger_base.debug("xLightsFrame being constructed.");

    SplashDialog splash(nullptr);
    splash.Show();
    splash.Update();
    wxYield();

    _fps = -1;
    mCurrentPerpective = nullptr;
    MenuItemPreviews = nullptr;
    _renderMode = false;
    _suspendAutoSave = false;
	_sequenceViewManager.SetModelManager(&AllModels);

    Bind(EVT_SELECTED_EFFECT_CHANGED, &xLightsFrame::SelectedEffectChanged, this);
    Bind(EVT_RENDER_RANGE, &xLightsFrame::RenderRange, this);
    wxHTTP::Initialize();
    Bind(wxEVT_IDLE, &xLightsFrame::OnIdle, this);

    //(*Initialize(xLightsFrame)
    wxStaticBoxSizer* StaticBoxSizer2;
    wxMenuItem* MenuItem31;
    wxMenu* MenuHelp;
    wxMenuItem* MenuItem8;
    wxFlexGridSizer* FlexGridSizerSetup;
    wxMenuItem* MenuItem26;
    wxMenuItem* MenuItem25;
    wxMenuItem* MenuItem5;
    wxMenuItem* MenuItem2;
    wxGridBagSizer* GridBagSizer1;
    wxMenuItem* MenuItem46;
    wxMenuItem* MenuItem4;
    wxMenuItem* MenuItem14;
    wxMenuItem* MenuItem11;
    wxStaticText* StaticText38;
    wxFlexGridSizer* FlexGridSizer9;
    wxMenuItem* MenuItem22;
    wxPanel* Panel1;
    wxMenuItem* MenuItem17;
    wxMenuItem* MenuItem13;
    wxMenuItem* MenuItem10;
    wxMenu* MenuItem_Grid_Icon_Backgrounds;
    wxMenuItem* MenuItem12;
    wxMenuItem* MenuItem24;
    wxMenuItem* MenuItem27;
    wxMenuItem* MenuItem44;
    wxFlexGridSizer* FlexGridSizerNetworks;
    wxMenuItem* MenuItem20;
    wxFlexGridSizer* FlexGridSizerPreview;
    wxMenuItem* MenuItem28;
    wxMenuItem* MenuItemDisplayElements;
    wxMenuItem* MenuItem6;
    wxMenuItem* MenuItem23;
    wxStaticText* StaticText28;
    wxBoxSizer* BoxSizer1;
    wxStaticBoxSizer* StaticBoxSizer1;
    wxMenuItem* MenuItem21;
    wxMenu* Menu2;
    wxMenuItem* MenuItem9;
    wxMenuItem* MenuItem45;
    wxMenuItem* MenuItemBatchRender;
    wxMenuItem* MenuItem47;
    wxMenuItem* MenuItem30;
    wxMenuItem* MenuItem48;
    wxMenuItem* MenuItem19;
    wxButton* Button03;

    Create(parent, wxID_ANY, _("<use variables in xLightsMain.h>"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    SetClientSize(wxSize(1411,1103));
    MainAuiManager = new wxAuiManager(this, wxAUI_MGR_ALLOW_FLOATING|wxAUI_MGR_ALLOW_ACTIVE_PANE|wxAUI_MGR_DEFAULT);
    MainToolBar = new xlAuiToolBar(this, ID_AUITOOLBAR_MAIN, wxDefaultPosition, wxDefaultSize, wxAUI_TB_DEFAULT_STYLE);
    MainToolBar->AddTool(ID_AUITOOLBAR_OPENSHOW, _("Open Show Directory"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_FOLDER_OPEN")),wxART_TOOLBAR), wxNullBitmap, wxITEM_NORMAL, _("Select Show Directory"), wxEmptyString, NULL);
    MainToolBar->AddTool(ID_AUITOOLBAR_NEWSEQUENCE, _("New Sequence"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_NEW")),wxART_TOOLBAR), wxNullBitmap, wxITEM_NORMAL, _("New Sequence"), wxEmptyString, NULL);
    MainToolBar->AddTool(ID_AUITOOLBAR_OPEN, _("Open Sequence"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_FILE_OPEN")),wxART_TOOLBAR), wxNullBitmap, wxITEM_NORMAL, _("Open Sequence"), wxEmptyString, NULL);
    MainToolBar->AddTool(ID_AUITOOLBAR_SAVE, _("Save"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_FILE_SAVE")),wxART_TOOLBAR), wxNullBitmap, wxITEM_NORMAL, _("Save"), wxEmptyString, NULL);
    MainToolBar->AddTool(ID_AUITOOLBAR_SAVEAS, _("Save As"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_FILE_SAVE_AS")),wxART_TOOLBAR), wxNullBitmap, wxITEM_NORMAL, _("Save As"), wxEmptyString, NULL);
    MainToolBar->AddTool(ID_AUITOOLBAR_RENDERALL, _("Render All"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlART_RENDER_ALL")),wxART_TOOLBAR), wxNullBitmap, wxITEM_NORMAL, _("Render All"), wxEmptyString, NULL);
    MainToolBar->Realize();
    MainAuiManager->AddPane(MainToolBar, wxAuiPaneInfo().Name(_T("Main Tool Bar")).ToolbarPane().Caption(_("Main Tool Bar")).CloseButton(false).Layer(10).Top().Gripper());
    PlayToolBar = new xlAuiToolBar(this, ID_AUITOOLBAR_PLAY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_DEFAULT_STYLE);
    PlayToolBar->AddTool(ID_AUITOOLBAR_PLAY_NOW, _("Play"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlART_PLAY")),wxART_TOOLBAR), wxNullBitmap, wxITEM_NORMAL, _("Play"), wxEmptyString, NULL);
    PlayToolBar->AddTool(ID_AUITOOLBAR_PAUSE, _("Pause"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlART_PAUSE")),wxART_TOOLBAR), wxNullBitmap, wxITEM_NORMAL, _("Pause"), wxEmptyString, NULL);
    PlayToolBar->AddTool(ID_AUITOOLBAR_STOP, _("Stop"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlART_STOP")),wxART_TOOLBAR), wxNullBitmap, wxITEM_NORMAL, _("Stop"), wxEmptyString, NULL);
    PlayToolBar->AddTool(ID_AUITOOLBAR_FIRST_FRAME, _("Item label"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlART_BACKWARD")),wxART_TOOLBAR), wxNullBitmap, wxITEM_NORMAL, _("First Frame"), wxEmptyString, NULL);
    PlayToolBar->AddTool(ID_AUITOOLBAR_LAST_FRAME, _("Item label"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlART_FORWARD")),wxART_TOOLBAR), wxNullBitmap, wxITEM_NORMAL, _("Last Frame"), wxEmptyString, NULL);
    PlayToolBar->AddTool(ID_AUITOOLBAR_REPLAY_SECTION, _("Item label"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlART_REPLAY")),wxART_TOOLBAR), wxNullBitmap, wxITEM_NORMAL, _("Replay Section"), wxEmptyString, NULL);
    PlayToolBar->Realize();
    MainAuiManager->AddPane(PlayToolBar, wxAuiPaneInfo().Name(_T("Play Tool Bar")).ToolbarPane().Caption(_("Play Tool Bar")).CloseButton(false).Layer(10).Position(11).Top().Gripper());
    WindowMgmtToolbar = new xlAuiToolBar(this, ID_AUIWINDOWTOOLBAR, wxDefaultPosition, wxDefaultSize, wxAUI_TB_DEFAULT_STYLE);
    WindowMgmtToolbar->AddTool(ID_AUITOOLBARITEM2, _("Effect Settings"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlART_EFFECTSETTINGS")),wxART_TOOLBAR), wxNullBitmap, wxITEM_CHECK, _("Effect Settings"), wxEmptyString, NULL);
    WindowMgmtToolbar->AddTool(ID_AUITOOLBARITEM5, _("Effect Colors"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlART_COLORS")),wxART_TOOLBAR), wxNullBitmap, wxITEM_CHECK, _("Effect Colors"), wxEmptyString, NULL);
    WindowMgmtToolbar->AddTool(ID_AUITOOLBARITEM7, _("Layer Settings"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlART_LAYERS")),wxART_TOOLBAR), wxNullBitmap, wxITEM_NORMAL, _("Layer Settings"), wxEmptyString, NULL);
    WindowMgmtToolbar->AddTool(ID_AUITOOLBARITEM3, _("Layer Blending"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlART_LAYERS2")),wxART_TOOLBAR), wxNullBitmap, wxITEM_NORMAL, _("Layer Blending"), wxEmptyString, NULL);
    WindowMgmtToolbar->AddTool(ID_AUITOOLBARITEM1, _("Model Preview"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlART_MODEL_PREVIEW")),wxART_TOOLBAR), wxNullBitmap, wxITEM_NORMAL, _("Model Preview"), wxEmptyString, NULL);
    WindowMgmtToolbar->AddTool(ID_AUITOOLBARITEM4, _("House Preview"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlART_HOUSE_PREVIEW")),wxART_TOOLBAR), wxNullBitmap, wxITEM_NORMAL, _("House Preview"), wxEmptyString, NULL);
    WindowMgmtToolbar->AddTool(ID_AUITOOLBARITEM6, _("Models"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlART_SEQUENCE_ELEMENTS")),wxART_TOOLBAR), wxNullBitmap, wxITEM_NORMAL, _("Display Elements"), wxEmptyString, NULL);
    WindowMgmtToolbar->AddTool(ID_AUITOOLBARITEM8, _("Effects"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlART_EFFECTS")),wxART_TOOLBAR), wxNullBitmap, wxITEM_NORMAL, _("Effects"), wxEmptyString, NULL);
    WindowMgmtToolbar->Realize();
    MainAuiManager->AddPane(WindowMgmtToolbar, wxAuiPaneInfo().Name(_T("Windows Tool Bar")).ToolbarPane().Caption(_("Windows Tool Bar")).CloseButton(false).Layer(10).Position(12).Top().Gripper());
    EditToolBar = new xlAuiToolBar(this, ID_AUITOOLBAR_EDIT, wxDefaultPosition, wxDefaultSize, wxAUI_TB_DEFAULT_STYLE);
    EditToolBar->AddTool(ID_PASTE_BY_TIME, _("Paste By Time"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlART_PASTE_BY_TIME")),wxART_TOOLBAR), wxNullBitmap, wxITEM_CHECK, _("Paste By Time"), wxEmptyString, NULL);
    EditToolBar->AddTool(ID_PASTE_BY_CELL, _("Paste By Cell"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlART_PASTE_BY_CELL")),wxART_TOOLBAR), wxNullBitmap, wxITEM_CHECK, _("Paste By Cell"), wxEmptyString, NULL);
    EditToolBar->Realize();
    MainAuiManager->AddPane(EditToolBar, wxAuiPaneInfo().Name(_T("Edit Tool Bar")).ToolbarPane().Caption(_("Pane caption")).CloseButton(false).Layer(10).Position(5).Top().Gripper());
    ACToolbar = new xlAuiToolBar(this, ID_AUITOOLBAR_AC, wxPoint(1,30), wxDefaultSize, wxAUI_TB_DEFAULT_STYLE);
    PanelSequenceVideoPanel = new SequenceVideoPanel( this );
    ChoiceParm1 = new wxChoice(ACToolbar, ID_CHOICE_PARM1, wxPoint(276,12), wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE_PARM1"));
    ChoiceParm1->SetSelection( ChoiceParm1->Append(_("0")) );
    ChoiceParm1->Append(_("10"));
    ChoiceParm1->Append(_("20"));
    ChoiceParm1->Append(_("25"));
    ChoiceParm1->Append(_("30"));
    ChoiceParm1->Append(_("33"));
    ChoiceParm1->Append(_("40"));
    ChoiceParm1->Append(_("50"));
    ChoiceParm1->Append(_("60"));
    ChoiceParm1->Append(_("66"));
    ChoiceParm1->Append(_("70"));
    ChoiceParm1->Append(_("75"));
    ChoiceParm1->Append(_("80"));
    ChoiceParm1->Append(_("90"));
    ChoiceParm1->Append(_("100"));
    ChoiceParm2 = new wxChoice(ACToolbar, ID_CHOICE_PARM2, wxPoint(476,11), wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE_PARM2"));
    ChoiceParm2->Append(_("0"));
    ChoiceParm2->Append(_("10"));
    ChoiceParm2->Append(_("20"));
    ChoiceParm2->Append(_("25"));
    ChoiceParm2->Append(_("30"));
    ChoiceParm2->Append(_("33"));
    ChoiceParm2->Append(_("40"));
    ChoiceParm2->Append(_("50"));
    ChoiceParm2->Append(_("60"));
    ChoiceParm2->Append(_("66"));
    ChoiceParm2->Append(_("70"));
    ChoiceParm2->Append(_("75"));
    ChoiceParm2->Append(_("80"));
    ChoiceParm2->Append(_("90"));
    ChoiceParm2->SetSelection( ChoiceParm2->Append(_("100")) );
    ACToolbar->AddTool(ID_AUITOOLBARITEM_ACDISABLED, _("Disable"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlAC_DISABLED")),wxART_TOOLBAR), wxNullBitmap, wxITEM_CHECK, wxEmptyString, wxEmptyString, NULL);
    ACToolbar->AddSeparator();
    ACToolbar->AddTool(ID_AUITOOLBARITEM_ACSELECT, _("Select"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlAC_SELECT")),wxART_TOOLBAR), wxNullBitmap, wxITEM_CHECK, _("Select - SHIFT L"), wxEmptyString, NULL);
    ACToolbar->AddTool(ID_AUITOOLBARITEM_ACOFF, _("Off"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlAC_OFF")),wxART_TOOLBAR), wxNullBitmap, wxITEM_CHECK, _("Off - DELETE"), wxEmptyString, NULL);
    ACToolbar->AddTool(ID_AUITOOLBARITEM_ACON, _("On"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlAC_ON")),wxART_TOOLBAR), wxNullBitmap, wxITEM_CHECK, _("On - O"), wxEmptyString, NULL);
    ACToolbar->AddTool(ID_AUITOOLBARITEM_ACSHIMMER, _("Shimmer"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlAC_SHIMMER")),wxART_TOOLBAR), wxNullBitmap, wxITEM_CHECK, _("Shimmer - S"), wxEmptyString, NULL);
    ACToolbar->AddTool(ID_AUITOOLBARITEM_ACTWINKLE, _("Twinkle"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlAC_TWINKLE")),wxART_TOOLBAR), wxNullBitmap, wxITEM_CHECK, _("Twinkle - K"), wxEmptyString, NULL);
    ACToolbar->AddSeparator();
    ACToolbar->AddTool(ID_AUITOOLBARITEM_ACINTENSITY, _("Intensity"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlAC_INTENSITY")),wxART_TOOLBAR), wxNullBitmap, wxITEM_CHECK, _("Intensity - I"), wxEmptyString, NULL);
    ACToolbar->AddTool(ID_AUITOOLBARITEM_ACRAMPUP, _("Ramp Up"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlAC_RAMPUP")),wxART_TOOLBAR), wxNullBitmap, wxITEM_CHECK, _("Ramp Up - U"), wxEmptyString, NULL);
    ACToolbar->AddTool(ID_AUITOOLBARITEM_ACRAMPDOWN, _("Ramp Down"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlAC_RAMPDOWN")),wxART_TOOLBAR), wxNullBitmap, wxITEM_CHECK, _("Ramp Down - D"), wxEmptyString, NULL);
    ACToolbar->AddTool(ID_AUITOOLBARITEM_ACRAMPUPDOWN, _("Ramp Up/Down"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlAC_RAMPUPDOWN")),wxART_TOOLBAR), wxNullBitmap, wxITEM_CHECK, _("Ramp Up/Down - A"), wxEmptyString, NULL);
    ACToolbar->AddControl(ChoiceParm1, _("Parm1"));
    ACToolbar->AddControl(ChoiceParm2, _("Parm2"));
    ACToolbar->AddSeparator();
    ACToolbar->AddTool(ID_AUITOOLBARITEM_ACFILL, _("Fill"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlAC_FILL")),wxART_TOOLBAR), wxNullBitmap, wxITEM_CHECK, _("Fill - F"), wxEmptyString, NULL);
    ACToolbar->AddTool(ID_AUITOOLBARITEM_ACCASCADE, _("Cascade"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlAC_CASCADE")),wxART_TOOLBAR), wxNullBitmap, wxITEM_CHECK, _("Cascade - H"), wxEmptyString, NULL);
    ACToolbar->AddSeparator();
    ACToolbar->AddTool(ID_AUITOOLBARITEM_ACFOREGROUND, _("Foreground"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlAC_FOREGROUND")),wxART_TOOLBAR), wxNullBitmap, wxITEM_CHECK, _("Foreground - G"), wxEmptyString, NULL);
    ACToolbar->AddTool(ID_AUITOOLBARITEM_ACBACKGROUND, _("Background"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlAC_BACKGROUND")),wxART_TOOLBAR), wxNullBitmap, wxITEM_CHECK, _("Background - B"), wxEmptyString, NULL);
    ACToolbar->Realize();
    MainAuiManager->AddPane(ACToolbar, wxAuiPaneInfo().Name(_T("ACToolbar")).ToolbarPane().Caption(_("AC Toolbar")).CloseButton(false).Layer(6).Top().Gripper());
    ViewToolBar = new xlAuiToolBar(this, ID_AUITOOLBAR_VIEW, wxDefaultPosition, wxDefaultSize, wxAUI_TB_DEFAULT_STYLE);
    ViewToolBar->AddTool(wxID_ZOOM_IN, _("Zoom In"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlART_ZOOM_IN")),wxART_TOOLBAR), wxNullBitmap, wxITEM_NORMAL, _("Zoom In"), wxEmptyString, NULL);
    ViewToolBar->AddTool(wxID_ZOOM_OUT, _("Zoom Out"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlART_ZOOM_OUT")),wxART_TOOLBAR), wxNullBitmap, wxITEM_NORMAL, _("Zoom Out"), wxEmptyString, NULL);
    ViewToolBar->AddTool(ID_AUITOOLBARITEM14, _("Sequence Settings"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlART_SETTINGS")),wxART_TOOLBAR), wxNullBitmap, wxITEM_NORMAL, _("Settings"), wxEmptyString, NULL);
    ViewToolBar->Realize();
    MainAuiManager->AddPane(ViewToolBar, wxAuiPaneInfo().Name(_T("View Tool Bar")).ToolbarPane().Caption(_("Pane caption")).CloseButton(false).Layer(10).Position(13).Top().Gripper());
    OutputToolBar = new xlAuiToolBar(this, ID_AUITOOLBAR_OUTPUT, wxDefaultPosition, wxDefaultSize, wxAUI_TB_DEFAULT_STYLE);
    OutputToolBar->AddTool(ID_BITMAPBUTTON_TAB_INFO, _("Information"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_INFORMATION")),wxART_TOOLBAR), wxNullBitmap, wxITEM_NORMAL, _("Tips for using current tab"), wxEmptyString, NULL);
    OutputToolBar->AddTool(ID_BUTTON_STOP_NOW, _("Stop"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlART_STOP_NOW")),wxART_TOOLBAR), wxNullBitmap, wxITEM_NORMAL, _("Stop Now!"), wxEmptyString, NULL);
    OutputToolBar->AddTool(ID_BUTTON_LIGHTS_OFF, _("Lights Off"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlART_LIGHTS_OFF")),wxART_TOOLBAR), wxNullBitmap, wxITEM_NORMAL, _("Lights Off"), wxEmptyString, NULL);
    OutputToolBar->AddTool(ID_CHECKBOX_LIGHT_OUTPUT, _("Output To Lights"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlART_OUTPUT_LIGHTS")),wxART_TOOLBAR), wxNullBitmap, wxITEM_CHECK, _("Output To Lights"), wxEmptyString, NULL);
    OutputToolBar->Realize();
    MainAuiManager->AddPane(OutputToolBar, wxAuiPaneInfo().Name(_T("Output Tool Bar")).ToolbarPane().Caption(_("Output Tool Bar")).CloseButton(false).Layer(10).Position(25).Top().Gripper());
    EffectsToolBar = new xlAuiToolBar(this, ID_AUIEFFECTSTOOLBAR, wxDefaultPosition, wxDefaultSize, wxAUI_TB_DEFAULT_STYLE);
    EffectsToolBar->Realize();
    MainAuiManager->AddPane(EffectsToolBar, wxAuiPaneInfo().Name(_T("EffectsToolBar")).ToolbarPane().Caption(_("Effects")).CloseButton(false).Layer(5).Top().Gripper());
    Notebook1 = new wxAuiNotebook(this, ID_NOTEBOOK1, wxDefaultPosition, wxDefaultSize, wxAUI_NB_SCROLL_BUTTONS|wxAUI_NB_TOP|wxNO_BORDER);
    PanelSetup = new wxPanel(Notebook1, ID_PANEL_SETUP, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_SETUP"));
    FlexGridSizerSetup = new wxFlexGridSizer(0, 1, 0, 0);
    FlexGridSizerSetup->AddGrowableCol(0);
    FlexGridSizerSetup->AddGrowableRow(1);
    StaticBoxSizer1 = new wxStaticBoxSizer(wxVERTICAL, PanelSetup, _("Directories"));
    GridBagSizer1 = new wxGridBagSizer(0, 0);
    StaticText38 = new wxStaticText(PanelSetup, wxID_ANY, _("Show Directory:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
    GridBagSizer1->Add(StaticText38, wxGBPosition(0, 0), wxDefaultSpan, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    Button03 = new wxButton(PanelSetup, ID_BUTTON3, _("Change"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
    GridBagSizer1->Add(Button03, wxGBPosition(0, 1), wxDefaultSpan, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ShowDirectoryLabel = new wxStaticText(PanelSetup, ID_STATICTEXT4, _("{Show Directory not set}"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
    GridBagSizer1->Add(ShowDirectoryLabel, wxGBPosition(0, 2), wxDefaultSpan, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    StaticText28 = new wxStaticText(PanelSetup, wxID_ANY, _("Media Directory:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
    GridBagSizer1->Add(StaticText28, wxGBPosition(1, 0), wxDefaultSpan, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    Button_Change_Media_Dir = new wxButton(PanelSetup, ID_BUTTON_CHANGE_MEDIA_DIR, _("Change"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON_CHANGE_MEDIA_DIR"));
    GridBagSizer1->Add(Button_Change_Media_Dir, wxGBPosition(1, 1), wxDefaultSpan, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    MediaDirectoryLabel = new wxStaticText(PanelSetup, ID_ANY, _("{Media Directory not set}"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_ANY"));
    GridBagSizer1->Add(MediaDirectoryLabel, wxGBPosition(1, 2), wxDefaultSpan, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    BitmapButton_Link_Dirs = new wxBitmapButton(PanelSetup, ID_BITMAPBUTTON_Link_Dirs, wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlART_UNLINK")),wxART_OTHER), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON_Link_Dirs"));
    BitmapButton_Link_Dirs->SetToolTip(_("Link Directories"));
    GridBagSizer1->Add(BitmapButton_Link_Dirs, wxGBPosition(0, 3), wxGBSpan(2, 1), wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticBoxSizer1->Add(GridBagSizer1, 1, wxALL|wxEXPAND, 5);
    FlexGridSizerSetup->Add(StaticBoxSizer1, 1, wxALL|wxEXPAND, 5);
    StaticBoxSizer2 = new wxStaticBoxSizer(wxHORIZONTAL, PanelSetup, _("Lighting Networks"));
    FlexGridSizerNetworks = new wxFlexGridSizer(0, 3, 0, 0);
    FlexGridSizerNetworks->AddGrowableCol(2);
    FlexGridSizerNetworks->AddGrowableRow(0);
    BoxSizer1 = new wxBoxSizer(wxVERTICAL);
    ButtonSaveSetup = new wxButton(PanelSetup, ID_BUTTON_SAVE_SETUP, _("Save Setup"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON_SAVE_SETUP"));
    BoxSizer1->Add(ButtonSaveSetup, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    ButtonAddDongle = new wxButton(PanelSetup, ID_BUTTON_ADD_DONGLE, _("Add USB"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON_ADD_DONGLE"));
    BoxSizer1->Add(ButtonAddDongle, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    ButtonAddE131 = new wxButton(PanelSetup, ID_BUTTON_ADD_E131, _("Add E1.31"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON_ADD_E131"));
    BoxSizer1->Add(ButtonAddE131, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    ButtonAddNull = new wxButton(PanelSetup, ID_BUTTON1, _("Add Null"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    BoxSizer1->Add(ButtonAddNull, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ButtonArtNET = new wxButton(PanelSetup, ID_BUTTON2, _("Add ArtNET"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
    BoxSizer1->Add(ButtonArtNET, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ButtonAddLOR = new wxButton(PanelSetup, ID_BUTTON_ADD_LOR, _("Add LOR"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON_ADD_LOR"));
    BoxSizer1->Add(ButtonAddLOR, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ButtonAddDDP = new wxButton(PanelSetup, ID_BUTTON_ADD_DDP, _("Add DDP"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON_ADD_DDP"));
    BoxSizer1->Add(ButtonAddDDP, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ButtonNetworkChange = new wxButton(PanelSetup, ID_BUTTON_NETWORK_CHANGE, _("Change"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON_NETWORK_CHANGE"));
    BoxSizer1->Add(ButtonNetworkChange, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    ButtonNetworkDelete = new wxButton(PanelSetup, ID_BUTTON_NETWORK_DELETE, _("Delete"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON_NETWORK_DELETE"));
    BoxSizer1->Add(ButtonNetworkDelete, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    ButtonNetworkDeleteAll = new wxButton(PanelSetup, ID_BUTTON_NETWORK_DELETE_ALL, _("Delete All"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON_NETWORK_DELETE_ALL"));
    BoxSizer1->Add(ButtonNetworkDeleteAll, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    StaticText5 = new wxStaticText(PanelSetup, ID_STATICTEXT8, _("\nE1.31 Sync Universe:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
    BoxSizer1->Add(StaticText5, 1, wxALL|wxALIGN_LEFT, 2);
    SpinCtrl_SyncUniverse = new wxSpinCtrl(PanelSetup, ID_SPINCTRL1, _T("0"), wxDefaultPosition, wxDefaultSize, 0, 0, 63999, 0, _T("ID_SPINCTRL1"));
    SpinCtrl_SyncUniverse->SetValue(_T("0"));
    SpinCtrl_SyncUniverse->SetToolTip(_("This should be left as 0 unless you have controllers which support it."));
    BoxSizer1->Add(SpinCtrl_SyncUniverse, 1, wxALL|wxEXPAND, 5);
    FlexGridSizerNetworks->Add(BoxSizer1, 1, wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 0);
    FlexGridSizer9 = new wxFlexGridSizer(0, 1, 0, 0);
    BitmapButtonMoveNetworkUp = new wxBitmapButton(PanelSetup, ID_BITMAPBUTTON1, wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_GO_UP")),wxART_BUTTON), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON1"));
    BitmapButtonMoveNetworkUp->SetToolTip(_("Move selected item up"));
    FlexGridSizer9->Add(BitmapButtonMoveNetworkUp, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    BitmapButtonMoveNetworkDown = new wxBitmapButton(PanelSetup, ID_BITMAPBUTTON2, wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_GO_DOWN")),wxART_BUTTON), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON2"));
    BitmapButtonMoveNetworkDown->SetToolTip(_("Move selected item down"));
    FlexGridSizer9->Add(BitmapButtonMoveNetworkDown, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizerNetworks->Add(FlexGridSizer9, 1, wxBOTTOM|wxLEFT|wxALIGN_LEFT|wxALIGN_TOP, 10);
    GridNetwork = new wxListCtrl(PanelSetup, ID_LISTCTRL_NETWORKS, wxDefaultPosition, wxDefaultSize, wxLC_REPORT, wxDefaultValidator, _T("ID_LISTCTRL_NETWORKS"));
    GridNetwork->SetToolTip(_("Drag an item to reorder the list"));
    FlexGridSizerNetworks->Add(GridNetwork, 1, wxEXPAND, 5);
    StaticBoxSizer2->Add(FlexGridSizerNetworks, 1, wxALL|wxEXPAND, 5);
    FlexGridSizerSetup->Add(StaticBoxSizer2, 1, wxALL|wxEXPAND, 5);
    PanelSetup->SetSizer(FlexGridSizerSetup);
    FlexGridSizerSetup->Fit(PanelSetup);
    FlexGridSizerSetup->SetSizeHints(PanelSetup);
    PanelPreview = new wxPanel(Notebook1, ID_PANEL_PREVIEW, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_PREVIEW"));
    FlexGridSizerPreview = new wxFlexGridSizer(1, 1, 0, 0);
    FlexGridSizerPreview->AddGrowableCol(0);
    FlexGridSizerPreview->AddGrowableRow(0);
    PanelPreview->SetSizer(FlexGridSizerPreview);
    FlexGridSizerPreview->Fit(PanelPreview);
    FlexGridSizerPreview->SetSizeHints(PanelPreview);
    PanelSequencer = new wxPanel(Notebook1, ID_PANEL7, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxWANTS_CHARS, _T("ID_PANEL7"));
    m_mgr = new wxAuiManager(PanelSequencer, wxAUI_MGR_ALLOW_FLOATING|wxAUI_MGR_ALLOW_ACTIVE_PANE|wxAUI_MGR_DEFAULT);
    Notebook1->AddPage(PanelSetup, _("Setup"), true);
    Notebook1->AddPage(PanelPreview, _("Layout"));
    Notebook1->AddPage(PanelSequencer, _("Sequencer"));
    MainAuiManager->AddPane(Notebook1, wxAuiPaneInfo().Name(_T("MainPain")).CenterPane().Caption(_("Pane caption")).PaneBorder(false));
    AUIStatusBar = new wxPanel(this, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    StatusBarSizer = new wxGridBagSizer(0, 0);
    StatusBarSizer->AddGrowableRow(0);
    StatusText = new wxStaticText(AUIStatusBar, ID_STATICTEXT6, _("Label"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
    StatusBarSizer->Add(StatusText, wxGBPosition(0, 0), wxDefaultSpan, wxALL|wxEXPAND, 2);
    Panel1 = new wxPanel(AUIStatusBar, ID_PANEL5, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL5"));
    Panel1->SetMinSize(wxDLG_UNIT(AUIStatusBar,wxSize(100,-1)));
    GaugeSizer = new wxFlexGridSizer(1, 1, 0, 0);
    GaugeSizer->AddGrowableCol(0);
    ProgressBar = new wxGauge(Panel1, ID_GAUGE1, 100, wxDefaultPosition, wxDLG_UNIT(Panel1,wxSize(100,-1)), 0, wxDefaultValidator, _T("ID_GAUGE1"));
    GaugeSizer->Add(ProgressBar, 0, wxEXPAND, 0);
    Panel1->SetSizer(GaugeSizer);
    GaugeSizer->Fit(Panel1);
    GaugeSizer->SetSizeHints(Panel1);
    StatusBarSizer->Add(Panel1, wxGBPosition(0, 1), wxDefaultSpan, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    FileNameText = new wxStaticText(AUIStatusBar, ID_STATICTEXT7, _("Label"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
    StatusBarSizer->Add(FileNameText, wxGBPosition(0, 2), wxDefaultSpan, wxALL|wxEXPAND, 2);
    AUIStatusBar->SetSizer(StatusBarSizer);
    StatusBarSizer->Fit(AUIStatusBar);
    StatusBarSizer->SetSizeHints(AUIStatusBar);
    MainAuiManager->AddPane(AUIStatusBar, wxAuiPaneInfo().Name(_T("Status Bar")).DefaultPane().Caption(_("Status bar")).CaptionVisible(false).CloseButton(false).Bottom().DockFixed().Dockable(false).Floatable(false).FloatingPosition(wxPoint(0,0)).FloatingSize(wxSize(0,0)).Movable(false).PaneBorder(false));
    MainAuiManager->Update();
    MenuBar = new wxMenuBar();
    MenuFile = new wxMenu();
    MenuItem3 = new wxMenuItem(MenuFile, ID_NEW_SEQUENCE, _("New Sequence\tCtrl-n"), wxEmptyString, wxITEM_NORMAL);
    MenuItem3->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_NEW")),wxART_OTHER));
    MenuFile->Append(MenuItem3);
    MenuItem_File_Open_Sequence = new wxMenuItem(MenuFile, ID_OPEN_SEQUENCE, _("Open Sequence\tCTRL-o"), wxEmptyString, wxITEM_NORMAL);
    MenuItem_File_Open_Sequence->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_FILE_OPEN")),wxART_OTHER));
    MenuFile->Append(MenuItem_File_Open_Sequence);
    MenuItem_File_Save = new wxMenuItem(MenuFile, IS_SAVE_SEQ, _("Save\tCTRL-S"), wxEmptyString, wxITEM_NORMAL);
    MenuItem_File_Save->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_FILE_SAVE")),wxART_OTHER));
    MenuFile->Append(MenuItem_File_Save);
    MenuItem_File_Save->Enable(false);
    MenuItem_File_SaveAs_Sequence = new wxMenuItem(MenuFile, ID_SAVE_AS_SEQUENCE, _("Save Sequence As"), wxEmptyString, wxITEM_NORMAL);
    MenuItem_File_SaveAs_Sequence->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_FILE_SAVE_AS")),wxART_OTHER));
    MenuFile->Append(MenuItem_File_SaveAs_Sequence);
    MenuItem_File_Close_Sequence = new wxMenuItem(MenuFile, ID_CLOSE_SEQ, _("Close Sequence"), wxEmptyString, wxITEM_NORMAL);
    MenuFile->Append(MenuItem_File_Close_Sequence);
    MenuItem_File_Close_Sequence->Enable(false);
    MenuFile->AppendSeparator();
    MenuItem_File_Export_Video = new wxMenuItem(MenuFile, ID_EXPORT_VIDEO, _("Export House Preview Video"), wxEmptyString, wxITEM_NORMAL);
    MenuFile->Append(MenuItem_File_Export_Video);
    MenuFile->AppendSeparator();
    MenuItem5 = new wxMenuItem(MenuFile, ID_MENUITEM2, _("Select Show Folder\tF9"), wxEmptyString, wxITEM_NORMAL);
    MenuItem5->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_FOLDER")),wxART_OTHER));
    MenuFile->Append(MenuItem5);
    MenuItemBackup = new wxMenuItem(MenuFile, ID_FILE_BACKUP, _("Backup\tF10"), wxEmptyString, wxITEM_NORMAL);
    MenuItemBackup->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_HARDDISK")),wxART_OTHER));
    MenuFile->Append(MenuItemBackup);
    mAltBackupMenuItem = new wxMenuItem(MenuFile, ID_FILE_ALTBACKUP, _("Alternate Backup\tF11"), wxEmptyString, wxITEM_NORMAL);
    MenuFile->Append(mAltBackupMenuItem);
    QuitMenuItem = new wxMenuItem(MenuFile, wxID_EXIT, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    QuitMenuItem->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_QUIT")),wxART_OTHER));
    MenuFile->Append(QuitMenuItem);
    MenuBar->Append(MenuFile, _("&File"));
    Menu3 = new wxMenu();
    MenuItem37 = new wxMenuItem(Menu3, wxID_UNDO, _("Undo\tCtrl-z"), wxEmptyString, wxITEM_NORMAL);
    MenuItem37->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_UNDO")),wxART_OTHER));
    Menu3->Append(MenuItem37);
    Menu3->AppendSeparator();
    MenuItem34 = new wxMenuItem(Menu3, wxID_CUT, _("Cut\tCTRL-x"), wxEmptyString, wxITEM_NORMAL);
    MenuItem34->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_CUT")),wxART_OTHER));
    Menu3->Append(MenuItem34);
    MenuItem35 = new wxMenuItem(Menu3, wxID_COPY, _("Copy\tCTRL-c"), wxEmptyString, wxITEM_NORMAL);
    MenuItem35->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_COPY")),wxART_OTHER));
    Menu3->Append(MenuItem35);
    MenuItem36 = new wxMenuItem(Menu3, wxID_PASTE, _("Paste\tCTRL-v"), wxEmptyString, wxITEM_NORMAL);
    MenuItem36->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_PASTE")),wxART_OTHER));
    Menu3->Append(MenuItem36);
    Menu3->AppendSeparator();
    MenuItemShiftEffects = new wxMenuItem(Menu3, ID_SHIFT_EFFECTS, _("Shift Effects"), _("Use this options to shift all effects in the sequence."), wxITEM_NORMAL);
    Menu3->Append(MenuItemShiftEffects);
    MenuBar->Append(Menu3, _("&Edit"));
    Menu1 = new wxMenu();
    ActionTestMenuItem = new wxMenuItem(Menu1, ID_MENUITEM13, _("&Test"), wxEmptyString, wxITEM_NORMAL);
    Menu1->Append(ActionTestMenuItem);
    MenuItemConvert = new wxMenuItem(Menu1, ID_MENUITEM_CONVERT, _("&Convert"), wxEmptyString, wxITEM_NORMAL);
    Menu1->Append(MenuItemConvert);
    Menu_GenerateCustomModel = new wxMenuItem(Menu1, ID_MENUITEM_GenerateCustomModel, _("&Generate Custom Model"), wxEmptyString, wxITEM_NORMAL);
    Menu1->Append(Menu_GenerateCustomModel);
    MenuItem_GenerateLyrics = new wxMenuItem(Menu1, ID_MNU_GENERATELYRICS, _("Generate &Lyrics From Data"), _("Generate lyric phenomes from data"), wxITEM_NORMAL);
    Menu1->Append(MenuItem_GenerateLyrics);
    MenuItemCheckSequence = new wxMenuItem(Menu1, ID_MNU_CHECKSEQ, _("C&heck Sequence"), wxEmptyString, wxITEM_NORMAL);
    Menu1->Append(MenuItemCheckSequence);
    MenuItem_ViewLog = new wxMenuItem(Menu1, ID_MENU_VIEW_LOG, _("&View Log"), wxEmptyString, wxITEM_NORMAL);
    Menu1->Append(MenuItem_ViewLog);
    MenuItem38 = new wxMenuItem(Menu1, ID_MENUITEM18, _("&Package Log FIles"), _("Packages up current configuration, logs and sequence for reporting a problem to development team."), wxITEM_NORMAL);
    Menu1->Append(MenuItem38);
    mExportModelsMenuItem = new wxMenuItem(Menu1, ID_EXPORT_MODELS, _("E&xport Models"), wxEmptyString, wxITEM_NORMAL);
    Menu1->Append(mExportModelsMenuItem);
    MenuItem_ExportEffects = new wxMenuItem(Menu1, ID_MNU_EXPORT_EFFECTS, _("Export &Effects"), wxEmptyString, wxITEM_NORMAL);
    Menu1->Append(MenuItem_ExportEffects);
    MenuItem_FPP_Connect = new wxMenuItem(Menu1, ID_MENU_FPP_CONNECT, _("&FPP Connect"), wxEmptyString, wxITEM_NORMAL);
    Menu1->Append(MenuItem_FPP_Connect);
    MenuItem_PackageSequence = new wxMenuItem(Menu1, ID_MNU_PACKAGESEQUENCE, _("Package &Sequence"), wxEmptyString, wxITEM_NORMAL);
    Menu1->Append(MenuItem_PackageSequence);
    MenuItemBatchRender = new wxMenuItem(Menu1, ID_MENU_BATCH_RENDER, _("&Batch Render"), wxEmptyString, wxITEM_NORMAL);
    Menu1->Append(MenuItemBatchRender);
    MenuItem_xSchedule = new wxMenuItem(Menu1, ID_MNU_XSCHEDULE, _("xSchedu&le"), wxEmptyString, wxITEM_NORMAL);
    Menu1->Append(MenuItem_xSchedule);
    MenuItem_PurgeVendorCache = new wxMenuItem(Menu1, iD_MNU_VENDORCACHEPURGE, _("Purge Download Cache"), wxEmptyString, wxITEM_NORMAL);
    Menu1->Append(MenuItem_PurgeVendorCache);
    MenuItem_CrashXLights = new wxMenuItem(Menu1, ID_MNU_CRASH, _("Crash xLights"), wxEmptyString, wxITEM_NORMAL);
    Menu1->Append(MenuItem_CrashXLights);
    MenuItem_LogRenderState = new wxMenuItem(Menu1, ID_MNU_DUMPRENDERSTATE, _("Log Render State"), wxEmptyString, wxITEM_NORMAL);
    Menu1->Append(MenuItem_LogRenderState);
    MenuBar->Append(Menu1, _("&Tools"));
    MenuView = new wxMenu();
    MenuItem_ViewZoomIn = new wxMenuItem(MenuView, wxID_ZOOM_IN, _("Zoom In"), wxEmptyString, wxITEM_NORMAL);
    MenuView->Append(MenuItem_ViewZoomIn);
    MenuItem_ViewZoomOut = new wxMenuItem(MenuView, wxID_ZOOM_OUT, _("Zoom Out"), wxEmptyString, wxITEM_NORMAL);
    MenuView->Append(MenuItem_ViewZoomOut);
    MenuView->AppendSeparator();
    MenuItem13 = new wxMenuItem(MenuView, ID_MENUITEM5, _("Reset Toolbars"), wxEmptyString, wxITEM_NORMAL);
    MenuView->Append(MenuItem13);
    MenuItem_ACLIghts = new wxMenuItem(MenuView, MNU_ID_ACLIGHTS, _("AC Lights Toolbar"), wxEmptyString, wxITEM_CHECK);
    MenuView->Append(MenuItem_ACLIghts);
    MenuItem_ShowACRamps = new wxMenuItem(MenuView, ID_MNU_SHOWRAMPS, _("Show AC Ramps"), _("Show on effects and twinkle effects as ramps."), wxITEM_CHECK);
    MenuView->Append(MenuItem_ShowACRamps);
    MenuView->AppendSeparator();
    MenuItemPerspectives = new wxMenu();
    MenuItemViewSavePerspective = new wxMenuItem(MenuItemPerspectives, ID_MENUITEM_SAVE_PERSPECTIVE, _("Save Current"), wxEmptyString, wxITEM_NORMAL);
    MenuItemPerspectives->Append(MenuItemViewSavePerspective);
    MenuItemViewSaveAsPerspective = new wxMenuItem(MenuItemPerspectives, ID_MENUITEM_SAVE_AS_PERSPECTIVE, _("Save As New"), wxEmptyString, wxITEM_NORMAL);
    MenuItemPerspectives->Append(MenuItemViewSaveAsPerspective);
    MenuItemLoadEditPerspective = new wxMenuItem(MenuItemPerspectives, ID_MENUITEM_LOAD_PERSPECTIVE, _("Edit/Load"), wxEmptyString, wxITEM_NORMAL);
    MenuItemPerspectives->Append(MenuItemLoadEditPerspective);
    MenuItem_PerspectiveAutosave = new wxMenuItem(MenuItemPerspectives, ID_MNU_PERSPECTIVES_AUTOSAVE, _("Auto Save"), _("Save the current perspective between sessions independent of the show directory."), wxITEM_CHECK);
    MenuItemPerspectives->Append(MenuItem_PerspectiveAutosave);
    MenuItemPerspectives->AppendSeparator();
    MenuView->Append(ID_MENUITEM7, _("Perspectives"), MenuItemPerspectives, wxEmptyString);
    MenuItem18 = new wxMenu();
    MenuItemDisplayElements = new wxMenuItem(MenuItem18, ID_MENUITEM_DISPLAY_ELEMENTS, _("Display Elements"), wxEmptyString, wxITEM_NORMAL);
    MenuItem18->Append(MenuItemDisplayElements);
    MenuItem20 = new wxMenuItem(MenuItem18, ID_MENUITEM12, _("Model Preview"), wxEmptyString, wxITEM_NORMAL);
    MenuItem18->Append(MenuItem20);
    MenuItem6 = new wxMenuItem(MenuItem18, ID_MENUITEM3, _("House Preview"), wxEmptyString, wxITEM_NORMAL);
    MenuItem18->Append(MenuItem6);
    MenuItem22 = new wxMenuItem(MenuItem18, ID_MENUITEM14, _("Effect Settings"), wxEmptyString, wxITEM_NORMAL);
    MenuItem18->Append(MenuItem22);
    MenuItem23 = new wxMenuItem(MenuItem18, ID_MENUITEM15, _("Colors"), wxEmptyString, wxITEM_NORMAL);
    MenuItem18->Append(MenuItem23);
    MenuItem24 = new wxMenuItem(MenuItem18, ID_MENUITEM16, _("Layer Blending"), wxEmptyString, wxITEM_NORMAL);
    MenuItem18->Append(MenuItem24);
    MenuItem32 = new wxMenuItem(MenuItem18, ID_MENUITEM9, _("Layer Settings"), wxEmptyString, wxITEM_NORMAL);
    MenuItem18->Append(MenuItem32);
    MenuItem25 = new wxMenuItem(MenuItem18, ID_MENUITEM17, _("Effect Dropper"), wxEmptyString, wxITEM_NORMAL);
    MenuItem18->Append(MenuItem25);
    MenuItemEffectAssistWindow = new wxMenuItem(MenuItem18, ID_MENUITEM_EFFECT_ASSIST_WINDOW, _("Effect Assist"), wxEmptyString, wxITEM_NORMAL);
    MenuItem18->Append(MenuItemEffectAssistWindow);
    MenuItemSelectEffect = new wxMenuItem(MenuItem18, ID_MENUITEM_SELECT_EFFECT, _("Select Effect"), wxEmptyString, wxITEM_NORMAL);
    MenuItem18->Append(MenuItemSelectEffect);
    MenuItem52 = new wxMenuItem(MenuItem18, ID_MENUITEM_VIDEOPREVIEW, _("Video Preview"), wxEmptyString, wxITEM_NORMAL);
    MenuItem18->Append(MenuItem52);
    MenuItem18->AppendSeparator();
    MenuItem26 = new wxMenuItem(MenuItem18, ID_MENUITEM_WINDOWS_PERSPECTIVE, _("Perspectives"), wxEmptyString, wxITEM_NORMAL);
    MenuItem18->Append(MenuItem26);
    MenuItem21 = new wxMenuItem(MenuItem18, ID_MENUITEM_WINDOWS_DOCKALL, _("Dock All"), wxEmptyString, wxITEM_NORMAL);
    MenuItem18->Append(MenuItem21);
    MenuItem33 = new wxMenuItem(MenuItem18, ID_MENUITEM11, _("Reset to Defaults"), wxEmptyString, wxITEM_NORMAL);
    MenuItem18->Append(MenuItem33);
    MenuView->Append(ID_MENUITEM10, _("Windows"), MenuItem18, wxEmptyString);
    MenuBar->Append(MenuView, _("&View"));
    AudioMenu = new wxMenu();
    MenuItem8 = new wxMenuItem(AudioMenu, ID_PLAY_FULL, _("Play Full Speed"), wxEmptyString, wxITEM_RADIO);
    AudioMenu->Append(MenuItem8);
    MenuItem39 = new wxMenuItem(AudioMenu, ID_MNU_1POINT5SPEED, _("Play 1.5x Speed"), wxEmptyString, wxITEM_RADIO);
    AudioMenu->Append(MenuItem39);
    MenuItem49 = new wxMenuItem(AudioMenu, ID_MN_2SPEED, _("Play 2x Speed"), wxEmptyString, wxITEM_RADIO);
    AudioMenu->Append(MenuItem49);
    MenuItem50 = new wxMenuItem(AudioMenu, ID_MNU_3SPEED, _("Play 3x Speed"), wxEmptyString, wxITEM_RADIO);
    AudioMenu->Append(MenuItem50);
    MenuItem51 = new wxMenuItem(AudioMenu, ID_MNU_4SPEED, _("Play 4x Speed"), wxEmptyString, wxITEM_RADIO);
    AudioMenu->Append(MenuItem51);
    MenuItem9 = new wxMenuItem(AudioMenu, ID_PLAY_3_4, _("Play 3/4 Speed"), wxEmptyString, wxITEM_RADIO);
    AudioMenu->Append(MenuItem9);
    MenuItem30 = new wxMenuItem(AudioMenu, ID_PLAY_1_2, _("Play 1/2 Speed"), wxEmptyString, wxITEM_RADIO);
    AudioMenu->Append(MenuItem30);
    MenuItem31 = new wxMenuItem(AudioMenu, ID_PLAY_1_4, _("Play 1/4 Speed"), wxEmptyString, wxITEM_RADIO);
    AudioMenu->Append(MenuItem31);
    AudioMenu->AppendSeparator();
    MenuItem_LoudVol = new wxMenuItem(AudioMenu, ID_MNU_LOUDVOLUME, _("Loud"), wxEmptyString, wxITEM_RADIO);
    AudioMenu->Append(MenuItem_LoudVol);
    MenuItem_MedVol = new wxMenuItem(AudioMenu, ID_MNU_MEDVOLUME, _("Medium"), wxEmptyString, wxITEM_RADIO);
    AudioMenu->Append(MenuItem_MedVol);
    MenuItem_QuietVol = new wxMenuItem(AudioMenu, ID_MNU_QUIET, _("Quiet"), wxEmptyString, wxITEM_RADIO);
    AudioMenu->Append(MenuItem_QuietVol);
    MenuItem_VQuietVol = new wxMenuItem(AudioMenu, ID_MNU_SUPERQUIET, _("Very Quiet"), wxEmptyString, wxITEM_RADIO);
    AudioMenu->Append(MenuItem_VQuietVol);
    MenuBar->Append(AudioMenu, _("&Audio"));
    Menu2 = new wxMenu();
    MenuItem_ImportEffects = new wxMenuItem(Menu2, ID_IMPORT_EFFECTS, _("Import Effects"), wxEmptyString, wxITEM_NORMAL);
    Menu2->Append(MenuItem_ImportEffects);
    MenuBar->Append(Menu2, _("&Import"));
    MenuSettings = new wxMenu();
    Menu_Settings_Sequence = new wxMenuItem(MenuSettings, ID_SEQ_SETTINGS, _("Sequence Settings"), wxEmptyString, wxITEM_NORMAL);
    MenuSettings->Append(Menu_Settings_Sequence);
    mRenderOnSaveMenuItem = new wxMenuItem(MenuSettings, ID_RENDER_ON_SAVE, _("Render On Save"), wxEmptyString, wxITEM_CHECK);
    MenuSettings->Append(mRenderOnSaveMenuItem);
    mRenderOnSaveMenuItem->Check(true);
    mBackupOnSaveMenuItem = new wxMenuItem(MenuSettings, ID_BACKUP_ON_SAVE, _("Backup On Save"), wxEmptyString, wxITEM_CHECK);
    MenuSettings->Append(mBackupOnSaveMenuItem);
    MenuItem_BackupOnLaunch = new wxMenuItem(MenuSettings, ID_MENU_BACKUP_ON_LAUNCH, _("Backup On Launch"), _("Recommended."), wxITEM_CHECK);
    MenuSettings->Append(MenuItem_BackupOnLaunch);
    MenuItem_BackupOnLaunch->Check(true);
    mAltBackupLocationMenuItem = new wxMenuItem(MenuSettings, ID_ALT_BACKUPLOCATION, _("Alt Backup Location"), wxEmptyString, wxITEM_NORMAL);
    MenuSettings->Append(mAltBackupLocationMenuItem);
    MenuItem_BackupSubfolders = new wxMenuItem(MenuSettings, ID_MNU_BACKUP, _("Backup Subfolders"), wxEmptyString, wxITEM_CHECK);
    MenuSettings->Append(MenuItem_BackupSubfolders);
    MenuItem_ExcludePresetsFromPackagedSequences = new wxMenuItem(MenuSettings, ID_MNU_EXCLUDEPRESETS, _("Exclude Presets From Packaged Sequences"), wxEmptyString, wxITEM_CHECK);
    MenuSettings->Append(MenuItem_ExcludePresetsFromPackagedSequences);
    MenuItem_ExcludeAudioPackagedSequence = new wxMenuItem(MenuSettings, ID_MNU_EXCLUDEAUDIOPKGSEQ, _("Exclude Audio From Packaged Sequences"), wxEmptyString, wxITEM_CHECK);
    MenuSettings->Append(MenuItem_ExcludeAudioPackagedSequence);
    ToolIconSizeMenu = new wxMenu();
    MenuItem10 = new wxMenuItem(ToolIconSizeMenu, ID_MENUITEM_ICON_SMALL, _("Small\tALT-1"), wxEmptyString, wxITEM_RADIO);
    ToolIconSizeMenu->Append(MenuItem10);
    MenuItem11 = new wxMenuItem(ToolIconSizeMenu, ID_MENUITEM_ICON_MEDIUM, _("Medium\tALT-2"), wxEmptyString, wxITEM_RADIO);
    ToolIconSizeMenu->Append(MenuItem11);
    MenuItem12 = new wxMenuItem(ToolIconSizeMenu, ID_MENUITEM_ICON_LARGE, _("Large\tALT-3"), wxEmptyString, wxITEM_RADIO);
    ToolIconSizeMenu->Append(MenuItem12);
    MenuItem14 = new wxMenuItem(ToolIconSizeMenu, ID_MENUITEM_ICON_XLARGE, _("Extra Large\tALT-4"), wxEmptyString, wxITEM_RADIO);
    ToolIconSizeMenu->Append(MenuItem14);
    MenuSettings->Append(ID_MENUITEM4, _("Tool Icon Size"), ToolIconSizeMenu, wxEmptyString);
    MenuItem_SmallWaveform = new wxMenuItem(MenuSettings, ID_MNU_SMALLWAVEFORM, _("Small Waveform"), wxEmptyString, wxITEM_CHECK);
    MenuSettings->Append(MenuItem_SmallWaveform);
    GridSpacingMenu = new wxMenu();
    MenuItem16 = new wxMenuItem(GridSpacingMenu, ID_MENUITEM_GRID_ICON_XSMALL, _("Extra Small"), wxEmptyString, wxITEM_RADIO);
    GridSpacingMenu->Append(MenuItem16);
    MenuItem17 = new wxMenuItem(GridSpacingMenu, ID_MENUITEM_GRID_ICON_SMALL, _("Small"), wxEmptyString, wxITEM_RADIO);
    GridSpacingMenu->Append(MenuItem17);
    MenuItem19 = new wxMenuItem(GridSpacingMenu, ID_MENUITEM_GRID_ICON_MEDIUM, _("Medium"), wxEmptyString, wxITEM_RADIO);
    GridSpacingMenu->Append(MenuItem19);
    MenuItem27 = new wxMenuItem(GridSpacingMenu, ID_MENUITEM_GRID_ICON_LARGE, _("Large"), wxEmptyString, wxITEM_RADIO);
    GridSpacingMenu->Append(MenuItem27);
    MenuItem28 = new wxMenuItem(GridSpacingMenu, ID_MENUITEM_GRID_ICON_XLARGE, _("Extra Large"), wxEmptyString, wxITEM_RADIO);
    GridSpacingMenu->Append(MenuItem28);
    MenuSettings->Append(ID_MENUITEM6, _("Grid Spacing"), GridSpacingMenu, wxEmptyString);
    MenuItem_Grid_Icon_Backgrounds = new wxMenu();
    MenuItemGridIconBackgroundOn = new wxMenuItem(MenuItem_Grid_Icon_Backgrounds, ID_MENUITEM_GRID_ICON_BACKGROUND_ON, _("On"), wxEmptyString, wxITEM_CHECK);
    MenuItem_Grid_Icon_Backgrounds->Append(MenuItemGridIconBackgroundOn);
    MenuItemGridIconBackgroundOn->Check(true);
    MenuItemGridIconBackgroundOff = new wxMenuItem(MenuItem_Grid_Icon_Backgrounds, ID_MENUITEM_GRID_ICON_BACKGROUND_OFF, _("Off"), wxEmptyString, wxITEM_CHECK);
    MenuItem_Grid_Icon_Backgrounds->Append(MenuItemGridIconBackgroundOff);
    MenuSettings->Append(ID_MENUITEM_Grid_Icon_Backgrounds, _("Grid Icon Backgrounds"), MenuItem_Grid_Icon_Backgrounds, wxEmptyString);
    MenuItem1 = new wxMenu();
    MenuItemGridNodeValuesOn = new wxMenuItem(MenuItem1, ID_MENUITEM_GRID_NODE_VALUES_ON, _("On"), wxEmptyString, wxITEM_CHECK);
    MenuItem1->Append(MenuItemGridNodeValuesOn);
    MenuItemGridNodeValuesOn->Check(true);
    MenuItemGridNodeValuesOff = new wxMenuItem(MenuItem1, ID_MENUITEM_GRID_NODE_VALUES_OFF, _("Off"), wxEmptyString, wxITEM_CHECK);
    MenuItem1->Append(MenuItemGridNodeValuesOff);
    MenuSettings->Append(ID_MENUITEM8, _("Grid Node Values"), MenuItem1, wxEmptyString);
    MenuItemColorManager = new wxMenuItem(MenuSettings, ID_COLOR_MANAGER, _("Color Manager"), wxEmptyString, wxITEM_NORMAL);
    MenuSettings->Append(MenuItemColorManager);
    MenuItemRenderMode = new wxMenu();
    MenuItemRenderEraseMode = new wxMenuItem(MenuItemRenderMode, ID_MENU_CANVAS_ERASE_MODE, _("Erase Mode"), wxEmptyString, wxITEM_CHECK);
    MenuItemRenderMode->Append(MenuItemRenderEraseMode);
    MenuItemRenderEraseMode->Check(true);
    MenuItemRenderCanvasMode = new wxMenuItem(MenuItemRenderMode, ID_MENU_CANVAS_CANVAS_MODE, _("Canvas Mode"), wxEmptyString, wxITEM_CHECK);
    MenuItemRenderMode->Append(MenuItemRenderCanvasMode);
    MenuSettings->Append(ID_MENUITEM_RENDER_MODE, _("Render Mode"), MenuItemRenderMode, wxEmptyString)->Enable(false);
    MenuItem7 = new wxMenu();
    MenuItemEffectAssistAlwaysOn = new wxMenuItem(MenuItem7, ID_MENUITEM_EFFECT_ASSIST_ALWAYS_ON, _("Always On"), _("Effect Assist Window will always be active if member of perspective."), wxITEM_CHECK);
    MenuItem7->Append(MenuItemEffectAssistAlwaysOn);
    MenuItemEffectAssistAlwaysOff = new wxMenuItem(MenuItem7, ID_MENUITEM_EFFECT_ASSIST_ALWAYS_OFF, _("Always Off"), _("Effect Assist Window will always be inactive."), wxITEM_CHECK);
    MenuItem7->Append(MenuItemEffectAssistAlwaysOff);
    MenuItemEffectAssistToggleMode = new wxMenuItem(MenuItem7, ID_MENUITEM_EFFECT_ASSIST_TOGGLE, _("Toggle Mode"), _("Effect Assist Window will show only when a supported effect is selected."), wxITEM_CHECK);
    MenuItem7->Append(MenuItemEffectAssistToggleMode);
    MenuItemEffectAssistToggleMode->Check(true);
    MenuSettings->Append(ID_MENUITEM_EFFECT_ASSIST, _("Effect Assist Window"), MenuItem7, wxEmptyString);
    MenuItem15 = new wxMenu();
    MenuItemTimingEditMode = new wxMenuItem(MenuItem15, ID_MENU_TIMING_EDIT_MODE, _("Edit Text"), wxEmptyString, wxITEM_CHECK);
    MenuItem15->Append(MenuItemTimingEditMode);
    MenuItemTimingEditMode->Check(true);
    MenuItemTimingPlayMode = new wxMenuItem(MenuItem15, ID_MENU_TIMING_PLAY_MODE, _("Play Timing"), wxEmptyString, wxITEM_CHECK);
    MenuItem15->Append(MenuItemTimingPlayMode);
    MenuSettings->Append(ID_MENUITEM_Timing_DClick_Mode, _("Timing DClick Mode"), MenuItem15, wxEmptyString);
    OpenGLMenu = new wxMenu();
    MenuItem40 = new wxMenuItem(OpenGLMenu, ID_MENU_OPENGL_AUTO, _("Auto Detect"), wxEmptyString, wxITEM_RADIO);
    OpenGLMenu->Append(MenuItem40);
    MenuItem41 = new wxMenuItem(OpenGLMenu, ID_MENU_OPENGL_3, _("3.x"), wxEmptyString, wxITEM_RADIO);
    OpenGLMenu->Append(MenuItem41);
    MenuItem42 = new wxMenuItem(OpenGLMenu, ID_MENU_OPENGL_2, _("2.x"), wxEmptyString, wxITEM_RADIO);
    OpenGLMenu->Append(MenuItem42);
    MenuItem43 = new wxMenuItem(OpenGLMenu, ID_MENU_OPENGL_1, _("1.x"), wxEmptyString, wxITEM_RADIO);
    OpenGLMenu->Append(MenuItem43);
    MenuSettings->Append(ID_MENUITEM19, _("OpenGL"), OpenGLMenu, wxEmptyString);
    MenuItem_PlayControlsOnPreview = new wxMenuItem(MenuSettings, ID_MNU_PLAYCONTROLSONPREVIEW, _("Play Controls On Preview"), wxEmptyString, wxITEM_CHECK);
    MenuSettings->Append(MenuItem_PlayControlsOnPreview);
    MenuItem_AutoShowHousePreview = new wxMenuItem(MenuSettings, ID_MNU_AUTOSHOWHOUSEPREVIEW, _("Auto Show House Preview"), wxEmptyString, wxITEM_CHECK);
    MenuSettings->Append(MenuItem_AutoShowHousePreview);
    AutoSaveMenu = new wxMenu();
    MenuItem44 = new wxMenuItem(AutoSaveMenu, ID_MENUITEM_AUTOSAVE_0, _("Disabled"), wxEmptyString, wxITEM_RADIO);
    AutoSaveMenu->Append(MenuItem44);
    MenuItem45 = new wxMenuItem(AutoSaveMenu, ID_MENUITEM_AUTOSAVE_3, _("3 Minutes"), wxEmptyString, wxITEM_RADIO);
    AutoSaveMenu->Append(MenuItem45);
    MenuItem46 = new wxMenuItem(AutoSaveMenu, ID_MENUITEM_AUTOSAVE_10, _("10 Minutes"), wxEmptyString, wxITEM_RADIO);
    AutoSaveMenu->Append(MenuItem46);
    MenuItem47 = new wxMenuItem(AutoSaveMenu, ID_MENUITEM_AUTOSAVE_15, _("15 Minutes"), wxEmptyString, wxITEM_RADIO);
    AutoSaveMenu->Append(MenuItem47);
    MenuItem48 = new wxMenuItem(AutoSaveMenu, ID_MENUITEM_AUTOSAVE_30, _("30 Minutes"), wxEmptyString, wxITEM_RADIO);
    AutoSaveMenu->Append(MenuItem48);
    MenuSettings->Append(ID_MENUITEM20, _("Auto Save"), AutoSaveMenu, wxEmptyString);
    MenuItem29 = new wxMenu();
    MenuItem_SD_None = new wxMenuItem(MenuItem29, ID_MNU_SD_None, _("None"), wxEmptyString, wxITEM_RADIO);
    MenuItem29->Append(MenuItem_SD_None);
    MenuItem_SD_10 = new wxMenuItem(MenuItem29, ID_MNU_SD_10, _("10"), wxEmptyString, wxITEM_RADIO);
    MenuItem29->Append(MenuItem_SD_10);
    MenuItem_SD_20 = new wxMenuItem(MenuItem29, ID_MNU_SD_20, _("20"), wxEmptyString, wxITEM_RADIO);
    MenuItem29->Append(MenuItem_SD_20);
    MenuItem_SD_40 = new wxMenuItem(MenuItem29, ID_MNU_SD_40, _("40"), wxEmptyString, wxITEM_RADIO);
    MenuItem29->Append(MenuItem_SD_40);
    MenuSettings->Append(ID_MNU_SUPPRESSDUPLICATES, _("Duplicate Frames To Suppress"), MenuItem29, wxEmptyString);
    MenuItem_e131sync = new wxMenuItem(MenuSettings, ID_E131_Sync, _("Frame Sync"), _("Only enable this if your controllers support e1.31 sync. You will also need to set the synchronisation universe on the setup tab."), wxITEM_CHECK);
    MenuSettings->Append(MenuItem_e131sync);
    MenuItem_ForceLocalIP = new wxMenuItem(MenuSettings, ID_MNU_FORCEIP, _("&Force Local IP"), wxEmptyString, wxITEM_CHECK);
    MenuSettings->Append(MenuItem_ForceLocalIP);
    MenuItem_ModelBlendDefaultOff = new wxMenuItem(MenuSettings, ID_MNU_DEFAULTMODELBLENDOFF, _("Model Blend Default Off"), wxEmptyString, wxITEM_CHECK);
    MenuSettings->Append(MenuItem_ModelBlendDefaultOff);
    MenuItem_SnapToTimingMarks = new wxMenuItem(MenuSettings, ID_MNU_SNAP_TO_TIMING, _("Snap to Timing Marks"), wxEmptyString, wxITEM_CHECK);
    MenuSettings->Append(MenuItem_SnapToTimingMarks);
    MenuBar->Append(MenuSettings, _("&Settings"));
    MenuHelp = new wxMenu();
    MenuItem4 = new wxMenuItem(MenuHelp, idMenuHelpContent, _("Content\tF1"), wxEmptyString, wxITEM_NORMAL);
    MenuHelp->Append(MenuItem4);
    MenuItem_Help_Forum = new wxMenuItem(MenuHelp, ID_MENU_HELP_FORMUM, _("Forum"), wxEmptyString, wxITEM_NORMAL);
    MenuHelp->Append(MenuItem_Help_Forum);
    MenuItem_VideoTutorials = new wxMenuItem(MenuHelp, ID_MNU_VIDEOS, _("Video Tutorials"), wxEmptyString, wxITEM_NORMAL);
    MenuHelp->Append(MenuItem_VideoTutorials);
    MenuItem_Help_Download = new wxMenuItem(MenuHelp, ID_MENU_HELP_DOWNLOAD, _("Download"), wxEmptyString, wxITEM_NORMAL);
    MenuHelp->Append(MenuItem_Help_Download);
    MenuItem_Help_ReleaseNotes = new wxMenuItem(MenuHelp, ID_MNU_HELP_RELEASE_NOTES, _("Release Notes"), wxEmptyString, wxITEM_NORMAL);
    MenuHelp->Append(MenuItem_Help_ReleaseNotes);
    MenuItem_Help_Isue_Tracker = new wxMenuItem(MenuHelp, ID_MENU_HELP_ISSUE, _("Issue Tracker"), wxEmptyString, wxITEM_NORMAL);
    MenuHelp->Append(MenuItem_Help_Isue_Tracker);
    MenuItem_Help_Facebook = new wxMenuItem(MenuHelp, ID_MENU_HELP_FACEBOOK, _("Facebook"), wxEmptyString, wxITEM_NORMAL);
    MenuHelp->Append(MenuItem_Help_Facebook);
    MenuItem_Donate = new wxMenuItem(MenuHelp, ID_MNU_DONATE, _("Donate"), _("Donate to the xLights project."), wxITEM_NORMAL);
    MenuHelp->Append(MenuItem_Donate);
    MenuItem_Update = new wxMenuItem(MenuHelp, ID_MNU_UPDATE, _("Check for Updates"), _("Check for newer xLights updates"), wxITEM_NORMAL);
    MenuHelp->Append(MenuItem_Update);
    MenuItem_Update->Enable(false);
    MenuItem2 = new wxMenuItem(MenuHelp, wxID_ABOUT, _("About"), _("Show info about this application"), wxITEM_NORMAL);
    MenuHelp->Append(MenuItem2);
    MenuBar->Append(MenuHelp, _("&Help"));
    SetMenuBar(MenuBar);
    Timer1.SetOwner(this, ID_TIMER1);
    Timer_AutoSave.SetOwner(this, ID_TIMER2);
    EffectSettingsTimer.SetOwner(this, ID_TIMER_EFFECT_SETTINGS);

    Connect(ID_AUITOOLBAR_OPENSHOW,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnMenuOpenFolderSelected);
    Connect(ID_AUITOOLBAR_NEWSEQUENCE,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnButtonNewSequenceClick);
    Connect(ID_AUITOOLBAR_OPEN,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnBitmapButtonOpenSeqClick);
    Connect(ID_AUITOOLBAR_SAVE,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnBitmapButtonSaveSeqClick);
    Connect(ID_AUITOOLBAR_SAVEAS,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnButtonClickSaveAs);
    Connect(ID_AUITOOLBAR_RENDERALL,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnAuiToolBarItemRenderAllClick);
    Connect(ID_AUITOOLBAR_PLAY_NOW,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnAuiToolBarItemPlayButtonClick);
    Connect(ID_AUITOOLBAR_PAUSE,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnAuiToolBarItemPauseButtonClick);
    Connect(ID_AUITOOLBAR_STOP,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnAuiToolBarItemStopClick);
    Connect(ID_AUITOOLBAR_FIRST_FRAME,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnAuiToolBarFirstFrameClick);
    Connect(ID_AUITOOLBAR_LAST_FRAME,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnAuiToolBarLastFrameClick);
    Connect(ID_AUITOOLBAR_REPLAY_SECTION,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnAuiToolBarItemReplaySectionClick);
    Connect(ID_AUITOOLBARITEM2,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::ShowHideEffectSettingsWindow);
    Connect(ID_AUITOOLBARITEM5,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::ShowHideColorWindow);
    Connect(ID_AUITOOLBARITEM7,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::ShowHideBufferSettingsWindow);
    Connect(ID_AUITOOLBARITEM3,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::ShowHideLayerTimingWindow);
    Connect(ID_AUITOOLBARITEM1,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::ShowHideModelPreview);
    Connect(ID_AUITOOLBARITEM4,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::ShowHideHousePreview);
    Connect(ID_AUITOOLBARITEM6,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::ShowHideDisplayElementsWindow);
    Connect(ID_AUITOOLBARITEM8,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnAuiToolBarItemShowHideEffects);
    Connect(ID_PASTE_BY_TIME,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnAuiToolBarItemPasteByTimeClick);
    Connect(ID_PASTE_BY_CELL,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnAuiToolBarItemPasteByCellClick);
    Connect(ID_AUITOOLBARITEM_ACDISABLED,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnAC_DisableClick);
    Connect(ID_AUITOOLBARITEM_ACSELECT,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnAC_SelectClick);
    Connect(ID_AUITOOLBARITEM_ACOFF,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnAC_OffClick);
    Connect(ID_AUITOOLBARITEM_ACON,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnAC_OnClick);
    Connect(ID_AUITOOLBARITEM_ACSHIMMER,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnAC_ShimmerClick);
    Connect(ID_AUITOOLBARITEM_ACTWINKLE,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnAC_TwinkleClick);
    Connect(ID_AUITOOLBARITEM_ACINTENSITY,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnAC_IntensityClick);
    Connect(ID_AUITOOLBARITEM_ACRAMPUP,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnAC_RampUpClick);
    Connect(ID_AUITOOLBARITEM_ACRAMPDOWN,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnAC_RampDownClick);
    Connect(ID_AUITOOLBARITEM_ACRAMPUPDOWN,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnAC_RampUpDownClick);
    Connect(ID_CHOICE_PARM1,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnChoiceParm1Select);
    Connect(ID_CHOICE_PARM2,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnChoiceParm2Select);
    Connect(ID_AUITOOLBARITEM_ACFILL,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnAC_FillClick);
    Connect(ID_AUITOOLBARITEM_ACCASCADE,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnAC_CascadeClick);
    Connect(ID_AUITOOLBARITEM_ACFOREGROUND,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnAC_ForegroundClick);
    Connect(ID_AUITOOLBARITEM_ACBACKGROUND,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnAC_BackgroundClick);
    Connect(ID_AUITOOLBAR_AC,wxEVT_COMMAND_AUITOOLBAR_TOOL_DROPDOWN,(wxObjectEventFunction)&xLightsFrame::OnACToolbarDropdown);
    Connect(wxID_ZOOM_IN,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnAuiToolBarItemZoominClick);
    Connect(wxID_ZOOM_OUT,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnAuiToolBarItem_ZoomOutClick);
    Connect(ID_AUITOOLBARITEM14,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnMenu_Settings_SequenceSelected);
    Connect(ID_BITMAPBUTTON_TAB_INFO,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnBitmapButtonTabInfoClick);
    Connect(ID_BUTTON_STOP_NOW,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnButtonStopNowClick);
    Connect(ID_BUTTON_LIGHTS_OFF,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnButtonLightsOffClick);
    Connect(ID_CHECKBOX_LIGHT_OUTPUT,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnCheckBoxLightOutputClick);
    Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnMenuOpenFolderSelected);
    Connect(ID_BUTTON_CHANGE_MEDIA_DIR,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&xLightsFrame::ChangeMediaDirectory);
    Connect(ID_BITMAPBUTTON_Link_Dirs,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnBitmapButton_Link_DirsClick);
    Connect(ID_BUTTON_SAVE_SETUP,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnButtonSaveSetupClick);
    Connect(ID_BUTTON_ADD_DONGLE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnButtonAddDongleClick);
    Connect(ID_BUTTON_ADD_E131,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnButtonAddE131Click);
    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnButtonAddNullClick);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnButtonArtNETClick);
    Connect(ID_BUTTON_ADD_LOR,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnButtonAddLORClick);
    Connect(ID_BUTTON_ADD_DDP,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnButtonAddDDPClick);
    Connect(ID_BUTTON_NETWORK_CHANGE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnButtonNetworkChangeClick);
    Connect(ID_BUTTON_NETWORK_DELETE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnButtonNetworkDeleteClick);
    Connect(ID_BUTTON_NETWORK_DELETE_ALL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnButtonNetworkDeleteAllClick);
    Connect(ID_SPINCTRL1,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&xLightsFrame::OnSpinCtrl_SyncUniverseChange);
    Connect(ID_BITMAPBUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnButtonNetworkMoveUpClick);
    Connect(ID_BITMAPBUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&xLightsFrame::OnButtonNetworkMoveDownClick);
    Connect(ID_LISTCTRL_NETWORKS,wxEVT_COMMAND_LIST_BEGIN_DRAG,(wxObjectEventFunction)&xLightsFrame::OnGridNetworkBeginDrag);
    Connect(ID_LISTCTRL_NETWORKS,wxEVT_COMMAND_LIST_ITEM_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnGridNetworkItemSelect);
    Connect(ID_LISTCTRL_NETWORKS,wxEVT_COMMAND_LIST_ITEM_DESELECTED,(wxObjectEventFunction)&xLightsFrame::OnGridNetworkItemDeselect);
    Connect(ID_LISTCTRL_NETWORKS,wxEVT_COMMAND_LIST_ITEM_ACTIVATED,(wxObjectEventFunction)&xLightsFrame::OnGridNetworkItemActivated);
    Connect(ID_LISTCTRL_NETWORKS,wxEVT_COMMAND_LIST_ITEM_FOCUSED,(wxObjectEventFunction)&xLightsFrame::OnGridNetworkItemFocused);
    Connect(ID_LISTCTRL_NETWORKS,wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK,(wxObjectEventFunction)&xLightsFrame::OnGridNetworkItemRClick);
    Connect(ID_LISTCTRL_NETWORKS,wxEVT_COMMAND_LIST_KEY_DOWN,(wxObjectEventFunction)&xLightsFrame::OnGridNetworkKeyDown);
    m_mgr->Connect(wxEVT_AUI_PANE_CLOSE,(wxObjectEventFunction)&xLightsFrame::OnPaneClose,0,this);
    PanelSequencer->Connect(wxEVT_PAINT,(wxObjectEventFunction)&xLightsFrame::OnPanelSequencerPaint,0,this);
    Connect(ID_NOTEBOOK1,wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGED,(wxObjectEventFunction)&xLightsFrame::OnNotebook1PageChanged1);
    Connect(ID_NOTEBOOK1,wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGING,(wxObjectEventFunction)&xLightsFrame::OnNotebook1PageChanging);
    Connect(ID_NEW_SEQUENCE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnButtonNewSequenceClick);
    Connect(ID_OPEN_SEQUENCE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_File_Open_SequenceSelected);
    Connect(IS_SAVE_SEQ,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_File_Save_Selected);
    Connect(ID_SAVE_AS_SEQUENCE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_File_SaveAs_SequenceSelected);
    Connect(ID_CLOSE_SEQ,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_File_Close_SequenceSelected);
    Connect(ID_EXPORT_VIDEO,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_File_Export_VideoSelected);
    Connect(ID_MENUITEM2,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuOpenFolderSelected);
    Connect(ID_FILE_BACKUP,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItemBackupSelected);
    Connect(ID_FILE_ALTBACKUP,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnmAltBackupMenuItemSelected);
    Connect(wxID_EXIT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnQuit);
    Connect(ID_SHIFT_EFFECTS,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItemShiftEffectsSelected);
    Connect(ID_MENUITEM13,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnActionTestMenuItemSelected);
    Connect(ID_MENUITEM_CONVERT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItemConvertSelected);
    Connect(ID_MENUITEM_GenerateCustomModel,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenu_GenerateCustomModelSelected);
    Connect(ID_MNU_GENERATELYRICS,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_GenerateLyricsSelected);
    Connect(ID_MNU_CHECKSEQ,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItemCheckSequenceSelected);
    Connect(ID_MENU_VIEW_LOG,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_ViewLogSelected);
    Connect(ID_MENUITEM18,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItemPackageDebugFiles);
    Connect(ID_EXPORT_MODELS,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnmExportModelsMenuItemSelected);
    Connect(ID_MNU_EXPORT_EFFECTS,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_ExportEffectsSelected);
    Connect(ID_MENU_FPP_CONNECT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_FPP_ConnectSelected);
    Connect(ID_MNU_PACKAGESEQUENCE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_PackageSequenceSelected);
    Connect(ID_MENU_BATCH_RENDER,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItemBatchRenderSelected);
    Connect(ID_MNU_XSCHEDULE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_xScheduleSelected);
    Connect(iD_MNU_VENDORCACHEPURGE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_PurgeVendorCacheSelected);
    Connect(ID_MNU_CRASH,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_CrashXLightsSelected);
    Connect(ID_MNU_DUMPRENDERSTATE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_LogRenderStateSelected);
    Connect(wxID_ZOOM_IN,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnAuiToolBarItemZoominClick);
    Connect(wxID_ZOOM_OUT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnAuiToolBarItem_ZoomOutClick);
    Connect(ID_MENUITEM5,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::ResetToolbarLocations);
    Connect(MNU_ID_ACLIGHTS,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_ACLIghtsSelected);
    Connect(ID_MNU_SHOWRAMPS,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_ShowACRampsSelected);
    Connect(ID_MENUITEM_SAVE_PERSPECTIVE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItemViewSavePerspectiveSelected);
    Connect(ID_MENUITEM_SAVE_AS_PERSPECTIVE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItemViewSaveAsPerspectiveSelected);
    Connect(ID_MENUITEM_LOAD_PERSPECTIVE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItemLoadEditPerspectiveSelected);
    Connect(ID_MNU_PERSPECTIVES_AUTOSAVE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_PerspectiveAutosaveSelected);
    Connect(ID_MENUITEM_DISPLAY_ELEMENTS,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::ShowHideDisplayElementsWindow);
    Connect(ID_MENUITEM12,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::ShowHideModelPreview);
    Connect(ID_MENUITEM3,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::ShowHideHousePreview);
    Connect(ID_MENUITEM14,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::ShowHideEffectSettingsWindow);
    Connect(ID_MENUITEM15,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::ShowHideColorWindow);
    Connect(ID_MENUITEM16,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::ShowHideLayerTimingWindow);
    Connect(ID_MENUITEM9,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::ShowHideBufferSettingsWindow);
    Connect(ID_MENUITEM17,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::ShowHideEffectDropper);
    Connect(ID_MENUITEM_EFFECT_ASSIST_WINDOW,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::ShowHideEffectAssistWindow);
    Connect(ID_MENUITEM_SELECT_EFFECT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItemSelectEffectSelected);
    Connect(ID_MENUITEM_WINDOWS_PERSPECTIVE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::ShowHidePerspectivesWindow);
    Connect(ID_MENUITEM_WINDOWS_DOCKALL,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuDockAllSelected);
    Connect(ID_MENUITEM11,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::ResetWindowsToDefaultPositions);
    Connect(ID_PLAY_FULL,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::SetPlaySpeed);
    Connect(ID_MNU_1POINT5SPEED,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::SetPlaySpeed);
    Connect(ID_MN_2SPEED,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::SetPlaySpeed);
    Connect(ID_MNU_3SPEED,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::SetPlaySpeed);
    Connect(ID_MNU_4SPEED,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::SetPlaySpeed);
    Connect(ID_PLAY_3_4,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::SetPlaySpeed);
    Connect(ID_PLAY_1_2,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::SetPlaySpeed);
    Connect(ID_PLAY_1_4,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::SetPlaySpeed);
    Connect(ID_MNU_LOUDVOLUME,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_LoudVolSelected);
    Connect(ID_MNU_MEDVOLUME,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_MedVolSelected);
    Connect(ID_MNU_QUIET,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_QuietVolSelected);
    Connect(ID_MNU_SUPERQUIET,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_VQuietVolSelected);
    Connect(ID_IMPORT_EFFECTS,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItemImportEffects);
    Connect(ID_SEQ_SETTINGS,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenu_Settings_SequenceSelected);
    Connect(ID_RENDER_ON_SAVE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItemRenderOnSave);
    Connect(ID_BACKUP_ON_SAVE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnmBackupOnSaveSelected);
    Connect(ID_MENU_BACKUP_ON_LAUNCH,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_BackupOnLaunchSelected);
    Connect(ID_ALT_BACKUPLOCATION,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnmAltBackupLocationMenuItemSelected);
    Connect(ID_MNU_BACKUP,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_BackupSubfoldersSelected);
    Connect(ID_MNU_EXCLUDEPRESETS,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_ExcludePresetsFromPackagedSequencesSelected);
    Connect(ID_MNU_EXCLUDEAUDIOPKGSEQ,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_ExcludeAudioPackagedSequenceSelected);
    Connect(ID_MENUITEM_ICON_SMALL,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::SetToolIconSize);
    Connect(ID_MENUITEM_ICON_MEDIUM,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::SetToolIconSize);
    Connect(ID_MENUITEM_ICON_LARGE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::SetToolIconSize);
    Connect(ID_MENUITEM_ICON_XLARGE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::SetToolIconSize);
    Connect(ID_MNU_SMALLWAVEFORM,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_SmallWaveformSelected);
    Connect(ID_MENUITEM_GRID_ICON_XSMALL,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::SetIconSize);
    Connect(ID_MENUITEM_GRID_ICON_SMALL,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::SetIconSize);
    Connect(ID_MENUITEM_GRID_ICON_MEDIUM,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::SetIconSize);
    Connect(ID_MENUITEM_GRID_ICON_LARGE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::SetIconSize);
    Connect(ID_MENUITEM_GRID_ICON_XLARGE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::SetIconSize);
    Connect(ID_MENUITEM_GRID_ICON_BACKGROUND_ON,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnSetGridIconBackground);
    Connect(ID_MENUITEM_GRID_ICON_BACKGROUND_OFF,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnSetGridIconBackground);
    Connect(ID_MENUITEM_GRID_NODE_VALUES_ON,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnSetGridNodeValues);
    Connect(ID_MENUITEM_GRID_NODE_VALUES_OFF,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnSetGridNodeValues);
    Connect(ID_COLOR_MANAGER,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItemColorManagerSelected);
    Connect(ID_MENU_CANVAS_ERASE_MODE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItemRenderEraseModeSelected);
    Connect(ID_MENU_CANVAS_CANVAS_MODE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItemRenderCanvasModeSelected);
    Connect(ID_MENUITEM_EFFECT_ASSIST_ALWAYS_ON,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItemEffectAssistAlwaysOnSelected);
    Connect(ID_MENUITEM_EFFECT_ASSIST_ALWAYS_OFF,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItemEffectAssistAlwaysOffSelected);
    Connect(ID_MENUITEM_EFFECT_ASSIST_TOGGLE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItemEffectAssistToggleModeSelected);
    Connect(ID_MENU_TIMING_EDIT_MODE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItemTimingPlayOnDClick);
    Connect(ID_MENU_TIMING_PLAY_MODE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItemTimingPlayOnDClick);
    Connect(ID_MENU_OPENGL_AUTO,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuOpenGLSelected);
    Connect(ID_MENU_OPENGL_3,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuOpenGLSelected);
    Connect(ID_MENU_OPENGL_2,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuOpenGLSelected);
    Connect(ID_MENU_OPENGL_1,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuOpenGLSelected);
    Connect(ID_MNU_PLAYCONTROLSONPREVIEW,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_PlayControlsOnPreviewSelected);
    Connect(ID_MNU_AUTOSHOWHOUSEPREVIEW,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_AutoShowHousePreviewSelected);
    Connect(ID_MENUITEM_AUTOSAVE_0,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::AutoSaveIntervalSelected);
    Connect(ID_MENUITEM_AUTOSAVE_3,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::AutoSaveIntervalSelected);
    Connect(ID_MENUITEM_AUTOSAVE_10,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::AutoSaveIntervalSelected);
    Connect(ID_MENUITEM_AUTOSAVE_15,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::AutoSaveIntervalSelected);
    Connect(ID_MENUITEM_AUTOSAVE_30,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::AutoSaveIntervalSelected);
    Connect(ID_MNU_SD_None,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_SD_NoneSelected);
    Connect(ID_MNU_SD_10,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_SD_10Selected);
    Connect(ID_MNU_SD_20,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_SD_20Selected);
    Connect(ID_MNU_SD_40,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_SD_40Selected);
    Connect(ID_E131_Sync,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_e131syncSelected);
    Connect(ID_MNU_FORCEIP,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_ForceLocalIPSelected);
    Connect(ID_MNU_DEFAULTMODELBLENDOFF,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_ModelBlendDefaultOffSelected);
    Connect(ID_MNU_SNAP_TO_TIMING,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_SnapToTimingMarksSelected);
    Connect(idMenuHelpContent,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnBitmapButtonTabInfoClick);
    Connect(ID_MENU_HELP_FORMUM,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_Help_ForumSelected);
    Connect(ID_MNU_VIDEOS,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_VideoTutorialsSelected);
    Connect(ID_MENU_HELP_DOWNLOAD,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_Help_DownloadSelected);
    Connect(ID_MNU_HELP_RELEASE_NOTES,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_Help_ReleaseNotesSelected);
    Connect(ID_MENU_HELP_ISSUE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_Help_Isue_TrackerSelected);
    Connect(ID_MENU_HELP_FACEBOOK,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_Help_FacebookSelected);
    Connect(ID_MNU_DONATE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_DonateSelected);
    Connect(ID_MNU_UPDATE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnMenuItem_UpdateSelected);
    Connect(wxID_ABOUT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::OnAbout);
    Connect(ID_TIMER1,wxEVT_TIMER,(wxObjectEventFunction)&xLightsFrame::OnTimer1Trigger);
    Connect(ID_TIMER2,wxEVT_TIMER,(wxObjectEventFunction)&xLightsFrame::OnTimer_AutoSaveTrigger);
    Connect(ID_TIMER_EFFECT_SETTINGS,wxEVT_TIMER,(wxObjectEventFunction)&xLightsFrame::OnEffectSettingsTimerTrigger);
    Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&xLightsFrame::OnClose);
    Connect(wxEVT_SIZE,(wxObjectEventFunction)&xLightsFrame::OnResize);
    //*)

    _suppressDuplicateFrames = 0;

    AddWindowsMenu();

    // This is for keith ... I like my debug version to be distinctive so I can tell it apart from the prior version
    #ifndef NDEBUG
        logger_base.debug("xLights Crash Menu item not removed.");
        #ifdef _MSC_VER
            Notebook1->SetBackgroundColour(*wxGREEN);
        #endif
    #else
        // only keep the crash option if the EnableCrash.txt file exists
        if (!wxFile::Exists("EnableCrash.txt"))
        {
            MenuItem_CrashXLights->GetMenu()->Remove(MenuItem_CrashXLights);
            MenuItem_CrashXLights = nullptr;
            MenuItem_LogRenderState->GetMenu()->Remove(MenuItem_LogRenderState);
            MenuItem_LogRenderState = nullptr;
        }
        else
        {
            logger_base.debug("xLights Crash Menu item not removed.");
        }
    #endif

    logger_base.debug("xLightsFrame constructor UI code done.");

    //need to direct these menu items to different places depending on what is active
    Connect(wxID_UNDO, wxEVT_MENU,(wxObjectEventFunction)&xLightsFrame::DoMenuAction);
    Connect(wxID_REDO, wxEVT_MENU,(wxObjectEventFunction)&xLightsFrame::DoMenuAction);
    Connect(wxID_CUT, wxEVT_MENU,(wxObjectEventFunction)&xLightsFrame::DoMenuAction);
    Connect(wxID_COPY, wxEVT_MENU,(wxObjectEventFunction)&xLightsFrame::DoMenuAction);
    Connect(wxID_PASTE, wxEVT_MENU,(wxObjectEventFunction)&xLightsFrame::DoMenuAction);

	mRendering = false;

    AddEffectToolbarButtons(effectManager, EffectsToolBar);
    wxSize sz = EffectsToolBar->GetSize();
    wxAuiPaneInfo &info = MainAuiManager->GetPane("EffectsToolBar");
    info.BestSize(sz);
    MainAuiManager->Update();

    SetTitle( xlights_base_name + xlights_qualifier + " (Ver " + xlights_version_string + " " + GetBitness() + ") " + xlights_build_date );

    CheckBoxLightOutput = new AUIToolbarButtonWrapper(OutputToolBar, ID_CHECKBOX_LIGHT_OUTPUT);
    ButtonPasteByTime = new AUIToolbarButtonWrapper(EditToolBar, ID_PASTE_BY_TIME);
    ButtonPasteByCell = new AUIToolbarButtonWrapper(EditToolBar, ID_PASTE_BY_CELL);

    Button_ACDisabled = new AUIToolbarButtonWrapper(ACToolbar, ID_AUITOOLBARITEM_ACDISABLED);
    Button_ACDisabled->SetValue(false);
    Button_ACSelect = new AUIToolbarButtonWrapper(ACToolbar, ID_AUITOOLBARITEM_ACSELECT);
    Button_ACSelect->SetValue(true);
    Button_ACOn = new AUIToolbarButtonWrapper(ACToolbar, ID_AUITOOLBARITEM_ACON);
    Button_ACOn->SetValue(false);
    Button_ACOff = new AUIToolbarButtonWrapper(ACToolbar, ID_AUITOOLBARITEM_ACOFF);
    Button_ACOff->SetValue(false);
    Button_ACTwinkle = new AUIToolbarButtonWrapper(ACToolbar, ID_AUITOOLBARITEM_ACTWINKLE);
    Button_ACTwinkle->SetValue(false);
    Button_ACShimmer = new AUIToolbarButtonWrapper(ACToolbar, ID_AUITOOLBARITEM_ACSHIMMER);
    Button_ACShimmer->SetValue(false);

    Button_ACIntensity = new AUIToolbarButtonWrapper(ACToolbar, ID_AUITOOLBARITEM_ACINTENSITY);
    Button_ACIntensity->SetValue(true);
    Button_ACRampUp = new AUIToolbarButtonWrapper(ACToolbar, ID_AUITOOLBARITEM_ACRAMPUP);
    Button_ACRampUp->SetValue(false);
    Button_ACRampDown = new AUIToolbarButtonWrapper(ACToolbar, ID_AUITOOLBARITEM_ACRAMPDOWN);
    Button_ACRampDown->SetValue(false);
    Button_ACRampUpDown = new AUIToolbarButtonWrapper(ACToolbar, ID_AUITOOLBARITEM_ACRAMPUPDOWN);
    Button_ACRampUpDown->SetValue(false);

    Button_ACFill = new AUIToolbarButtonWrapper(ACToolbar, ID_AUITOOLBARITEM_ACFILL);
    Button_ACFill->SetValue(false);
    Button_ACCascade = new AUIToolbarButtonWrapper(ACToolbar, ID_AUITOOLBARITEM_ACCASCADE);
    Button_ACCascade->SetValue(false);

    Button_ACForeground = new AUIToolbarButtonWrapper(ACToolbar, ID_AUITOOLBARITEM_ACFOREGROUND);
    Button_ACForeground->SetValue(false);
    Button_ACBackground = new AUIToolbarButtonWrapper(ACToolbar, ID_AUITOOLBARITEM_ACBACKGROUND);
    Button_ACBackground->SetValue(false);

    ButtonPasteByTime->SetValue(true);
    ButtonPasteByCell->SetValue(false);
    mResetToolbars = false;
    mRenderOnSave = true;
    mBackupOnSave = false;
    mBackupOnLaunch = true;
    me131Sync = false;
    mLocalIP = "";
    mAltBackupDir = "";
    mIconSize = 16;

    _acParm1Intensity = 100;
    _acParm1RampUp = 0;
    _acParm2RampUp = 100;
    _acParm1RampDown = 0;
    _acParm2RampDown = 100;
    _acParm1RampUpDown = 0;
    _acParm2RampUpDown = 100;
    ChoiceParm1->SetStringSelection("100");
    ChoiceParm2->SetStringSelection("100");

    StatusBarSizer->AddGrowableCol(0,2);
    StatusBarSizer->AddGrowableCol(2,1);
    ProgressBar->Connect(wxEVT_LEFT_DOWN, (wxObjectEventFunction)&xLightsFrame::OnProgressBarDoubleClick, nullptr, this);
    ProgressBar->Hide();
    selectedEffectPalette = "";
    selectedEffect = nullptr;
    playStartTime = playEndTime = 0;
    replaySection = false;
    playType = 0;
    playModel = nullptr;
	mLoopAudio = false;
    playSpeed = 1.0;
    playVolume = 100;
    playAnimation = false;
    UnsavedNetworkChanges = false;

    UnsavedRgbEffectsChanges = false;
    UnsavedPlaylistChanges = false;
    mStoredLayoutGroup = "Default";
    mDefaultNetworkSaveBtnColor = ButtonSaveSetup->GetBackgroundColour();

    modelsChangeCount = 0;

    wxConfigBase* config = wxConfigBase::Get();
    if (config == nullptr)
    {
        logger_base.error("Null config ... this wont end well.");
    }
    logger_base.debug("Config: AppName '%s' Path '%s' Entries %d Groups %d Style %ld Vendor %s.", (const char *)config->GetAppName().c_str(), (const char *)config->GetPath().c_str(), (int)config->GetNumberOfEntries(), (int)config->GetNumberOfGroups(), config->GetStyle(), (const char*)config->GetVendorName().c_str());

    config->Read("xLightsPlayControlsOnPreview", &_playControlsOnPreview, false);
    MenuItem_PlayControlsOnPreview->Check(_playControlsOnPreview);
    logger_base.debug("Play Controls On Preview: %s.", _playControlsOnPreview ? "true" : "false");

    config->Read("xLightsAutoShowHousePreview", &_autoShowHousePreview, false);
    MenuItem_AutoShowHousePreview->Check(_autoShowHousePreview);
    logger_base.debug("Autoshow House Preview: %s.", _autoShowHousePreview ? "true" : "false");

    config->Read("xLightsSmallWaveform", &_smallWaveform, false);
    MenuItem_SmallWaveform->Check(_smallWaveform);
    logger_base.debug("Small Waveform: %s.", _smallWaveform ? "true" : "false");

    config->Read("xLightsModelBlendDefaultOff", &_modelBlendDefaultOff, false);
    MenuItem_ModelBlendDefaultOff->Check(_modelBlendDefaultOff);
    logger_base.debug("Model Blend Default Off: %s.", _modelBlendDefaultOff ? "true" : "false");

    config->Read("xLightsSnapToTimingMarks", &_snapToTimingMarks, false);
    MenuItem_SnapToTimingMarks->Check(_snapToTimingMarks);
    logger_base.debug("Snap To Timing Marks: %s.", _snapToTimingMarks ? "true" : "false");

    logger_base.debug("xLightsFrame constructor creating sequencer.");

    CreateSequencer();

    logger_base.debug("xLightsFrame constructor sequencer creation done.");

    layoutPanel = new LayoutPanel(PanelPreview, this, PanelSequencer);
    logger_base.debug("LayoutPanel creation done.");
    FlexGridSizerPreview->Add(layoutPanel, 1, wxALL | wxEXPAND, 5);
    FlexGridSizerPreview->Fit(PanelPreview);
    FlexGridSizerPreview->SetSizeHints(PanelPreview);

    modelPreview = layoutPanel->GetMainPreview();
    logger_base.debug("LayoutPanel setup done.");


    playIcon = wxBitmap(control_play_blue_icon);
    pauseIcon = wxBitmap(control_pause_blue_icon);

    Grid1HasFocus = false; //set this before grid gets any events -DJ

    wxIconBundle icons;
    icons.AddIcon(wxIcon(xlights_16_xpm));
    icons.AddIcon(wxIcon(xlights_32_xpm));
    icons.AddIcon(wxIcon(xlights_64_xpm));
    icons.AddIcon(wxIcon(xlights_128_xpm));
    icons.AddIcon(wxIcon(xlights_xpm));

    SetIcons(icons);
    logger_base.debug("IconBundle creation done.");

    SetName("xLights");
    wxPersistenceManager::Get().RegisterAndRestore(this);
    logger_base.debug("Window Location Restored.");

    effGridPrevX = 0;
    effGridPrevY = 0;
    mSavedChangeCount = 0;
    mLastAutosaveCount = 0;

    logger_base.debug("xLightsFrame constructor loading network list.");

    // Load headings into network list
    wxListItem itemCol;

    itemCol.SetText(_T("Number"));
    itemCol.SetImage(-1);
    GridNetwork->InsertColumn(0, itemCol);

    itemCol.SetText(_T("Network Type"));
    itemCol.SetImage(-1);
    GridNetwork->InsertColumn(1, itemCol);

    itemCol.SetText(_T("Port"));
    itemCol.SetAlign(wxLIST_FORMAT_LEFT);
    GridNetwork->InsertColumn(2, itemCol);

    itemCol.SetText(_T("Baud Rate or E1.31 Univ"));
    itemCol.SetAlign(wxLIST_FORMAT_CENTRE);
    GridNetwork->InsertColumn(3, itemCol);

    itemCol.SetText(_T("Num Channels"));
    itemCol.SetAlign(wxLIST_FORMAT_CENTRE);
    GridNetwork->InsertColumn(4, itemCol);

    itemCol.SetText(_T("xLights/Vixen/FPP Mapping"));
    itemCol.SetAlign(wxLIST_FORMAT_LEFT);
    GridNetwork->InsertColumn(5, itemCol);

    itemCol.SetText(_T("Active"));
    itemCol.SetAlign(wxLIST_FORMAT_LEFT);
    GridNetwork->InsertColumn(6, itemCol);

	itemCol.SetText(_T("Description"));
	itemCol.SetAlign(wxLIST_FORMAT_LEFT);
	GridNetwork->InsertColumn(7, itemCol);

    itemCol.SetText(_T("Duplicate Suppress"));
    itemCol.SetAlign(wxLIST_FORMAT_LEFT);
    GridNetwork->InsertColumn(8, itemCol);

    GridNetwork->SetColumnWidth(0, wxLIST_AUTOSIZE_USEHEADER);
    GridNetwork->SetColumnWidth(1, wxLIST_AUTOSIZE_USEHEADER);
    GridNetwork->SetColumnWidth(2, 100);
    GridNetwork->SetColumnWidth(3, wxLIST_AUTOSIZE_USEHEADER);
    GridNetwork->SetColumnWidth(4, 100);
    GridNetwork->SetColumnWidth(5, 170);
    GridNetwork->SetColumnWidth(6, wxLIST_AUTOSIZE_USEHEADER);
	GridNetwork->SetColumnWidth(7, wxLIST_AUTOSIZE);
    GridNetwork->SetColumnWidth(8, wxLIST_AUTOSIZE_USEHEADER);

    _scrollTimer.Connect(wxEVT_TIMER,
        wxTimerEventHandler(xLightsFrame::OnGridNetworkScrollTimer), nullptr, this);

    // get list of most recently used directories
    wxString dir;
    for (int i=0; i<MRU_LENGTH; i++)
    {
        wxString mru_name=wxString::Format("mru%d",i);
        dir.clear();
        if ( config->Read(mru_name, &dir) )
        {
            if (!dir.IsEmpty())
            {
                int idx=mru.Index(dir);
                if (idx == wxNOT_FOUND) mru.Add(dir);
            }
        }
        mru_MenuItem[i] = nullptr;
    }

    logger_base.debug("xLightsFrame constructor loading config.");

    dir.clear();
    bool ok = true;
    if (!xLightsApp::showDir.IsNull())
    {
        dir = xLightsApp::showDir;
    }
    else
    {
        ok = config->Read("LastDir", &dir);
        //wxString ConvertDir;
        //ConvertDir.clear();
        //if (ok && !config->Read("ConvertDir", &ConvertDir))
        //{
        //    ConvertDir=dir;
        //}
        //FileDialogConvert->SetDirectory(ConvertDir);
    }
    logger_base.debug("Show directory %s.", (const char *)dir.c_str());

    if (!xLightsApp::mediaDir.IsNull())
    {
        mediaDirectory = xLightsApp::mediaDir;
    }
    else if (ok && !config->Read(_("MediaDir"), &mediaDirectory))
    {
        mediaDirectory=dir;
    }
    ObtainAccessToURL(mediaDirectory.ToStdString());

    logger_base.debug("Media directory %s.", (const char *)mediaDirectory.c_str());
    MediaDirectoryLabel->SetLabel(mediaDirectory);

    wxString tbData = config->Read("ToolbarLocations");
    if (tbData.StartsWith(TOOLBAR_SAVE_VERSION))
    {
        MainAuiManager->LoadPerspective(tbData.Right(tbData.size() - 5));
    }
    logger_base.debug("Perspectives loaded.");

    config->Read("xLightsBackupSubdirectories", &_backupSubfolders, false);
    MenuItem_BackupSubfolders->Check(_backupSubfolders);
    logger_base.debug("Backup subdirectories: %s.", _backupSubfolders ? "true" : "false");

    config->Read("xLightsExcludePresetsPkgSeq", &_excludePresetsFromPackagedSequences, false);
    MenuItem_ExcludePresetsFromPackagedSequences->Check(_excludePresetsFromPackagedSequences);
    logger_base.debug("Exclude Presets From Packaged Sequences: %s.", _excludePresetsFromPackagedSequences ? "true" : "false");

    config->Read("xLightsExcludeAudioPkgSeq", &_excludeAudioFromPackagedSequences, false);
    MenuItem_ExcludeAudioPackagedSequence->Check(_excludeAudioFromPackagedSequences);
    logger_base.debug("Exclude Audio From Packaged Sequences: %s.", _excludeAudioFromPackagedSequences ? "true" : "false");

    config->Read("xLightsShowACLights", &_showACLights, false);
    MenuItem_ACLIghts->Check(_showACLights);
    logger_base.debug("Show AC Lights toolbar: %s.", _showACLights ? "true" : "false");

    config->Read("xLightsShowACRamps", &_showACRamps, false);
    MenuItem_ShowACRamps->Check(_showACRamps);
    logger_base.debug("Show AC Ramps: %s.", _showACRamps ? "true" : "false");

    config->Read("xLightsAutoSavePerspectives", &_autoSavePerspecive, false);
    MenuItem_PerspectiveAutosave->Check(_autoSavePerspecive);
    logger_base.debug("Autosave perspectives: %s.", _autoSavePerspecive ? "true" : "false");

    config->Read("xLightsRenderOnSave", &mRenderOnSave, true);
    mRenderOnSaveMenuItem->Check(mRenderOnSave);
    logger_base.debug("Render on save: %s.", mRenderOnSave? "true" : "false");

    config->Read("xLightsBackupOnSave", &mBackupOnSave, false);
    mBackupOnSaveMenuItem->Check(mBackupOnSave);
    logger_base.debug("Backup on save: %s.", mBackupOnSave? "true" : "false");

    config->Read("xLightsBackupOnLaunch", &mBackupOnLaunch, true);
    MenuItem_BackupOnLaunch->Check(mBackupOnLaunch);
    logger_base.debug("Backup on launch: %s.", mBackupOnLaunch? "true" : "false");

    config->Read(_("xLightsAltBackupDir"), &mAltBackupDir);
    logger_base.debug("Alternate Backup Dir: '%s'.", (const char *)mAltBackupDir.c_str());

    if (wxDir::Exists(mAltBackupDir))
    {
        mAltBackupMenuItem->SetHelp(mAltBackupDir);
    }
    else
    {
        mAltBackupMenuItem->SetHelp("");
    }

    config->Read("xLightsIconSize", &mIconSize, 16);
    int isid = ID_MENUITEM_ICON_SMALL;
    if (mIconSize == 24) {
        isid = ID_MENUITEM_ICON_MEDIUM;
    } else if (mIconSize == 32) {
        isid = ID_MENUITEM_ICON_LARGE;
    } else if (mIconSize == 48) {
        isid = ID_MENUITEM_ICON_XLARGE;
    }

    wxCommandEvent event(wxEVT_NULL, isid);
    SetToolIconSize(event);
    logger_base.debug("Icon size: %d.", mIconSize);

    config->Read("AutoSaveInterval", &AutoSaveInterval, 3);
    long mid = ID_MENUITEM_AUTOSAVE_0;
    switch (AutoSaveInterval) {
        case 3:
            mid = ID_MENUITEM_AUTOSAVE_3;
            break;
        case 10:
            mid = ID_MENUITEM_AUTOSAVE_10;
            break;
        case 15:
            mid = ID_MENUITEM_AUTOSAVE_15;
            break;
        case 30:
            mid = ID_MENUITEM_AUTOSAVE_30;
            break;
        default:
            break;
    }
    wxCommandEvent asEvent(wxEVT_NULL, mid);
    AutoSaveIntervalSelected(asEvent);
    logger_base.debug("Autosave interval: %d.", AutoSaveInterval);

    int glVer = 99;
    config->Read("ForceOpenGLVer", &glVer, 99);
    if (glVer != 99) {
        int lastGlVer;
        config->Read("LastOpenGLVer", &lastGlVer, 0);
        if (lastGlVer == 0) {
            config->Write("ForceOpenGLVer", 99);
            glVer = 99;
        } else if (glVer != lastGlVer) {
            CallAfter(&xLightsFrame::MaybePackageAndSendDebugFiles);
        }
    }
    logger_base.debug("Force OpenGL version: %d.", glVer);

    config->Write("LastOpenGLVer", glVer);
    switch (glVer) {
        case 1:
            OpenGLMenu->Check(ID_MENU_OPENGL_1, true);
            break;
        case 2:
            OpenGLMenu->Check(ID_MENU_OPENGL_2, true);
            break;
        case 3:
            OpenGLMenu->Check(ID_MENU_OPENGL_3, true);
            break;
        default:
            OpenGLMenu->Check(ID_MENU_OPENGL_AUTO, true);
            break;
    }

    config->Read("xLightsGridSpacing", &mGridSpacing, 16);
    if (mGridSpacing != 16)
    {
        int idi = ID_MENUITEM_GRID_ICON_MEDIUM;
        if (mGridSpacing == 32)
        {
            idi = ID_MENUITEM_GRID_ICON_LARGE;
        }
        else if (mGridSpacing >= 48)
        {
            idi = ID_MENUITEM_GRID_ICON_XLARGE;
        }
        else if (mGridSpacing <= 12)
        {
            idi = ID_MENUITEM_GRID_ICON_XSMALL;
        }
        wxCommandEvent eventi(wxEVT_NULL, idi);
        SetIconSize(eventi);
    }
    logger_base.debug("Grid spacing: %d.", mGridSpacing);

    config->Read("xLightsGridIconBackgrounds", &mGridIconBackgrounds, true);
    {
        int idb = mGridIconBackgrounds ? ID_MENUITEM_GRID_ICON_BACKGROUND_ON : ID_MENUITEM_GRID_ICON_BACKGROUND_OFF;
        wxCommandEvent eventb(wxEVT_NULL, idb);
        OnSetGridIconBackground(eventb);
    }
    logger_base.debug("Grid icon backgrounds: %s.", mGridIconBackgrounds ? "true" : "false");

    config->Read("xLightsTimingPlayOnDClick", &mTimingPlayOnDClick, true);
    {
        int idb = mTimingPlayOnDClick ? ID_MENU_TIMING_PLAY_MODE : ID_MENU_TIMING_EDIT_MODE;
        wxCommandEvent eventb(wxEVT_NULL, idb);
        OnMenuItemTimingPlayOnDClick(eventb);
    }
    logger_base.debug("Timing Play on DClick: %s.", mTimingPlayOnDClick ? "true" : "false");

    config->Read("xLightsGridNodeValues", &mGridNodeValues, true);
    {
        int idg = mGridNodeValues ? ID_MENUITEM_GRID_NODE_VALUES_ON : ID_MENUITEM_GRID_NODE_VALUES_OFF;
        wxCommandEvent eventg(wxEVT_NULL, idg);
        OnSetGridNodeValues(eventg);
    }
    logger_base.debug("Grid node values: %s.", mGridNodeValues ? "true" : "false");

    config->Read("xLightsEffectAssistMode", &mEffectAssistMode, EFFECT_ASSIST_TOGGLE_MODE);
    MenuItemEffectAssistAlwaysOn->Check(mEffectAssistMode==EFFECT_ASSIST_ALWAYS_ON);
    MenuItemEffectAssistAlwaysOff->Check(mEffectAssistMode==EFFECT_ASSIST_ALWAYS_OFF);
    MenuItemEffectAssistToggleMode->Check(mEffectAssistMode==EFFECT_ASSIST_TOGGLE_MODE);
    logger_base.debug("Effect Assist Mode: %s.", mEffectAssistMode ? "true" : "false");

    _setupChanged = false;
    InitEffectsPanel(EffectsPanel1);
    logger_base.debug("Effects panel initialised.");

    EffectTreeDlg = nullptr;  // must be before any call to SetDir

    SetPlayMode(play_off);
    ResetEffectsXml();
    EnableSequenceControls(true);
    if (ok && !dir.IsEmpty())
    {
        if (!SetDir(dir)) {
            if (!PromptForShowDirectory())
            {
                CurrentDir = "";
                splash.Hide();
                return;
            }
        }
    }
    else
    {
        if (!PromptForShowDirectory())
        {
            CurrentDir = "";
            splash.Hide();
            return;
        }
    }

    MixTypeChanged=true;

    Notebook1->ChangeSelection(SETUPTAB);
    EnableNetworkChanges();

    wxImage::AddHandler(new wxGIFHandler);

    config->Read("xLightse131Sync", &me131Sync, false);
    MenuItem_e131sync->Check(me131Sync);
    _outputManager.SetSyncEnabled(me131Sync);
    logger_base.debug("e1.31 Sync: %s.", me131Sync ? "true" : "false");
    ShowHideSync();

    config->Read("xLightsLocalIP", &mLocalIP, "");
    _outputManager.SetForceFromIP(mLocalIP.ToStdString());

    if (mLocalIP != "")
    {
        MenuItem_ForceLocalIP->Check(true);
    }
    else
    {
        MenuItem_ForceLocalIP->Check(false);
    }

    switch(_outputManager.GetSuppressFrames())
    {
    case 10:
        MenuItem_SD_10->Check();
        break;
    case 20:
        MenuItem_SD_20->Check();
        break;
    case 40:
        MenuItem_SD_40->Check();
        break;
    default:
    case 0:
        MenuItem_SD_None->Check();
        break;
    }

    UpdateACToolbar();
    ShowACLights();

    //start out with 50ms timer, once we load a file or create a new one, we'll reset
    //to whatever the timing that is selected
    Timer1.Start(50, wxTIMER_CONTINUOUS);

    // What makes 4 the right answer ... try 10 ... why ... usually it is one thread that runs slow and that model
    // holds up others so in the time while we wait for the busy thread we can actually run a lot more models
    // what is the worst that could happen ... all models want to run hard so we lose some efficiency while we churn between
    // threads ... a minor loss of efficiency ... I think the one thread blocks the others is more common.
    // Dan is concerned on 32 bit windows 10 will chew up too much heap memory ... so splitting the difference we get 7

    // CAUTION ... if this results in a value < 20 then it will set it to 20. If > 250 then it will set it to 250 ... that is not obvious until you step into the code
    jobPool.Start(wxThread::GetCPUCount() * 7);

    if (!xLightsApp::sequenceFiles.IsEmpty())
    {
        logger_base.debug("Opening sequence: %s.", (const char *)xLightsApp::sequenceFiles[0].c_str());
        OpenSequence(xLightsApp::sequenceFiles[0], nullptr);
    }

	SetAudioControls();

#ifdef __WXOSX_MAC__
    // we remove this on OSX because xSchedule is not simple to locate ... at least I dont know how to do it
    MenuItem_xSchedule->GetMenu()->Remove(MenuItem_xSchedule->GetId());
#endif

    DrawingContext::Initialize(this);

    MenuItem_File_Save->Enable(true);
    MenuItem_File_Save->SetItemLabel("Save Setup\tCTRL-s");

    splash.Hide();

#ifdef __WXMSW__
    check32AppOn64Machine();
#endif

    logger_base.debug("xLightsFrame construction complete.");
}

xLightsFrame::~xLightsFrame()
{
    static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));
    static bool reenter = false;

    if (reenter)
    {
        logger_base.error("~xLightsFrame re-entered ... this wont end well.");
    }
    reenter = true;

    Timer_AutoSave.Stop();
    EffectSettingsTimer.Stop();
    Timer1.Stop();
    DrawingContext::CleanUp();

    selectedEffect = nullptr;
    _outputManager.DeleteAllOutputs();

    wxConfigBase* config = wxConfigBase::Get();
    if (mResetToolbars)
    {
        config->DeleteEntry("ToolbarLocations");
    }
    else
    {
        config->Write("ToolbarLocations", TOOLBAR_SAVE_VERSION + MainAuiManager->SavePerspective());
    }
    config->Write("xLightsIconSize", mIconSize);
    config->Write("xLightsGridSpacing", mGridSpacing);
    config->Write("xLightsGridIconBackgrounds", mGridIconBackgrounds);
    config->Write("xLightsTimingPlayOnDClick", mTimingPlayOnDClick);
    config->Write("xLightsGridNodeValues", mGridNodeValues);
    config->Write("xLightsRenderOnSave", mRenderOnSave);
    config->Write("xLightsBackupSubdirectories", _backupSubfolders);
    config->Write("xLightsExcludePresetsPkgSeq", _excludePresetsFromPackagedSequences);
    config->Write("xLightsExcludeAudioPkgSeq", _excludeAudioFromPackagedSequences);
    config->Write("xLightsShowACLights", _showACLights);
    config->Write("xLightsShowACRamps", _showACRamps);
    config->Write("xLightsPlayControlsOnPreview", _playControlsOnPreview);
    config->Write("xLightsAutoShowHousePreview", _autoShowHousePreview);
    config->Write("xLightsModelBlendDefaultOff", _modelBlendDefaultOff);
    config->Write("xLightsSnapToTimingMarks", _snapToTimingMarks);
    config->Write("xLightsAutoSavePerspectives", _autoSavePerspecive);
    config->Write("xLightsBackupOnSave", mBackupOnSave);
    config->Write("xLightsBackupOnLaunch", mBackupOnLaunch);
    config->Write("xLightse131Sync", me131Sync);
    config->Write("xLightsLocalIP", mLocalIP);
    config->Write("xLightsEffectAssistMode", mEffectAssistMode);
    config->Write("xLightsAltBackupDir", mAltBackupDir);

    //definitely not outputting data anymore
    config->Write("OutputActive", false);

    config->Flush();

    wxFileName kbf;
    kbf.AssignDir(CurrentDir);
    kbf.SetFullName("xlights_keybindings.xml");
    mainSequencer->keyBindings.Save(kbf);

    //must call these or the app will crash on exit
    m_mgr->UnInit();
    MainAuiManager->UnInit();

    for (int x = 0; x < Notebook1->GetPageCount(); x++) {
        wxWindow *w = Notebook1->GetPage(x);
        if (w->GetEventHandler() == m_mgr) {
            w->RemoveEventHandler(m_mgr);
        }
    }
    Notebook1->DeleteAllPages();
    delete m_mgr;
    delete MainAuiManager;

    if( CurrentSeqXmlFile )
    {
        delete CurrentSeqXmlFile;
        CurrentSeqXmlFile = nullptr;
    }

    delete CheckBoxLightOutput;

    //(*Destroy(xLightsFrame)
    //*)

    reenter = false;
}

void xLightsFrame::OnIdle(wxIdleEvent& event) {
    static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));
    logger_base.debug("Idle event called");
    Unbind(wxEVT_IDLE, &xLightsFrame::OnIdle,this);
    CheckForUpdate(false);
}

void xLightsFrame::DoMenuAction(wxMenuEvent &evt) {
    static bool inMenuAction = false;
    if (inMenuAction) {
        return;
    }
    inMenuAction= true;
    wxWindow *w = FindFocus();
    evt.Skip();
    if (w != nullptr && w->GetEventHandler()) {
        w->GetEventHandler()->ProcessEventLocally(evt);
    }
    inMenuAction= false;
}


void xLightsFrame::OnQuit(wxCommandEvent& event)
{
    static bool inQuit = false;

    if (inQuit) return;

    inQuit = true;

    static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));
    logger_base.info("Quit");
	wxCloseEvent evt;
    if (QuitMenuItem->IsEnabled())
    {
        OnClose(evt);
    }

    inQuit = false;
}

void xLightsFrame::InitEffectsPanel(EffectsPanel* panel)
{
    panel->CurrentDir = &CurrentDir;
}


void xLightsFrame::OnAbout(wxCommandEvent& event)
{
    wxString hdg = wxString::Format(_("About xLights %s %s"), xlights_version_string, GetBitness());
    wxString ver = wxString::Format(_("xLights\nVersion: %s  -  %s\n\n"), xlights_version_string, GetBitness());
    wxMessageDialog dlg(this, ver + XLIGHTS_LICENSE, hdg);
    dlg.ShowModal();
}

void xLightsFrame::SetPlayMode(play_modes newmode)
{
    switch (newmode)
    {
    case play_off:
        SetStatusText(_("Playback: off"));
        break;
    default:
        break;
    }

    play_mode=newmode;
    starttime = wxDateTime::UNow();
}

void xLightsFrame::OnTimer1Trigger(wxTimerEvent& event)
{
    wxTimeSpan ts = wxDateTime::UNow() - starttime;
    long curtime = ts.GetMilliseconds().ToLong();
    _outputManager.StartFrame(curtime);
    switch (Notebook1->GetSelection())
    {
    case NEWSEQUENCER:
        if( playAnimation ) {
            TimerRgbSeq(curtime * playSpeed);
        } else {
            TimerRgbSeq(curtime);
        }
        break;
    default:
        break;
    }
    _outputManager.EndFrame();
}

void xLightsFrame::OnBitmapButtonTabInfoClick(wxCommandEvent& event)
{
    wxString caption,msg;

    switch (Notebook1->GetSelection())
    {
    case SETUPTAB:
        caption=_("Setup Tab");
        msg=_("Show Directory\n\nThe first thing you need to know about xLights is that it expects you to organize all of your sequence files and associated audio or video files into a single directory. For example, you can have a directory called '2012 Show'. Once you have your show directory created and populated with the relevant files, you are ready to proceed. Tell xLights where your new show directory is by clicking the 'Change' button on the Setup tab, navigate to your show directory, then click 'OK'.\n\nLighting Networks\n\nThe next thing you will need to do is define your lighting network(s). xLights ignores most of the information about your lighting network contained in your LOR or Vixen sequence. Thus this step is very important! Add a row in the lower half of the Setup tab for each network used in your display. xLights can drive a mixture of network types (for example, the first network can be DMX, and the second one LOR, and the third one Renard). When you are finished, do not forget to SAVE YOUR CHANGES by clicking the 'Save Setup' button.");
        break;
    case LAYOUTTAB:
        caption=_("Layout Tab");
        msg=_("Create display elements by clicking on the Models buttons. You can drag your cursor across the preview area to move the element. Don't forget to click the Save button to save your preview!\n\nClick the Open button to select an xLights sequence to be previewed. Note that any xLights sequence can be previewed, not just those created on the Sequencer tab. Click Play to start preview playback. Use the Pause button to stop play, and then the Play button to resume. You can drag the slider that appears across the top of the preview area to move playback to any spot in your sequence. The Stop Now button in the upper left will also stop playback.");
        break;
    case NEWSEQUENCER:
        caption=_("Sequencer Tab");
        msg=_("The Sequencer tab can be used to create RGB sequences. First, create a model of your RGB display element(s) by clicking on the Models button. Then try the different effects and settings until you create something you like. You can save the settings as a preset by clicking the New Preset button. From then on, that preset will be available in the presets drop-down list. You can combine effects by creating a second effect in the Effect 2 area, then choosing a Layering Method. To create a series of effects that will be used in a sequence, click the open file icon to open an xLights sequence. Choose which display elements/models you will use in this sequence. Then click the insert rows icon and type in the start time in seconds when that effect should begin. Rows will automatically sort by start time. To add an effect to the sequence, click on the grid cell in the desired display model column and the desired start time row, then click the Update button. When you are done creating effects for the sequence, click the save icon and the xLights sequence will be updated with the effects you stored in the grid.");
        break;
    default:
        break;
    }
    wxMessageBox(msg,caption);
}

void xLightsFrame::ResetAllSequencerWindows()
{
	wxAuiPaneInfoArray &info = m_mgr->GetAllPanes();
	bool update = false;
	for (size_t x = 0; x < info.size(); x++)
	{
		if (info[x].IsFloating() && info[x].IsShown())
		{
			info[x].Dock();
			update = true;
		}
	}
	if (update)
	{
		m_mgr->Update();
	}
}

void xLightsFrame::ShowHideAllSequencerWindows(bool show)
{
    static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));

    // this logging is extra until we find out why this function crashes
    logger_base.debug("xLightsFrame::ShowHideAllSequencerWindows");

    if (m_mgr == nullptr)
    {
        logger_base.crit("ShowHideAllSequencerWindows m_mgr is null ... this is going to crash");
    }
    wxAuiPaneInfoArray &info = m_mgr->GetAllPanes();
    bool update = false;
    if (show)
    {
        logger_base.debug("xLightsFrame::ShowHideAllSequencerWindows - show");
        for (size_t x = 0; x < info.size(); x++)
        {
            if (info[x].IsOk() &&
                savedPaneShown.find(info[x].name) != savedPaneShown.end() &&
                savedPaneShown[info[x].name])
            {
                if (info[x].frame != nullptr)
                    info[x].frame->Show();
            }
        }
        savedPaneShown.clear();
    }
    else
    {
        savedPaneShown.clear();
        logger_base.debug("xLightsFrame::ShowHideAllSequencerWindows - hide");
        for (size_t x = 0; x < info.size(); x++)
        {
            savedPaneShown[info[x].name] = false;
            if (info[x].IsOk())
            {
                if (info[x].IsFloating() && info[x].IsShown())
                {
                    savedPaneShown[info[x].name] = true;
                    info[x].frame->Hide();
                }
            }
            else
            {
                logger_base.warn("Pane %d was not valid ... ShowHideAllSequencerWindows", x);
            }
        }
    }

    if (update)
    {
        logger_base.debug("xLightsFrame::ShowHideAllSequencerWindows - update");
        m_mgr->Update();
    }
    // show/hide Layout Previews
    for (auto it = LayoutGroups.begin(); it != LayoutGroups.end(); ++it) {
        logger_base.debug("xLightsFrame::ShowHideAllSequencerWindows - layout previews");
        LayoutGroup* grp = *it;
        if (grp != nullptr) {
            if (grp->GetMenuItem() == nullptr)
            {
                logger_base.crit("ShowHideAllSequencerWindows grp->GetMenuItem() is null ... this is going to crash");
            }
            if( grp->GetMenuItem()->IsChecked() ) {
                grp->SetPreviewActive(show);
            }
        }
        logger_base.debug("xLightsFrame::ShowHideAllSequencerWindows - layout previews - done");
    }
}

void xLightsFrame::RecalcModels(bool force)
{
    if (force || _setupChanged)
    {
        SetCursor(wxCURSOR_WAIT);
        // Now notify the layout as the model start numbers may have been impacted
        AllModels.OldRecalcStartChannels();
        //AllModels.NewRecalcStartChannels();
        if (layoutPanel != nullptr) {
            layoutPanel->RefreshLayout();
        }

        _setupChanged = false;
        SetCursor(wxCURSOR_ARROW);
    }
}

void xLightsFrame::OnNotebook1PageChanging(wxAuiNotebookEvent& event)
{
    if (layoutPanel == nullptr) {
        event.Veto();
        return;
    }
    if (event.GetOldSelection() == NEWSEQUENCER)
    {
        ShowHideAllSequencerWindows(false);
    }
    else if (event.GetOldSelection() == SETUPTAB)
    {
        layoutPanel->UnSelectAllModels();
        RecalcModels();
    }
}

void xLightsFrame::OnNotebook1PageChanged1(wxAuiNotebookEvent& event)
{
    heartbeat("tab change", true); //tell fido to stop watching -DJ
    int pagenum=event.GetSelection(); //Notebook1->GetSelection();
	if (pagenum == LAYOUTTAB)
    {
        // these commented out lines were already setup when rgbeffects file was loaded and it messes up multiple preview loading.
        //modelPreview->InitializePreview(mBackgroundImage,mBackgroundBrightness);
        //modelPreview->SetScaleBackgroundImage(mScaleBackgroundImage);
        UpdatePreview();
        SetStatusText(_(""));
        MenuItem_File_Save->Enable(true);
        MenuItem_File_Save->SetItemLabel("Save Layout\tCTRL-s");
    }
    else if (pagenum == NEWSEQUENCER)
    {
        InitSequencer();
        ShowHideAllSequencerWindows(true);
        EffectSettingsTimer.Start(50);
        MenuItem_File_Save->SetItemLabel("Save Sequence\tCTRL-s");
        MenuItem_File_Save->Enable(MenuItem_File_SaveAs_Sequence->IsEnabled());
    }
    else if (pagenum == SETUPTAB)
    {
        MenuItem_File_Save->SetItemLabel("Save Setup\tCTRL-s");
        MenuItem_File_Save->Enable(true);
        SetStatusText(_(""));
    }
    else
    {
        MenuItem_File_Save->SetItemLabel("Save");
        SetStatusText(_(""));
    }
}

void xLightsFrame::OnButtonLightsOffClick(wxCommandEvent& event)
{
    if (_outputManager.IsOutputting())
    {
        CheckBoxLightOutput->SetValue(false);
        _outputManager.AllOff();
        _outputManager.StopOutput();
        CheckBoxLightOutput->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlART_OUTPUT_LIGHTS")), wxART_TOOLBAR));
        EnableNetworkChanges();
    }
}

bool xLightsFrame::EnableOutputs()
{
    bool ok = true;

    if (CheckBoxLightOutput->IsChecked() && !_outputManager.IsOutputting())
    {
        if (_outputManager.IsOutputOpenInAnotherProcess())
        {
            wxMessageBox("Another process seems to be outputing to lights right now. This may not generate the result expected.");
        }

        ok = _outputManager.StartOutput();
        if (ok)
        {
            CheckBoxLightOutput->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlART_OUTPUT_LIGHTS_ON")), wxART_TOOLBAR));
        }
    }
    else if (!CheckBoxLightOutput->IsChecked() && _outputManager.IsOutputting())
    {
        _outputManager.AllOff();
        _outputManager.StopOutput();
        CheckBoxLightOutput->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlART_OUTPUT_LIGHTS")),wxART_TOOLBAR));
    }
    EnableNetworkChanges();
    return ok;
}

void xLightsFrame::EnableNetworkChanges()
{
    bool flag=(!_outputManager.IsOutputting() && !CurrentDir.IsEmpty());
    ButtonAddDongle->Enable(flag);
    ButtonAddE131->Enable(flag);
    ButtonArtNET->Enable(flag);
    ButtonAddNull->Enable(flag);
    ButtonNetworkChange->Enable(flag);
    ButtonNetworkDelete->Enable(flag);
    ButtonNetworkDeleteAll->Enable(flag);
    BitmapButtonMoveNetworkUp->Enable(flag);
    BitmapButtonMoveNetworkDown->Enable(flag);
    SpinCtrl_SyncUniverse->Enable(flag);
    ButtonSaveSetup->Enable(!CurrentDir.IsEmpty());
    CheckBoxLightOutput->Enable(!CurrentDir.IsEmpty());
}

void xLightsFrame::OnCheckBoxLightOutputClick(wxCommandEvent& event)
{
    EnableOutputs();
    //CheckChannelList=true;  // cause status bar to be updated if in test mode
}


//factored out from below so it can be reused by play/pause button -DJ
void xLightsFrame::StopNow(void)
{
    int actTab = Notebook1->GetSelection();
	if (CurrentSeqXmlFile != nullptr && CurrentSeqXmlFile->GetMedia() != nullptr)
	{
		CurrentSeqXmlFile->GetMedia()->Stop();
	}
    heartbeat("playback end", true); //tell fido to stop watching -DJ
    SetPlayMode(play_off);
    switch (actTab)
    {
    case NEWSEQUENCER:
        {
            wxCommandEvent playEvent(EVT_STOP_SEQUENCE);
            wxPostEvent(this, playEvent);
        }
        break;
    default:
        break;
    }
}

bool xLightsFrame::ShowFolderIsInBackup(const std::string showdir)
{
    int i = showdir.length() - 1;
    wxString dir = "";
    while (i >= 0)
    {
        if (showdir[i] == '\\' || showdir == '/')
        {
            if (dir.Lower() == "backup")
            {
                return true;
            }
            dir = "";
        }
        else if (showdir[i] == ':')
        {
            return false;
        }
        else
        {
            dir = showdir[i] + dir;
        }
        i--;
    }

    return false;
}

void xLightsFrame::OnButtonStopNowClick(wxCommandEvent& event)
{
    StopNow();
}

//make these static so they can be accessed outside of xLightsFrame: -DJ
//NOTE: this assumes there will only be one xLightsMain object
wxString xLightsFrame::CurrentDir = "";
wxString xLightsFrame::PlaybackMarker = "";
wxString xLightsFrame::xlightsFilename = "";
xLightsXmlFile* xLightsFrame::CurrentSeqXmlFile = nullptr;

void xLightsFrame::OnClose(wxCloseEvent& event)
{
    static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));

    if (!QuitMenuItem->IsEnabled()) {
        return;
    }

    static bool inClose = false;

    if (inClose) return;

    inClose = true;

	logger_base.info("xLights Closing");

	StopNow();

	if (!CloseSequence())
    {
		logger_base.info("Closing aborted.");
		event.Veto();
        inClose = false;
        return;
    }
    selectedEffect = nullptr;

    CheckUnsavedChanges();

    ShowHideAllSequencerWindows(false);

    // destroy preview windows
    for (auto it = PreviewWindows.begin(); it != PreviewWindows.end(); ++it) {
        ModelPreview* preview = *it;
        delete preview;
    }

    heartbeat("exit", true); //tell fido about graceful exit -DJ

    Destroy();
	logger_base.info("xLights Closed.");

    inClose = false;
}

void xLightsFrame::DoBackup(bool prompt, bool startup, bool forceallfiles)
{
    static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));
    wxString folderName;
    time_t cur;
    time(&cur);
    wxFileName newDirH;
    wxDateTime curTime(cur);

    //  first make sure there is a Backup sub directory

    wxString newDirBackup = CurrentDir + wxFileName::GetPathSeparator() + "Backup";
    if (!wxDirExists(newDirBackup) && !newDirH.Mkdir(newDirBackup))
    {
        logger_base.error("Unable to create backup directory '%s'", (const char *)newDirBackup.c_str());
        wxMessageBox("Unable to create directory Backup!", "Error", wxICON_ERROR | wxOK);
        return;
    }

    wxString newDir = CurrentDir + wxFileName::GetPathSeparator() + wxString::Format(
        "Backup%c%s-%s", wxFileName::GetPathSeparator(),
        curTime.FormatISODate(), curTime.Format("%H%M%S"));
    if (startup)
    {
        newDir += "_OnStart";
    }

    int tries = 0;
    while (wxDirExists(newDir) && tries < 11)
    {
        logger_base.warn("Backup directory '%s' already existed ... trying again", (const char *)newDir.c_str());

        newDir = CurrentDir + wxFileName::GetPathSeparator() + wxString::Format(
            "Backup%c%s-%s", wxFileName::GetPathSeparator(),
            curTime.FormatISODate(), curTime.Format("%H%M%S")) + "_" + char(65 + tries);
        if (startup)
        {
            newDir += "_OnStart";
        }

        tries++;
    }

    if (tries == 11)
    {
        logger_base.error("Unable to find a unique name for backup directory");
        wxMessageBox("Unable to find a unique name for backup directory! Backup failed.", "Error", wxICON_ERROR | wxOK);
        return;
    }

    if (prompt)
    {
        if (wxNO == wxMessageBox("All xml files under " + wxString::Format("%i", MAXBACKUPFILE_MB) + "MB in your xlights directory will be backed up to \"" +
            newDir + "\". Proceed?", "Backup", wxICON_QUESTION | wxYES_NO))
        {
            return;
        }
    }

    if (!newDirH.Mkdir(newDir))
    {
        logger_base.error("Unable to create backup directory '%s'", (const char *)newDir.c_str());
        wxMessageBox("Unable to create directory! Backup failed.", "Error", wxICON_ERROR | wxOK);
        return;
    }
    else
    {
        logger_base.info("Backup directory '%s' created", (const char *)newDir.c_str());
    }

    BackupDirectory(CurrentDir, newDir, newDir, forceallfiles);
}

void xLightsFrame::OnMenuItemBackupSelected(wxCommandEvent& event)
{
    SaveWorking();

    DoBackup(true);
}

void xLightsFrame::CreateMissingDirectories(wxString targetDirName, wxString lastCreatedDirectory)
{
    static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));
    if (wxDir::Exists(targetDirName)) return;

    wxFileName tgt(targetDirName);
    wxFileName lst(lastCreatedDirectory);

    if (!tgt.GetFullPath().StartsWith(lst.GetFullPath())) return;

    //wxArrayString tgtd = tgt.GetDirs();
    //wxArrayString lstd = lst.GetDirs();
    wxArrayString tgtd = wxSplit(targetDirName, wxFileName::GetPathSeparator());
    wxArrayString lstd = wxSplit(lastCreatedDirectory, wxFileName::GetPathSeparator());
    wxString newDir = lastCreatedDirectory;

    for (size_t i = lstd.Count(); i < tgtd.Count(); i++)
    {
        wxDir dir(newDir);
        newDir += wxFileName::GetPathSeparator() + tgtd[i];
        if (!wxDir::Exists(newDir))
        {
            logger_base.debug("    Create folder '%s'.", (const char*)newDir.c_str());
            if (!dir.Make(newDir))
            {
                logger_base.error("        Folder Create failed.");
            }
        }
    }
}

bool xLightsFrame::CopyFiles(const wxString& wildcard, wxDir& srcDir, wxString& targetDirName, wxString lastCreatedDirectory, bool forceallfiles)
{
    static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));
    bool res = false;
    wxString fname;
    wxString srcDirName = srcDir.GetNameWithSep();
    wxFileName srcFile;
    srcFile.SetPath(srcDir.GetNameWithSep());

    bool cont = srcDir.GetFirst(&fname, wildcard, wxDIR_FILES);
    while (cont)
    {
        logger_base.debug("Backing up file %s.", (const char *)(srcDirName + fname).c_str());
        res = true;

        CreateMissingDirectories(targetDirName, lastCreatedDirectory);

        srcFile.SetFullName(fname);

        wxULongLong fsize = srcFile.GetSize();
        if (!forceallfiles && fsize > MAXBACKUPFILE_MB * 1024 * 1024) // skip any xml files > MAXBACKUPFILE_MB mbytes, they are something other than xml files
        {
            logger_base.warn("    Skipping file as it is too large.");
            cont = srcDir.GetNext(&fname);
            continue;
        }

        logger_base.debug("    to %s.", (const char *)(targetDirName + wxFileName::GetPathSeparator() + fname).c_str());
        SetStatusText("Copying File \"" + srcFile.GetFullPath());
        bool success = wxCopyFile(srcDirName + fname,
            targetDirName + wxFileName::GetPathSeparator() + fname);
        if (!success)
        {
            logger_base.error("    Copy Failed.");
            wxMessageBox("Unable to copy file \"" + srcDir.GetNameWithSep() + fname + "\"",
                "Error", wxICON_ERROR | wxOK);
        }
        cont = srcDir.GetNext(&fname);
    }

    return res;
}

void xLightsFrame::BackupDirectory(wxString sourceDir, wxString targetDirName, wxString lastCreatedDirectory, bool forceallfiles)
{
    wxDir srcDir(sourceDir);

    if(!srcDir.IsOpened())
    {
        return;
    }

    if (CopyFiles("*.xml", srcDir, targetDirName, lastCreatedDirectory, forceallfiles) +
        CopyFiles("*.xbkp", srcDir, targetDirName, lastCreatedDirectory, forceallfiles) +
        CopyFiles("*.xschedule", srcDir, targetDirName, lastCreatedDirectory, forceallfiles) > 0)
    {
        lastCreatedDirectory = targetDirName;
    }

    // recurse through all directories but folders named Backup
    if (_backupSubfolders)
    {
        wxString dir;
        bool cont = srcDir.GetFirst(&dir, "", wxDIR_DIRS);
        while (cont)
        {
            if (dir != "Backup")
            {
                wxDir subdir(srcDir.GetNameWithSep() + dir);
                BackupDirectory(subdir.GetNameWithSep(), targetDirName + wxFileName::GetPathSeparator() + dir, lastCreatedDirectory, forceallfiles);
            }
            cont = srcDir.GetNext(&dir);
        }
    }

    SetStatusText("All xml files backed up.");
}

bool xLightsFrame::isRandom_(wxControl* ctl, const char*debug)
{
    bool retval = (buttonState[std::string(ctl->GetName())] != Locked);
    return retval;
}

void xLightsFrame::OnEffectsPanel1Paint(wxPaintEvent& event) {}

void xLightsFrame::OnGrid1SetFocus(wxFocusEvent& event)
{
    Grid1HasFocus = true;
}

void xLightsFrame::OnGrid1KillFocus(wxFocusEvent& event)
{
    Grid1HasFocus = false;
}

void xLightsFrame::OntxtCtrlSparkleFreqText(wxCommandEvent& event) {}

static void AddNonDupAttr(wxXmlNode* node, const wxString& name, const wxString& value)
{
    wxString junk;
    if (node->GetAttribute(name, &junk)) node->DeleteAttribute(name); //kludge: avoid dups
    if (!value.empty()) node->AddAttribute(name, value);
}

//sigh; a function like this should have been built into wxWidgets
wxXmlNode* xLightsFrame::FindNode(wxXmlNode* parent, const wxString& tag, const wxString& attr, const wxString& value, bool create /*= false*/)
{
#if 0
    static struct
    {
        std::unordered_map<const char*, wxXmlNode*> nodes;
        std::string parent, child;
    } cached_names;

    if (parent->GetName() != cached_names.parent) //reload cache
    {
        cached_names.nodes.clear();
        for (wxXmlNode* node = parent->GetChildren(); node != nullptr; node = node->GetNext())
            cached_names.nodes[node->GetName()] = node;
        cached_names.parent = parent;
    }
    if (cached_names.nodes.find(tag) == cached_names.nodes.end()) //not found
    {
        if (!create) return 0;
        parent->AddChild(cached_names.nodes[tag] = new wxXmlNode(wxXML_ELEMENT_NODE, tag));
    }
    return cached_names.nodes[tag];
#endif // 0
    for (wxXmlNode* node = parent->GetChildren(); node != nullptr; node = node->GetNext())
    {
        if (!tag.empty() && (node->GetName() != tag)) continue;
        if (!value.empty() && (node->GetAttribute(attr) != value)) continue;
        return node;
    }
    if (!create) return 0; //CAUTION: this will give null ptr exc if caller does not check
    wxXmlNode* retnode = new wxXmlNode(wxXML_ELEMENT_NODE, tag); //NOTE: assumes !tag.empty()
    parent->AddChild(retnode);
    if (!value.empty()) AddNonDupAttr(retnode, attr, value);
    return retnode;
}

void xLightsFrame::SetPreviewSize(int width,int height)
{
    SetXmlSetting("previewWidth", wxString::Format(wxT("%i"),width));
    SetXmlSetting("previewHeight", wxString::Format(wxT("%i"),height));
    modelPreview->SetCanvasSize(width, height);
    modelPreview->Refresh();
    _housePreviewPanel->GetModelPreview()->SetVirtualCanvasSize(width, height);
    _housePreviewPanel->Refresh();
}

void xLightsFrame::SetXmlSetting(const wxString& settingName,const wxString& value)
{
    // Delete existing setting node
    for(wxXmlNode* e = SettingsNode->GetChildren(); e != nullptr; e = e->GetNext())
    {
        if (e->GetName() == settingName)
        {
            SettingsNode->RemoveChild(e);
        }
    }

    // Add new one
    wxXmlNode* setting = new wxXmlNode(wxXML_ELEMENT_NODE, settingName);
    setting->AddAttribute("value", value);
    SettingsNode->AddChild(setting);
}

wxString xLightsFrame::GetXmlSetting(const wxString& settingName, const wxString& defaultValue)
{
    // Delete existing setting node
    for(wxXmlNode* e = SettingsNode->GetChildren(); e != nullptr; e = e->GetNext())
    {
        if (e->GetName() == settingName)
        {
            return e->GetAttribute("value", defaultValue);
        }
    }

    return defaultValue;
}

void xLightsFrame::OnButtonClickSaveAs(wxCommandEvent& event)
{
    SaveAsSequence();
}

wxString xLightsFrame::GetSeqXmlFileName()
{
    if (CurrentSeqXmlFile == nullptr)
    {
        return "";
    }

    return CurrentSeqXmlFile->GetFullPath();
}

void xLightsFrame::OnMenu_Settings_SequenceSelected(wxCommandEvent& event)
{
    if( xLightsFrame::CurrentSeqXmlFile == nullptr ) return;

    // abort any in progress render ... as it may be using any already open media
    bool aborted = false;
    if (CurrentSeqXmlFile->GetMedia() != nullptr)
    {
        aborted = AbortRender();
    }

    // populate dialog
    SeqSettingsDialog dialog(this, xLightsFrame::CurrentSeqXmlFile, mediaDirectory, wxEmptyString);
    dialog.Fit();
    int ret_code = dialog.ShowModal();

    if (ret_code == NEEDS_RENDER || aborted)
    {
        RenderAll();
    }

    if (ret_code != wxID_OK) return;  // user pressed cancel

	if(CurrentSeqXmlFile->GetMedia() != nullptr)
	{
		if (CurrentSeqXmlFile->GetMedia()->GetFrameInterval() < 0)
		{
			CurrentSeqXmlFile->GetMedia()->SetFrameInterval(CurrentSeqXmlFile->GetFrameMS());
		}
	}
	SetAudioControls();
}

void xLightsFrame::OnAuiToolBarItemPlayButtonClick(wxCommandEvent& event)
{
    if (Notebook1->GetSelection() == NEWSEQUENCER)
    {
        wxCommandEvent playEvent(EVT_PLAY_SEQUENCE);
        wxPostEvent(this, playEvent);
    }
}

void xLightsFrame::EnableToolbarButton(wxAuiToolBar* toolbar,int id, bool enable)
{
    wxAuiToolBarItem* button = toolbar->FindTool(id);
	if (button != nullptr)
	{
		int state = enable ? wxAUI_BUTTON_STATE_NORMAL : wxAUI_BUTTON_STATE_DISABLED;
		button->SetState(state);
		toolbar->Refresh();
	}
}

void xLightsFrame::OnAuiToolBarItemPauseButtonClick(wxCommandEvent& event)
{
    if (Notebook1->GetSelection() == NEWSEQUENCER)
    {
        wxCommandEvent playEvent(EVT_PAUSE_SEQUENCE);
        wxPostEvent(this, playEvent);
    }
}

void xLightsFrame::OnAuiToolBarItemStopClick(wxCommandEvent& event)
{
    if (Notebook1->GetSelection() == NEWSEQUENCER)
    {
        //playStartTime = playEndTime = 0;
        wxCommandEvent playEvent(EVT_STOP_SEQUENCE);
        wxPostEvent(this, playEvent);
    }
}

void xLightsFrame::OnAuiToolBarFirstFrameClick(wxCommandEvent& event)
{
    if (Notebook1->GetSelection() == NEWSEQUENCER)
    {
        wxCommandEvent playEvent(EVT_SEQUENCE_FIRST_FRAME);
        wxPostEvent(this, playEvent);
    }
}

void xLightsFrame::OnAuiToolBarLastFrameClick(wxCommandEvent& event)
{
    if (Notebook1->GetSelection() == NEWSEQUENCER)
    {
        wxCommandEvent playEvent(EVT_SEQUENCE_LAST_FRAME);
        wxPostEvent(this, playEvent);
    }
}

void xLightsFrame::OnAuiToolBarItemReplaySectionClick(wxCommandEvent& event)
{
    if (Notebook1->GetSelection() == NEWSEQUENCER)
    {
        wxCommandEvent playEvent(EVT_SEQUENCE_REPLAY_SECTION);
        wxPostEvent(this, playEvent);
    }
}

void xLightsFrame::OnAuiToolBarItemZoominClick(wxCommandEvent& event)
{
    if (Notebook1->GetSelection() == NEWSEQUENCER)
    {
        mainSequencer->PanelTimeLine->ZoomIn();
    }
}

void xLightsFrame::OnAuiToolBarItem_ZoomOutClick(wxCommandEvent& event)
{
    if (Notebook1->GetSelection() == NEWSEQUENCER)
    {
        mainSequencer->PanelTimeLine->ZoomOut();
    }
}


void xLightsFrame::OnMenuItem_File_Open_SequenceSelected(wxCommandEvent& event)
{
    OpenSequence("", nullptr);
}

void xLightsFrame::OnMenuItem_File_SaveAs_SequenceSelected(wxCommandEvent& event)
{
    SaveAsSequence();
}

void xLightsFrame::OnMenuItem_File_Close_SequenceSelected(wxCommandEvent& event)
{
    static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));
    logger_base.info("Closing sequence.");
	CloseSequence();
	logger_base.info("Sequence closed.");

    // force refreshes since grid has been cleared
    mainSequencer->PanelTimeLine->RaiseChangeTimeline();
    wxCommandEvent eventRowHeaderChanged(EVT_ROW_HEADINGS_CHANGED);
    wxPostEvent(this, eventRowHeaderChanged);
}

void xLightsFrame::OnMenuItem_File_Export_VideoSelected(wxCommandEvent& event)
{
	int frameCount = SeqData.NumFrames();

	if (CurrentSeqXmlFile == nullptr || frameCount == 0)
		return;

    bool visible = m_mgr->GetPane("HousePreview").IsShown();
    if (!visible)
    {
        m_mgr->GetPane("HousePreview").Show();
        m_mgr->Update();
    }

    ModelPreview *housePreview = _housePreviewPanel->GetModelPreview();
    if (housePreview == nullptr)
        return;

    const char wildcard[] = "MP4 files (*.mp4)|*.mp4";
	wxFileDialog *pExportDlg = new wxFileDialog(this, _("Export House Preview Video"), wxEmptyString, CurrentSeqXmlFile->GetName(), wildcard, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
   int exportChoice = pExportDlg->ShowModal();

   if (exportChoice != wxID_OK)
   {
      delete pExportDlg;
      return;
   }

	wxString path( pExportDlg->GetPath() );
   delete pExportDlg;

   int playStatus = mainSequencer->GetPlayStatus();
   mainSequencer->SetPlayStatus( PLAY_TYPE_STOPPED );

	static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));
	logger_base.debug("Writing house-preview video to %s.", (const char *)path.c_str());

	int width = housePreview->getWidth();
	int height = housePreview->getHeight();
	double contentScaleFactor = GetContentScaleFactor();

	int audioChannelCount = 0;
	int audioSampleRate = 0;
	AudioManager *audioMgr = CurrentSeqXmlFile->GetMedia();
	if (audioMgr != nullptr)
	{
		audioSampleRate = audioMgr->GetRate();
		audioChannelCount = audioMgr->GetChannels();
	}
	int audioFrameIndex = 0;

	VideoExporter videoExporter(this, width, height, contentScaleFactor, SeqData.FrameTime(), SeqData.NumFrames(), audioChannelCount, audioSampleRate, logger_base);

	videoExporter.SetGetAudioFrameCallback(
		[audioMgr, &audioFrameIndex](float *samples, int frameSize, int numChannels) {
		   int trackSize = audioMgr->GetTrackSize();
			int clampedSize = std::min(frameSize, trackSize - audioFrameIndex );

			const float *leftptr = audioMgr->GetLeftDataPtr(audioFrameIndex);
			const float *rightptr = audioMgr->GetRightDataPtr(audioFrameIndex);

			memcpy(samples, leftptr, clampedSize * sizeof(float));
			samples += clampedSize;
			memcpy(samples, rightptr, clampedSize * sizeof(float));

			audioFrameIndex += frameSize;
			return true;
	   }
	);

   xlGLCanvas::CaptureHelper captureHelper( width, height, contentScaleFactor );

	videoExporter.SetGetVideoFrameCallback(
		[this, housePreview, &captureHelper](unsigned char *buf, int bufSize, int width, int height, float scaleFactor, unsigned frameIndex) {
			const FrameData frameData = this->SeqData[frameIndex];
			const unsigned char *data = frameData[0];
         //logger_base.debug( "Requesting house-preview frame %d for video-export", frameIndex );
			housePreview->Render(data, false);
         //logger_base.debug( "Received house-preview frame %d for video-export", frameIndex );
			bool convertStatus = captureHelper.ToRGB(buf, bufSize, true);
         //logger_base.debug( " CaptureHelper RGB conversion %s", convertStatus ? "successful" : "failed" );
         return convertStatus;
		}
	);

   captureHelper.SetActive( true );
	bool exportStatus = videoExporter.Export(path.ToStdString().c_str());
	captureHelper.SetActive(false);

   mainSequencer->SetPlayStatus( playStatus );

    if (!visible)
    {
        m_mgr->GetPane("HousePreview").Hide();
        m_mgr->Update();
    }

    if ( exportStatus )
    {
       logger_base.debug( "Finished writing house-preview video." );
    }
    else
    {
       logger_base.error( "Exporting house-preview video failed!" );
       wxMessageBox( _( "Exporting house preview video failed" ), _( "Export failure" ), wxOK | wxCENTRE, this );
    }
}

void xLightsFrame::OnResize(wxSizeEvent& event)
{
}

void xLightsFrame::OnAuiToolBarItemRenderAllClick(wxCommandEvent& event)
{
    RenderAll();
}

bool AUIToolbarButtonWrapper::IsChecked()
{
    return toolbar->GetToolToggled(id);
}
void AUIToolbarButtonWrapper::SetValue(bool b)
{
    toolbar->ToggleTool(id, b);
}
void AUIToolbarButtonWrapper::SetBitmap(const wxBitmap &bmp)
{
    toolbar->SetToolBitmap(id,bmp);
    toolbar->Refresh();
}

void AUIToolbarButtonWrapper::Enable(bool b)
{
    toolbar->EnableTool(id, b);
}

void xLightsFrame::SetIconSize(wxCommandEvent& event)
{
    int size = 48;
    if (event.GetId() == ID_MENUITEM_GRID_ICON_XSMALL)
    {
        size = 10;
    }
    else if (event.GetId() == ID_MENUITEM_GRID_ICON_SMALL)
    {
        size = 16;
    }
    else if (event.GetId() == ID_MENUITEM_GRID_ICON_MEDIUM)
    {
        size = 24;
    }
    else if (event.GetId() == ID_MENUITEM_GRID_ICON_LARGE)
    {
        size = 32;
    }
    mGridSpacing = size;

    DEFAULT_ROW_HEADING_HEIGHT = size + 6;
    mainSequencer->PanelRowHeadings->Refresh();
    mainSequencer->PanelEffectGrid->Refresh();
    GridSpacingMenu->Check(event.GetId(), true);
}

void xLightsFrame::ResetToolbarLocations(wxCommandEvent& event)
{
    wxMessageBox("Toolbar locations will reset to defaults upon restart.");
    mResetToolbars = true;
}

void xLightsFrame::SetToolIconSize(wxCommandEvent& event)
{
    int size = 48;
    if (event.GetId() == ID_MENUITEM_ICON_SMALL)
    {
        size = 16;
    }
    else if (event.GetId() == ID_MENUITEM_ICON_MEDIUM)
    {
        size = 24;
    }
    else if (event.GetId() == ID_MENUITEM_ICON_LARGE)
    {
        size = 32;
    }

    mIconSize = size;

    for (size_t x = 0; x < EffectsToolBar->GetToolCount(); x++)
    {
        EffectsToolBar->FindToolByIndex(x)->GetWindow()->SetSizeHints(size, size, size, size);
        EffectsToolBar->FindToolByIndex(x)->SetMinSize(EffectsToolBar->FindToolByIndex(x)->GetWindow()->GetMinSize());
    }
    EffectsToolBar->Realize();
    wxSize sz = EffectsToolBar->GetSize();
    wxAuiPaneInfo &info = MainAuiManager->GetPane("EffectsToolBar");
    info.BestSize(sz);
    MainAuiManager->Update();

    const wxWindowList& lst =effectPalettePanel->GetChildren();
    for (size_t x = 0; x < lst.size(); x++)
    {
        lst[x]->SetSizeHints(size, size, size, size);
    }
    effectPalettePanel->Layout();
    ToolIconSizeMenu->Check(event.GetId(), true);

    effectsPnl->BitmapButtonSelectedEffect->SetSizeHints(size, size, size, size);
    effectsPnl->Layout();
}

void xLightsFrame::OnMenuItemRenderEraseModeSelected(wxCommandEvent& event)
{
    MenuItemRenderEraseMode->Check(true);
    MenuItemRenderCanvasMode->Check(false);
    CurrentSeqXmlFile->SetRenderMode(xLightsXmlFile::ERASE_MODE);
}

void xLightsFrame::OnMenuItemRenderCanvasModeSelected(wxCommandEvent& event)
{
    MenuItemRenderEraseMode->Check(false);
    MenuItemRenderCanvasMode->Check(true);
    CurrentSeqXmlFile->SetRenderMode(xLightsXmlFile::CANVAS_MODE);
}

void xLightsFrame::UpdateRenderMode()
{
    if( CurrentSeqXmlFile->GetRenderMode() == xLightsXmlFile::CANVAS_MODE )
    {
        MenuItemRenderEraseMode->Check(false);
        MenuItemRenderCanvasMode->Check(true);
    }
    else
    {
        MenuItemRenderEraseMode->Check(true);
        MenuItemRenderCanvasMode->Check(false);
    }
}

void xLightsFrame::SetFrequency(int frequency)
{
    mSequenceElements.SetFrequency(frequency);
    mainSequencer->PanelTimeLine->SetTimeFrequency(frequency);
    mainSequencer->PanelWaveForm->SetTimeFrequency(frequency);
}

void xLightsFrame::OnSetGridIconBackground(wxCommandEvent& event)
{
    if (event.GetId() == ID_MENUITEM_GRID_ICON_BACKGROUND_ON)
    {
        mGridIconBackgrounds = true;
    }
    else if (event.GetId() == ID_MENUITEM_GRID_ICON_BACKGROUND_OFF)
    {
        mGridIconBackgrounds = false;
    }
    MenuItemGridIconBackgroundOn->Check(mGridIconBackgrounds);
    MenuItemGridIconBackgroundOff->Check(!mGridIconBackgrounds);
    mainSequencer->PanelEffectGrid->SetEffectIconBackground(mGridIconBackgrounds);
    mainSequencer->PanelEffectGrid->Refresh();
}

void xLightsFrame::OnSetGridNodeValues(wxCommandEvent& event)
{
    if (event.GetId() == ID_MENUITEM_GRID_NODE_VALUES_ON)
    {
        mGridNodeValues = true;
    }
    else if (event.GetId() == ID_MENUITEM_GRID_NODE_VALUES_OFF)
    {
        mGridNodeValues = false;
    }
    MenuItemGridNodeValuesOn->Check(mGridNodeValues);
    MenuItemGridNodeValuesOff->Check(!mGridNodeValues);
    mainSequencer->PanelEffectGrid->SetEffectNodeValues(mGridNodeValues);
    mainSequencer->PanelEffectGrid->Refresh();
}

void xLightsFrame::SetPlaySpeed(wxCommandEvent& event)
{
    playSpeed = 1.0;
    playAnimation = false;
    if (event.GetId() == ID_PLAY_FULL)
    {
        playSpeed = 1.0;
    }
    else if (event.GetId() == ID_PLAY_3_4)
    {
        playSpeed = 0.75;
    }
    else if (event.GetId() == ID_PLAY_1_2)
    {
        playSpeed = 0.5;
    }
    else if (event.GetId() == ID_PLAY_1_4)
    {
        playSpeed = 0.25;
    }
    else if (event.GetId() == ID_MNU_1POINT5SPEED)
    {
        playSpeed = 1.5;
    }
    else if (event.GetId() == ID_MN_2SPEED)
    {
        playSpeed = 2.0;
    }
    else if (event.GetId() == ID_MNU_3SPEED)
    {
        playSpeed = 3.0;
    }
    else if (event.GetId() == ID_MNU_4SPEED)
    {
        playSpeed = 4.0;
    }
    AudioManager::SetPlaybackRate(playSpeed);
	if (CurrentSeqXmlFile != nullptr)
	{
	    if( CurrentSeqXmlFile->GetMedia() == nullptr )
        {
            playAnimation = true;
        }
	}
}

void xLightsFrame::OnBitmapButton_Link_DirsClick(wxCommandEvent& event)
{
    wxConfigBase* config = wxConfigBase::Get();
    long LinkFlag=0;
    config->Read(_("LinkFlag"), &LinkFlag);
    if( LinkFlag )
    {
        LinkFlag = 0;
        BitmapButton_Link_Dirs->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlART_UNLINK")),wxART_OTHER));
        Button_Change_Media_Dir->Enable(true);
        BitmapButton_Link_Dirs->SetToolTip("Link Directories");
    }
    else
    {
        LinkFlag = 1;
        BitmapButton_Link_Dirs->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlART_LINK")),wxART_OTHER));
        Button_Change_Media_Dir->Enable(false);
        mediaDirectory = CurrentDir;
        config->Write(_("MediaDir"), mediaDirectory);
        MediaDirectoryLabel->SetLabel(mediaDirectory);
        MediaDirectoryLabel->GetParent()->Layout();
        static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));
        logger_base.debug("Media directory set to : %s.", (const char *)mediaDirectory.c_str());
        BitmapButton_Link_Dirs->SetToolTip("Unlink Directories");
    }
    config->Write(_("LinkFlag"), LinkFlag);
}

void xLightsFrame::OnMenuItemRenderOnSave(wxCommandEvent& event)
{
    mRenderOnSave = event.IsChecked();
}

void xLightsFrame::OnMenuItemEffectAssistAlwaysOnSelected(wxCommandEvent& event)
{
    MenuItemEffectAssistAlwaysOn->Check(true);
    MenuItemEffectAssistAlwaysOff->Check(false);
    MenuItemEffectAssistToggleMode->Check(false);
    mEffectAssistMode = EFFECT_ASSIST_ALWAYS_ON;
    SetEffectAssistWindowState(true);
}

void xLightsFrame::OnMenuItemEffectAssistAlwaysOffSelected(wxCommandEvent& event)
{
    MenuItemEffectAssistAlwaysOn->Check(false);
    MenuItemEffectAssistAlwaysOff->Check(true);
    MenuItemEffectAssistToggleMode->Check(false);
    mEffectAssistMode = EFFECT_ASSIST_ALWAYS_OFF;
    SetEffectAssistWindowState(false);
}

void xLightsFrame::OnMenuItemEffectAssistToggleModeSelected(wxCommandEvent& event)
{
    MenuItemEffectAssistAlwaysOn->Check(false);
    MenuItemEffectAssistAlwaysOff->Check(false);
    MenuItemEffectAssistToggleMode->Check(true);
    mEffectAssistMode = EFFECT_ASSIST_TOGGLE_MODE;
}

void xLightsFrame::SetEffectAssistWindowState(bool show)
{
    bool visible = m_mgr->GetPane("EffectAssist").IsShown();
    if (visible && !show)
    {
        m_mgr->GetPane("EffectAssist").Hide();
        m_mgr->Update();
    }
    else if(!visible && show)
    {
        m_mgr->GetPane("EffectAssist").Show();
        m_mgr->Update();
    }
}

void xLightsFrame::UpdateEffectAssistWindow(Effect* effect, RenderableEffect* ren_effect)
{
    if( effect == nullptr || ren_effect == nullptr )
    {
        sEffectAssist->SetPanel(nullptr);
        return;
    }

    bool effect_is_supported = ren_effect->HasAssistPanel();

    if( mEffectAssistMode == EFFECT_ASSIST_TOGGLE_MODE )
    {
        if( effect_is_supported )
        {
            SetEffectAssistWindowState(true);
        }
        else
        {
            SetEffectAssistWindowState(false);
        }
    }

    AssistPanel *panel;
    if (effect_is_supported) {
        panel = ren_effect->GetAssistPanel(sEffectAssist, this);
    } else {
        panel = sEffectAssist->GetDefaultAssistPanel();
    }
    panel->SetEffectInfo(effect, this);
    sEffectAssist->SetPanel(panel);
}

void xLightsFrame::CheckUnsavedChanges()
{
    if (UnsavedRgbEffectsChanges)
    {
        // This is not necessary but it shows the user that the save button is red which I am hoping makes it clearer
        // to the user what this prompt is for
        Notebook1->SetSelection(LAYOUTTAB);

        if (wxYES == wxMessageBox("Save Models, Views, Perspectives, and Preset changes?",
            "RGB Effects File Changes Confirmation", wxICON_QUESTION | wxYES_NO | wxNO_DEFAULT))
        {
            SaveEffectsFile();
        }
        else
        {
            wxFileName effectsFile;
            effectsFile.AssignDir(CurrentDir);
            effectsFile.SetFullName(_(XLIGHTS_RGBEFFECTS_FILE));
            wxFileName fn(effectsFile.GetFullPath());
            if (wxFile::Exists(fn.GetFullPath()))
            {
                fn.Touch();
            }
        }
    }

    if (UnsavedNetworkChanges)
    {
        // This is not necessary but it shows the user that the save button is red which I am hoping makes it clearer
        // to the user what this prompt is for
        Notebook1->SetSelection(SETUPTAB);

        if (wxYES == wxMessageBox("Save Network Setup changes?",
            "Networks Changes Confirmation", wxICON_QUESTION | wxYES_NO | wxNO_DEFAULT))
        {
            SaveNetworksFile();
        }
    }
}

void xLightsFrame::MarkEffectsFileDirty(bool modelStructureChange)
{
    layoutPanel->SetDirtyHiLight(true);
    UnsavedRgbEffectsChanges=true;
    if (modelStructureChange) {
        modelsChangeCount++;
    }
}

unsigned int xLightsFrame::GetMaxNumChannels() {
    return std::max(_outputManager.GetTotalChannels(), (long)AllModels.GetLastChannel() + 1);
}

void xLightsFrame::UpdateSequenceLength()
{
    if( CurrentSeqXmlFile->GetSequenceLoaded() )
    {
        wxString mss = CurrentSeqXmlFile->GetSequenceTiming();
        int ms = wxAtoi(mss);

        SeqData.init(GetMaxNumChannels(), CurrentSeqXmlFile->GetSequenceDurationMS() / ms, ms);
        mSequenceElements.IncrementChangeCount(nullptr);

        mainSequencer->PanelTimeLine->SetTimeLength(CurrentSeqXmlFile->GetSequenceDurationMS());
        mainSequencer->PanelTimeLine->Initialize();
        int maxZoom = mainSequencer->PanelTimeLine->GetMaxZoomLevel();
        mainSequencer->PanelTimeLine->SetZoomLevel(maxZoom);
        mainSequencer->PanelWaveForm->SetZoomLevel(maxZoom);
        mainSequencer->PanelTimeLine->RaiseChangeTimeline();
        mainSequencer->PanelWaveForm->UpdatePlayMarker();
    }
}

void xLightsFrame::OnActionTestMenuItemSelected(wxCommandEvent& event)
{
	// save the media playing state and stop it if it is playing
	MEDIAPLAYINGSTATE mps = MEDIAPLAYINGSTATE::STOPPED;
	if (CurrentSeqXmlFile != nullptr && CurrentSeqXmlFile->GetMedia() != nullptr)
	{
		mps = CurrentSeqXmlFile->GetMedia()->GetPlayingState();
		if (mps == MEDIAPLAYINGSTATE::PLAYING)
		{
			CurrentSeqXmlFile->GetMedia()->Pause();
			SetAudioControls();
		}
	}

	Timer1.Stop();

	// save the output state and turn it off
	bool output = CheckBoxLightOutput->IsChecked();
	if (output)
	{
        _outputManager.AllOff();
        CheckBoxLightOutput->SetValue(false);
        EnableOutputs();
	}

	// creating the dialog can take some time so display an hourglass
	SetCursor(wxCURSOR_WAIT);

	// display the test dialog
    PixelTestDialog dialog(this, &_outputManager, networkFile, &AllModels);
    dialog.CenterOnParent();
    dialog.ShowModal();

	SetCursor(wxCURSOR_DEFAULT);

	// resume output if it was set
	if (output)
	{
        CheckBoxLightOutput->SetValue(true);
        EnableOutputs();
	}

	Timer1.Start();

	// resume playing the media if it was playing
	if (mps == MEDIAPLAYINGSTATE::PLAYING)
	{
		CurrentSeqXmlFile->GetMedia()->Play();
		SetAudioControls();
	}
}

void xLightsFrame::OnAuiToolBarItemPasteByTimeClick(wxCommandEvent& event)
{
    ButtonPasteByTime->SetValue(true);
    ButtonPasteByCell->SetValue(false);
    mainSequencer->SetPasteByCell(false);
}

void xLightsFrame::OnAuiToolBarItemPasteByCellClick(wxCommandEvent& event)
{
    ButtonPasteByTime->SetValue(false);
    ButtonPasteByCell->SetValue(true);
    mainSequencer->SetPasteByCell(true);
}

void xLightsFrame::OnMenuItemConvertSelected(wxCommandEvent& event)
{
    UpdateChannelNames();
    ConvertDialog dialog(this, SeqData, &_outputManager, mediaFilename, ChannelNames, ChannelColors, ChNames);
    dialog.CenterOnParent();
    dialog.ShowModal();
}

void xLightsFrame::OnMenu_GenerateCustomModelSelected(wxCommandEvent& event)
{
    // save the media playing state and stop it if it is playing
    MEDIAPLAYINGSTATE mps = MEDIAPLAYINGSTATE::STOPPED;
    if (CurrentSeqXmlFile != nullptr && CurrentSeqXmlFile->GetMedia() != nullptr)
    {
        mps = CurrentSeqXmlFile->GetMedia()->GetPlayingState();
        if (mps == MEDIAPLAYINGSTATE::PLAYING)
        {
            CurrentSeqXmlFile->GetMedia()->Pause();
            SetAudioControls();
        }
    }

    Timer1.Stop();

    // save the output state and turn it off
    bool output = CheckBoxLightOutput->IsChecked();
    if (output)
    {
        _outputManager.AllOff();
        CheckBoxLightOutput->SetValue(false);
        EnableOutputs();
    }

    // creating the dialog can take some time so display an hourglass
    SetCursor(wxCURSOR_WAIT);

    GenerateCustomModelDialog dialog(this, &_outputManager);
    dialog.CenterOnParent();
    dialog.ShowModal();

    SetCursor(wxCURSOR_DEFAULT);

    // resume output if it was set
    if (output)
    {
        CheckBoxLightOutput->SetValue(true);
        EnableOutputs();
    }

    Timer1.Start();

    // resume playing the media if it was playing
    if (mps == MEDIAPLAYINGSTATE::PLAYING)
    {
        CurrentSeqXmlFile->GetMedia()->Play();
        SetAudioControls();
    }
}

void xLightsFrame::OnPaneClose(wxAuiManagerEvent& event)
{
    SetFocus();
}

void xLightsFrame::SendReport(const wxString &loc, wxDebugReportCompress &report) {
    wxHTTP http;
    http.Connect("dankulp.com");

    const char *bound = "--------------------------b29a7c2fe47b9481";

    wxDateTime now = wxDateTime::Now();
    int millis = wxGetUTCTimeMillis().GetLo() % 1000;
    wxString ts = wxString::Format("%04d-%02d-%02d_%02d-%02d-%02d-%03d", now.GetYear(), now.GetMonth()+1, now.GetDay(), now.GetHour(), now.GetMinute(), now.GetSecond(), millis);

    wxString fn = wxString::Format("xlights-%s_%s_%s_%s.zip",  wxPlatformInfo::Get().GetOperatingSystemFamilyName().c_str(), xlights_version_string, GetBitness(), ts);
    const char *ct = "Content-Type: application/octet-stream\n";
    std::string cd = "Content-Disposition: form-data; name=\"userfile\"; filename=\"" + fn.ToStdString() + "\"\n\n";

    wxMemoryBuffer memBuff;
    memBuff.AppendData(bound, strlen(bound));
    memBuff.AppendData("\n", 1);
    memBuff.AppendData(ct, strlen(ct));
    memBuff.AppendData(cd.c_str(), strlen(cd.c_str()));


    wxFile f_in(report.GetCompressedFileName());
    wxFileOffset fLen=f_in.Length();
    void* tmp=memBuff.GetAppendBuf(fLen);
    size_t iRead=f_in.Read(tmp, fLen);
    memBuff.UngetAppendBuf(iRead);
    f_in.Close();

    memBuff.AppendData("\n", 1);
    memBuff.AppendData(bound, strlen(bound));
    memBuff.AppendData("--\n", 3);

    http.SetMethod("POST");
    http.SetPostBuffer("multipart/form-data; boundary=------------------------b29a7c2fe47b9481", memBuff);
    wxInputStream * is = http.GetInputStream("/" + loc + "/index.php");
    char buf[1024];
    is->Read(buf, 1024);
    //printf("%s\n", buf);
    delete is;
    http.Close();
}

void xLightsFrame::MaybePackageAndSendDebugFiles() {
    wxString message = "You forced the OpenGL setting to a non-default value.  Is it OK to send the debug logs to the developers for analysis?";
    wxMessageDialog dlg(this, message, "Send Debug Files",wxYES_NO|wxCENTRE);
    if (dlg.ShowModal() == wxID_YES) {
        wxTextEntryDialog ted(this, "Can you briefly describe what wasn't working?\n"
                              "Also include who you are (email, forum username, etc..)", "Additional Information", "",
                              wxTE_MULTILINE|wxOK|wxCENTER);
        ted.SetSize(400, 400);
        ted.ShowModal();

        wxDebugReportCompress report;
        report.SetCompressedFileBaseName("xlights_debug");
        report.SetCompressedFileDirectory(wxFileName::GetTempDir());
        report.AddText("description", ted.GetValue(), "description");
        AddDebugFilesToReport(report);
        report.Process();
        SendReport("oglUpload", report);
        wxRemoveFile(report.GetCompressedFileName());
    }
}
void xLightsFrame::OnMenuItemPackageDebugFiles(wxCommandEvent& event)
{
    static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));

    wxLogNull logNo; //kludge: avoid "error 0" message from wxWidgets after new file is written
    wxFileDialog fd(this, "Zip file to create.", CurrentDir, "xLightsProblem.zip", "zip file(*.zip)|*.zip", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (fd.ShowModal() == wxID_CANCEL) return;

    // make sure everything is up to date
    if (Notebook1->GetSelection() != LAYOUTTAB)
        layoutPanel->UnSelectAllModels();
    RecalcModels();

    // check the curent sequence to ensure this analysis is in the log
    CheckSequence(false);

    logger_base.debug("Dumping registry configuration:");
    wxConfigBase* config = wxConfigBase::Get();
    wxString key;
    long index;
    bool ce = config->GetFirstEntry(key, index);

    while (ce)
    {
        wxString value = "<UNKNOWN>";
        wxString type = "<UNREAD>";
        switch(config->GetEntryType(key))
        {
        case wxConfigBase::EntryType::Type_String:
            type = "String";
            config->Read(key, &value);
            break;
        case wxConfigBase::EntryType::Type_Boolean:
            type = "Boolean";
            value = wxString::Format("%s", config->ReadBool(key, false) ? "True" : "False");
            break;
        case wxConfigBase::EntryType::Type_Integer: // long
            type = "Integer";
            value = wxString::Format("%ld", config->ReadLong(key, 0));
            break;
        case wxConfigBase::EntryType::Type_Float: // double
            type = "Float";
            value = wxString::Format("%f", config->ReadDouble(key, 0.0));
            break;
        case wxConfigBase::EntryType::Type_Unknown:
            type = "Unknown";
            break;
        default:
            break;
        }

        logger_base.debug("      '%s' (%s) ='%s'", (const char*)key.c_str(), (const char*)type.c_str(), (const char*)value.c_str());

        ce = config->GetNextEntry(key, index);
    }

    wxDebugReportCompress report;
    report.SetCompressedFileBaseName(wxFileName(fd.GetFilename()).GetName());
    report.SetCompressedFileDirectory(fd.GetDirectory());
    AddDebugFilesToReport(report);

    // export the models to an easy to read file
    wxString filename = wxFileName::CreateTempFileName("Models") + ".csv";
    ExportModels(filename);
    wxFileName fn(filename);
    report.AddFile(fn.GetFullPath(), "All Models");

    // export the effects to an easy to read file
    if (CurrentSeqXmlFile != nullptr)
    {
        wxString filenamee = wxFileName::CreateTempFileName("Effects") + ".csv";
        ExportEffects(filenamee);
        wxFileName fne(filenamee);
        report.AddFile(fne.GetFullPath(), "All Effects");
    }

    report.Process();

    wxRemoveFile(filename);
}

static void AddLogFile(const wxString &CurrentDir, const wxString &fileName, wxDebugReport &report) {
    wxString dir;
#ifdef __WXMSW__
    wxGetEnv("APPDATA", &dir);
    wxString filename = dir + "/" + fileName;
#endif
#ifdef __WXOSX_MAC__
    wxFileName home;
    home.AssignHomeDir();
    dir = home.GetFullPath();
    wxString filename = dir + "/Library/Logs/" + fileName;
#endif
#ifdef __LINUX__
    wxString filename = "/tmp/" + fileName;
#endif
    if (wxFile::Exists(filename))
    {
        report.AddFile(filename, fileName);
    }
    else if (wxFile::Exists(wxFileName(CurrentDir, fileName).GetFullPath()))
    {
        report.AddFile(wxFileName(CurrentDir, fileName).GetFullPath(), fileName);
    }
    else if (wxFile::Exists(wxFileName(wxGetCwd(), fileName).GetFullPath()))
    {
        report.AddFile(wxFileName(wxGetCwd(), fileName).GetFullPath(), fileName);
    }
}

void xLightsFrame::AddDebugFilesToReport(wxDebugReport &report) {


    wxFileName fn(CurrentDir, OutputManager::GetNetworksFileName());
    if (fn.Exists()) {
        report.AddFile(fn.GetFullPath(), OutputManager::GetNetworksFileName());
    }
    if (wxFileName(CurrentDir, "xlights_rgbeffects.xml").Exists()) {
        report.AddFile(wxFileName(CurrentDir, "xlights_rgbeffects.xml").GetFullPath(), "xlights_rgbeffects.xml");
    }
    if (UnsavedRgbEffectsChanges &&  wxFileName(CurrentDir, "xlights_rgbeffects.xbkp").Exists()) {
        report.AddFile(wxFileName(CurrentDir, "xlights_rgbeffects.xbkp").GetFullPath(), "xlights_rgbeffects.xbkp");
    }

    AddLogFile(CurrentDir, "xLights_l4cpp.log", report);
    //if the rolled log exists, add it to just in case it has the information we need
    AddLogFile(CurrentDir, "xLights_l4cpp.log.1", report);

    if (GetSeqXmlFileName() != "") {
        wxFileName fn2(GetSeqXmlFileName());
        if (fn2.Exists() && !fn2.IsDir()) {
            report.AddFile(GetSeqXmlFileName(), fn2.GetName());
            if (mSavedChangeCount != mSequenceElements.GetChangeCount())
            {
                wxFileName fnb(fn2.GetPath() + "/" + fn2.GetName() + ".xbkp");
                if (fnb.Exists())
                {
                    report.AddFile(fnb.GetFullPath(), fnb.GetName());
                }
            }
        }
        else
        {
            if (mSavedChangeCount != mSequenceElements.GetChangeCount())
            {
                wxFileName fnb(CurrentDir + "/" + "__.xbkp");
                if (fnb.Exists())
                {
                    report.AddFile(fnb.GetFullPath(), fnb.GetName());
                }
            }
        }
    }
    else
    {
        if (mSavedChangeCount != mSequenceElements.GetChangeCount())
        {
            wxFileName fnb(CurrentDir + "/" + "__.xbkp");
            if (fnb.Exists())
            {
                report.AddFile(fnb.GetFullPath(), fnb.GetName());
            }
        }
    }
    //report.AddAll(wxDebugReport::Context_Current);
}

void xLightsFrame::OnMenuOpenGLSelected(wxCommandEvent& event)
{
    wxConfigBase* config = wxConfigBase::Get();
    OpenGLMenu->Check(ID_MENU_OPENGL_AUTO, false);
    OpenGLMenu->Check(ID_MENU_OPENGL_3, false);
    OpenGLMenu->Check(ID_MENU_OPENGL_2, false);
    OpenGLMenu->Check(ID_MENU_OPENGL_1, false);

    if (event.GetId() == ID_MENU_OPENGL_AUTO) {
        config->Write("ForceOpenGLVer", 99);
    } else if (event.GetId() == ID_MENU_OPENGL_3) {
        config->Write("ForceOpenGLVer", 3);
    } else if (event.GetId() == ID_MENU_OPENGL_2) {
        config->Write("ForceOpenGLVer", 2);
    } else if (event.GetId() == ID_MENU_OPENGL_1) {
        config->Write("ForceOpenGLVer", 1);
    }
    OpenGLMenu->Check(event.GetId(), true);
    wxMessageBox("OpenGL changes require a restart\n");
}

void xLightsFrame::SaveWorkingLayout()
{
    // update xml with offsets and scale
    for (size_t i = 0; i < modelPreview->GetModels().size(); i++)
    {
        modelPreview->GetModels()[i]->UpdateXmlWithScale();
    }
    SaveEffectsFile(true);
}

void xLightsFrame::SaveWorking()
{
    // dont save if no file in existence
    if (CurrentSeqXmlFile == nullptr) return;

    // dont save if currently saving
    std::unique_lock<std::mutex> lock(saveLock, std::try_to_lock);
    if (!lock.owns_lock()) return;

    wxString p = CurrentSeqXmlFile->GetPath();
    wxString fn = CurrentSeqXmlFile->GetFullName();
    wxString tmp;

    if (fn == "")
    {
        tmp = p + "/" + "__.xbkp";
    }
    else
    {
        wxFileName fnp(fn);
        tmp = p + "/" + fnp.GetName() + ".xbkp";
    }
    wxFileName ftmp(tmp);

    CurrentSeqXmlFile->SetPath(ftmp.GetPath());
    CurrentSeqXmlFile->SetFullName(ftmp.GetFullName());

    CurrentSeqXmlFile->Save(mSequenceElements);

    CurrentSeqXmlFile->SetPath(p);
    CurrentSeqXmlFile->SetFullName(fn);
}

void xLightsFrame::OnTimer_AutoSaveTrigger(wxTimerEvent& event)
{
    static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));
    // dont save if currently playing or in render mode
    if (playType != PLAY_TYPE_MODEL && !_renderMode && !_suspendAutoSave) {
        logger_base.debug("Autosaving backup of sequence.");
        wxStopWatch sw;
        if (mSavedChangeCount != mSequenceElements.GetChangeCount())
        {
            if (mSequenceElements.GetChangeCount() != mLastAutosaveCount)
            {
                SaveWorking();
                mLastAutosaveCount = mSequenceElements.GetChangeCount();
            }
            else
            {
                logger_base.debug("    Autosave skipped ... no changes detected since last autosave.");
            }
        }
        else
        {
            logger_base.debug("    Autosave skipped ... no changes detected since last save.");
            mLastAutosaveCount = mSequenceElements.GetChangeCount();
        }
        if (UnsavedRgbEffectsChanges)
        {
            logger_base.debug("    Autosaving backup of layout.");
            SaveWorkingLayout();
        }
        logger_base.debug("    AutoSave took %d ms.", sw.Time());
    }
    else
    {
        logger_base.debug("AutoSave skipped because sequence is playing or batch rendering or suspended.");
    }

    if (AutoSaveInterval > 0) {
        Timer_AutoSave.StartOnce(AutoSaveInterval * 60000);
    }
}

void xLightsFrame::AutoSaveIntervalSelected(wxCommandEvent& event)
{
    wxString v = AutoSaveMenu->GetLabel(event.GetId());
    AutoSaveMenu->Check(event.GetId(), true);
    int nasi = -1;
    if (v == "Disabled") {
        Timer_AutoSave.Stop();
    } else {
        nasi = wxAtoi(v);
    }
    if (nasi != AutoSaveInterval) {
        wxConfigBase* config = wxConfigBase::Get();
        config->Write("AutoSaveInterval", nasi);
        AutoSaveInterval = nasi;
    }
    if (AutoSaveInterval > 0) {
        Timer_AutoSave.StartOnce(AutoSaveInterval * 60000);
    } else {
        Timer_AutoSave.Stop();
    }
}

void xLightsFrame::AddPreviewOption(LayoutGroup* grp)
{
    bool menu_created = false;
    if( MenuItemPreviews == nullptr ) {
        MenuItemPreviews = new wxMenu();
        MenuView->Insert(3, ID_MENU_ITEM_PREVIEWS, _("Previews"), MenuItemPreviews, wxEmptyString);
        menu_created = true;
    }
    if( LayoutGroups.size() > 1 ) {
        wxMenuItem* first_item = MenuItemPreviews->GetMenuItems().GetFirst()->GetData();
        if( first_item->GetId() != ID_MENU_ITEM_PREVIEWS_SHOW_ALL ) {
            wxMenuItem* menu_item = new wxMenuItem(MenuItemPreviews, ID_MENU_ITEM_PREVIEWS_SHOW_ALL, _("Show All"), wxEmptyString, wxITEM_CHECK);
            MenuItemPreviews->Insert(0, menu_item);
            Connect(ID_MENU_ITEM_PREVIEWS_SHOW_ALL,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::ShowHideAllPreviewWindows, nullptr, this);
        }
    }
    grp->AddToPreviewMenu(MenuItemPreviews);
    Connect(grp->GetMenuId(),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::ShowHidePreviewWindow, nullptr, this);
    if( menu_created ) {
        MenuItemPreviewSeparator = MenuView->InsertSeparator(4);
    }
}

void xLightsFrame::RemovePreviewOption(LayoutGroup* grp)
{
    Disconnect(grp->GetMenuId(),wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::ShowHidePreviewWindow, nullptr, this);
    grp->RemoveFromPreviewMenu(MenuItemPreviews);
    if( LayoutGroups.size() == 1 ) {
        Disconnect(ID_MENU_ITEM_PREVIEWS_SHOW_ALL,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xLightsFrame::ShowHideAllPreviewWindows, nullptr, this);
        MenuView->Delete(ID_MENU_ITEM_PREVIEWS);
        MenuItemPreviews = nullptr;
        MenuView->Delete(MenuItemPreviewSeparator);
    }
}

void xLightsFrame::ShowHidePreviewWindow(wxCommandEvent& event)
{
    wxMenuItem* item = MenuItemPreviews->FindItem(event.GetId());
    for (auto it = LayoutGroups.begin(); it != LayoutGroups.end(); ++it) {
        LayoutGroup* grp = (LayoutGroup*)(*it);
        if (grp != nullptr) {
            if( grp->GetMenuItem() == item ) {
                grp->ShowPreview(item->IsChecked());
            }
        }
    }
}

void xLightsFrame::ShowHideAllPreviewWindows(wxCommandEvent& event)
{
    wxMenuItem* first_item = MenuItemPreviews->GetMenuItems().GetFirst()->GetData();
    for (auto it = LayoutGroups.begin(); it != LayoutGroups.end(); ++it) {
        LayoutGroup* grp = (LayoutGroup*)(*it);
        if (grp != nullptr) {
            grp->ShowPreview(first_item->IsChecked());
        }
    }
}

void xLightsFrame::OnmBackupOnSaveSelected(wxCommandEvent& event)
{
    mBackupOnSave = event.IsChecked();
}

void xLightsFrame::OnmAltBackupLocationMenuItemSelected(wxCommandEvent& event)
{
    wxDirDialog dir(this, _("Select alternate backup directory"), wxEmptyString, wxDD_DEFAULT_STYLE, wxDefaultPosition, wxDefaultSize, _T("wxDirDialog"));
    if (dir.ShowModal() == wxID_OK)
    {
        mAltBackupDir = dir.GetPath();
        static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));
        logger_base.info("Alternate backup location set to %s.", (const char *)mAltBackupDir.c_str());
    }

    if (wxDir::Exists(mAltBackupDir))
    {
        mAltBackupMenuItem->SetHelp(mAltBackupDir);
    }
    else
    {
        mAltBackupMenuItem->SetHelp("");
    }
}

void xLightsFrame::DoAltBackup(bool prompt)
{

    wxString folderName;
    time_t cur;
    time(&cur);
    wxFileName newDirH;
    wxDateTime curTime(cur);

    //  first make sure there is a Backup sub directory
    wxString newDirBackup = mAltBackupDir + wxFileName::GetPathSeparator() + "Backup";
    if (!wxDirExists(newDirBackup) && !newDirH.Mkdir(newDirBackup))
    {
        wxMessageBox("Unable to create directory Backup!", "Error", wxICON_ERROR | wxOK);
        return;
    }

    wxString newDir = mAltBackupDir + wxFileName::GetPathSeparator() + wxString::Format(
        "Backup%c%s-%s", wxFileName::GetPathSeparator(),
        curTime.FormatISODate(), curTime.Format("%H%M%S"));

    if (prompt)
    {
        if (wxNO == wxMessageBox("All xml files under " + wxString::Format("%i", MAXBACKUPFILE_MB) + "MB in your xlights directory will be backed up to \"" +
            newDir + "\". Proceed?", "Backup", wxICON_QUESTION | wxYES_NO))
        {
            return;
        }
    }

    if (!newDirH.Mkdir(newDir))
    {
        wxMessageBox("Unable to create directory!", "Error", wxICON_ERROR | wxOK);
        return;
    }

    BackupDirectory(CurrentDir, newDir, newDir, false);
}

void xLightsFrame::OnmAltBackupMenuItemSelected(wxCommandEvent& event)
{
    if (mAltBackupDir == "")
    {
        OnmAltBackupLocationMenuItemSelected(event);
    }

    if (mAltBackupDir == "")
    {
        return;
    }

    SaveWorking();

    DoAltBackup();
}

void xLightsFrame::ExportModels(wxString filename)
{
    wxFile f(filename);

    if (!f.Create(filename, true) || !f.IsOpened())
    {
        wxMessageBox(wxString::Format("Unable to create file %s. Error %d\n", filename, f.GetLastError()), _("Export Error"));
        return;
    }

    // make sure everything is up to date
    if (Notebook1->GetSelection() != LAYOUTTAB)
        layoutPanel->UnSelectAllModels();
    RecalcModels();

    long minchannel = 99999999;
    long maxchannel = -1;

    f.Write(_("Model Name,Description,Display As,String Type,String Count,Node Count,Light Count,Est Current (Amps),Channels Per Node, Channel Count,Start Channel,Start Channel No,End Channel No,Preview,Controller Connection,Controller Type,Controller Description,Output,IP,Universe,Controller Channel,Inactive\n"));

    for (auto m = AllModels.begin(); m != AllModels.end(); ++m)
    {
        Model* model = m->second;
        if (model->GetDisplayAs() == "ModelGroup")
        {
            ModelGroup* mg = static_cast<ModelGroup*>(model);
            std::string models;
            for (auto it = mg->ModelNames().begin(); it != mg->ModelNames().end(); ++it)
            {
                if (models == "")
                {
                    models = *it;
                }
                else
                {
                    models += ", " + *it;
                }
            }
            f.Write(wxString::Format("\"%s\",\"%s\",\"%s\",,,,,,,%d,,%d,%d,%s,,,,,,,\n",
                model->name,
                models.c_str(), // No description ... use list of models
                model->GetDisplayAs(),
                model->GetChanCount(),
                model->NodeStartChannel(0) + 1,
                model->NodeStartChannel(0) + 1 + model->GetChanCount() - 1,
                model->GetLayoutGroup()
            ));
        }
        else
        {
            wxString stch = model->GetModelXml()->GetAttribute("StartChannel", wxString::Format("%d?", model->NodeStartChannel(0) + 1)); //NOTE: value coming from model is probably not what is wanted, so show the base ch# instead
            int ch = model->GetNumberFromChannelString(model->ModelStartChannel);
            if (ch == 0) ch = 1;
            std::string type, description, ip, universe, inactive;
            long channeloffset;
            int output;
            GetControllerDetailsForChannel(ch, type, description, channeloffset, ip, universe, inactive, output);

            std::string current = "";

            wxString stype = wxString(model->GetStringType());

            long lightcount = (long)(model->GetNodeCount() * model->GetLightsPerNode());
            if (!stype.Contains("Node"))
            {
                if (model->GetNodeCount() == 1)
                {
                    lightcount = model->GetCoordCount(0);
                }
                else
                {
                    lightcount = model->NodesPerString() * model->GetLightsPerNode();
                }
            }

            if (stype.Contains("Node") || stype.Contains("Channel RGB"))
            {
                current = wxString::Format("%0.2f", (float)lightcount * 0.06).ToStdString();
            }

            f.Write(wxString::Format("\"%s\",\"%s\",\"%s\",\"%s\",%li,%li,%li,%s,%i,%li,%s,%i,%i,%s,%s,%s,\"%s\",%i,%s,%s,%li,%s\n",
                model->name,
                model->description,
                model->GetDisplayAs(),
                model->GetStringType(),
                (long)model->GetNumPhysicalStrings(),
                (long)model->GetNodeCount(),
                lightcount,
                current,
                model->GetChanCountPerNode(),
                (long)model->GetActChanCount(),
                stch,
                ch,
                model->GetLastChannel() + 1,
                model->GetLayoutGroup(),
                model->GetControllerConnection(),
                type,
                description,
                output,
                ip,
                universe,
                channeloffset,
                inactive));
            if (ch < minchannel)
            {
                minchannel = ch;
            }
            if (ch + model->GetChanCount() - 1 > maxchannel)
            {
                maxchannel = ch + model->GetChanCount() - 1;
            }
        }
    }

    int bulbs = 0;
    int usedchannels = 0;
    if (minchannel == 99999999)
    {
        // No channels so we dont do this
        minchannel = 0;
        maxchannel = 0;
    }
    else
    {
        int* chused = (int*)malloc((maxchannel - minchannel + 1) * sizeof(int));
        memset(chused, 0x00, (maxchannel - minchannel + 1) * sizeof(int));

        for (auto m = AllModels.begin(); m != AllModels.end(); m++)
        {
            Model* model = m->second;
            if (model->GetDisplayAs() != "ModelGroup")
            {
                wxString stch = model->GetModelXml()->GetAttribute("StartChannel", wxString::Format("%d?", model->NodeStartChannel(0) + 1)); //NOTE: value coming from model is probably not what is wanted, so show the base ch# instead
                int ch = model->GetNumberFromChannelString(model->ModelStartChannel);
                int endch = ch + model->GetChanCount() - 1;

                int uniquechannels = 0;
                for (int i = ch; i <= endch; i++)
                {
                    if (chused[i - minchannel] == 0)
                    {
                        uniquechannels++;
                    }
                    chused[i - minchannel]++;
                }

                if (wxString(model->GetStringType()).StartsWith("Single Color"))
                {
                    bulbs += uniquechannels * model->GetCoordCount(0);
                }
                else if (wxString(model->GetStringType()).StartsWith("3 Channel"))
                {
                    bulbs += uniquechannels * model->GetNodeCount() / 3 * model->GetCoordCount(0);
                }
                else if (wxString(model->GetStringType()).StartsWith("4 Channel"))
                {
                    bulbs += uniquechannels * model->GetNodeCount() / 4 * model->GetCoordCount(0);
                }
                else if (wxString(model->GetStringType()).StartsWith("Strobes"))
                {
                    bulbs += uniquechannels * model->GetNodeCount() * model->GetCoordCount(0);
                }
                else
                {
                    int den = model->GetChanCountPerNode();
                    if (den == 0) den = 1;
                    bulbs += uniquechannels / den * model->GetLightsPerNode();
                }
            }
        }

        for (int i = 0; i < (maxchannel - minchannel + 1); i++)
        {
            if (chused[i] > 0)
            {
                usedchannels++;
            }
        }

        free(chused);
    }

    f.Write("\n");

    f.Write(wxString::Format("\"Model Count\",%d\n", AllModels.size()));
    f.Write(wxString::Format("\"First Used Channel\",%ld\n", minchannel));
    f.Write(wxString::Format("\"Last Used Channel\",%ld\n", maxchannel));
    f.Write(wxString::Format("\"Actual Used Channel\",%d\n", usedchannels));
    f.Write(wxString::Format("\"Bulbs\",%d\n", bulbs));

    f.Close();
}

void xLightsFrame::OnmExportModelsMenuItemSelected(wxCommandEvent& event)
{
    wxLogNull logNo; //kludge: avoid "error 0" message from wxWidgets after new file is written
    wxString filename = wxFileSelector(_("Choose output file"), wxEmptyString, wxEmptyString, wxEmptyString, "Export files (*.csv)|*.csv", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (filename.IsEmpty()) return;

    ExportModels(filename);
}

void xLightsFrame::OnMenuItem_BackupOnLaunchSelected(wxCommandEvent& event)
{
    mBackupOnLaunch = event.IsChecked();
}

void xLightsFrame::OnMenuItem_ViewLogSelected(wxCommandEvent& event)
{
    static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));
    wxString dir;
    wxString fileName = "xLights_l4cpp.log";
#ifdef __WXMSW__
    wxGetEnv("APPDATA", &dir);
    wxString filename = dir + "/" + fileName;
#endif
#ifdef __WXOSX_MAC__
    wxFileName home;
    home.AssignHomeDir();
    dir = home.GetFullPath();
    wxString filename = dir + "/Library/Logs/" + fileName;
#endif
#ifdef __LINUX__
    wxString filename = "/tmp/" + fileName;
#endif
    wxString fn = "";
    if (wxFile::Exists(filename))
    {
        fn = filename;
    }
    else if (wxFile::Exists(wxFileName(CurrentDir, fileName).GetFullPath()))
    {
        fn = wxFileName(CurrentDir, fileName).GetFullPath();
    }
    else if (wxFile::Exists(wxFileName(wxGetCwd(), fileName).GetFullPath()))
    {
        fn = wxFileName(wxGetCwd(), fileName).GetFullPath();
    }

    wxFileType *ft = wxTheMimeTypesManager->GetFileTypeFromExtension("txt");
    if (fn != "" && ft)
    {
        wxString command = ft->GetOpenCommand("foo.txt");
        command.Replace("foo.txt", fn);

        logger_base.debug("Viewing log file %s.", (const char *)fn.c_str());

        wxUnsetEnv("LD_PRELOAD");
        wxExecute(command);
        delete ft;
    }
    else
    {
        logger_base.warn("Unable to view log file %s.", (const char *)fn.c_str());
        wxMessageBox(_("Unable to show log file."), _("Error"));
    }
}

void LogAndWrite(wxFile& f, const std::string& msg)
{
    static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));
    logger_base.debug("CheckSequence: " + msg);
    if (f.IsOpened())
    {
        f.Write(msg + "\r\n");
    }
}

// recursively check whether a start channel refers to a model in a way that creates a referencing loop
bool xLightsFrame::CheckStart(wxFile& f, const std::string& startmodel, std::list<std::string>& seen, std::string& nextmodel)
{
    Model* m = AllModels.GetModel(nextmodel);
    if (m == nullptr)
    {
        return true; // this is actually an error but we have already reported these errors
    }
    else
    {
        std::string start = m->ModelStartChannel;

        if (start[0] == '>' || start[0] == '@')
        {
            seen.push_back(nextmodel);
            size_t colon = start.find(':', 1);
            std::string reference = start.substr(1, colon - 1);

            if (std::find(seen.begin(), seen.end(), reference) != seen.end())
            {
                wxString msg = wxString::Format("    ERR: Model '%s' start channel results in a reference loop.", startmodel);
                LogAndWrite(f, msg.ToStdString());
                for (auto it = seen.begin(); it != seen.end(); ++it)
                {
                    msg = wxString::Format("       '%s' ->", *it);
                    LogAndWrite(f, msg.ToStdString());
                }
                msg = wxString::Format("       '%s'", reference);
                LogAndWrite(f, msg.ToStdString());
                return false;
            }
            else
            {
                return CheckStart(f, startmodel, seen, reference);
            }
        }
        else
        {
            // it resolves to something ok
            return true;
        }
    }
}

bool compare_modelstartchannel(const Model* first, const Model* second)
{
    int firstmodelstart = first->GetNumberFromChannelString(first->ModelStartChannel);
    int secondmodelstart = second->GetNumberFromChannelString(second->ModelStartChannel);

    return firstmodelstart < secondmodelstart;
}

void xLightsFrame::CheckSequence(bool display)
{
    static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));

    // make sure everything is up to date
    if (Notebook1->GetSelection() != LAYOUTTAB)
        layoutPanel->UnSelectAllModels();
    RecalcModels();

    int errcount = 0;
    int warncount = 0;

    wxFile f;
    wxString filename = wxFileName::CreateTempFileName("xLightsCheckSequence") + ".txt";

    if (display)
    {
        f.Open(filename, wxFile::write);
        if (!f.IsOpened())
        {
            logger_base.warn("Unable to create results file for Check Sequence. Aborted.");
            wxMessageBox(_("Unable to create results file for Check Sequence. Aborted."), _("Error"));
            return;
        }
    }

    LogAndWrite(f, "Checking sequence.");
    wxDatagramSocket *testSocket;
    wxIPV4address addr;
    wxString fullhostname = wxGetFullHostName();
    if (mLocalIP != "")
    {
        addr.Hostname(mLocalIP);
        testSocket = new wxDatagramSocket(addr, wxSOCKET_NOWAIT);
    }
    else
    {
        addr.AnyAddress();
        testSocket = new wxDatagramSocket(addr, wxSOCKET_NOWAIT);
        addr.Hostname(fullhostname);
        if (addr.IPAddress() == "255.255.255.255")
        {
            addr.Hostname(wxGetHostName());
        }
    }

    LogAndWrite(f, "");
    LogAndWrite(f, "Full host name: " + fullhostname.ToStdString());
    LogAndWrite(f, "IP Address we are outputing data from: " + addr.IPAddress().ToStdString());
    LogAndWrite(f, "If your PC has multiple network connections (such as wired and wireless) this should be the IP Address of the adapter your controllers are connected to. If it isnt your controllers may not receive output data.");
    LogAndWrite(f, "If you are experiencing this problem you may need to set the local IP address to use.");

    if (testSocket == nullptr || !testSocket->IsOk() || testSocket->Error() != wxSOCKET_NOERROR)
    {
        wxString msg("    ERR: Cannot create socket on IP address '");
        msg += addr.IPAddress();
        msg += "'. Is the network connected?    ";
        msg = msg + " Ok : " + (testSocket->IsOk() ? "TRUE" : "FALSE");
        if (testSocket != nullptr && testSocket->IsOk()) {
            msg += wxString::Format(" : Error %d : ", testSocket->LastError()) + IPOutput::DecodeError(testSocket->LastError());
        }
        LogAndWrite(f, msg.ToStdString());
        errcount++;
    }

    if (testSocket != nullptr)
    {
        delete testSocket;
    }

    int errcountsave = errcount;
    int warncountsave = warncount;

    LogAndWrite(f, "");
    LogAndWrite(f, "Working in a backup directory");

    if (ShowFolderIsInBackup(GetShowDirectory().ToStdString()))
    {
        wxString msg = wxString::Format("    ERR: Show directory is a (or is under a) backup show directory. %s.", GetShowDirectory());
        LogAndWrite(f, msg.ToStdString());
        errcount++;
    }

    if (errcount + warncount == errcountsave + warncountsave)
    {
        LogAndWrite(f, "    No problems found");
    }

    errcountsave = errcount;
    warncountsave = warncount;

    LogAndWrite(f, "");
    LogAndWrite(f, "Inactive Outputs");

    // Check for inactive outputs
    auto outputs = _outputManager.GetOutputs();
    for (auto it = outputs.begin(); it != outputs.end(); ++it)
    {
        if (!(*it)->IsEnabled())
        {
            wxString msg = wxString::Format("    WARN: Inactive output %d %s:%s:%s:%s:'%s'.",
                (*it)->GetOutputNumber(), (*it)->GetType(), (*it)->GetIP(), (*it)->GetUniverseString(), (*it)->GetCommPort(), (*it)->GetDescription());
            LogAndWrite(f, msg.ToStdString());
            warncount++;
        }
    }

    if (errcount + warncount == errcountsave + warncountsave)
    {
        LogAndWrite(f, "    No problems found");
    }
    errcountsave = errcount;
    warncountsave = warncount;

    // multiple outputs to same universe and same IP
    LogAndWrite(f, "");
    LogAndWrite(f, "Multiple outputs sending to same destination");

    std::list<std::string> used;
    outputs = _outputManager.GetOutputs();
    for (auto n = outputs.begin(); n != outputs.end(); ++n)
    {
        if ((*n)->IsIpOutput())
        {
            std::string usedval = (*n)->GetIP() + "|" + (*n)->GetUniverseString();

            if (std::find(used.begin(), used.end(), usedval) != used.end())
            {
                wxString msg = wxString::Format("    ERR: Multiple outputs being sent to the same controller '%s' (%s) and universe %s.", (const char*)(*n)->GetDescription().c_str(), (const char*)(*n)->GetIP().c_str(), (const char *)(*n)->GetUniverseString().c_str());
                LogAndWrite(f, msg.ToStdString());
                errcount++;
            }
            else
            {
                used.push_back(usedval);
            }
        }
        else if ((*n)->IsSerialOutput())
        {
            if (std::find(used.begin(), used.end(), (*n)->GetCommPort()) != used.end())
            {
                wxString msg = wxString::Format("    ERR: Multiple outputs being sent to the same comm port %s '%s' %s.", (const char *)(*n)->GetType().c_str(), (const char *)(*n)->GetCommPort().c_str(), (const char*)(*n)->GetDescription().c_str());
                LogAndWrite(f, msg.ToStdString());
                errcount++;
            }
            else
            {
                used.push_back((*n)->GetCommPort());
            }
        }
    }

    if (errcount + warncount == errcountsave + warncountsave)
    {
        LogAndWrite(f, "    No problems found");
    }
    errcountsave = errcount;
    warncountsave = warncount;

    // controllers sending to routable IP addresses
    LogAndWrite(f, "");
    LogAndWrite(f, "Invalid controller IP addresses");

    outputs = _outputManager.GetOutputs();
    for (auto n = outputs.begin(); n != outputs.end(); ++n)
    {
        if ((*n)->IsIpOutput())
        {
            if ((*n)->GetIP() != "MULTICAST")
            {
                if (!IPOutput::IsIPValid((*n)->GetIP()))
                {
                    wxString msg = wxString::Format("    WARN: IP address '%s' on controller '%s' universe %s does not look valid.", (const char*)(*n)->GetIP().c_str(), (const char*)(*n)->GetDescription().c_str(), (const char *)(*n)->GetUniverseString().c_str());
                    LogAndWrite(f, msg.ToStdString());
                    warncount++;
                }
                else
                {
                    wxArrayString ipElements = wxSplit((*n)->GetIP(), '.');
                    int ip1 = wxAtoi(ipElements[0]);
                    int ip2 = wxAtoi(ipElements[1]);
                    int ip3 = wxAtoi(ipElements[2]);
                    int ip4 = wxAtoi(ipElements[3]);

                    if (ip1 == 10)
                    {
                        if (ip2 == 255 && ip3 == 255 && ip4 == 255) {
                            wxString msg = wxString::Format("    ERR: IP address '%s' on controller '%s' universe %s is a broadcast address.", (const char*)(*n)->GetIP().c_str(), (const char*)(*n)->GetDescription().c_str(), (const char *)(*n)->GetUniverseString().c_str());
                            LogAndWrite(f, msg.ToStdString());
                            errcount++;
                        }
                        // else this is valid
                    }
                    else if (ip1 == 192 && ip2 == 168)
                    {
                        if (ip3 == 255 && ip4 == 255) {
                            wxString msg = wxString::Format("    ERR: IP address '%s' on controller '%s' universe %s is a broadcast address.", (const char*)(*n)->GetIP().c_str(), (const char*)(*n)->GetDescription().c_str(), (const char *)(*n)->GetUniverseString().c_str());
                            LogAndWrite(f, msg.ToStdString());
                            errcount++;
                        }
                        // else this is valid
                    }
                    else if (ip1 == 172 && ip2 >= 16 && ip2 <= 31)
                    {
                        // this is valid
                    }
                    else if (ip1 == 255 && ip2 == 255 && ip3 == 255 && ip4 == 255)
                    {
                        wxString msg = wxString::Format("    ERR: IP address '%s' on controller '%s' universe %s is a broadcast address.", (const char*)(*n)->GetIP().c_str(), (const char*)(*n)->GetDescription().c_str(), (const char *)(*n)->GetUniverseString().c_str());
                        LogAndWrite(f, msg.ToStdString());
                        errcount++;
                    }
                    else if (ip1 == 0)
                    {
                        wxString msg = wxString::Format("    ERR: IP address '%s' on controller '%s' universe %s not valid.", (const char*)(*n)->GetIP().c_str(), (const char*)(*n)->GetDescription().c_str(), (const char *)(*n)->GetUniverseString().c_str());
                        LogAndWrite(f, msg.ToStdString());
                        errcount++;
                    }
                    else if (ip1 >= 224 && ip1 <= 239)
                    {
                        wxString msg = wxString::Format("    ERR: IP address '%s' on controller '%s' universe %s is a multicast address.", (const char*)(*n)->GetIP().c_str(), (const char*)(*n)->GetDescription().c_str(), (const char *)(*n)->GetUniverseString().c_str());
                        LogAndWrite(f, msg.ToStdString());
                        errcount++;
                    }
                    else
                    {
                        wxString msg = wxString::Format("    WARN: IP address '%s' on controller '%s' universe %s in internet routable ... are you sure you meant to do this.", (const char*)(*n)->GetIP().c_str(), (const char*)(*n)->GetDescription().c_str(), (const char *)(*n)->GetUniverseString().c_str());
                        LogAndWrite(f, msg.ToStdString());
                        warncount++;
                    }
                }
            }
        }
    }

    if (errcount + warncount == errcountsave + warncountsave)
    {
        LogAndWrite(f, "    No problems found");
    }
    errcountsave = errcount;
    warncountsave = warncount;

    LogAndWrite(f, "");
    LogAndWrite(f, "Models spanning controllers");
    for (auto it = AllModels.begin(); it != AllModels.end(); ++it)
    {
        if (it->second->GetDisplayAs() != "ModelGroup")
        {
            long start = it->second->GetFirstChannel()+1;
            long end = it->second->GetLastChannel()+1;

            long sc;
            Output* ostart = _outputManager.GetOutput(start, sc);
            Output* oend = _outputManager.GetOutput(end, sc);

            if (ostart != nullptr && oend == nullptr)
            {
                wxString msg = wxString::Format("    WARN: Model '%s' starts on controller '%s' but ends at channel %ld which is not on a controller.", it->first, ostart->GetDescription(), end);
                LogAndWrite(f, msg.ToStdString());
                warncount++;
            }
            else if (ostart == nullptr || oend == nullptr)
            {
                wxString msg = wxString::Format("    WARN: Model '%s' is not configured for a controller.", it->first);
                LogAndWrite(f, msg.ToStdString());
                warncount++;
            }
            else if (ostart->GetType() != oend->GetType())
            {
                wxString msg = wxString::Format("    WARN: Model '%s' starts on controller '%s' of type '%s' but ends on a controller '%s' of type '%s'.", it->first, ostart->GetDescription(), ostart->GetType(), oend->GetDescription(), oend->GetType());
                LogAndWrite(f, msg.ToStdString());
                warncount++;
            }
            else if (ostart->GetIP() == "MULTICAST" && oend->GetIP() == "MULTICAST")
            {
                // ignore these
            }
            else if (ostart->GetIP() + oend->GetIP() != "")
            {
                if (ostart->GetIP() != oend->GetIP())
                {
                    wxString msg = wxString::Format("    WARN: Model '%s' starts on controller '%s' with IP '%s' but ends on a controller '%s' with IP '%s'.", it->first, ostart->GetDescription(), ostart->GetIP(), oend->GetDescription(), oend->GetIP());
                    LogAndWrite(f, msg.ToStdString());
                    warncount++;
                }
            }
            else if (ostart->GetCommPort() + oend->GetCommPort() != "")
            {
                if (ostart->GetCommPort() != oend->GetCommPort())
                {
                    wxString msg = wxString::Format("    WARN: Model '%s' starts on controller '%s' with CommPort '%s' but ends on a controller '%s' with CommPort '%s'.", it->first, ostart->GetDescription(), ostart->GetCommPort(), oend->GetDescription(), oend->GetCommPort());
                    LogAndWrite(f, msg.ToStdString());
                    warncount++;
                }
            }
        }
    }

    if (errcount + warncount == errcountsave + warncountsave)
    {
        LogAndWrite(f, "    No problems found");
    }
    errcountsave = errcount;
    warncountsave = warncount;

    LogAndWrite(f, "");
    LogAndWrite(f, "Invalid start channels");

    for (auto it = AllModels.begin(); it != AllModels.end(); ++it)
    {
        if (it->second->GetDisplayAs() != "ModelGroup")
        {
            std::string start = it->second->ModelStartChannel;

            if (start[0] == '>' || start[0] == '@')
            {
                size_t colon = start.find(':', 1);
                std::string reference = start.substr(1, colon - 1);

                if (reference == it->first)
                {
                    wxString msg = wxString::Format("    ERR: Model '%s' start channel '%s' refers to itself.", it->first, start);
                    LogAndWrite(f, msg.ToStdString());
                    errcount++;
                }
                else
                {
                    Model *m = AllModels.GetModel(reference);
                    if (m == nullptr)
                    {
                        wxString msg = wxString::Format("    ERR: Model '%s' start channel '%s' refers to non existent model '%s'.", it->first, start, reference);
                        LogAndWrite(f, msg.ToStdString());
                        errcount++;
                    }
                }
            }
            if (it->second->GetLastChannel() == (unsigned int)-1)
            {
                wxString msg = wxString::Format("    ERR: Model '%s' start channel '%s' evaluates to an illegal channel number.", it->first, start);
                LogAndWrite(f, msg.ToStdString());
                errcount++;
            }
        }
    }

    for (auto it = AllModels.begin(); it != AllModels.end(); ++it)
    {
        if (it->second->GetDisplayAs() != "ModelGroup")
        {
            std::string start = it->second->ModelStartChannel;

            if (start[0] == '>' || start[0] == '@')
            {
                std::list<std::string> seen;
                seen.push_back(it->first);
                size_t colon = start.find(':', 1);
                if (colon != std::string::npos)
                {
                    std::string reference = start.substr(1, colon - 1);
                    if (reference != it->first)
                    {
                        if (!CheckStart(f, it->first, seen, reference))
                        {
                            errcount++;
                        }
                    }
                }
                else
                {
                    wxString msg = wxString::Format("    ERR: Model '%s' start channel '%s' invalid.", it->first, start);
                    LogAndWrite(f, msg.ToStdString());
                    errcount++;
                }
            }
            else if (start[0] == '#')
            {
                size_t colon = start.find(':', 1);
                if (colon != std::string::npos)
                {
                    size_t colon2 = start.find(':',colon+1);
                    if (colon2 == -1) {
                        colon2 = colon;
                        colon = 0;
                    }
                    int universe = wxAtoi(wxString(start.substr(colon+1,colon2-1)));

                    Output* o = _outputManager.GetOutput(universe, "");

                    if (o == nullptr)
                    {
                        wxString msg = wxString::Format("    ERR: Model '%s' start channel '%s' refers to undefined universe %d.", it->first, start, universe);
                        LogAndWrite(f, msg.ToStdString());
                        errcount++;
                    }
                }
                else
                {
                    wxString msg = wxString::Format("    ERR: Model '%s' start channel '%s' invalid.", it->first, start);
                    LogAndWrite(f, msg.ToStdString());
                    errcount++;
                }
            }
            else if (start.find(':') != std::string::npos)
            {
                size_t colon = start.find(':');
                int output = wxAtoi(wxString(start.substr(0, colon)));

                auto cnt = _outputManager.GetOutputCount();

                if (output < 1 || output > cnt)
                {
                    wxString msg = wxString::Format("    ERR: Model '%s' start channel '%s' refers to undefined output %d. Only %d outputs are defined.", it->first, start, output, cnt);
                    LogAndWrite(f, msg.ToStdString());
                    errcount++;
                }
            }
        }
    }

    if (errcount + warncount == errcountsave + warncountsave)
    {
        LogAndWrite(f, "    No problems found");
    }
    errcountsave = errcount;
    warncountsave = warncount;

    LogAndWrite(f, "");
    LogAndWrite(f, "Overlapping model channels");

    // Check for overlapping channels in models
    for (auto it = AllModels.begin(); it != AllModels.end(); ++it)
    {
        if (it->second->GetDisplayAs() != "ModelGroup")
        {
            int m1start = it->second->GetNumberFromChannelString(it->second->ModelStartChannel);
            int m1end = m1start + it->second->GetChanCount() - 1;
            //int m1end = m1start + it->second->GetActChanCount() - 1; // I dont think this is a good idea

            auto it2 = it;
            ++it2;
            for (; it2 != AllModels.end(); ++it2)
            {
                if (it2->second->GetDisplayAs() != "ModelGroup")
                {
                    int m2start = it2->second->GetNumberFromChannelString(it2->second->ModelStartChannel);
                    int m2end = m2start + it2->second->GetChanCount() - 1;
                    //int m2end = m2start + it2->second->GetActChanCount() - 1; // I dont think this is a good idea

                    if (m2start <= m1end && m2end >= m1start)
                    {
                        wxString msg = wxString::Format("    WARN: Probable model overlap '%s' (%d-%d) and '%s' (%d-%d).", it->first, m1start, m1end, it2->first, m2start, m2end);
                        LogAndWrite(f, msg.ToStdString());
                        warncount++;
                    }
                }
            }
        }
    }
    if (errcount + warncount == errcountsave + warncountsave)
    {
        LogAndWrite(f, "    No problems found");
    }
    errcountsave = errcount;
    warncountsave = warncount;

    LogAndWrite(f, "");
    LogAndWrite(f, "Non contiguous channels on controller ports");

    std::map<std::string, std::list<Model*>*> modelsByPort;

    // Check for non contiguous models on the same controller connection
    for (auto it = AllModels.begin(); it != AllModels.end(); ++it)
    {
        if (it->second->GetDisplayAs() != "ModelGroup")
        {
            if (it->second->GetControllerConnection() != "")
            {
                long start = it->second->GetFirstChannel() + 1;
                long sc;
                Output* o = _outputManager.GetOutput(start, sc);

                if (o != nullptr && o->IsIpOutput())
                {
                    std::string key = o->GetIP() + it->second->GetControllerConnection();
                    if (modelsByPort.find(key) == modelsByPort.end())
                    {
                        modelsByPort[key] = new std::list<Model*>();
                    }
                    modelsByPort[key]->push_back(it->second);
                }
            }
        }
    }

    for (auto it = modelsByPort.begin(); it != modelsByPort.end(); ++it)
    {
        if (it->second->size() == 1)
        {
            // we dont need to check this one
        }
        else
        {
            it->second->sort(compare_modelstartchannel);

            auto it2 = it->second->begin();
            auto it3 = it2;
            ++it3;

            while (it3 != it->second->end())
            {
                int m1start = (*it2)->GetNumberFromChannelString((*it2)->ModelStartChannel);
                int m1end = m1start + (*it2)->GetChanCount() - 1;
                int m2start = (*it3)->GetNumberFromChannelString((*it3)->ModelStartChannel);

                if (m1end + 1 != m2start)
                {
                    long sc;
                    Output* o = _outputManager.GetOutput(m1start, sc);
                    wxString msg = wxString::Format("    ERR: Model '%s' and Model '%s' are on controller IP '%s' Output Connection '%s' but there is a gap of %d channels between them.",
                        (*it2)->GetName(),
                        (*it3)->GetName(),
                        o->GetIP(),
                        (*it2)->GetControllerConnection(),
                        m2start - m1end - 1);
                    LogAndWrite(f, msg.ToStdString());
                    errcount++;
                }

                ++it2;
                ++it3;
            }
        }
        delete it->second;
        it->second = nullptr;
    }

    if (errcount + warncount == errcountsave + warncountsave)
    {
        LogAndWrite(f, "    No problems found");
    }
    errcountsave = errcount;
    warncountsave = warncount;

    LogAndWrite(f, "");
    LogAndWrite(f, "Model nodes not allocated to layers correctly");

    for (auto it = AllModels.begin(); it != AllModels.end(); ++it)
    {
        if (it->second->GetDisplayAs() != "ModelGroup")
        {
            if (wxString(it->second->GetStringType()).EndsWith("Nodes") && !it->second->AllNodesAllocated())
            {
                wxString msg = wxString::Format("    WARN: %s model '%s' Node Count and Layer Size allocations dont match.", it->second->GetDisplayAs().c_str(), it->first);
                LogAndWrite(f, msg.ToStdString());
                warncount++;
            }
        }
    }

    if (errcount + warncount == errcountsave + warncountsave)
    {
        LogAndWrite(f, "    No problems found");
    }
    errcountsave = errcount;
    warncountsave = warncount;

    LogAndWrite(f, "");
    LogAndWrite(f, "Models with issues");

    for (auto it = AllModels.begin(); it != AllModels.end(); ++it)
    {
        std::list<std::string> warnings = it->second->CheckModelSettings();
        for (auto s = warnings.begin(); s != warnings.end(); ++s)
        {
            LogAndWrite(f, *s);
            if (s->find("WARN:") != std::string::npos)
            {
                warncount++;
            }
            else if (s->find("ERR:") != std::string::npos)
            {
                errcount++;
            }
        }
    }

    if (errcount + warncount == errcountsave + warncountsave)
    {
        LogAndWrite(f, "    No problems found");
    }
    errcountsave = errcount;
    warncountsave = warncount;

    LogAndWrite(f, "");
    LogAndWrite(f, "Model Groups containing models from different previews");

    for (auto it = AllModels.begin(); it != AllModels.end(); ++it)
    {
        if (it->second->GetDisplayAs() == "ModelGroup")
        {
            std::string mgp = it->second->GetLayoutGroup();

            ModelGroup* mg = dynamic_cast<ModelGroup*>(it->second);
            if (mg == nullptr)
            {
                // this should never happen
                logger_base.error("Model %s says it is a model group but it doesnt cast as one.", (const char *)it->second->GetName().c_str());
            }
            else
            {
                auto models = mg->ModelNames();

                for (auto it2 = models.begin(); it2 != models.end(); ++it2)
                {
                    Model* m = AllModels.GetModel(*it2);
                    if (m == nullptr)
                    {
                        // this should never happen
                        logger_base.error("Model Group %s contains non existent model %s.", (const char *)mg->GetName().c_str(), (const char *)(*it2).c_str());
                    }
                    else if (m->GetDisplayAs() != "ModelGroup")
                    {
                        // If model is in all previews dont report it as a problem
                        if (m->GetLayoutGroup() != "All Previews" && mgp != m->GetLayoutGroup())
                        {
                            wxString msg = wxString::Format("    WARN: Model Group '%s' in preview '%s' contains model '%s' which is in preview '%s'. This will cause the '%s' model to also appear in the '%s' preview.", mg->GetName(), mg->GetLayoutGroup(), m->GetName(), m->GetLayoutGroup(), m->GetName(), mg->GetLayoutGroup());
                            LogAndWrite(f, msg.ToStdString());
                            warncount++;
                        }
                    }
                }
            }
        }
    }

    if (errcount + warncount == errcountsave + warncountsave)
    {
        LogAndWrite(f, "    No problems found");
    }
    errcountsave = errcount;
    warncountsave = warncount;

    // Check for duplicate model/model group names
    LogAndWrite(f, "");
    LogAndWrite(f, "Model/Model Groups without distinct names");

    for (auto it = AllModels.begin(); it != AllModels.end(); ++it)
    {
        auto it2 = it;
        ++it2;
        for (; it2 != AllModels.end(); ++it2)
        {
            if (it->second->GetName() == it2->second->GetName())
            {
                wxString msg = wxString::Format("    ERR: Duplicate Model/Model Group Name '%s'.", it->second->GetName());
                LogAndWrite(f, msg.ToStdString());
                errcount++;
            }
        }
    }

    if (errcount + warncount == errcountsave + warncountsave)
    {
        LogAndWrite(f, "    No problems found");
    }
    errcountsave = errcount;
    warncountsave = warncount;

    // Check for model groups containing itself or other model groups
    LogAndWrite(f, "");
    LogAndWrite(f, "Model Groups containing non existent models");

    std::list<std::string> emptyModelGroups;

    for (auto it = AllModels.begin(); it != AllModels.end(); ++it)
    {
        if (it->second->GetDisplayAs() == "ModelGroup")
        {
            ModelGroup* mg = dynamic_cast<ModelGroup*>(it->second);
            if (mg != nullptr) // this should never fail
            {
                auto models = mg->ModelNames();

                int modelCount = 0;

                for (auto m = models.begin(); m != models.end(); ++m)
                {
                    Model* model = AllModels.GetModel(*m);

                    if (model == nullptr)
                    {
                        wxString msg = wxString::Format("    ERR: Model group '%s' refers to non existent model '%s'.", mg->GetName(), model->GetName());
                        LogAndWrite(f, msg.ToStdString());
                        errcount++;
                    }
                    else
                    {
                        modelCount++;
                        if (model->GetName() == mg->GetName())
                        {
                            wxString msg = wxString::Format("    ERR: Model group '%s' contains reference to itself.", mg->GetName());
                            LogAndWrite(f, msg.ToStdString());
                            errcount++;
                        }
                    }
                }
                if (modelCount == 0)
                {
                    emptyModelGroups.push_back(it->first);
                }
            }
        }
    }

    if (errcount + warncount == errcountsave + warncountsave)
    {
        LogAndWrite(f, "    No problems found");
    }
    errcountsave = errcount;
    warncountsave = warncount;

    // Check for model groups containing no valid models
    LogAndWrite(f, "");
    LogAndWrite(f, "Model Groups containing no models that exist");

    for (auto it = emptyModelGroups.begin(); it != emptyModelGroups.end(); ++it)
    {
        wxString msg = wxString::Format("    ERR: Model group '%s' contains no models.", *it);
        LogAndWrite(f, msg.ToStdString());
        errcount++;
    }

    if (errcount + warncount == errcountsave + warncountsave)
    {
        LogAndWrite(f, "    No problems found");
    }
    errcountsave = errcount;
    warncountsave = warncount;

    // Check for submodels with no nodes
    LogAndWrite(f, "");
    LogAndWrite(f, "Submodels with no nodes");

    for (auto it = AllModels.begin(); it != AllModels.end(); ++it)
    {
        if (it->second->GetDisplayAs() != "ModelGroup")
        {
            for (int i = 0; i < it->second->GetNumSubModels(); ++i)
            {
                Model* sm = it->second->GetSubModel(i);
                if (sm->GetNodeCount() == 0)
                {
                    wxString msg = wxString::Format("    ERR: Submodel '%s' contains no nodes.", sm->GetFullName());
                    LogAndWrite(f, msg.ToStdString());
                    errcount++;
                }
            }
        }
    }

    if (errcount + warncount == errcountsave + warncountsave)
    {
        LogAndWrite(f, "    No problems found");
    }
    errcountsave = errcount;
    warncountsave = warncount;

    // Check for large blocks of unused channels
    LogAndWrite(f, "");
    LogAndWrite(f, "Large blocks of unused channels that bloats memory usage and the the fseq file.");

    std::list<Model*> modelssorted;
    for (auto it = AllModels.begin(); it != AllModels.end(); ++it)
    {
        if (it->second->GetDisplayAs() != "ModelGroup")
        {
            modelssorted.push_back(it->second);
        }
    }

    modelssorted.sort(compare_modelstartchannel);

    long last = 0;
    Model* lastm = nullptr;
    for (auto m = modelssorted.begin(); m != modelssorted.end(); ++m)
    {
        long start = (*m)->GetNumberFromChannelString((*m)->ModelStartChannel);
        long gap = start - last - 1;
        if (gap > 511) // 511 is the maximum acceptable gap ... at that point the user has wasted an entire universe
        {
            wxString level = "WARN";
            if (gap > 49999) // anyything 50,000 or greater should be an error
            {
                level = "ERR";
                errcount++;
            }
            else
            {
                warncount++;
            }
            wxString msg;
            if (lastm == nullptr)
            {
                msg = wxString::Format("    %s: First Model '%s' starts at channel %ld leaving a block of %ld of unused channels.", level, (*m)->GetName(), start, start - 1);
            }
            else
            {
                msg = wxString::Format("    %s: Model '%s' starts at channel %ld leaving a block of %ld of unused channels between this and the prior model '%s'.", level, (*m)->GetName(), start, gap, lastm->GetName());
            }
            LogAndWrite(f, msg.ToStdString());
        }
        long newlast = start + (*m)->GetChanCount() - 1;
        if (newlast > last)
        {
            last = newlast;
            lastm = *m;
        }
    }
    if (errcount + warncount == errcountsave + warncountsave)
    {
        LogAndWrite(f, "    No problems found");
    }
    errcountsave = errcount;
    warncountsave = warncount;

    if (CurrentSeqXmlFile != nullptr)
    {
        LogAndWrite(f, "");
        LogAndWrite(f, "Render Mode");

        if (CurrentSeqXmlFile->GetRenderMode() == xLightsXmlFile::CANVAS_MODE)
        {
            wxString msg = wxString::Format("    WARN: Render mode set to canvas mode. Unless you specifically know you need this it is not recommended.");
            LogAndWrite(f, msg.ToStdString());
            warncount++;
        }
        else
        {
            LogAndWrite(f, "    No problems found");
        }
        errcountsave = errcount;
        warncountsave = warncount;

        if (CurrentSeqXmlFile->GetSequenceType() == "Media")
        {
            LogAndWrite(f, "");
            LogAndWrite(f, "Checking media file");

            if (!wxFileExists(CurrentSeqXmlFile->GetMediaFile()))
            {
                wxString msg = wxString::Format("    ERR: media file %s does not exist.", CurrentSeqXmlFile->GetMediaFile());
                LogAndWrite(f, msg.ToStdString());
                errcount++;
            }
            else
            {
                LogAndWrite(f, "    No problems found");
            }
        }
        errcountsave = errcount;
        warncountsave = warncount;

        LogAndWrite(f, "");
        LogAndWrite(f, "Checking autosave");

        if (CurrentSeqXmlFile->FileExists())
        {
            // set to log if >1MB and autosave is more than every 10 minutes
            wxULongLong size = CurrentSeqXmlFile->GetSize();
            if (size > 1000000 && AutoSaveInterval < 10)
            {
                wxULongLong mbull = size / 100000;
                double mb = mbull.ToDouble() / 10.0;
                wxString msg = wxString::Format("    WARN: Sequence file size %.1fMb is large. Consider making autosave less frequent to prevent xlights pausing too often when it autosaves.", mb);
                LogAndWrite(f, msg.ToStdString());
                warncount++;
            }

            if (errcount + warncount == errcountsave + warncountsave)
            {
                LogAndWrite(f, "    No problems found");
            }
            errcountsave = errcount;
            warncountsave = warncount;
        }
        else
        {
            LogAndWrite(f, "    Test skipped as sequence has never been saved.");
        }

        // Only warn about model hiding if model blending is turned off.
        if (!CurrentSeqXmlFile->supportsModelBlending())
        {
            LogAndWrite(f, "");
            LogAndWrite(f, "Models hidden by effects on groups");

            // Check for groups that contain models that have appeared before the group at the bottom of the master view
            wxString models = mSequenceElements.GetViewModels(mSequenceElements.GetViewName(0));
            wxArrayString modelnames = wxSplit(models, ',');

            std::list<std::string> seenmodels;
            for (auto it = modelnames.begin(); it != modelnames.end(); ++it)
            {
                Model* m = AllModels.GetModel(it->ToStdString());
                if (m == nullptr)
                {
                    wxString msg = wxString::Format("    ERR: Model %s in your sequence does not seem to exist in the layout. This will need to be deleted or remapped to another model next time you load this sequence.", it->ToStdString());
                    LogAndWrite(f, msg.ToStdString());
                    errcount++;
                }
                else
                {
                    if (m->GetDisplayAs() == "ModelGroup")
                    {
                        ModelGroup* mg = dynamic_cast<ModelGroup*>(m);
                        if (mg == nullptr) logger_base.crit("CheckSequence ModelGroup cast was null. We are about to crash.");
                        auto cm = mg->Models();
                        for (auto it2 = cm.begin(); it2 != cm.end(); ++it2)
                        {
                            if (std::find(seenmodels.begin(), seenmodels.end(), (*it2)->GetName()) != seenmodels.end())
                            {
                                wxString msg = wxString::Format("    WARN: Model Group '%s' will hide effects on model '%s'.", mg->GetName(), (*it2)->GetName());
                                LogAndWrite(f, msg.ToStdString());
                                warncount++;
                            }
                        }
                    }
                    else
                    {
                        seenmodels.push_back(m->GetName());
                    }
                }
            }

            if (errcount + warncount == errcountsave + warncountsave)
            {
                LogAndWrite(f, "    No problems found");
            }
            errcountsave = errcount;
            warncountsave = warncount;
        }

        LogAndWrite(f, "");
        LogAndWrite(f, "Effect problems");

        // check all effects
        for (int i = 0; i < mSequenceElements.GetElementCount(MASTER_VIEW); i++)
        {
            Element* e = mSequenceElements.GetElement(i);
            if (e->GetType() != ELEMENT_TYPE_TIMING)
            {
                CheckElement(e, f, errcount, warncount, e->GetFullName(), e->GetName());

                if (e->GetType() == ELEMENT_TYPE_MODEL)
                {
                    ModelElement *me = dynamic_cast<ModelElement *>(e);

                    for (int j = 0; j < me->GetStrandCount(); ++j)
                    {
                        StrandElement* se = me->GetStrand(j);
                        CheckElement(se, f, errcount, warncount, se->GetFullName(), e->GetName());

                        for(int k = 0; k < se->GetNodeLayerCount(); ++k)
                        {
                            NodeLayer* nl = se->GetNodeLayer(k);
                            for (int l = 0; l < nl->GetEffectCount(); l++)
                            {
                                Effect* ef = nl->GetEffect(l);
                                CheckEffect(ef, f, errcount, warncount, wxString::Format("%sStrand %d/Node %d", se->GetFullName(), j+1, l+1).ToStdString(), e->GetName(), true);
                            }
                        }
                    }
                    for (int j = 0; j < me->GetSubModelCount(); ++j)
                    {
                        Element* sme = me->GetSubModel(j);
                        if (sme->GetType() == ELEMENT_TYPE_SUBMODEL)
                        {
                            CheckElement(sme, f, errcount, warncount, sme->GetFullName(), e->GetName());
                        }
                    }
                }
            }
        }

        if (errcount + warncount == errcountsave + warncountsave)
        {
            LogAndWrite(f, "    No problems found");
        }
        errcountsave = errcount;
        warncountsave = warncount;
    }
    else
    {
        LogAndWrite(f, "");
        LogAndWrite(f, "No sequence loaded so sequence checks skipped.");
    }

    LogAndWrite(f, "");
    LogAndWrite(f, "Check sequence done.");
    LogAndWrite(f, wxString::Format("Errors: %d. Warnings: %d", errcount, warncount).ToStdString());

    if (f.IsOpened())
    {
        f.Close();

        wxFileType *ft = wxTheMimeTypesManager->GetFileTypeFromExtension("txt");
        if (ft != nullptr)
        {
            wxString command = ft->GetOpenCommand(filename);

            if (command == "")
            {
                logger_base.error("Unable to view check sequence results due to no open command. %s", (const char *)filename.c_str());
                wxMessageBox(_("Unable to show xLights Check Sequence results. See your log for the content."), _("Error"));
            }
            else
            {
                logger_base.debug("Viewing xLights Check Sequence results %s. Command: '%s'", (const char *)filename.c_str(), (const char*)command.c_str());
                wxUnsetEnv("LD_PRELOAD");
                wxExecute(command);
            }
            delete ft;
        }
        else
        {
            logger_base.warn("Unable to view xLights Check Sequence results %s.", (const char *)filename.c_str());
            wxMessageBox(_("Unable to show xLights Check Sequence results. See your log for the content."), _("Error"));
        }
    }
}

void xLightsFrame::CheckEffect(Effect* ef, wxFile& f, int& errcount, int& warncount, const std::string& name, const std::string& modelName, bool node)
{
    EffectManager& em = mSequenceElements.GetEffectManager();
    SettingsMap& sm = ef->GetSettings();

    // check excessive fadein/fadeout time
    float fadein = sm.GetFloat("T_TEXTCTRL_Fadein", 0.0);
    float fadeout = sm.GetFloat("T_TEXTCTRL_Fadeout", 0.0);
    float efdur = (ef->GetEndTimeMS() - ef->GetStartTimeMS()) / 1000.0;

    if (fadein > efdur)
    {
        wxString msg = wxString::Format("    WARN: Transition in time %.2f on effect %s at start time %s  on Model '%s' is greater than effect duration %.2f.", fadein, ef->GetEffectName(), FORMATTIME(ef->GetStartTimeMS()), name, efdur);
        LogAndWrite(f, msg.ToStdString());
        warncount++;
    }
    if (fadeout > efdur)
    {
        wxString msg = wxString::Format("    WARN: Transition out time %.2f on effect %s at start time %s  on Model '%s' is greater than effect duration %.2f.", fadeout, ef->GetEffectName(), FORMATTIME(ef->GetStartTimeMS()), name, efdur);
        LogAndWrite(f, msg.ToStdString());
        warncount++;
    }
    if (fadein <= efdur && fadeout <= efdur && fadein + fadeout > efdur)
    {
        wxString msg = wxString::Format("    WARN: Transition in time %.2f + transition out time %.2f = %.2f on effect %s at start time %s  on Model '%s' is greater than effect duration %.2f.", fadein, fadeout, fadein + fadeout, ef->GetEffectName(), FORMATTIME(ef->GetStartTimeMS()), name, efdur);
        LogAndWrite(f, msg.ToStdString());
        warncount++;
    }

    // effect that runs past end of the sequence
    if (ef->GetEndTimeMS() > CurrentSeqXmlFile->GetSequenceDurationMS())
    {
        wxString msg = wxString::Format("    WARN: Effect %s ends at %s after the sequence end %s. Model: '%s' Start: %s", ef->GetEffectName(), FORMATTIME(ef->GetEndTimeMS()), FORMATTIME(CurrentSeqXmlFile->GetSequenceDurationMS()), name, FORMATTIME(ef->GetStartTimeMS()));
        LogAndWrite(f, msg.ToStdString());
        warncount++;
    }

    if (ef->GetEffectIndex() >= 0)
    {
        RenderableEffect* re = em.GetEffect(ef->GetEffectIndex());

        // check effect is appropriate for a node
        if (node && !re->AppropriateOnNodes())
        {
            wxString msg = wxString::Format("    WARN: Effect %s at start time %s  on Model '%s' really shouldnt be used at the node level.",
                ef->GetEffectName(), FORMATTIME(ef->GetStartTimeMS()), name);
            LogAndWrite(f, msg.ToStdString());
            warncount++;
        }

        std::list<std::string> warnings = re->CheckEffectSettings(sm, CurrentSeqXmlFile->GetMedia(), AllModels.GetModel(modelName), ef);
        for (auto s = warnings.begin(); s != warnings.end(); ++s)
        {
            LogAndWrite(f, *s);
            if (s->find("WARN:") != std::string::npos)
            {
                warncount++;
            }
            else if (s->find("ERR:") != std::string::npos)
            {
                errcount++;
            }
        }
    }
}

void xLightsFrame::CheckElement(Element* e, wxFile& f, int& errcount, int& warncount, const std::string& name, const std::string& modelName)
{
    for (int j = 0; j < e->GetEffectLayerCount(); j++)
    {
        EffectLayer* el = e->GetEffectLayer(j);

        for (int k = 0; k < el->GetEffectCount(); k++)
        {
            Effect* ef = el->GetEffect(k);

            // Check there are nodes to actually render on
            Model* m = AllModels[modelName];
            if (m != nullptr)
            {
                if (e->GetType() == ELEMENT_TYPE_MODEL)
                {
                    if (m->GetNodeCount() == 0)
                    {
                        wxString msg = wxString::Format("    ERR: Effect %s (%s-%s) on Model '%s' layer %d Has no nodes and wont do anything.",
                            ef->GetEffectName(), FORMATTIME(ef->GetStartTimeMS()), FORMATTIME(ef->GetEndTimeMS()),
                            name, j + 1);
                        LogAndWrite(f, msg.ToStdString());
                        errcount++;
                    }
                }
                else if (e->GetType() == ELEMENT_TYPE_STRAND)
                {
                    StrandElement* se = (StrandElement*)e;
                    if (m->GetStrandLength(se->GetStrand()) == 0)
                    {
                        wxString msg = wxString::Format("    ERR: Effect %s (%s-%s) on Model '%s' layer %d Has no nodes and wont do anything.",
                            ef->GetEffectName(), FORMATTIME(ef->GetStartTimeMS()), FORMATTIME(ef->GetEndTimeMS()),
                            name, j + 1);
                        LogAndWrite(f, msg.ToStdString());
                        errcount++;
                    }
                }
                else if (e->GetType() == ELEMENT_TYPE_SUBMODEL)
                {
                    Model* se = AllModels[name];
                    if (se != nullptr)
                    {
                        if (se->GetNodeCount() == 0)
                        {
                            wxString msg = wxString::Format("    ERR: Effect %s (%s-%s) on Model '%s' layer %d Has no nodes and wont do anything.",
                                ef->GetEffectName(), FORMATTIME(ef->GetStartTimeMS()), FORMATTIME(ef->GetEndTimeMS()),
                                name, j + 1);
                            LogAndWrite(f, msg.ToStdString());
                            errcount++;
                        }
                    }
                }
            }

            CheckEffect(ef, f, errcount, warncount, name, modelName);
        }

        // This assumes effects are stored in start time order per layer
        Effect* lastEffect = nullptr;
        for (int k = 0; k < el->GetEffectCount(); k++)
        {
            Effect* ef = el->GetEffect(k);

            if (lastEffect != nullptr)
            {
                // the start time of an effect should not be before the end of the prior effect
                if (ef->GetStartTimeMS() < lastEffect->GetEndTimeMS())
                {
                    wxString msg = wxString::Format("    ERR: Effect %s (%s-%s) overlaps with Effect %s (%s-%s) on Model '%s' layer %d. This shouldn't be possible.",
                        ef->GetEffectName(), FORMATTIME(ef->GetStartTimeMS()), FORMATTIME(ef->GetEndTimeMS()), lastEffect->GetEffectName(), FORMATTIME(lastEffect->GetStartTimeMS()), FORMATTIME(lastEffect->GetEndTimeMS()), name, j + 1);
                    LogAndWrite(f, msg.ToStdString());
                    errcount++;
                }
            }

            lastEffect = ef;
        }
    }
}

void xLightsFrame::OnMenuItemCheckSequenceSelected(wxCommandEvent& event)
{
    CheckSequence(true);
}

void xLightsFrame::OnMenuItem_e131syncSelected(wxCommandEvent& event)
{
    NetworkChange();
    me131Sync = event.IsChecked();
    _outputManager.SetSyncEnabled(me131Sync);
    ShowHideSync();
    if (me131Sync)
    {
        // recycle output connections if necessary
        if (_outputManager.IsOutputting())
        {
            _outputManager.StopOutput();
            _outputManager.StartOutput();
        }
    }
    NetworkChange();
}

void xLightsFrame::ShowHideSync()
{
    if (me131Sync)
    {
        SpinCtrl_SyncUniverse->Show();
        StaticText5->Show();
        SetSyncUniverse(SpinCtrl_SyncUniverse->GetValue());
    }
    else
    {
        SpinCtrl_SyncUniverse->Hide();
        StaticText5->Hide();
        SetSyncUniverse(0);
    }
}

void xLightsFrame::OnMenuItem_Help_ForumSelected(wxCommandEvent& event)
{
    ::wxLaunchDefaultBrowser("http://nutcracker123.com/forum/");
}

void xLightsFrame::OnMenuItem_Help_DownloadSelected(wxCommandEvent& event)
{
    ::wxLaunchDefaultBrowser("http://xlights.org");
}

void xLightsFrame::OnMenuItem_Help_ReleaseNotesSelected(wxCommandEvent& event)
{
    wxFileType *ft = wxTheMimeTypesManager->GetFileTypeFromExtension("txt");
    if (ft)
    {
        wxString command = ft->GetOpenCommand("README.txt");
        wxUnsetEnv("LD_PRELOAD");
        wxExecute(command);
    }
}

void xLightsFrame::OnMenuItem_Help_Isue_TrackerSelected(wxCommandEvent& event)
{
    ::wxLaunchDefaultBrowser("https://github.com/smeighan/xLights/issues");
}

void xLightsFrame::OnMenuItem_Help_FacebookSelected(wxCommandEvent& event)
{
    ::wxLaunchDefaultBrowser("https://www.facebook.com/groups/628061113896314/");
}

int xLightsFrame::ExportNodes(wxFile& f, StrandElement* e, NodeLayer* nl, int n, std::map<std::string, int>& effectfrequency, std::map<std::string, int>& effectTotalTime, std::list<std::string>& allfiles)
{
    int effects = 0;
    wxString type = "Node";
    wxString name = wxString::Format("%sStrand %d/Node %d", e->GetFullName(), e->GetStrand()+1, n);

    for (int k = 0; k < nl->GetEffectCount(); k++)
    {
        Effect* ef = nl->GetEffect(k);

        std::string fs = "";
        if (ef->GetEffectIndex() >= 0)
        {
            RenderableEffect *eff = effectManager[ef->GetEffectIndex()];
            auto files = eff->GetFileReferences(ef->GetSettings());

            for (auto it = files.begin(); it != files.end(); ++it)
            {
                if (fs != "")
                {
                    fs += ",";
                }
                fs += (*it);
            }

            allfiles.splice(allfiles.end(), files);
        }

        int duration = ef->GetEndTimeMS() - ef->GetStartTimeMS();
        if (effectfrequency.find(ef->GetEffectName()) != effectfrequency.end())
        {
            effectfrequency[ef->GetEffectName()]++;
            effectTotalTime[ef->GetEffectName()] += duration;
        }
        else
        {
            effectfrequency[ef->GetEffectName()] = 1;
            effectTotalTime[ef->GetEffectName()] = duration;
        }

        SettingsMap& sm = ef->GetSettings();
        f.Write(wxString::Format("\"%s\",%02d:%02d.%03d,%02d:%02d.%03d,%02d:%02d.%03d,\"%s\",\"%s\",%s,%s\n",
            ef->GetEffectName(),
            ef->GetStartTimeMS() / 60000,
            (ef->GetStartTimeMS() % 60000) / 1000,
            ef->GetStartTimeMS() % 1000,
            ef->GetEndTimeMS() / 60000,
            (ef->GetEndTimeMS() % 60000) / 1000,
            ef->GetEndTimeMS() % 1000,
            duration / 60000,
            (duration % 60000) / 1000,
            duration % 1000,
            sm.Contains("X_Effect_Description") ? sm["X_Effect_Description"] : "",
            name,
            type,
            fs
        ));
        effects++;
    }

    return effects;
}

int xLightsFrame::ExportElement(wxFile& f, Element* e, std::map<std::string, int>& effectfrequency, std::map<std::string, int>& effectTotalTime, std::list<std::string>& allfiles)
{
    int effects = 0;

    if (e->GetType() != ELEMENT_TYPE_TIMING)
    {
        Model* m = AllModels.GetModel(e->GetModelName());

        wxString type = "Unknown";
        wxString subname = "";
        switch (e->GetType())
        {
        case     ELEMENT_TYPE_MODEL:
            if (m->GetDisplayAs() == "ModelGroup")
            {
                type = "Model Group";
            }
            else
            {
                type = "Model";
            }
            break;
        case ELEMENT_TYPE_SUBMODEL:
            type = "Submodel";
            break;
        case ELEMENT_TYPE_STRAND:
            type = "Strand";
            subname = wxString::Format("Strand %d", dynamic_cast<StrandElement*>(e)->GetStrand() + 1);
            break;
        case ELEMENT_TYPE_TIMING:
            type = "Timing";
            break;
        }

        for (int j = 0; j < e->GetEffectLayerCount(); j++)
        {
            EffectLayer* el = e->GetEffectLayer(j);

            for (int k = 0; k < el->GetEffectCount(); k++)
            {
                Effect* ef = el->GetEffect(k);
                std::string fs = "";
                if (ef->GetEffectIndex() >= 0)
                {
                    RenderableEffect *eff = effectManager[ef->GetEffectIndex()];
                    auto files = eff->GetFileReferences(ef->GetSettings());

                    for (auto it = files.begin(); it != files.end(); ++it)
                    {
                        if (fs != "")
                        {
                            fs += ",";
                        }
                        fs += (*it);
                    }

                    allfiles.splice(allfiles.end(), files);
                }

                int duration = ef->GetEndTimeMS() - ef->GetStartTimeMS();
                if (effectfrequency.find(ef->GetEffectName()) != effectfrequency.end())
                {
                    effectfrequency[ef->GetEffectName()]++;
                    effectTotalTime[ef->GetEffectName()] += duration;
                }
                else
                {
                    effectfrequency[ef->GetEffectName()] = 1;
                    effectTotalTime[ef->GetEffectName()] = duration;
                }

                SettingsMap& sm = ef->GetSettings();
                f.Write(wxString::Format("\"%s\",%02d:%02d.%03d,%02d:%02d.%03d,%02d:%02d.%03d,\"%s\",\"%s\",%s,%s\n",
                    ef->GetEffectName(),
                    ef->GetStartTimeMS() / 60000,
                    (ef->GetStartTimeMS() % 60000) / 1000,
                    ef->GetStartTimeMS() % 1000,
                    ef->GetEndTimeMS() / 60000,
                    (ef->GetEndTimeMS() % 60000) / 1000,
                    ef->GetEndTimeMS() % 1000,
                    duration / 60000,
                    (duration % 60000) / 1000,
                    duration % 1000,
                    sm.Contains("X_Effect_Description") ? sm["X_Effect_Description"] : "",
                    (const char *)(e->GetFullName() + subname).c_str(),
                    type,
                    fs
                ));
                effects++;
            }
        }
    }

    return effects;
}

void xLightsFrame::OnMenuItem_ExportEffectsSelected(wxCommandEvent& event)
{
    if (CurrentSeqXmlFile == nullptr)
    {
        wxMessageBox("No sequence open", "Error", wxOK|wxCENTRE, this);
        return;
    }

    wxLogNull logNo; //kludge: avoid "error 0" message from wxWidgets after new file is written
    wxString filename = wxFileSelector(_("Choose output file"), wxEmptyString, wxEmptyString, wxEmptyString, "Export files (*.csv)|*.csv", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (filename.IsEmpty()) return;

    ExportEffects(filename);
}

void xLightsFrame::ExportEffects(wxString filename)
{
    wxFile f(filename);

    if (!f.Create(filename, true) || !f.IsOpened())
    {
        wxMessageBox(wxString::Format("Unable to create file %s. Error %d\n", filename, f.GetLastError()), _("Export Error"));
        return;
    }

    std::map<std::string, int> effectfrequency;
    std::map<std::string, int> effecttotaltime;

    int effects = 0;
    f.Write(_("Effect Name,StartTime,EndTime,Duration,Description,Element,ElementType,Files\n"));

    std::list<std::string> files;

    for (size_t i = 0; i < mSequenceElements.GetElementCount(0); i++)
    {
        Element* e = mSequenceElements.GetElement(i);
        effects += ExportElement(f, e, effectfrequency, effecttotaltime, files);

        if (dynamic_cast<ModelElement*>(e) != nullptr)
        {
            for (size_t s = 0; s < dynamic_cast<ModelElement*>(e)->GetSubModelCount(); s++) {
                SubModelElement *se = dynamic_cast<ModelElement*>(e)->GetSubModel(s);
                effects += ExportElement(f, se, effectfrequency, effecttotaltime, files);
            }
            for (size_t s = 0; s < dynamic_cast<ModelElement*>(e)->GetStrandCount(); s++) {
                StrandElement *se = dynamic_cast<ModelElement*>(e)->GetStrand(s);
                int node = 1;
                for (size_t n = 0; n < se->GetNodeLayerCount(); n++)
                {
                    NodeLayer* nl = se->GetNodeLayer(n);
                    effects += ExportNodes(f, se, nl, node++, effectfrequency, effecttotaltime, files);
                }
            }
        }
    }
    f.Write(wxString::Format("\"Effect Count\",%d\n", effects));
    f.Write(_("\n"));
    f.Write(_("Effect Usage Summary\n"));
    f.Write(_("Effect Name,Occurences,TotalTime\n"));
    for (auto it = effectfrequency.begin(); it != effectfrequency.end(); ++it)
    {
        int tt = effecttotaltime[it->first];
        f.Write(wxString::Format("\"%s\",%d,%02d:%02d.%03d\n",
            (const char *)it->first.c_str(),
            it->second,
            tt / 60000,
            (tt % 60000) / 1000,
            tt % 1000
        ));
    }
    f.Write(_("\n"));
    f.Write(_("Summary of files used\n"));

    files.sort();
    files.unique();
    for (auto it = files.begin(); it != files.end(); ++it)
    {
        f.Write(wxString::Format("%s\n", *it));
    }

    f.Close();
}

void xLightsFrame::OnMenuItemShiftEffectsSelected(wxCommandEvent& event)
{
    wxTextEntryDialog ted(this, "Enter the number of milliseconds to shift all effects:\n\n"
                                "Note: Will be rounded to the nearest timing interval.\n"
                                "      This operation cannot be reversed with Undo.\n"
                                "      Effects shifted left may be truncated or deleted.",
                                "Shift Effects", "", wxOK | wxCANCEL|wxCENTER);
    if( ted.ShowModal() == wxID_OK ) {
        int milliseconds = wxAtoi(ted.GetValue());
        if( CurrentSeqXmlFile->GetSequenceLoaded() ) {
            wxString mss = CurrentSeqXmlFile->GetSequenceTiming();
            int ms = wxAtoi(mss);
            milliseconds /= ms;
            milliseconds *= ms;
        }
        for(int elem=0;elem<mSequenceElements.GetElementCount(MASTER_VIEW);elem++) {
            Element* ele = mSequenceElements.GetElement(elem, MASTER_VIEW);
            for(int layer=0;layer<ele->GetEffectLayerCount();layer++) {
                EffectLayer* el = ele->GetEffectLayer(layer);
                ShiftEffectsOnLayer(el, milliseconds);
            }
            if (ele->GetType() == ELEMENT_TYPE_MODEL)
            {
                ModelElement *me = dynamic_cast<ModelElement *>(ele);
                for (int i = 0; i < me->GetStrandCount(); ++i)
                {
                    Element* se = me->GetStrand(i);
                    StrandElement* ste = dynamic_cast<StrandElement*>(se);
                    for (int k = 0; k < ste->GetNodeLayerCount(); ++k)
                    {
                        NodeLayer* nl = ste->GetNodeLayer(k, false);
                        if (nl != nullptr)
                        {
                            ShiftEffectsOnLayer(nl, milliseconds);
                        }
                    }
                }
                for (int i = 0; i < me->GetSubModelCount(); ++i)
                {
                    Element* se = me->GetSubModel(i);
                    for(int layer=0;layer<se->GetEffectLayerCount();layer++) {
                        EffectLayer* sel = se->GetEffectLayer(layer);
                        ShiftEffectsOnLayer(sel, milliseconds);
                    }
                }
            }
        }
        mainSequencer->PanelEffectGrid->Refresh();
    }
}

void xLightsFrame::ShiftEffectsOnLayer(EffectLayer* el, int milliseconds)
{
    for(int ef=el->GetEffectCount()-1; ef >= 0; ef--) {  // count backwards so we can delete if needed
        Effect* eff = el->GetEffect(ef);
        int start_ms = eff->GetStartTimeMS();
        int end_ms = eff->GetEndTimeMS();
        if( start_ms+milliseconds < 0 ) {
            if( end_ms+milliseconds < 0 ) {
                // effect shifted off screen - delete

                el->RemoveEffect(ef);
                if (eff == selectedEffect) {
                    UnselectEffect();
                }
                continue;
            } else {
                // truncate start of effect
                eff->SetStartTimeMS(0);
            }
        } else {
            eff->SetStartTimeMS(start_ms+milliseconds);
        }
        eff->SetEndTimeMS(end_ms+milliseconds);
    }
}

// returns the lost files path if required
std::string AddFileToZipFile(const std::string& baseDirectory, const std::string& file, wxZipOutputStream& zip, const std::string& actualfile = "")
{
    static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));

    std::string filetoactuallyzip = actualfile;
    if (actualfile == "") filetoactuallyzip = file;

    std::string lost = "";
    if (wxFile::Exists(filetoactuallyzip))
    {
        wxFileName bd(baseDirectory);
        std::string showdir = bd.GetName().ToStdString();

        wxFileName fn(file);
        wxString f(file);
#ifdef __WXMSW__
        // Windows doesnt care about case so we can be more permissive
        if (f.Lower().StartsWith(wxString(baseDirectory).Lower()))
#else
        if (f.StartsWith(baseDirectory))
#endif
        {
            // this is in our folder
            std::string tgt = file.substr(baseDirectory.length());
            if (tgt != "" && (tgt[0] == '\\' || tgt[0] == '/'))
            {
                tgt = tgt.substr(1);
            }
            tgt = showdir + "/" + tgt;
            if (zip.PutNextEntry(tgt))
            {
                wxFileInputStream fis(filetoactuallyzip);
                if (fis.IsOk())
                {
                    zip.Write(fis);
                }
                else
                {
                    logger_base.warn("Error adding %s to %s due to failure to create input stream.", (const char*)file.c_str(), (const char *)tgt.c_str());
                }
                zip.CloseEntry();
            }
            else
            {
                logger_base.warn("    Error zipping %s to %s.", (const char*)file.c_str(), (const char *)tgt.c_str());
            }
        }
        else
        {
            // this isnt
            std::string tgt = "_lost/" + fn.GetName().ToStdString() + "." + fn.GetExt().ToStdString();
            tgt = showdir + "/" + tgt;
            lost = tgt;
            if (zip.PutNextEntry(tgt))
            {
                wxFileInputStream fis(filetoactuallyzip);
                zip.Write(fis);
                zip.CloseEntry();
            }
            else
            {
                logger_base.warn("    Error zipping %s to %s.", (const char*)file.c_str(), (const char *)tgt.c_str());
            }
        }
    }
    return lost;
}

std::string FixFile(const std::string& showdir, const std::string& sourcefile, const std::map<std::string, std::string>& lostfiles)
{
    std::string newfile = "";

    if (lostfiles.size() > 0)
    {
        // create a temporary file
        newfile = wxFileName::CreateTempFileName("rgbe").ToStdString();

        // read all of the existing file into memory
        wxFile in(sourcefile);
        wxString data;
        in.ReadAll(&data);
        in.Close();

        // use regex to search and replace all the lost file locations
        for (auto it = lostfiles.begin(); it != lostfiles.end(); ++it)
        {
            // strip off the show folder
            wxString replace(it->second);
            wxString newreplace = replace.AfterFirst('/');
            if (newreplace == replace)
            {
                newreplace = replace.AfterFirst('\\');
            }

            data.Replace(it->first, showdir + "/" + newreplace, true);
        }

        // write the file out
        wxFile out(newfile, wxFile::write);
        out.Write(data);
        out.Close();
    }

    return newfile;
}

std::string StripPresets(const std::string& sourcefile)
{
    std::string newfile = wxFileName::CreateTempFileName("rgbe").ToStdString();

    // read all of the existing file into memory
    wxFile in(sourcefile);
    wxString data;
    in.ReadAll(&data);
    in.Close();

    int start = data.Find("<effects version=\"");
    int end = data.Find("</effects>") + 10;

    if (end >= 10)
    {
        data = data.substr(0, start) + "<effects version=\"0006\"/>" + data.substr(end);
    }

    // write the file out
    wxFile out(newfile, wxFile::write);
    out.Write(data);
    out.Close();

    return newfile;
}

#pragma region Tools Menu

void xLightsFrame::OnMenuItem_FPP_ConnectSelected(wxCommandEvent& event)
{
    // make sure everything is up to date
    if (Notebook1->GetSelection() != LAYOUTTAB)
        layoutPanel->UnSelectAllModels();
    RecalcModels();

    FPPConnectDialog dlg(this, &_outputManager);

    dlg.ShowModal();
}

void xLightsFrame::OnMenuItem_PackageSequenceSelected(wxCommandEvent& event)
{
    static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));

    wxLogNull logNo; //kludge: avoid "error 0" message from wxWidgets after new file is written

    if (mSavedChangeCount != mSequenceElements.GetChangeCount())
    {
        wxMessageBox("Your sequence has unsaved changes. These changes will not be packaged but any new referenced files will be. We suggest you consider saving and trying this again.", "Warning");
    }

    wxFileName fn(CurrentSeqXmlFile->GetFullPath());
    std::string filename = fn.GetName().ToStdString() + ".zip";

    wxFileDialog fd(this, "Zip file to create.", CurrentDir, filename, "zip file(*.zip)|*.zip", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (fd.ShowModal() == wxID_CANCEL) return;

    // make sure everything is up to date
    if (Notebook1->GetSelection() != LAYOUTTAB)
        layoutPanel->UnSelectAllModels();
    RecalcModels();

    wxFileName fnZip(fd.GetPath());
    logger_base.debug("Packaging sequence into %s.", (const char*)fnZip.GetFullPath().c_str());

    wxFFileOutputStream out(fnZip.GetFullPath());
    wxZipOutputStream zip(out);

    wxProgressDialog prog("Package Sequence", "", 100, this, wxPD_APP_MODAL | wxPD_AUTO_HIDE);
    prog.Show();

    std::map<std::string, std::string> lostfiles;

    wxFileName fnNetworks(CurrentDir, OutputManager::GetNetworksFileName());
    prog.Update(1, fnNetworks.GetFullName());
    AddFileToZipFile(CurrentDir.ToStdString(), fnNetworks.GetFullPath().ToStdString(), zip);

    // Add house image
    wxFileName fnHouse(mBackgroundImage);
    prog.Update(5, fnHouse.GetFullName());
    auto lost = AddFileToZipFile(CurrentDir.ToStdString(), fnHouse.GetFullPath().ToStdString(), zip);
    if (lost != "")
    {
        lostfiles[fnHouse.GetFullPath().ToStdString()] = lost;
    }

    prog.Update(10);

    // Add any model images
    std::list<std::string> modelfiles;
    for (auto it = AllModels.begin(); it != AllModels.end(); ++it)
    {
        modelfiles.merge((*it).second->GetFaceFiles());
        modelfiles.merge((*it).second->GetFileReferences());
    }
    modelfiles.sort();
    modelfiles.unique();

    float i = 0;
    for (auto f = modelfiles.begin(); f != modelfiles.end(); ++f)
    {
        i++;
        wxFileName fnf(*f);
        if (fnf.Exists())
        {
            prog.Update(10 + (int)(10.0 * i / (float)modelfiles.size()), fnf.GetFullName());
            lost = AddFileToZipFile(CurrentDir.ToStdString(), fnf.GetFullPath().ToStdString(), zip);
            if (lost != "")
            {
                lostfiles[fnf.GetFullPath().ToStdString()] = lost;
            }
        }
        else
        {
            prog.Update(10 + (int)(10.0 * i / (float)modelfiles.size()));
        }
    }

    wxFileName fnRGBEffects(CurrentDir, "xlights_rgbeffects.xml");
    std::string fixfile = FixFile(CurrentDir.ToStdString(), fnRGBEffects.GetFullPath().ToStdString(), lostfiles);

    if (_excludePresetsFromPackagedSequences)
    {
        if (fixfile == "")
        {
            fixfile = StripPresets(fnRGBEffects.GetFullPath().ToStdString());
        }
        else
        {
            auto oldfile = fixfile;
            fixfile = StripPresets(fixfile);
            wxRemoveFile(oldfile);
        }
    }

    prog.Update(25, fnRGBEffects.GetFullName());
    AddFileToZipFile(CurrentDir.ToStdString(), fnRGBEffects.GetFullPath().ToStdString(), zip, fixfile);
    if (fixfile != "") wxRemoveFile(fixfile);

    lostfiles.clear();

    if (!_excludeAudioFromPackagedSequences)
    {
        // Add the media file
        wxFileName fnMedia(CurrentSeqXmlFile->GetMediaFile());
        prog.Update(30, fnMedia.GetFullName());
        lost = AddFileToZipFile(CurrentDir.ToStdString(), fnMedia.GetFullPath().ToStdString(), zip);
        if (lost != "")
        {
            lostfiles[fnMedia.GetFullPath().ToStdString()] = lost;
        }
        prog.Update(35, fnMedia.GetFullName());
    }
    else
    {
        prog.Update(35, "Skipping audio.");
    }

    // Add any iseq files
    DataLayerSet& data_layers = CurrentSeqXmlFile->GetDataLayers();
    for (int j = 0; j < data_layers.GetNumLayers(); ++j)
    {
        DataLayer* dl = data_layers.GetDataLayer(j);

        if (dl->GetName() != "Nutcracker")
        {
            wxFileName fndl(dl->GetDataSource());

            lost = AddFileToZipFile(CurrentDir.ToStdString(), fndl.GetFullPath().ToStdString(), zip);
            if (lost != "")
            {
                lostfiles[fndl.GetFullPath().ToStdString()] = lost;
            }
        }
    }

    // Add any effects images/videos/glediator files
    std::list<std::string> effectfiles;
    for (size_t j = 0; j < mSequenceElements.GetElementCount(0); j++)
    {
        Element* e = mSequenceElements.GetElement(j);
        effectfiles.merge(e->GetFileReferences(effectManager));

        if (dynamic_cast<ModelElement*>(e) != nullptr)
        {
            for (size_t s = 0; s < dynamic_cast<ModelElement*>(e)->GetSubModelCount(); s++) {
                SubModelElement *se = dynamic_cast<ModelElement*>(e)->GetSubModel(s);
                effectfiles.merge(se->GetFileReferences(effectManager));
            }
            for (size_t s = 0; s < dynamic_cast<ModelElement*>(e)->GetStrandCount(); s++) {
                StrandElement *se = dynamic_cast<ModelElement*>(e)->GetStrand(s);
                effectfiles.merge(se->GetFileReferences(effectManager));
            }
        }
    }
    effectfiles.sort();
    effectfiles.unique();

    i = 0;
    for (auto f = effectfiles.begin(); f != effectfiles.end(); ++f)
    {
        i++;
        wxFileName fnf(*f);
        if (fnf.Exists())
        {
            prog.Update(35 + (int)(59.0 * i / (float)effectfiles.size()), fnf.GetFullName());
            lost = AddFileToZipFile(CurrentDir.ToStdString(), fnf.GetFullPath().ToStdString(), zip);
            if (lost != "")
            {
                lostfiles[fnf.GetFullPath().ToStdString()] = lost;
            }
        }
        else
        {
            prog.Update(30 + (int)(64.0 * i / (float)effectfiles.size()));
        }
    }

    fixfile =  FixFile(CurrentDir.ToStdString(), CurrentSeqXmlFile->GetFullPath().ToStdString(), lostfiles);

    prog.Update(95, CurrentSeqXmlFile->GetFullName());
    AddFileToZipFile(CurrentDir.ToStdString(), CurrentSeqXmlFile->GetFullPath().ToStdString(), zip, fixfile);
    if (fixfile != "") wxRemoveFile(fixfile);

    if (!zip.Close())
    {
        logger_base.warn("Error packaging sequence into %s.", (const char*)fd.GetFilename().c_str());
    }
    out.Close();

    prog.Update(100);
}

void xLightsFrame::OnMenuItem_xScheduleSelected(wxCommandEvent& event)
{
#ifdef LINUX
    // Handle xschedule not in path
    wxFileName f(wxStandardPaths::Get().GetExecutablePath());
    wxString appPath(f.GetPath());
    wxString cmdline(appPath+wxT("/xSchedule"));
    wxExecute(cmdline, wxEXEC_ASYNC,NULL,NULL);
#else
    wxExecute("xSchedule.exe");
#endif
}

#pragma endregion Tools Menu

void xLightsFrame::ValidateWindow()
{
}

void xLightsFrame::TimerOutput(int period)
{
    if (CheckBoxLightOutput->IsChecked())
    {
        _outputManager.SetManyChannels(0, &SeqData[period][0], SeqData.NumChannels());
    }
}

void xLightsFrame::PlayerError(const wxString& msg)
{
    log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));
    wxString m = msg;
    m.Replace("\n", " ", true);
    m.Replace("%", "%%", true);

    logger_base.warn(m);
    wxMessageBox(msg, _("Error"), wxOK | wxICON_EXCLAMATION);
}

#pragma region Settings Menu

void xLightsFrame::OnMenuItem_BackupSubfoldersSelected(wxCommandEvent& event)
{
    _backupSubfolders = MenuItem_BackupSubfolders->IsChecked();
}

void xLightsFrame::OnMenuItem_ForceLocalIPSelected(wxCommandEvent& event)
{
    IPEntryDialog dlg(this);
    dlg.TextCtrl_IPAddress->SetValue(mLocalIP);

    if (dlg.ShowModal() == wxID_OK)
    {
        mLocalIP = dlg.TextCtrl_IPAddress->GetValue();
        _outputManager.SetForceFromIP(mLocalIP.ToStdString());

        if (_outputManager.IsOutputting())
        {
            _outputManager.StopOutput();
            _outputManager.StartOutput();
        }
    }

    if (mLocalIP == "")
    {
        MenuItem_ForceLocalIP->Check(false);
    }
    else
    {
        MenuItem_ForceLocalIP->Check(true);
    }
}

void xLightsFrame::OnMenuItem_ExcludePresetsFromPackagedSequencesSelected(wxCommandEvent& event)
{
    _excludePresetsFromPackagedSequences = MenuItem_ExcludePresetsFromPackagedSequences->IsChecked();
}

void xLightsFrame::OnMenuItem_ExcludeAudioPackagedSequenceSelected(wxCommandEvent& event)
{
    _excludeAudioFromPackagedSequences = MenuItem_ExcludeAudioPackagedSequence->IsChecked();
}

void xLightsFrame::ShowACLights()
{
    wxAuiPaneInfo& tb = MainAuiManager->GetPane(_T("ACToolbar"));
    if (tb.IsOk())
    {
        if (_showACLights)
        {
            tb.Show();
        }
        else
        {
            tb.Hide();
        }
        MainAuiManager->Update();
    }
}

void xLightsFrame::OnMenuItem_ACLIghtsSelected(wxCommandEvent& event)
{
    _showACLights = MenuItem_ACLIghts->IsChecked();
    ShowACLights();
}

void xLightsFrame::OnMenuItem_ShowACRampsSelected(wxCommandEvent& event)
{
    _showACRamps = MenuItem_ShowACRamps->IsChecked();
    mainSequencer->PanelEffectGrid->Refresh();
}

void xLightsFrame::OnMenuItemColorManagerSelected(wxCommandEvent& event)
{
    ColorManagerDialog dlg(this, color_mgr);
    dlg.Fit();
    dlg.SetMainSequencer(mainSequencer);
    dlg.ShowModal();
}

void xLightsFrame::OnMenuItemTimingPlayOnDClick(wxCommandEvent& event)
{
    if (event.GetId() == ID_MENU_TIMING_PLAY_MODE)
    {
        mTimingPlayOnDClick = true;
    }
    else if (event.GetId() == ID_MENU_TIMING_EDIT_MODE)
    {
        mTimingPlayOnDClick = false;
    }
    MenuItemTimingPlayMode->Check(mTimingPlayOnDClick);
    MenuItemTimingEditMode->Check(!mTimingPlayOnDClick);
    mainSequencer->PanelEffectGrid->SetTimingClickPlayMode(mTimingPlayOnDClick);
}

bool xLightsFrame::IsDrawRamps()
{
    return _showACRamps;
}

#pragma endregion Settings Menu
#pragma region Help Menu

void xLightsFrame::OnMenuItem_VideoTutorialsSelected(wxCommandEvent& event)
{
    ::wxLaunchDefaultBrowser("http://videos.xlights.org");
}

void xLightsFrame::DoDonate()
{
    static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));

    std::string html = "<html><body><form action = \"https://www.paypal.com/cgi-bin/webscr\" method=\"post\" id=\"paypal\">";
    html += "<input name=\"cmd\" type=\"hidden\" value=\"_s-xclick\"><input name=\"encrypted\" type=\"hidden\" ";
    html += "value=\"-----BEGIN PKCS7-----MIIHLwYJKoZIhvcNAQcEoIIHIDCCBxwCAQExggEwMIIBLAIBADCBlDCBjjELMAkGA1UEBhM";
    html += "CVVMxCzAJBgNVBAgTAkNBMRYwFAYDVQQHEw1Nb3VudGFpbiBWaWV3MRQwEgYDVQQKEwtQYXlQYWwgSW5jLjETMBEGA1UECxQKbGl2ZV9jZXJ0czERMA8GA1UEAxQIbGl2Z";
    html += "V9hcGkxHDAaBgkqhkiG9w0BCQEWDXJlQHBheXBhbC5jb20CAQAwDQYJKoZIhvcNAQEBBQAEgYB6eVIAMC2zoeDtrWp8JDY0kg9aWdLUVR7OLuzygBndSQtvcAxs9GBKSBv";
    html += "EhIBPRyVITPHMHSWJ0sFKphFP8hv4PUHGrJ/jRVJU7Jg4fj3nmzEBykEfV2Ygx6RO7bHOjsVC7wtosSZOkLg1stWv4/9j1k5GdMSUYb5mdnApLHYegDELMAkGBSsOAwIaB";
    html += "QAwgawGCSqGSIb3DQEHATAUBggqhkiG9w0DBwQIF1t+W/JzgKyAgYi1sMjxlEOuJigFRwFXYhQVKQ5Q9iUdxeRK/jpT6dVobbQRw1OtQLKl+LGcJvonJLiFTzAh/O95b2/";
    html += "1OTdNM161soQlUAt/8vbDGkQFVjLlO/C68a1a2pSXEUWYX1CVbb5UT/6wzuJFSZbfl86gCVT1Vv+pyj2+SjFVau/rdMpO9MKyNukXHTDFoIIDhzCCA4MwggLsoAMCAQICA";
    html += "QAwDQYJKoZIhvcNAQEFBQAwgY4xCzAJBgNVBAYTAlVTMQswCQYDVQQIEwJDQTEWMBQGA1UEBxMNTW91bnRhaW4gVmlldzEUMBIGA1UEChMLUGF5UGFsIEluYy4xEzARBgN";
    html += "VBAsUCmxpdmVfY2VydHMxETAPBgNVBAMUCGxpdmVfYXBpMRwwGgYJKoZIhvcNAQkBFg1yZUBwYXlwYWwuY29tMB4XDTA0MDIxMzEwMTMxNVoXDTM1MDIxMzEwMTMxNVowg";
    html += "Y4xCzAJBgNVBAYTAlVTMQswCQYDVQQIEwJDQTEWMBQGA1UEBxMNTW91bnRhaW4gVmlldzEUMBIGA1UEChMLUGF5UGFsIEluYy4xEzARBgNVBAsUCmxpdmVfY2VydHMxETA";
    html += "PBgNVBAMUCGxpdmVfYXBpMRwwGgYJKoZIhvcNAQkBFg1yZUBwYXlwYWwuY29tMIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDBR07d/ETMS1ycjtkpkvjXZe9k+6Cie";
    html += "LuLsPumsJ7QC1odNz3sJiCbs2wC0nLE0uLGaEtXynIgRqIddYCHx88pb5HTXv4SZeuv0Rqq4+axW9PLAAATU8w04qqjaSXgbGLP3NmohqM6bV9kZZwZLR/klDaQGo1u9uD";
    html += "b9lr4Yn+rBQIDAQABo4HuMIHrMB0GA1UdDgQWBBSWn3y7xm8XvVk/UtcKG+wQ1mSUazCBuwYDVR0jBIGzMIGwgBSWn3y7xm8XvVk/UtcKG+wQ1mSUa6GBlKSBkTCBjjELM";
    html += "AkGA1UEBhMCVVMxCzAJBgNVBAgTAkNBMRYwFAYDVQQHEw1Nb3VudGFpbiBWaWV3MRQwEgYDVQQKEwtQYXlQYWwgSW5jLjETMBEGA1UECxQKbGl2ZV9jZXJ0czERMA8GA1U";
    html += "EAxQIbGl2ZV9hcGkxHDAaBgkqhkiG9w0BCQEWDXJlQHBheXBhbC5jb22CAQAwDAYDVR0TBAUwAwEB/zANBgkqhkiG9w0BAQUFAAOBgQCBXzpWmoBa5e9fo6ujionW1hUhP";
    html += "kOBakTr3YCDjbYfvJEiv/2P+IobhOGJr85+XHhN0v4gUkEDI8r2/rNk1m0GA8HKddvTjyGw/XqXa+LSTlDYkqI8OwR8GEYj4efEtcRpRYBxV8KxAW93YDWzFGvruKnnLbD";
    html += "AF6VR5w/cCMn5hzGCAZowggGWAgEBMIGUMIGOMQswCQYDVQQGEwJVUzELMAkGA1UECBMCQ0ExFjAUBgNVBAcTDU1vdW50YWluIFZpZXcxFDASBgNVBAoTC1BheVBhbCBJb";
    html += "mMuMRMwEQYDVQQLFApsaXZlX2NlcnRzMREwDwYDVQQDFAhsaXZlX2FwaTEcMBoGCSqGSIb3DQEJARYNcmVAcGF5cGFsLmNvbQIBADAJBgUrDgMCGgUAoF0wGAYJKoZIhvc";
    html += "NAQkDMQsGCSqGSIb3DQEHATAcBgkqhkiG9w0BCQUxDxcNMTUwMzIyMDcwMTM5WjAjBgkqhkiG9w0BCQQxFgQUS+bqsAykJyPDOSftCR69oXQRd6YwDQYJKoZIhvcNAQEBB";
    html += "QAEgYCfmPNOECi2mAVRxYEDVYWJ/QxrX5dvMmrcHC1/0Eb2X89pdO+2pDwuI1uzZ6h1In4UiBJPwVNzxCHUOniej7CQ+xHfo87M/Pb0+9LD9GZYSQbnRP5qs4/FImWV6k2";
    html += "9HKecWmJdow3/AP97eoVFQ4iD1aq7vVl4vdzB6yrC1bNj4Q==-----END PKCS7-----\"><input alt = \"PayPal - The safer, easier way to pay online!\" ";
    html += "name=\"submit\" src=\"https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif\" type=\"image\"><img style=\"display: none ! important;\" ";
    html += "hidden=\"\" src=\"https://www.paypalobjects.com/en_US/i/scr/pixel.gif\" alt=\"\" width=\"1\" height=\"1\">";
    html += "</form><script>document.getElementById(\"paypal\").submit();</script></body></html>";

    wxString filename = wxFileName::CreateTempFileName("Donate") + ".html";

    wxFile f;
    if (f.Create(filename, true))
    {
        f.Write(html);
        f.Close();
        ::wxLaunchDefaultBrowser("file://" + filename);

        // this is a bit dodgy ... basically I wait threee seconds and then delete the temporary file.
        // To keep the app responsive I yield frequently
        for (int i = 0; i < 300; ++i)
        {
            wxYield();
            wxMilliSleep(10);
        }

        wxRemoveFile(filename);
    }
    else
    {
        logger_base.error("Unable to create temp file %s.", (const char *)filename.c_str());
    }
}

void xLightsFrame::OnMenuItem_DonateSelected(wxCommandEvent& event)
{
    DoDonate();
}

#pragma endregion Help Menu

#pragma region AC Sequencing

void xLightsFrame::OnAC_OnClick(wxCommandEvent& event)
{
    bool wasSelect = false;
    if (Button_ACSelect->IsChecked())
    {
        wasSelect = true;
    }

    Button_ACOn->SetValue(true);
    Button_ACOff->SetValue(false);
    Button_ACTwinkle->SetValue(false);
    Button_ACShimmer->SetValue(false);
    Button_ACCascade->SetValue(false);
    if (!Button_ACIntensity->IsChecked() && !Button_ACRampUp->IsChecked() && !Button_ACRampDown->IsChecked() && !Button_ACRampUpDown->IsChecked())
    {
        Button_ACIntensity->SetValue(true);
    }
    Button_ACSelect->SetValue(false);

    if (wasSelect)
    {
        if (mainSequencer != nullptr && mainSequencer->PanelEffectGrid != nullptr)
        {
            mainSequencer->PanelEffectGrid->DoACDraw();
        }
    }
}

void xLightsFrame::OnAC_OffClick(wxCommandEvent& event)
{
    bool wasSelect = false;
    if (Button_ACSelect->IsChecked())
    {
        wasSelect = true;
    }

    Button_ACOn->SetValue(false);
    Button_ACOff->SetValue(true);
    Button_ACTwinkle->SetValue(false);
    Button_ACShimmer->SetValue(false);
    Button_ACCascade->SetValue(false);
    Button_ACFill->SetValue(false);
    Button_ACSelect->SetValue(false);

    if (wasSelect)
    {
        if (mainSequencer != nullptr && mainSequencer->PanelEffectGrid != nullptr)
        {
            mainSequencer->PanelEffectGrid->DoACDraw();
        }
    }
}

void xLightsFrame::UpdateACToolbar(bool forceState)
{
    if (Button_ACDisabled->IsChecked() && SeqData.NumFrames() != 0 && _showACLights && forceState)
    {
        wxAuiToolBarItem* button = ACToolbar->FindTool(ID_AUITOOLBARITEM_ACON);
        int state = button->GetState();
        if (state & wxAUI_BUTTON_STATE_DISABLED)
        {
            EnableToolbarButton(ACToolbar, ID_AUITOOLBARITEM_ACSELECT, true);
            EnableToolbarButton(ACToolbar, ID_AUITOOLBARITEM_ACON, true);
            EnableToolbarButton(ACToolbar, ID_AUITOOLBARITEM_ACOFF, true);
            EnableToolbarButton(ACToolbar, ID_AUITOOLBARITEM_ACTWINKLE, true);
            EnableToolbarButton(ACToolbar, ID_AUITOOLBARITEM_ACSHIMMER, true);
            EnableToolbarButton(ACToolbar, ID_AUITOOLBARITEM_ACINTENSITY, true);
            EnableToolbarButton(ACToolbar, ID_AUITOOLBARITEM_ACRAMPUP, true);
            EnableToolbarButton(ACToolbar, ID_AUITOOLBARITEM_ACRAMPDOWN, true);
            EnableToolbarButton(ACToolbar, ID_AUITOOLBARITEM_ACRAMPUPDOWN, true);
            EnableToolbarButton(ACToolbar, ID_AUITOOLBARITEM_ACCASCADE, true);
            EnableToolbarButton(ACToolbar, ID_AUITOOLBARITEM_ACFILL, true);
            EnableToolbarButton(ACToolbar, ID_AUITOOLBARITEM_ACFOREGROUND, true);
            EnableToolbarButton(ACToolbar, ID_AUITOOLBARITEM_ACBACKGROUND, true);
        }
        ChoiceParm1->Enable(true);
        if (Button_ACIntensity->IsChecked())
        {
            ChoiceParm2->Enable(false);
        }
        else
        {
            ChoiceParm2->Enable(true);
        }
    }
    else
    {
        EnableToolbarButton(ACToolbar, ID_AUITOOLBARITEM_ACSELECT, false);
        EnableToolbarButton(ACToolbar, ID_AUITOOLBARITEM_ACON, false);
        EnableToolbarButton(ACToolbar, ID_AUITOOLBARITEM_ACOFF, false);
        EnableToolbarButton(ACToolbar, ID_AUITOOLBARITEM_ACTWINKLE, false);
        EnableToolbarButton(ACToolbar, ID_AUITOOLBARITEM_ACSHIMMER, false);
        EnableToolbarButton(ACToolbar, ID_AUITOOLBARITEM_ACINTENSITY, false);
        EnableToolbarButton(ACToolbar, ID_AUITOOLBARITEM_ACRAMPUP, false);
        EnableToolbarButton(ACToolbar, ID_AUITOOLBARITEM_ACRAMPDOWN, false);
        EnableToolbarButton(ACToolbar, ID_AUITOOLBARITEM_ACRAMPUPDOWN, false);
        EnableToolbarButton(ACToolbar, ID_AUITOOLBARITEM_ACCASCADE, false);
        EnableToolbarButton(ACToolbar, ID_AUITOOLBARITEM_ACFILL, false);
        EnableToolbarButton(ACToolbar, ID_AUITOOLBARITEM_ACFOREGROUND, false);
        EnableToolbarButton(ACToolbar, ID_AUITOOLBARITEM_ACBACKGROUND, false);
        ChoiceParm1->Enable(false);
        ChoiceParm2->Enable(false);
    }
    mainSequencer->PanelEffectGrid->Refresh();
}

void xLightsFrame::OnAC_DisableClick(wxCommandEvent& event)
{
    UpdateACToolbar();
    if (Button_ACDisabled->IsChecked() && SeqData.NumFrames() != 0 && _showACLights)
    {
        ACToolbar->SetToolBitmap(ID_AUITOOLBARITEM_ACDISABLED, wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlAC_ENABLED"))));
        Button_ACSelect->SetValue(true);
        Button_ACIntensity->SetValue(true);
    }
    else
    {
        ACToolbar->SetToolBitmap(ID_AUITOOLBARITEM_ACDISABLED, wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlAC_DISABLED"))));
    }
    UpdateACToolbar();
    //MainAuiManager->Update();
    EnableSequenceControls(true);
}

#pragma endregion AC Sequencing

void xLightsFrame::OnACToolbarDropdown(wxAuiToolBarEvent& event)
{
}

void xLightsFrame::OnAC_ShimmerClick(wxCommandEvent& event)
{
    bool wasSelect = false;
    if (Button_ACSelect->IsChecked())
    {
        wasSelect = true;
    }

    Button_ACOn->SetValue(false);
    Button_ACOff->SetValue(false);
    Button_ACTwinkle->SetValue(false);
    Button_ACShimmer->SetValue(true);
    Button_ACSelect->SetValue(false);
    if (!Button_ACIntensity->IsChecked() && !Button_ACRampUp->IsChecked() && !Button_ACRampDown->IsChecked() && !Button_ACRampUpDown->IsChecked())
    {
        Button_ACIntensity->SetValue(true);
    }

    if (wasSelect)
    {
        if (mainSequencer != nullptr && mainSequencer->PanelEffectGrid != nullptr)
        {
            mainSequencer->PanelEffectGrid->DoACDraw();
        }
    }
}

void xLightsFrame::OnAC_TwinkleClick(wxCommandEvent& event)
{
    bool wasSelect = false;
    if (Button_ACSelect->IsChecked())
    {
        wasSelect = true;
    }

    Button_ACOn->SetValue(false);
    Button_ACOff->SetValue(false);
    Button_ACTwinkle->SetValue(true);
    Button_ACShimmer->SetValue(false);
    Button_ACSelect->SetValue(false);
    if (!Button_ACIntensity->IsChecked() && !Button_ACRampUp->IsChecked() && !Button_ACRampDown->IsChecked() && !Button_ACRampUpDown->IsChecked())
    {
        Button_ACIntensity->SetValue(true);
    }

    if (wasSelect)
    {
        if (mainSequencer != nullptr && mainSequencer->PanelEffectGrid != nullptr)
        {
            mainSequencer->PanelEffectGrid->DoACDraw();
        }
    }
}

bool xLightsFrame::IsACActive()
{
    return SeqData.NumFrames() != 0 && _showACLights && Button_ACDisabled->IsChecked();
}

void xLightsFrame::OnAC_BackgroundClick(wxCommandEvent& event)
{
    Button_ACForeground->SetValue(false);
}

void xLightsFrame::OnAC_ForegroundClick(wxCommandEvent& event)
{
    Button_ACBackground->SetValue(false);
}

void xLightsFrame::OnAC_CascadeClick(wxCommandEvent& event)
{
    bool wasSelect = false;
    if (Button_ACSelect->IsChecked())
    {
        wasSelect = true;
    }

    if (!Button_ACCascade->IsChecked())
    {
        if (!Button_ACOn->IsChecked() && !Button_ACOff->IsChecked() && !Button_ACTwinkle->IsChecked() && !Button_ACShimmer->IsChecked())
        {
            Button_ACSelect->SetValue(true);
        }
        if (!Button_ACRampUp->IsChecked() && !Button_ACRampDown->IsChecked() && !Button_ACRampUpDown->IsChecked())
        {
            Button_ACIntensity->SetValue(true);
        }
    }
    else
    {
        Button_ACIntensity->SetValue(false);
        Button_ACRampUp->SetValue(false);
        Button_ACRampDown->SetValue(false);
        Button_ACRampUpDown->SetValue(false);
        Button_ACFill->SetValue(false);
        Button_ACForeground->SetValue(false);
        Button_ACBackground->SetValue(false);
    }

    if (Button_ACFill->IsChecked())
    {
        EnableToolbarButton(ACToolbar, ID_AUITOOLBARITEM_ACOFF, false);
    }
    else
    {
        EnableToolbarButton(ACToolbar, ID_AUITOOLBARITEM_ACOFF, true);
    }

    if (Button_ACCascade->IsChecked() && wasSelect)
    {
        if (mainSequencer != nullptr && mainSequencer->PanelEffectGrid != nullptr)
        {
            mainSequencer->PanelEffectGrid->DoACDraw();
        }
    }
}

void xLightsFrame::OnAC_FillClick(wxCommandEvent& event)
{
    bool wasSelect = false;
    if (Button_ACSelect->IsChecked())
    {
        wasSelect = true;
    }

    if (Button_ACFill->IsChecked())
    {
        EnableToolbarButton(ACToolbar, ID_AUITOOLBARITEM_ACOFF, false);
        Button_ACForeground->SetValue(false);
        Button_ACBackground->SetValue(false);
    }
    else
    {
        EnableToolbarButton(ACToolbar, ID_AUITOOLBARITEM_ACOFF, true);
    }

    Button_ACCascade->SetValue(false);

    if (!Button_ACOn->IsChecked() && !Button_ACTwinkle->IsChecked() && !Button_ACShimmer->IsChecked())
    {
        if (Button_ACFill->IsChecked())
        {
            Button_ACOn->SetValue(true);
        }
        else
        {
            if (!Button_ACOff->IsChecked())
            {
                Button_ACSelect->SetValue(true);
            }
        }
    }

    if (!Button_ACIntensity->IsChecked() && !Button_ACRampUp->IsChecked() && !Button_ACRampDown->IsChecked() && !Button_ACRampUpDown->IsChecked())
    {
        Button_ACIntensity->SetValue(true);
    }

    if (wasSelect && Button_ACFill->IsChecked())
    {
        if (mainSequencer != nullptr && mainSequencer->PanelEffectGrid != nullptr)
        {
            mainSequencer->PanelEffectGrid->DoACDraw();
        }
    }
}

void xLightsFrame::OnAC_RampUpDownClick(wxCommandEvent& event)
{
    bool wasSelect = false;
    if (Button_ACSelect->IsChecked())
    {
        wasSelect = true;
    }

    Button_ACIntensity->SetValue(false);
    Button_ACRampUp->SetValue(false);
    Button_ACRampDown->SetValue(false);
    Button_ACRampUpDown->SetValue(true);
    Button_ACFill->SetValue(false);
    Button_ACCascade->SetValue(false);
    if (Button_ACOff->IsChecked())
    {
        Button_ACOff->SetValue(false);
        Button_ACOn->SetValue(true);
    }
    ChoiceParm2->Enable(true);
    ChoiceParm1->SetStringSelection(wxString::Format("%i", _acParm1RampUpDown));
    ChoiceParm2->SetStringSelection(wxString::Format("%i", _acParm2RampUpDown));

    if (wasSelect)
    {
        if (mainSequencer != nullptr && mainSequencer->PanelEffectGrid != nullptr)
        {
            mainSequencer->PanelEffectGrid->DoACDraw();
        }
    }
}

void xLightsFrame::OnAC_RampDownClick(wxCommandEvent& event)
{
    bool wasSelect = false;
    if (Button_ACSelect->IsChecked())
    {
        wasSelect = true;
    }

    Button_ACIntensity->SetValue(false);
    Button_ACRampUp->SetValue(false);
    Button_ACRampDown->SetValue(true);
    Button_ACRampUpDown->SetValue(false);
    Button_ACFill->SetValue(false);
    Button_ACCascade->SetValue(false);
    if (Button_ACOff->IsChecked())
    {
        Button_ACOff->SetValue(false);
        Button_ACOn->SetValue(true);
        Button_ACSelect->SetValue(false);
    }
    ChoiceParm2->Enable(true);
    ChoiceParm1->SetStringSelection(wxString::Format("%i", _acParm1RampDown));
    ChoiceParm2->SetStringSelection(wxString::Format("%i", _acParm2RampDown));

    if (wasSelect)
    {
        if (mainSequencer != nullptr && mainSequencer->PanelEffectGrid != nullptr)
        {
            mainSequencer->PanelEffectGrid->DoACDraw();
        }
    }
}

void xLightsFrame::OnAC_RampUpClick(wxCommandEvent& event)
{
    bool wasSelect = false;
    if (Button_ACSelect->IsChecked())
    {
        wasSelect = true;
    }

    Button_ACIntensity->SetValue(false);
    Button_ACRampUp->SetValue(true);
    Button_ACRampDown->SetValue(false);
    Button_ACRampUpDown->SetValue(false);
    Button_ACFill->SetValue(false);
    Button_ACCascade->SetValue(false);
    if (Button_ACOff->IsChecked())
    {
        Button_ACOff->SetValue(false);
        Button_ACOn->SetValue(true);
        Button_ACSelect->SetValue(false);
    }
    ChoiceParm2->Enable(true);
    ChoiceParm1->SetStringSelection(wxString::Format("%i", _acParm1RampUp));
    ChoiceParm2->SetStringSelection(wxString::Format("%i", _acParm2RampUp));

    if (wasSelect)
    {
        if (mainSequencer != nullptr && mainSequencer->PanelEffectGrid != nullptr)
        {
            mainSequencer->PanelEffectGrid->DoACDraw();
        }
    }
}

void xLightsFrame::OnAC_IntensityClick(wxCommandEvent& event)
{
    bool wasSelect = false;
    if (Button_ACSelect->IsChecked())
    {
        wasSelect = true;
    }

    Button_ACIntensity->SetValue(true);
    Button_ACRampUp->SetValue(false);
    Button_ACRampDown->SetValue(false);
    Button_ACRampUpDown->SetValue(false);
    Button_ACFill->SetValue(false);
    Button_ACCascade->SetValue(false);
    if (Button_ACOff->IsChecked())
    {
        Button_ACOff->SetValue(false);
        Button_ACOn->SetValue(true);
        Button_ACSelect->SetValue(false);
    }
    ChoiceParm2->Enable(false);
    ChoiceParm1->SetStringSelection(wxString::Format("%i", _acParm1Intensity));

    if (wasSelect)
    {
        if (mainSequencer != nullptr && mainSequencer->PanelEffectGrid != nullptr)
        {
            mainSequencer->PanelEffectGrid->DoACDraw();
        }
    }
}

void xLightsFrame::GetACSettings(ACTYPE& type, ACSTYLE& style, ACTOOL& tool, ACMODE& mode)
{
    if (Button_ACSelect->IsChecked())
    {
        type = ACTYPE::SELECT;
    }
    else if (Button_ACOn->IsChecked())
    {
        type = ACTYPE::ON;
    }
    else if (Button_ACOff->IsChecked())
    {
        type = ACTYPE::OFF;
    }
    else if (Button_ACTwinkle->IsChecked())
    {
        type = ACTYPE::TWINKLE;
    }
    else if (Button_ACShimmer->IsChecked())
    {
        type = ACTYPE::SHIMMER;
    }

    if (Button_ACIntensity->IsChecked())
    {
        style = ACSTYLE::INTENSITY;
    }
    else if (Button_ACRampUp->IsChecked())
    {
        style = ACSTYLE::RAMPUP;
    }
    else if (Button_ACRampDown->IsChecked())
    {
        style = ACSTYLE::RAMPDOWN;
    }
    else if (Button_ACRampUpDown->IsChecked())
    {
        style = ACSTYLE::RAMPUPDOWN;
    }

    tool = ACTOOL::TOOLNIL;
    if (Button_ACFill->IsChecked())
    {
        tool = ACTOOL::FILL;
    }
    else if (Button_ACCascade->IsChecked())
    {
        tool = ACTOOL::CASCADE;
    }

    mode = ACMODE::MODENIL;
    if (Button_ACForeground->IsChecked())
    {
        mode = ACMODE::FOREGROUND;
    }
    else if (Button_ACBackground->IsChecked())
    {
        mode = ACMODE::BACKGROUND;
    }
}

int xLightsFrame::GetACIntensity()
{
    return _acParm1Intensity;
}

void xLightsFrame::GetACRampValues(int& a, int& b)
{
    a = 0;
    b = 100;
    if (Button_ACRampUp->IsChecked())
    {
        a = std::min(_acParm1RampUp, _acParm2RampUp);
        b = std::max(_acParm1RampUp, _acParm2RampUp);
    }
    else if (Button_ACRampDown->IsChecked())
    {
        a = std::max(_acParm1RampDown, _acParm2RampDown);
        b = std::min(_acParm1RampDown, _acParm2RampDown);
    }
    else if (Button_ACRampUpDown->IsChecked())
    {
        a = _acParm1RampUpDown;
        b = _acParm2RampUpDown;
    }
}

void xLightsFrame::OnChoiceParm1Select(wxCommandEvent& event)
{
    if (Button_ACIntensity->IsChecked())
    {
        _acParm1Intensity = wxAtoi(ChoiceParm1->GetStringSelection());
    }
    else if (Button_ACRampUp->IsChecked())
    {
        _acParm1RampUp = wxAtoi(ChoiceParm1->GetStringSelection());
    }
    else if (Button_ACRampDown->IsChecked())
    {
        _acParm1RampDown = wxAtoi(ChoiceParm1->GetStringSelection());
    }
    else if (Button_ACRampUpDown->IsChecked())
    {
        _acParm1RampUpDown = wxAtoi(ChoiceParm1->GetStringSelection());
    }
}

void xLightsFrame::OnChoiceParm2Select(wxCommandEvent& event)
{
    if (Button_ACRampUp->IsChecked())
    {
        _acParm2RampUp = wxAtoi(ChoiceParm2->GetStringSelection());
    }
    else if (Button_ACRampDown->IsChecked())
    {
        _acParm2RampDown = wxAtoi(ChoiceParm2->GetStringSelection());
    }
    else if (Button_ACRampUpDown->IsChecked())
    {
        _acParm2RampUpDown = wxAtoi(ChoiceParm2->GetStringSelection());
    }
}

void xLightsFrame::OnAC_SelectClick(wxCommandEvent& event)
{
    Button_ACOn->SetValue(false);
    Button_ACOff->SetValue(false);
    Button_ACTwinkle->SetValue(false);
    Button_ACShimmer->SetValue(false);
    Button_ACSelect->SetValue(true);
    Button_ACFill->SetValue(false);
    Button_ACCascade->SetValue(false);
    if (!Button_ACIntensity->IsChecked() && !Button_ACRampUp->IsChecked() && !Button_ACRampDown->IsChecked() && !Button_ACRampUpDown->IsChecked())
    {
        Button_ACIntensity->SetValue(true);
    }
}

void xLightsFrame::SetACSettings(ACTOOL tool)
{
    wxCommandEvent event;
    switch (tool)
    {
    case ACTOOL::CASCADE:
        Button_ACCascade->SetValue(true);
        OnAC_CascadeClick(event);
        break;
    case ACTOOL::FILL:
        Button_ACFill->SetValue(true);
        OnAC_FillClick(event);
        break;
    default:
        break;
    }

    if (Button_ACSelect->IsChecked() || Button_ACOff->IsChecked())
    {
        Button_ACOn->SetValue(true);
        Button_ACSelect->SetValue(false);
        Button_ACOff->SetValue(false);
    }
    ACToolbar->Refresh();
}

void xLightsFrame::SetACSettings(ACSTYLE style)
{
    if (Button_ACSelect->IsChecked() || Button_ACOff->IsChecked())
    {
        Button_ACOn->SetValue(true);
        Button_ACSelect->SetValue(false);
        Button_ACOff->SetValue(false);
    }

    wxCommandEvent event;
    switch (style)
    {
    case ACSTYLE::INTENSITY:
        Button_ACIntensity->SetValue(true);
        OnAC_IntensityClick(event);
        break;
    case ACSTYLE::RAMPUP:
        Button_ACRampUp->SetValue(true);
        OnAC_RampUpClick(event);
        break;
    case ACSTYLE::RAMPDOWN:
        Button_ACRampDown->SetValue(true);
        OnAC_RampDownClick(event);
        break;
    case ACSTYLE::RAMPUPDOWN:
        Button_ACRampUpDown->SetValue(true);
        OnAC_RampUpDownClick(event);
        break;
    default:
        break;
    }
    ACToolbar->Refresh();
}

void xLightsFrame::SetACSettings(ACMODE mode)
{
    wxCommandEvent event;
    switch (mode)
    {
    case ACMODE::FOREGROUND:
        Button_ACForeground->SetValue(true);
        OnAC_ForegroundClick(event);
        break;
    case ACMODE::BACKGROUND:
        Button_ACBackground->SetValue(true);
        OnAC_BackgroundClick(event);
        break;
    default:
        break;
    }
    ACToolbar->Refresh();
}

void xLightsFrame::SetACSettings(ACTYPE type)
{
    wxCommandEvent event;
    switch (type)
    {
    case ACTYPE::SELECT:
        Button_ACSelect->SetValue(true);
        OnAC_SelectClick(event);
        break;
    case ACTYPE::OFF:
        Button_ACOff->SetValue(true);
        OnAC_OffClick(event);
        break;
    case ACTYPE::ON:
        Button_ACOn->SetValue(true);
        OnAC_OnClick(event);
        break;
    case ACTYPE::SHIMMER:
        Button_ACShimmer->SetValue(true);
        OnAC_ShimmerClick(event);
        break;
    case ACTYPE::TWINKLE:
        Button_ACTwinkle->SetValue(true);
        OnAC_TwinkleClick(event);
        break;
    default:
        break;
    }

    if (Button_ACOn->IsChecked() || Button_ACShimmer->IsChecked() || Button_ACTwinkle->IsChecked())
    {
        if (!Button_ACIntensity->IsChecked() && !Button_ACRampUp->IsChecked() && !Button_ACRampDown->IsChecked() && !Button_ACRampUpDown->IsChecked())
        {
            Button_ACIntensity->SetValue(true);
        }
    }
    ACToolbar->Refresh();
}


void xLightsFrame::OnMenuItem_PerspectiveAutosaveSelected(wxCommandEvent& event)
{
    _autoSavePerspecive = MenuItem_PerspectiveAutosave->IsChecked();
}

void xLightsFrame::OnMenuItem_SD_10Selected(wxCommandEvent& event)
{
    _suppressDuplicateFrames = 10;
    _outputManager.SetSuppressFrames(_suppressDuplicateFrames);
    NetworkChange();
}

void xLightsFrame::OnMenuItem_SD_20Selected(wxCommandEvent& event)
{
    _suppressDuplicateFrames = 20;
    _outputManager.SetSuppressFrames(_suppressDuplicateFrames);
    NetworkChange();
}

void xLightsFrame::OnMenuItem_SD_40Selected(wxCommandEvent& event)
{
    _suppressDuplicateFrames = 40;
    _outputManager.SetSuppressFrames(_suppressDuplicateFrames);
    NetworkChange();
}

void xLightsFrame::OnMenuItem_SD_NoneSelected(wxCommandEvent& event)
{
    _suppressDuplicateFrames = 0;
    _outputManager.SetSuppressFrames(_suppressDuplicateFrames);
    NetworkChange();
}

void xLightsFrame::OnMenuItem_PlayControlsOnPreviewSelected(wxCommandEvent& event)
{
    _playControlsOnPreview = MenuItem_PlayControlsOnPreview->IsChecked();
    _housePreviewPanel->SetToolbar(_playControlsOnPreview);
    _housePreviewPanel->PostSizeEvent();
}

void xLightsFrame::OnMenuItem_AutoShowHousePreviewSelected(wxCommandEvent& event)
{
    _autoShowHousePreview = MenuItem_AutoShowHousePreview->IsChecked();
}

bool xLightsFrame::IsPaneDocked(wxWindow* window) const
{
    if (m_mgr == nullptr) return true;

    return m_mgr->GetPane(window).IsDocked();
}

void xLightsFrame::OnMenuItem_GenerateLyricsSelected(wxCommandEvent& event)
{
    GenerateLyricsDialog dlg(this, SeqData.NumChannels());

    if (dlg.ShowModal() == wxID_OK)
    {
        std::map<std::string, std::list<long>> face; // these channels need to be set to not zero when this phenome is displayed

        std::list<long> data = dlg.GetChannels("AI");
        if (data.size() > 0) face["AI"] = data;
        std::list<long> all = data; // this is all the channels used by one or more phenomes
        data = dlg.GetChannels("E");
        if (data.size() > 0) face["E"] = data;
        all.merge(data);
        data = dlg.GetChannels("etc");
        if (data.size() > 0) face["etc"] = data;
        all.merge(data);
        data = dlg.GetChannels("FV");
        if (data.size() > 0) face["FV"] = data;
        all.merge(data);
        data = dlg.GetChannels("L");
        if (data.size() > 0) face["L"] = data;
        all.merge(data);
        data = dlg.GetChannels("MBP");
        if (data.size() > 0) face["MBP"] = data;
        all.merge(data);
        data = dlg.GetChannels("O");
        if (data.size() > 0) face["O"] = data;
        all.merge(data);
        data = dlg.GetChannels("rest");
        if (data.size() > 0) face["rest"] = data;
        all.merge(data);
        data = dlg.GetChannels("U");
        if (data.size() > 0) face["U"] = data;
        all.merge(data);
        data = dlg.GetChannels("WQ");
        if (data.size() > 0) face["WQ"] = data;
        all.merge(data);
        all.unique();

        std::map<std::string, std::list<long>> notface; // this is the list of channels that must be zero for a given phenome
        for (auto it = face.begin(); it != face.end(); ++it)
        {
            std::list<long> notdata;

            for (auto it2 = all.begin(); it2 != all.end(); ++it2)
            {
                if (std::find(it->second.begin(), it->second.end(), *it2) == it->second.end())
                {
                    notdata.push_back(*it2);
                }
            }
            notface[it->first] = notdata;
        }

        // now create the phenome timing track
        std::string name = mSequenceElements.UniqueElementName(dlg.GetLyricName());
        int timingCount = mSequenceElements.GetNumberOfTimingElements();
        Element* e = mSequenceElements.AddElement(timingCount, name, "timing", true, false, true, false);
        mSequenceElements.AddTimingToCurrentView(name);
        TimingElement* timing = dynamic_cast<TimingElement*>(e);
        timing->AddEffectLayer();
        timing->AddEffectLayer();
        EffectLayer* tl = timing->AddEffectLayer();

        Effect* lastEffect = nullptr;
        std::string lastPhenome = "";

        for (size_t i = 0; i < SeqData.NumFrames(); ++i)
        {
            bool phenomeFound = false;
            for (auto it = face.begin(); it != face.end(); ++it)
            {
                bool match = true;
                for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2)
                {
                    if (SeqData[i][*it2-1] == 0)
                    {
                        match = false;
                        break;
                    }
                }
                for (auto it2 = notface[it->first].begin(); it2 != notface[it->first].end(); ++it2)
                {
                    if (SeqData[i][*it2-1] != 0)
                    {
                        match = false;
                        break;
                    }
                }

                if (match)
                {
                    if (lastEffect != nullptr && lastPhenome == it->first)
                    {
                        lastEffect->SetEndTimeMS(lastEffect->GetEndTimeMS() + CurrentSeqXmlFile->GetFrameMS());
                        phenomeFound = true;
                    }
                    else
                    {
                        long start = i * CurrentSeqXmlFile->GetFrameMS();
                        lastEffect = tl->AddEffect(0, it->first, "", "", start, start + CurrentSeqXmlFile->GetFrameMS(), false, false);
                        lastPhenome = it->first;
                    }
                    phenomeFound = true;

                    break;
                }
            }

            if (!phenomeFound)
            {
                lastPhenome = "";
                lastEffect = nullptr;
            }
        }

        wxCommandEvent eventRowHeaderChanged(EVT_ROW_HEADINGS_CHANGED);
        wxPostEvent(this, eventRowHeaderChanged);
    }
}

void xLightsFrame::OnMenuItem_CrashXLightsSelected(wxCommandEvent& event)
{
    static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));
    logger_base.crit("^^^^^ xLights crashing on purpose ... bye bye cruel world.");
    int *p = nullptr;
    *p = 0xFFFFFFFF;
}

void xLightsFrame::OnMenuItemBatchRenderSelected(wxCommandEvent& event)
{
    static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));
    BatchRenderDialog dlg(this);
    if (dlg.Prepare(this->GetShowDirectory()) && dlg.ShowModal() == wxID_OK && CloseSequence()) {
        wxArrayString files = dlg.GetFileList();
        wxArrayString filesToRender;
        for (auto f = files.begin(); f != files.end(); f++) {
            wxFileName fname(this->GetShowDirectory() + wxFileName::GetPathSeparator() + *f);
            if(fname.FileExists())
                filesToRender.push_back(fname.GetFullPath());
            else
                logger_base.info("BatchRender: Sequence File not Found: %s.", fname.GetFullPath().ToStdString().c_str());
        }
        if (filesToRender.size() > 0) {
            _renderMode = true;
            OpenRenderAndSaveSequences(filesToRender, false);
            _renderMode = false;
        }
        else
        {
            logger_base.info("BatchRender: No Sequences Selected.");
        }
    }
}

void xLightsFrame::OnMenuItem_UpdateSelected(wxCommandEvent& event)
{
    bool update_found = CheckForUpdate(true);
    if (!update_found) {
        wxMessageBox("No update found", "Update check complete");
    }
}

bool xLightsFrame::CheckForUpdate(bool force)
{
    bool found_update = false;
    wxRegEx reVersion("^.*(2[0-9]*\\.[0-9]*)\\..*$");
    #ifdef LINUX
      wxString hostname =  wxT("www.adebenham.com");
      wxString path = wxT("/wp-content/uploads/xlights/latest.php");
      wxString downloadUrl = wxT("https://www.adebenham.com/xlights-linux");
      MenuItem_Update->Enable(true);
    #else
      #ifdef  __WXOSX_MAC__
        wxString hostname = _T("dankulp.com");
        wxString path = _T("/xLightsLatest.php");
        wxString downloadUrl =  wxT("http://dankulp.com/xlights/");
        MenuItem_Update->Enable(true);
      #else
        wxString hostname = _T("xlights.org");
        wxString path = _T("/xLightsLatest.php");
        wxString downloadUrl = wxT("https://xlights.org/releases/");
        MenuItem_Update->Enable(false);
        return false; // No checking on windows yet
      #endif
    #endif

    wxHTTP get;
    get.SetTimeout(5); // 5 seconds of timeout instead of 10 minutes ...

    if (force) {
        while (!get.Connect(hostname))  // only the server, no pages here yet ...
            wxSleep(5);
    } else {
        if (!get.Connect(hostname))
            return true;
    }

    wxInputStream *httpStream = get.GetInputStream(path);
    if (get.GetError() == wxPROTO_NOERR) {
        wxString res;
        wxString configver = wxT("");
        wxStringOutputStream out_stream(&res);
        httpStream->Read(out_stream);
        wxString urlVersion = wxString(out_stream.GetString());
        reVersion.Replace(&urlVersion,"\\1",1);
        wxConfigBase* config = wxConfigBase::Get();
        if (!force && (config != nullptr))
        {
            config->Read("SkipVersion",&configver);
        }
        if ((!urlVersion.Matches(configver))
             && (!urlVersion.Matches(xlights_version_string))) {
            found_update=true;
            UpdaterDialog *dialog = new UpdaterDialog(this);
            dialog->urlVersion=urlVersion;
            dialog->force=force;
            dialog->downloadUrl=downloadUrl;
            dialog->StaticTextUpdateLabel->SetLabel(wxT("You are currently running xLights "
                                                       + xlights_version_string + "\n"
                                                       + "Whereas the most recent release is " + urlVersion));
            dialog->Show();
        }
    } else {
        wxMessageBox(_T("Unable to connect!"));
    }

    wxDELETE(httpStream);
    get.Close();
    return found_update;
}

void xLightsFrame::check32AppOn64Machine()
{
    wxConfigBase* config = wxConfigBase::Get();
    bool alreadyRun = config->ReadBool("xLights32bitCheck", false);

    if (!alreadyRun && wxIsPlatform64Bit() && sizeof(size_t) == 4)
    {
        config->Write("xLights32bitCheck", true);
        wxMessageBox("You are running the 32 bit version of xLights on a 64 bit Computer\nPlease Update to the 64 bit version of xLights", "Update to 64bit");
    }
}

void xLightsFrame::OnMenuItem_SmallWaveformSelected(wxCommandEvent& event)
{
    _smallWaveform = MenuItem_SmallWaveform->IsChecked();
    if (_smallWaveform)
    {
        mainSequencer->SetSmallWaveform();
    }
    else
    {
        mainSequencer->SetLargeWaveform();
    }
}

void xLightsFrame::OnMenuItem_LogRenderStateSelected(wxCommandEvent& event)
{
    LogRenderStatus();
}

void xLightsFrame::OnMenuItem_ModelBlendDefaultOffSelected(wxCommandEvent& event)
{
    _modelBlendDefaultOff = MenuItem_ModelBlendDefaultOff->IsChecked();
}

void xLightsFrame::OnMenuItem_File_Save_Selected(wxCommandEvent& event)
{
     switch (Notebook1->GetSelection()) {
         case SETUPTAB:
             SaveNetworksFile();
             break;
         case LAYOUTTAB:
             layoutPanel->SaveEffects();
             break;
         case NEWSEQUENCER:
             SaveSequence();
             break;
         default:
             break;
    }
}

void xLightsFrame::OnMenuItem_SnapToTimingMarksSelected(wxCommandEvent& event)
{
    _snapToTimingMarks = MenuItem_SnapToTimingMarks->IsChecked();
}

void xLightsFrame::OnMenuItem_PurgeVendorCacheSelected(wxCommandEvent& event)
{
    VendorModelDialog::GetCache().ClearCache();
    VendorModelDialog::GetCache().Save();
}

void xLightsFrame::OnMenuItem_LoudVolSelected(wxCommandEvent& event)
{
    playVolume = 100;
    SDL::SetGlobalVolume(playVolume);
}

void xLightsFrame::OnMenuItem_MedVolSelected(wxCommandEvent& event)
{
    playVolume = 66;
    SDL::SetGlobalVolume(playVolume);
}

void xLightsFrame::OnMenuItem_QuietVolSelected(wxCommandEvent& event)
{
    playVolume = 33;
    SDL::SetGlobalVolume(playVolume);
}

void xLightsFrame::OnMenuItem_VQuietVolSelected(wxCommandEvent& event)
{
    playVolume = 10;
    SDL::SetGlobalVolume(playVolume);
}

void xLightsFrame::OnMenuItemSelectEffectSelected(wxCommandEvent& event)
{
    bool visible = m_mgr->GetPane("SelectEffect").IsShown();
    if (visible)
    {
        m_mgr->GetPane("SelectEffect").Hide();
    }
    else
    {
        m_mgr->GetPane("SelectEffect").Show();
    }
    m_mgr->Update();
}

