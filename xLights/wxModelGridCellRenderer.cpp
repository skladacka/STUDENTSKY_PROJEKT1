/***************************************************************
 * This source files comes from the xLights project
 * https://www.xlights.org
 * https://github.com/xLightsSequencer/xLights
 * See the github commit history for a record of contributing
 * developers.
 * Copyright claimed based on commit dates recorded in Github
 * License: https://github.com/xLightsSequencer/xLights/blob/master/License.txt
 **************************************************************/

#include "wxModelGridCellRenderer.h"

wxModelGridCellRenderer::wxModelGridCellRenderer(wxImage* image_, wxGrid& grid)
: image(image_),
  draw_picture(true),
  lightness(0)
{
    UpdateSize(grid, true, lightness);
}

void wxModelGridCellRenderer::Draw(wxGrid &grid, wxGridCellAttr &attr, wxDC &dc, const wxRect &rect, int row, int col, bool isSelected)
{
    // erase only this cells background
    if( !isSelected ) {
        wxGridCellRenderer::Draw(grid, attr, dc, rect, row, col, isSelected);
    }

    // draw bitmap slice
    if( image != nullptr && draw_picture ) {
        if (bmp.IsOk()) {
            if( (rect.x + rect.width ) <= bmp.GetWidth() &&
                (rect.y + rect.height) <= bmp.GetHeight() ) {
                dc.Blit(rect.x, rect.y, rect.width, rect.height, &bmpDC, rect.x, rect.y);
            }
        }
    }

    // draw selection color over image if needed
    if( isSelected ) {
        wxGridCellRenderer::Draw(grid, attr, dc, rect, row, col, isSelected);
    }

    // draw the text
    SetTextColoursAndFont(grid, attr, dc, isSelected);
    grid.DrawTextRectangle(dc, grid.GetCellValue(row, col), rect,  wxALIGN_CENTRE,  wxALIGN_CENTRE);
}

void wxModelGridCellRenderer::UpdateSize(wxGrid& grid, bool draw_picture_, int lightness_)
{
    draw_picture = draw_picture_;
    lightness = lightness_;
    DetermineGridSize(grid);
    CreateImage();
}

void wxModelGridCellRenderer::CreateImage()
{
    if( image != nullptr && image->IsOk())
    {
        wxImage img(*image);
        img.Rescale(width, height);

        if (img.IsOk()) {
            img.InitAlpha();
            int alpha = (100 - lightness) * 255 / 100;

            for (int x = 0; x < img.GetWidth(); x++) {
                for (int y = 0; y < img.GetHeight(); y++) {
                    //int temp = img.GetAlpha(x, y);
                    img.SetAlpha(x, y, alpha);
                }
            }
            bmp = wxBitmap(img);
            if (bmp.IsOk()) {
                wxBitmap bmp2(width, height);
                bmpDC.SelectObjectAsSource(bmp2);
                wxBrush b(wxSystemSettings::GetColour(wxSYS_COLOUR_LISTBOX));
                wxPen p(wxSystemSettings::GetColour(wxSYS_COLOUR_LISTBOX));
                bmpDC.SetBrush(b);
                bmpDC.SetBackground(b);
                bmpDC.SetPen(p);
                bmpDC.DrawRectangle(0, 0, width, height);
                bmpDC.DrawBitmap(bmp, 0, 0);
            } else {
                bmpDC.SelectObjectAsSource(wxNullBitmap);
            }
        }
    }
}

void wxModelGridCellRenderer::SetImage(wxImage* image_)
{
    image = image_;
    CreateImage();
}

void wxModelGridCellRenderer::DetermineGridSize(wxGrid& grid)
{
    wxFont font = grid.GetDefaultCellFont();
    width = 0;
    height = 0;
    for (int c = 0; c < grid.GetNumberCols(); ++c) {
        width += 2 * font.GetPixelSize().y;
    }
    for (int r = 0; r < grid.GetNumberRows(); ++r) {
        height += int(1.5 * (float)font.GetPixelSize().y);
    }
}
