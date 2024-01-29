/***************************************************************
 * This source files comes from the xLights project
 * https://www.xlights.org
 * https://github.com/xLightsSequencer/xLights
 * See the github commit history for a record of contributing
 * developers.
 * Copyright claimed based on commit dates recorded in Github
 * License: https://github.com/xLightsSequencer/xLights/blob/master/License.txt
 **************************************************************/

#include <wx/dcclient.h>
#include <wx/stopwatch.h>
#include <wx/display.h>

#include "PlayerWindow.h"
#include "../VirtualMatrix.h"

extern "C"
{
    #include <libswscale/swscale.h>
    #include <libavutil/frame.h>
}

#include <log4cpp/Category.hh>

BEGIN_EVENT_TABLE(PlayerWindow, wxWindow)
    EVT_MOTION(PlayerWindow::OnMouseMove)
    EVT_LEFT_DOWN(PlayerWindow::OnMouseLeftDown)
    EVT_LEFT_UP(PlayerWindow::OnMouseLeftUp)
    EVT_PAINT(PlayerWindow::Paint)
END_EVENT_TABLE()

PlayerWindow::PlayerWindow(wxWindow* parent, bool topMost, wxImageResizeQuality quality, int swsQuality, wxWindowID id, const wxPoint& pos, const wxSize& size)
{
    _quality = quality;
    _swsQuality = swsQuality;
    _image = wxImage(size, true);
    _inputImage = wxImage(1, 1, true);
    _imageChanged = false;
    SetDoubleBuffered(true);
    _dragging = false;

    wxWindow* wind = FindFocus();

    if (topMost) {
        Create(parent, id, "Player Window", pos, size, wxBORDER_NONE | wxSTAY_ON_TOP, _T("id"));
    }
    else {
        Create(parent, id, "Player Window", pos, size, wxBORDER_NONE, _T("id"));
    }

    SetBackgroundColour(*wxBLACK);
    SetClientSize(size);
    Move(pos);
    Show();

    Connect(wxEVT_LEFT_DOWN, (wxObjectEventFunction)&PlayerWindow::OnMouseLeftDown, 0, this);
    Connect(wxEVT_LEFT_UP, (wxObjectEventFunction)&PlayerWindow::OnMouseLeftUp, 0, this);
    Connect(wxEVT_MOTION, (wxObjectEventFunction)&PlayerWindow::OnMouseMove, 0, this);
    Connect(wxEVT_PAINT, (wxObjectEventFunction)&PlayerWindow::Paint, 0, this);

    // prevent this window from stealing focus
    if (wind != nullptr) {
        wind->SetFocus();
    }

    static log4cpp::Category& logger_base = log4cpp::Category::getInstance(std::string("log_base"));
    int w, h;
    GetSize(&w, &h);
    int x, y;
    GetPosition(&x, &y);
    logger_base.info("Player window created location (%d, %d) size (%d, %d) Quality: %s.", x, y, w, h, (const char*)VirtualMatrix::DecodeScalingQuality(quality, swsQuality).c_str());
}

PlayerWindow::~PlayerWindow()
{
}

bool PlayerWindow::PrepareImage()
{
    static log4cpp::Category& logger_frame = log4cpp::Category::getInstance(std::string("log_frame"));

    if (_imageChanged) {
        if (_mutex.try_lock_for(std::chrono::milliseconds(1))) {
            wxStopWatch sw;

            if (_inputImage.IsOk()) {
                logger_frame.debug("Updating Player Window image");

                int width = 0;
                int height = 0;
                GetSize(&width, &height);

                int srcWidth = _inputImage.GetWidth();
                int srcHeight = _inputImage.GetHeight();

                if (_swsQuality < 0) {
                    _image.Destroy();
                    _image = _inputImage.Copy();
                    if (srcWidth != width || srcHeight != height) {
                        _image.Rescale(width, height, _quality);
                    }
                }
                else {
                    // THIS DOES NOT WORK YET ... NOT SURE WHY
                    wxASSERT(false);
                    if (_image.GetWidth() != width || _image.GetHeight() != height) {
                        _image.Destroy();
                        _image = wxImage(width, height);
                    }

                    if (srcWidth != width || srcHeight != height) {

                        SwsContext* swsCtx = sws_getContext(srcWidth, srcHeight, AVPixelFormat::AV_PIX_FMT_RGB24,
                            width, height, AVPixelFormat::AV_PIX_FMT_RGB24,
                            _swsQuality, nullptr, nullptr, nullptr);

                        const int srcRow = srcWidth * 3;
                        const int dstRow = width * 3;
                        const uint8_t* srcPtr = (uint8_t*)_inputImage.GetData();
                        uint8_t* const dstPtr = (uint8_t*)_image.GetData();

                        sws_scale(swsCtx,
                            &srcPtr, &srcRow,
                            0, height,
                            &dstPtr, &dstRow);

                        sws_freeContext(swsCtx);
                    }
                    else {
                        _image.Destroy();
                        _image = _inputImage.Copy();
                    }
                }
                logger_frame.debug("Player Window image updated %ldms", sw.Time());
            }
            _mutex.unlock();
        }
        else {
            return false;
        }
    }

    return true;
}

void PlayerWindow::SetImage(const wxImage& image)
{
    static log4cpp::Category& logger_frame = log4cpp::Category::getInstance(std::string("log_frame"));

    if (image.IsOk()) {
        int srcWidth = image.GetWidth();
        int srcHeight = image.GetHeight();

        std::unique_lock<std::timed_mutex> lock(_mutex);

        int tgtWidth = _inputImage.GetWidth();
        int tgtHeight = _inputImage.GetHeight();

        bool changed = srcWidth != tgtWidth ||
            srcHeight != tgtHeight ||
            memcmp(_inputImage.GetData(), image.GetData(), srcWidth * srcHeight * 3) != 0;

        if (changed) {
            _inputImage.Destroy();
            _inputImage = image.Copy();
            _imageChanged = true;
            Refresh(false); // force a paint on the main thread
        }
    }
}

void PlayerWindow::Paint(wxPaintEvent& event)
{
    wxASSERT(wxThread::IsMain());

    // ensure the image is ready for drawing
    if (!PrepareImage()) {
        Refresh(false); // we failed to get the new image so we will need to paint again
    }

    wxPaintDC dc(this);
    dc.DrawBitmap(_image, 0, 0);
}

void PlayerWindow::OnMouseLeftUp(wxMouseEvent& event)
{
    if (_dragging) {
        int cwpx, cwpy;
        GetPosition(&cwpx, &cwpy);
        wxPoint currentWindowPos(cwpx, cwpy);

        int x = currentWindowPos.x + event.GetPosition().x - _startMousePos.x;
        int y = currentWindowPos.y + event.GetPosition().y - _startMousePos.y;

        Move(_startDragPos.x + x, _startDragPos.y + y);
        _dragging = false;
    }
}

void PlayerWindow::OnMouseLeftDown(wxMouseEvent& event)
{
    _dragging = true;
    int x, y;
    GetPosition(&x, &y);
    _startDragPos = wxPoint(x, y);
    _startMousePos = event.GetPosition() + _startDragPos;
}

void PlayerWindow::OnMouseMove(wxMouseEvent& event)
{
    if (_dragging) {
        int cwpx, cwpy;
        GetPosition(&cwpx, &cwpy);
        wxPoint currentWindowPos(cwpx, cwpy);

        int x = currentWindowPos.x + event.GetPosition().x - _startMousePos.x;
        int y = currentWindowPos.y + event.GetPosition().y - _startMousePos.y;

        Move(_startDragPos.x + x, _startDragPos.y + y);
    }
}
