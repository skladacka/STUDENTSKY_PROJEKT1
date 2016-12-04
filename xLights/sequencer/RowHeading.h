#ifndef ROWHEADING_H
#define ROWHEADING_H
#include "wx/window.h"
#include <wx/xml/xml.h>
#include "SequenceElements.h"
#include "wx/wx.h"
#include "../Color.h"

#define HORIZONTAL_PADDING          10
#define PIXELS_PER_MAJOR_HASH       100
#define DEFAULT_ROW_HEADING_HEIGHT  RowHeading::rowHeadingSize
#define DEFAULT_ROW_HEADING_MARGIN  16

wxDECLARE_EVENT(EVT_ROW_HEADINGS_CHANGED, wxCommandEvent);
wxDECLARE_EVENT(EVT_SHOW_DISPLAY_ELEMENTS, wxCommandEvent);
class MainSequencer;

class RowHeading : public wxWindow
{
    public:
        //RowHeading(wxScrolledWindow* parent);
        RowHeading(MainSequencer* parent, wxWindowID id, const wxPoint &pos=wxDefaultPosition, const wxSize &size=wxDefaultSize,
                       long style=0, const wxString &name=wxPanelNameStr);
        virtual ~RowHeading();

        void SetSequenceElements(SequenceElements* elements);
        void SetSelectedRow(int row_number);
        static const xlColor* GetTimingColor(int index);
        int getWidth();
        int getHeight();
        int GetMaxRows();
        void SetCanPaste(bool value) { mCanPaste = value; }

    static int rowHeadingSize;

    protected:
    private:
        DECLARE_EVENT_TABLE()
        void Draw();
        void render( wxPaintEvent& event );
        void mouseLeftDown( wxMouseEvent& event);
        void rightClick( wxMouseEvent& event);
        void leftDoubleClick(wxMouseEvent &event);
        void OnLayerPopup(wxCommandEvent& event);
        void DrawHeading(wxPaintDC* dc, wxXmlNode* model,int width,int row);
        bool HitTestCollapseExpand(int row,int x, bool* IsCollapsed);
        bool HitTestTimingActive(int row,int x, bool* IsActive);
        void BreakdownTimingPhrases(TimingElement* element);
        void BreakdownTimingWords(TimingElement* element);
        const xlColor* GetHeaderColor(Row_Information_Struct* info);
        const xlColor * mHeaderColorModel;
        const xlColor * mHeaderColorTiming;
        const xlColor * mHeaderSelectedColor;
        wxBitmap papagayo_icon;
        wxBitmap model_group_icon;

        int mSelectedRow;
        SequenceElements* mSequenceElements;
        bool mCanPaste;

        static const long ID_ROW_MNU_INSERT_LAYER_ABOVE;
        static const long ID_ROW_MNU_INSERT_LAYER_BELOW;
        static const long ID_ROW_MNU_INSERT_LAYERS_BELOW;
        static const long ID_ROW_MNU_DELETE_LAYER;
		static const long ID_ROW_MNU_DELETE_LAYERS_BELOW;
        static const long ID_ROW_MNU_LAYER;
        static const long ID_ROW_MNU_PLAY_MODEL;
        static const long ID_ROW_MNU_EXPORT_MODEL;
        static const long ID_ROW_MNU_EXPORT_RENDERED_MODEL;
        static const long ID_ROW_MNU_EDIT_DISPLAY_ELEMENTS;
        static const long ID_ROW_MNU_TOGGLE_STRANDS;
        static const long ID_ROW_MNU_TOGGLE_NODES;
        static const long ID_ROW_MNU_CONVERT_TO_EFFECTS;
        static const long ID_ROW_MNU_PROMOTE_EFFECTS;
        static const long ID_ROW_MNU_COPY_ROW;
        static const long ID_ROW_MNU_PASTE_ROW;

        static const long ID_ROW_MNU_ADD_TIMING_TRACK;
        static const long ID_ROW_MNU_RENAME_TIMING_TRACK;
        static const long ID_ROW_MNU_DELETE_TIMING_TRACK;
        static const long ID_ROW_MNU_IMPORT_TIMING_TRACK;
        static const long ID_ROW_MNU_IMPORT_NOTES;
        static const long ID_ROW_MNU_EXPORT_TIMING_TRACK;
        static const long ID_ROW_MNU_IMPORT_LYRICS;
        static const long ID_ROW_MNU_BREAKDOWN_TIMING_PHRASES;
        static const long ID_ROW_MNU_BREAKDOWN_TIMING_WORDS;

};

#endif // ROWHEADING_H
