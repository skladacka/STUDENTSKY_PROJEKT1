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

#include "wx/wx.h"
#include "../../xlGridCanvas.h"
#include "../../sequencer/Effect.h"

wxDECLARE_EVENT(EVT_EFFECT_CHANGED, wxCommandEvent);
wxDECLARE_EVENT(EVT_IMAGE_FILE_SELECTED, wxCommandEvent);
wxDECLARE_EVENT(EVT_IMAGE_SIZE, wxCommandEvent);
wxDECLARE_EVENT(EVT_EYEDROPPER_COLOR, wxCommandEvent);

class xlGridCanvasPictures : public xlGridCanvas
{
    public:

        enum PaintMode
        {
            PAINT_PENCIL,
            PAINT_ERASER,
            PAINT_EYEDROPPER,
            PAINT_SELECTCOPY
        };

        xlGridCanvasPictures(wxWindow* parent, wxWindowID id, const wxPoint &pos=wxDefaultPosition,
                             const wxSize &size=wxDefaultSize,long style=0, const wxString &name=wxPanelNameStr);
        virtual ~xlGridCanvasPictures();

        virtual void SetEffect(Effect* effect_);
        void SetMessageParent(wxWindow* parent) { mMessageParent = parent; }
        void LoadImage();
        void SaveImage();
        void SaveAsImage();
        void ResizeImage();
        virtual void Copy();
        virtual void Paste();
        virtual void Cancel();

        void CreateNewImage(wxString& image_dir);
        void SetPaintColor( xlColor& color ) { mPaintColor = color; }
        void SetPaintMode( PaintMode mode ) { mPaintMode = mode; render(); }

    private:

        enum ImageDisplayMode
        {
            IMAGE_NONE,
            IMAGE_SINGLE_OVERSIZED,
            IMAGE_MULTIPLE_OVERSIZED,
            IMAGE_SINGLE_FITS,
            IMAGE_MULTIPLE_FITS
        };

        void mouseMoved(wxMouseEvent& event);
        void mouseRightDown(wxMouseEvent& event);
        void mouseLeftDown(wxMouseEvent& event);
        void mouseDown(int x, int y);
        void mouseLeftUp(wxMouseEvent& event);
        void mouseRightUp(wxMouseEvent& event);
        void mouseUp();
        void render(wxPaintEvent& event);
        void render();
        void DrawPicturesEffect(xlGraphicsContext *ctx);
        void DrawSelection(xlGraphicsContext *ctx);
        void LoadAndProcessImage();
        void ProcessNewImage();
        wxString GetImageFilename();
        void SaveImageToFile();
        void UpdateRenderedImage();
        void CalcSelection();
        void ProcessHoverDrag(int column, int row);

        bool mRightDown;
        bool mLeftDown;

        enum ImageDisplayMode img_mode;
        bool mModified;
        int imageCount;
        int imageIndex;
        int imageWidth;
        int imageHeight;
        int frame;
        int maxmovieframes;
        bool use_ping;
        double scaleh;
        double scalew;
        wxImage image;
        wxImage image_copy;
        wxString PictureName;
        wxString NewPictureName;
        xlColor mPaintColor;
        xlColor mEraseColor;
        PaintMode mPaintMode;
        xlColor* mSelectionColor;
        wxWindow* mMessageParent;

        int mDragStartX;
        int mDragStartY;
        int mDragEndX;
        int mDragEndY;
        int mHoverDragRow;
        int mHoverDragCol;
        int mStartRow;
        int mStartCol;
        int mEndRow;
        int mEndCol;
        bool mHoverSelection;
        bool mCopyAvailable;


    bool imagesValid;
    wxImage scaledImage;
    xlTexture *imageTexture;
    xlTexture *imageCopy;


    DECLARE_EVENT_TABLE()
};
