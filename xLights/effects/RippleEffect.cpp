/***************************************************************
 * This source files comes from the xLights project
 * https://www.xlights.org
 * https://github.com/smeighan/xLights
 * See the github commit history for a record of contributing
 * developers.
 * Copyright claimed based on commit dates recorded in Github
 * License: https://github.com/smeighan/xLights/blob/master/License.txt
 **************************************************************/

#include "RippleEffect.h"
#include "RipplePanel.h"

#include "../sequencer/Effect.h"
#include "../RenderBuffer.h"
#include "../UtilClasses.h"


#include "../../include/ripple-16.xpm"
#include "../../include/ripple-24.xpm"
#include "../../include/ripple-32.xpm"
#include "../../include/ripple-48.xpm"
#include "../../include/ripple-64.xpm"

RippleEffect::RippleEffect(int id) : RenderableEffect(id, "Ripple", ripple_16, ripple_24, ripple_32, ripple_48, ripple_64)
{
    //ctor
}

RippleEffect::~RippleEffect()
{
    //dtor
}
xlEffectPanel *RippleEffect::CreatePanel(wxWindow *parent) {
    return new RipplePanel(parent);
}

#define RENDER_RIPPLE_CIRCLE     0
#define RENDER_RIPPLE_SQUARE     1
#define RENDER_RIPPLE_TRIANGLE   2
#define RENDER_RIPPLE_STAR       3
#define RENDER_RIPPLE_POLYGON    4
#define RENDER_RIPPLE_HEART      5
#define RENDER_RIPPLE_TREE       6
#define RENDER_RIPPLE_CANDYCANE  7
#define RENDER_RIPPLE_SNOWFLAKE  8
#define RENDER_RIPPLE_CRUCIFIX   9
#define RENDER_RIPPLE_PRESENT    10

#define MOVEMENT_EXPLODE    0
#define MOVEMENT_IMPLODE    1

void RippleEffect::SetDefaultParameters()
{
    RipplePanel* rp = (RipplePanel*)panel;
    if (rp == nullptr) {
        return;
    }

    rp->BitmapButton_Ripple_CyclesVC->SetActive(false);
    rp->BitmapButton_Ripple_ThicknessVC->SetActive(false);
    rp->BitmapButton_Ripple_RotationVC->SetActive(false);

    SetChoiceValue(rp->Choice_Ripple_Object_To_Draw, "Circle");
    SetChoiceValue(rp->Choice_Ripple_Movement, "Explode");

    SetSliderValue(rp->Slider_Ripple_Thickness, 3);
    SetSliderValue(rp->Slider_Ripple_Cycles, 10);
    SetSliderValue(rp->Slider_Ripple_Points, 5);
    SetSliderValue(rp->Slider_Ripple_Rotation, 0);

    SetCheckBoxValue(rp->CheckBox_Ripple3D, false);
    SetCheckBoxValue(rp->CheckBox_RippleSolid, false);
}

void RippleEffect::Render(Effect* effect, const SettingsMap& SettingsMap, RenderBuffer& buffer)
{
    float oset = buffer.GetEffectTimeIntervalPosition();
    const std::string& Object_To_DrawStr = SettingsMap["CHOICE_Ripple_Object_To_Draw"];
    const std::string& MovementStr = SettingsMap["CHOICE_Ripple_Movement"];
    int Ripple_Thickness = GetValueCurveInt("Ripple_Thickness", 3, SettingsMap, oset, RIPPLE_THICKNESS_MIN, RIPPLE_THICKNESS_MAX, buffer.GetStartTimeMS(), buffer.GetEndTimeMS());
    bool CheckBox_Ripple3D = SettingsMap.GetBool("CHECKBOX_Ripple3D", false);
    bool CheckBox_RippleSolid = SettingsMap.GetBool("CHECKBOX_RippleSolid", false);
    float cycles = GetValueCurveDouble("Ripple_Cycles", 1.0, SettingsMap, oset, RIPPLE_CYCLES_MIN, RIPPLE_CYCLES_MAX, buffer.GetStartTimeMS(), buffer.GetEndTimeMS(), 10);
    int points = SettingsMap.GetInt("SLIDER_RIPPLE_POINTS", 5);
    int rotation = GetValueCurveInt("Ripple_Rotation", 0, SettingsMap, oset, RIPPLE_ROTATION_MIN, RIPPLE_ROTATION_MAX, buffer.GetStartTimeMS(), buffer.GetEndTimeMS());
    int xcc = GetValueCurveInt("Ripple_XC", 0, SettingsMap, oset, RIPPLE_XC_MIN, RIPPLE_XC_MAX, buffer.GetStartTimeMS(), buffer.GetEndTimeMS());
    int ycc = GetValueCurveInt("Ripple_YC", 0, SettingsMap, oset, RIPPLE_YC_MIN, RIPPLE_YC_MAX, buffer.GetStartTimeMS(), buffer.GetEndTimeMS());

    int Object_To_Draw;
    if (Object_To_DrawStr == "Circle") {
        Object_To_Draw = RENDER_RIPPLE_CIRCLE;
    } else if (Object_To_DrawStr == "Square") {
        Object_To_Draw = RENDER_RIPPLE_SQUARE;
    } else if (Object_To_DrawStr == "Triangle") {
        Object_To_Draw = RENDER_RIPPLE_TRIANGLE;
    } else if (Object_To_DrawStr == "Star") {
        Object_To_Draw = RENDER_RIPPLE_STAR;
    } else if (Object_To_DrawStr == "Polygon") {
        Object_To_Draw = RENDER_RIPPLE_POLYGON;
    } else if (Object_To_DrawStr == "Heart") {
        Object_To_Draw = RENDER_RIPPLE_HEART;
    } else if (Object_To_DrawStr == "Tree") {
        Object_To_Draw = RENDER_RIPPLE_TREE;
    } else if (Object_To_DrawStr == "Candy Cane") {
        Object_To_Draw = RENDER_RIPPLE_CANDYCANE;
    } else if (Object_To_DrawStr == "Snow Flake") {
        Object_To_Draw = RENDER_RIPPLE_SNOWFLAKE;
    } else if (Object_To_DrawStr == "Crucifix") {
        Object_To_Draw = RENDER_RIPPLE_CRUCIFIX;
    } else if (Object_To_DrawStr == "Present") {
        Object_To_Draw = RENDER_RIPPLE_PRESENT;
    } else {
        Object_To_Draw = RENDER_RIPPLE_CIRCLE;
    }
    int Movement = "Explode" == MovementStr ? MOVEMENT_EXPLODE : MOVEMENT_IMPLODE;

    int ColorIdx;
    int xc, yc;

    HSVValue hsv;
    size_t colorcnt = buffer.GetColorCount();

    double position = buffer.GetEffectTimeIntervalPosition(cycles); // how far are we into the row> value is 0.0 to 1.0

    float rx = position;
    xc = buffer.BufferWi / 2 + xcc * (buffer.BufferWi / 2) / 100;
    yc = buffer.BufferHt / 2 + ycc * (buffer.BufferHt / 2) / 100;
    double maxRadius = 0.0;
    double maxRadiusX = std::max(xc, buffer.BufferWi - xc);
    double maxRadiusY = std::max(yc, buffer.BufferHt - yc);
    if (buffer.BufferWi > buffer.BufferHt) {
        maxRadius = maxRadiusX;
    } else {
        maxRadius = maxRadiusY;
    }

    ColorIdx = static_cast<int>(rx * colorcnt);
    if (ColorIdx == colorcnt)
        ColorIdx--; // ColorIdx goes from 0-3 when colorcnt goes from 1-4. Make sure that is true

    double radius;
    double radiusX;
    double radiusY;
    double side;
    if (Movement == MOVEMENT_IMPLODE) {
        radius = maxRadius - (maxRadius * rx);
        side = maxRadius - (maxRadius * rx);
        radiusX = maxRadiusX - (maxRadiusX * rx);
        radiusY = maxRadiusY - (maxRadiusY * rx);
    } else {
        radius = (maxRadius * rx);
        side = (maxRadius * rx);
        radiusX = (maxRadiusX * rx);
        radiusY = (maxRadiusY * rx);
    }

    buffer.palette.GetHSV(ColorIdx, hsv); // Now go and get the hsv value for this ColorIdx
    switch (Object_To_Draw) {
    case RENDER_RIPPLE_SQUARE: {
        int x1 = xc - radiusX;
        int x2 = xc + radiusX;
        int y1 = yc - radiusY;
        int y2 = yc + radiusY;
        Drawsquare(buffer, Movement, x1, x2, y1, y2, Ripple_Thickness, CheckBox_Ripple3D, hsv, CheckBox_RippleSolid);
    } break;
    case RENDER_RIPPLE_CIRCLE:
        Drawcircle(buffer, Movement, xc, yc, radius, hsv, Ripple_Thickness, CheckBox_Ripple3D);
        break;
    case RENDER_RIPPLE_STAR:
        Drawstar(buffer, Movement, xc, yc, radius, points, hsv, Ripple_Thickness, CheckBox_Ripple3D, rotation);
        break;
    case RENDER_RIPPLE_TRIANGLE:
        Drawtriangle(buffer, Movement, xc, yc, side, hsv, Ripple_Thickness, CheckBox_Ripple3D);
        break;
    case RENDER_RIPPLE_POLYGON:
        Drawpolygon(buffer, Movement, xc, yc, radius, points, hsv, Ripple_Thickness, CheckBox_Ripple3D, CheckBox_RippleSolid, rotation);
        break;
    case RENDER_RIPPLE_HEART:
        Drawheart(buffer, Movement, xc, yc, radius, hsv, Ripple_Thickness, CheckBox_Ripple3D);
        break;
    case RENDER_RIPPLE_TREE:
        Drawtree(buffer, Movement, xc, yc, radius, hsv, Ripple_Thickness, CheckBox_Ripple3D);
        break;
    case RENDER_RIPPLE_CANDYCANE:
        Drawcandycane(buffer, Movement, xc, yc, radius, hsv, Ripple_Thickness, CheckBox_Ripple3D);
        break;
    case RENDER_RIPPLE_SNOWFLAKE:
        Drawsnowflake(buffer, Movement, xc, yc, radius, points, hsv, Ripple_Thickness, CheckBox_Ripple3D, rotation);
        break;
    case RENDER_RIPPLE_CRUCIFIX:
        Drawcrucifix(buffer, Movement, xc, yc, radius, hsv, Ripple_Thickness, CheckBox_Ripple3D);
        break;
    case RENDER_RIPPLE_PRESENT:
        Drawpresent(buffer, Movement, xc, yc, radius, hsv, Ripple_Thickness, CheckBox_Ripple3D);
        break;
    default:
        wxASSERT(false);
        break;
    }
}

static void FillRegion(RenderBuffer& buffer, const std::vector<std::pair<int, int>>& oldpoints, const std::vector<std::pair<int, int>>& newpoints, const xlColor& color, bool close = true)
{
    if (oldpoints.empty())
        return;
    if (newpoints.size() != oldpoints.size())
        return;

    std::vector<std::pair<int, int>> quad(4);
    for (size_t i = 0; i < newpoints.size() - 1; ++i) {
        quad[0] = oldpoints[i];
        quad[1] = oldpoints[i + 1];
        quad[2] = newpoints[i + 1];
        quad[3] = newpoints[i];

        buffer.FillConvexPoly(quad, color);
    }

    if (close) {
        quad[0] = oldpoints[oldpoints.size() - 1];
        quad[1] = oldpoints[0];
        quad[2] = newpoints[0];
        quad[3] = newpoints[oldpoints.size() - 1];
        buffer.FillConvexPoly(quad, color);
    }
}

// It is somewhat redundant with 3-sided polygon isn't it?
//  Especially now that it is equilateral
static void getTrianglePoints(std::vector<std::pair<double, double>>& tpts)
{
    tpts.resize(3);
    #define ROOT3DIV3 0.577350269
    #define SIN30 0.5
    #define COS30 0.866025404

    tpts[0] = { 0, 1 };
    tpts[1] = { -COS30, -SIN30 };
    tpts[2] = { COS30, SIN30 };
}

// Now see, the square becomes a rectangle later, if desired
// It is somewhat redundant with 4-side polygon isn't it...
static void getSquarePoints(std::vector<std::pair<double, double>>& spts)
{
    spts.resize(4);
    spts[0] = {  1,  1 };
    spts[1] = {  1, -1 };
    spts[0] = { -1, -1 };
    spts[0] = { -1,  1 };
}

// Get polygon points; someone else applies that
static void getPolygonPoints(std::vector<std::pair<double, double>>& ppts, int points)
{
    double rotation = 0;
    if (points % 2 != 0)
        rotation += 90;
    if (points == 4)
        rotation += 45;
    if (points == 8)
        rotation += 22.5;

    double increment = 360.0 / points;
    ppts.resize(points);

    for (int i = 0; i<points; ++i, rotation += increment)
    {
        double radian = (rotation) * M_PI / 180.0;
        ppts[i] = { cos(radian), sin(radian) };
    }
}

// OK can we just admit it is a polygon with a lot of points?
static void getCirclePoints(std::vector<std::pair<double, double>>& ppts)
{
    getPolygonPoints(ppts, 100);
}

static void getCrossPoints(std::vector<std::pair<double, double>>& ppts)
{
    const wxPoint points[] = { wxPoint(2, 0),
                            wxPoint(2, 6),
                            wxPoint(0, 6),
                            wxPoint(0, 7),
                            wxPoint(2, 7),
                            wxPoint(2, 10),
                            wxPoint(3, 10), 
                            wxPoint(3, 7),
                            wxPoint(5, 7),
                            wxPoint(5, 6),
                            wxPoint(3, 6),
                            wxPoint(3, 0)};
    ppts.clear();
    for (const auto &pt : points) {
        ppts.push_back({ (pt.x - 2.5) / 7.0, (pt.y - 6.5) / 10 });
    }
}

static void getTreePoints(std::vector<std::pair<double, double>>& ppts)
{
    const wxPoint points[] = {
        wxPoint(3, 3),
        wxPoint(3, 0),
        wxPoint(5, 0),
        wxPoint(5, 3),
        wxPoint(0, 3),
        wxPoint(2, 6),
        wxPoint(1, 6),
        wxPoint(3, 9),
        wxPoint(2, 9),
        wxPoint(4, 11),
        wxPoint(6, 9),
        wxPoint(5, 9),
        wxPoint(7, 6),
        wxPoint(6,6),
        wxPoint(8,3)
    };
    ppts.clear();
    for (const auto& pt : points) {
        ppts.push_back({ (pt.x - 4.0) / 11.0, (pt.y - 4.0) / 11.0 });
    }
}

static void getPresentPoints(std::vector<std::pair<double, double>>& ppts)
{
    const wxPoint points[] = {
        wxPoint(5, 9),
        wxPoint(2, 11),
        wxPoint(2, 9),
        wxPoint(5, 9),
        wxPoint(8, 11),
        wxPoint(5, 9),
        wxPoint(0, 9),
        wxPoint(0, 0),
        wxPoint(10, 0),
        wxPoint(10, 9),
        wxPoint(5, 9),
        wxPoint(5, 0)
    };

    for (const auto& pt : points) {
        ppts.push_back({ (pt.x - 5.0) / 7.0, (pt.y - 5.5) / 10.0 });
    }
}

static void getHeartPoints(std::vector<std::pair<double, double>>& pts)
{
    std::vector<std::pair<double, double>> rpts;

    double xincr = 1.0 / 64;
    for (double x = -2.0; x <= 2.0; x += xincr) {
        double y1 = std::sqrt(1.0 - (std::abs(x) - 1.0) * (std::abs(x) - 1.0));
        double y2 = std::acos(1.0 - std::abs(x)) - M_PI;

        double xx1 = std::round(x / 2.0);
        double yy1 = (y1) / 2.0;
        double yy2 = (y2) / 2.0;

        pts.push_back({ xx1, yy1 });
        rpts.push_back({ xx1, yy2 });
    }

    while (!rpts.empty()) {
        pts.push_back(rpts.back());
        rpts.pop_back();
    }
}

static void getCanePoints(std::vector<std::pair<double, double>>& pts)
{
    // the stick
    double ys1 = 1.0 / 6;
    double ys2 = -1.0 / 2;
    double xs = 1.0 / 2;
    pts.clear();
    pts.push_back({ xs, ys2 });
    pts.push_back({ xs, ys1 });

    // The hook
    for (double degrees = 0.0; degrees < 180; degrees += 1.0) {
        double radian = degrees * (M_PI / 180.0);
        pts.push_back({cos(radian)/3, sin(radian)/3 + ys1 });
    }
    // MoC: The candy cane is a bit smaller than other things, and squat, so it is tempting to increase height a bit
}

static void getStarPoints(std::vector<std::pair<double, double>>& pts, int npts)
{
    double offsetangle = 90 - 360 / npts; // May as well be 90.  We just want a point that is up.

    double InnerRadius = 1.0 / 2.618034; // divide by golden ratio squared

    double increment = 360.0 / npts;

    double deg = offsetangle;
    for (int i = 0; i<npts; deg += increment, ++i) {
        double radian = (deg) * (M_PI / 180.0);
        double xouter = cos(radian);
        double youter = sin(radian);

        radian = (deg + increment / 2.0) * (M_PI / 180.0);
        double xinner = InnerRadius * cos(radian);
        double yinner = InnerRadius * sin(radian);

        pts.push_back({ xouter, youter });
        pts.push_back({ xinner, yinner });
    }
}

static void getSnowflakePoints(std::vector<std::pair<double, double>>& pts, int npts)
{
    // The original is not, really, all that good for our purpose as it has no width.  Considering what to do instead.
    npts *= 2;

    double increment = 360.0 / npts;
    pts.resize(npts * 3);

    double rotation = 0; // The original always has spoke horizontal, don't like it, set rotation to 90

    for (int i = 0; i < npts; ++i, rotation += increment) {
        double delta = increment / 20;
        double inrad = .05;

        double r1 = (rotation - delta) * M_PI / 180.0;
        double r2 = (rotation + delta) * M_PI / 180.0;
        double r3 = (rotation + increment / 2) * M_PI / 180.0;

        pts[i * 3 + 0] = { cos(r1), sin(r1) };
        pts[i * 3 + 1] = { cos(r2), sin(r2) };
        pts[i * 3 + 2] = { inrad*cos(r3), inrad * sin(r3) };
    }
}

// TODO:
//   AND THEN the logic for doing something with it.
//     Like shockwave - fade in and out?
//     Acceleration, R1 & R2 explict, width1, width2?
//   Shape has:
//     Growth
//     Direction, velocity, SVG
//     Thickness?  firing with timing or music?
//   AND THEN the colors
//     Temporally, radially, or outward
//   AND THEN debug the abrash routine or switch to the brush routine
//   AND THEN svg

void RippleEffect::Drawtriangle(RenderBuffer& buffer, int Movement, int xc, int yc, double side, HSVValue& hsv, int Ripple_Thickness, int CheckBox_Ripple3D)
{
    if (side < 0)
        return;

    xlColor color(hsv);

    for (int i = 0; i < Ripple_Thickness; i++) {
        double radius = (side + i) * ROOT3DIV3;
        double ytop = yc + radius;
        int xtop = xc;

        double xleft = xc - radius * COS30;
        double yleft = yc - radius * SIN30;

        double xright = xleft + side + i; // This is kinda weird
        double yright = yleft;

        if (CheckBox_Ripple3D) {
            if (buffer.allowAlpha) {
                color.alpha = 255.0 * (1.0 - ((float(i) / 2.0) / float(Ripple_Thickness)));
            } else {
                hsv.value *= 1.0 - ((float(i) / 2.0) / float(Ripple_Thickness)); // we multiply by 1.0 when Ripple_Thickness=0
                color = hsv;
            }
        }

        buffer.DrawLine(xtop, ytop, xleft, yleft, color);
        buffer.DrawLine(xtop, ytop, xright, yright, color);
        buffer.DrawLine(xleft, yleft, xright, yright, color);
    }
}

void RippleEffect::Drawsquare(RenderBuffer& buffer, int Movement, int x1, int x2, int y1, int y2, int Ripple_Thickness, int CheckBox_Ripple3D, HSVValue& hsv, int CheckBox_RippleSolid)
{
    xlColor color(hsv);

    for (int i = 0; i < Ripple_Thickness; i++) {
        if (CheckBox_Ripple3D) {
            if (buffer.allowAlpha) {
                color.alpha = 255.0 * (1.0 - ((float(i) / 2.0) / float(Ripple_Thickness)));
            } else {
                hsv.value *= 1.0 - ((float(i) / 2.0) / float(Ripple_Thickness)); // we multiply by 1.0 when Ripple_Thickness=0
                color = hsv;
            }
        }
        if (Movement == MOVEMENT_EXPLODE) {
            if (CheckBox_RippleSolid) {
                for (int y = y1 + i; y <= y2 - i; y++) {
                    for (int x = x1 + i; x <= x2 - i; x++) {
                        buffer.SetPixel(x, y, color);
                    }
                }
            } else {
                for (int y = y1 + i; y <= y2 - i; y++) {
                    buffer.SetPixel(x1 + i, y, color); // Turn pixel
                    buffer.SetPixel(x2 - i, y, color); // Turn pixel
                }
                for (int x = x1 + i; x <= x2 - i; x++) {
                    buffer.SetPixel(x, y1 + i, color); // Turn pixel
                    buffer.SetPixel(x, y2 - i, color); // Turn pixel
                }
            }
        }
        if (Movement == MOVEMENT_IMPLODE) {
            if (CheckBox_RippleSolid) {
                for (int y = y2 + i; y >= y1 - i; y--) {
                    for (int x = x2 + i; x >= x1 - i; x--) {
                        buffer.SetPixel(x, y, color);
                    }
                }            
            } else {
                for (int y = y2 + i; y >= y1 - i; y--) {
                    buffer.SetPixel(x1 - i, y, color); // Turn pixel
                    buffer.SetPixel(x2 + i, y, color); // Turn pixel
                }
                for (int x = x2 + i; x >= x1 - i; x--) {
                    buffer.SetPixel(x, y1 - i, color); // Turn pixel
                    buffer.SetPixel(x, y2 + i, color); // Turn pixel
                }
            }
        }
    }
}

void RippleEffect::Drawcircle(RenderBuffer& buffer, int Movement, int xc, int yc, double radius, HSVValue& hsv, int Ripple_Thickness, int CheckBox_Ripple3D)
{
    xlColor color(hsv);

    for (float i = 0; i < Ripple_Thickness; i += .5) {
        if (CheckBox_Ripple3D) {
            if (buffer.allowAlpha) {
                color.alpha = 255.0 * (1.0 - (float(i) / float(Ripple_Thickness)));
            } else {
                hsv.value *= 1.0 - (float(i) / float(Ripple_Thickness)); // we multiply by 1.0 when steps=0
                color = hsv;
            }
        }
        if (Movement == MOVEMENT_EXPLODE) {
            radius = radius + i;
        } else {
            radius = radius - i;
        }

        if (radius >= 0.0) {
            for (double degrees = 0.0; degrees < 360.0; degrees += 1.0) {
                double radian = degrees * (M_PI / 180.0);
                int x = radius * cos(radian) + xc;
                int y = radius * sin(radian) + yc;
                buffer.SetPixel(x, y, color); // Turn pixel
            }
        }
    }
}

void RippleEffect::Drawstar(RenderBuffer& buffer, int Movement, int xc, int yc, double radius, int points, HSVValue& hsv, int Ripple_Thickness, int CheckBox_Ripple3D, float rotation)
{
    double offsetangle = 0.0;
    switch (points) {
    case 3:
        offsetangle = 90.0 - 360.0 / 3;
    case 4:
        break;
    case 5:
        offsetangle = 90.0 - 360.0 / 5;
        break;
    case 6:
        offsetangle = 30.0;
        break;
    case 7:
        offsetangle = 90.0 - 360.0 / 7;
        break;
    case 8:
        offsetangle = 90.0 - 360.0 / 8;
        break;
    default:
        wxASSERT(false);
        break;
    }

    xlColor color(hsv);

    for (double i = 0; i < Ripple_Thickness; i += .5) {
        if (CheckBox_Ripple3D) {
            if (buffer.allowAlpha) {
                color.alpha = 255.0 * (1.0 - (float(i) / float(Ripple_Thickness)));
            } else {
                hsv.value *= 1.0 - (float(i) / float(Ripple_Thickness)); // we multiply by 1.0 when steps=0
                color = hsv;
            }
        }
        if (Movement == MOVEMENT_EXPLODE) {
            radius = radius + i;
        } else {
            radius = radius - i;
        }

        if (radius >= 0.0) {
            double InnerRadius = radius / 2.618034; // divide by golden ratio squared

            double increment = 360.0 / points;

            for (double degrees = 0.0; degrees < 361.0; degrees += increment) // 361 because it allows for small rounding errors
            {
                if (degrees > 360.0)
                    degrees = 360.0;
                double radian = (rotation + offsetangle + degrees) * (M_PI / 180.0);
                int xouter = radius * cos(radian) + xc;
                int youter = radius * sin(radian) + yc;

                radian = (rotation + offsetangle + degrees + increment / 2.0) * (M_PI / 180.0);
                int xinner = InnerRadius * cos(radian) + xc;
                int yinner = InnerRadius * sin(radian) + yc;

                buffer.DrawLine(xinner, yinner, xouter, youter, color);

                radian = (rotation + offsetangle + degrees - increment / 2.0) * (M_PI / 180.0);
                xinner = InnerRadius * cos(radian) + xc;
                yinner = InnerRadius * sin(radian) + yc;

                buffer.DrawLine(xinner, yinner, xouter, youter, color);
            }
        }
    }
}

void RippleEffect::Drawpolygon(RenderBuffer& buffer, int Movement, int xc, int yc, double radius, int points, HSVValue& hsv, int Ripple_Thickness, int CheckBox_Ripple3D, int CheckBox_Solid, float rotation)
{
    double increment = 360.0 / points;

    if (points % 2 != 0)
        rotation += 90;
    if (points == 4)
        rotation += 45;
    if (points == 8)
        rotation += 22.5;

    xlColor color(hsv);

    std::vector<std::pair<int, int>> oldpts, newpts;

    for (double i = 0; i < Ripple_Thickness; i += .5) {
        if (CheckBox_Ripple3D) {
            if (buffer.allowAlpha) {
                color.alpha = 255.0 * (1.0 - (float(i) / float(Ripple_Thickness)));
            } else {
                hsv.value *= 1.0 - (float(i) / float(Ripple_Thickness)); // we multiply by 1.0 when steps=0
                color = hsv;
            }
        }
        if (Movement == MOVEMENT_EXPLODE) {
            radius = radius + i;
        } else {
            radius = radius - i;
        }

        if (radius >= 0) {
            if (CheckBox_Solid) {
                oldpts = newpts;
                newpts.clear();

                for (double degrees = 0.0; degrees < 359.5; degrees += increment) // 359 because it allows for small rounding errors
                {
                    double radian = (rotation + degrees) * M_PI / 180.0;
                    int x1 = std::round(radius * cos(radian)) + xc;
                    int y1 = std::round(radius * sin(radian)) + yc;
                    newpts.push_back({ x1, y1 });
                }
                FillRegion(buffer, oldpts, newpts, color);
            } else {
                for (double degrees = 0.0; degrees < 361.0; degrees += increment) // 361 because it allows for small rounding errors
                {
                    if (degrees > 360.0)
                        degrees = 360.0;
                    double radian = (rotation + degrees) * M_PI / 180.0;
                    int x1 = std::round(radius * cos(radian)) + xc;
                    int y1 = std::round(radius * sin(radian)) + yc;

                    radian = (rotation + degrees + increment) * M_PI / 180.0;
                    int x2 = std::round(radius * cos(radian)) + xc;
                    int y2 = std::round(radius * sin(radian)) + yc;

                    buffer.DrawLine(x1, y1, x2, y2, color);

                    if (degrees == 360.0)
                        degrees = 361.0;
                }
            }
        } else {
            break;
        }
    }
}

void RippleEffect::Drawsnowflake(RenderBuffer& buffer, int Movement, int xc, int yc, double radius, int points, HSVValue& hsv, int Ripple_Thickness, int CheckBox_Ripple3D, double rotation)
{
    double increment = 360.0 / (points * 2);
    double angle = rotation;

    xlColor color(hsv);

    if (radius >= 0) {
        for (int i = 0; i < points * 2; i++) {
            double radian = angle * M_PI / 180.0;

            int x1 = std::round(radius * cos(radian)) + xc;
            int y1 = std::round(radius * sin(radian)) + yc;

            radian = (180 + angle) * M_PI / 180.0;

            int x2 = std::round(radius * cos(radian)) + xc;
            int y2 = std::round(radius * sin(radian)) + yc;

            buffer.DrawLine(x1, y1, x2, y2, color);

            angle += increment;
        }
    }
}

void RippleEffect::Drawheart(RenderBuffer& buffer, int Movement, int xc, int yc, double radius, HSVValue& hsv, int Ripple_Thickness, int CheckBox_Ripple3D)
{
    xlColor color(hsv);

    for (float i = 0; i < Ripple_Thickness; i += 0.5) {
        if (CheckBox_Ripple3D) {
            if (buffer.allowAlpha) {
                color.alpha = 255.0 * (1.0 - (float(i) / float(Ripple_Thickness)));
            } else {
                hsv.value *= 1.0 - (float(i) / float(Ripple_Thickness)); // we multiply by 1.0 when steps=0
                color = hsv;
            }
        }

        if (Movement == MOVEMENT_EXPLODE) {
            radius = radius + i;
        } else {
            radius = radius - i;
        }

        if (radius >= 0) {
            double xincr = 0.01;
            for (double x = -2.0; x <= 2.0; x += xincr) {

                double y1 = std::sqrt(1.0 - (std::abs(x) - 1.0) * (std::abs(x) - 1.0));
                double y2 = std::acos(1.0 - std::abs(x)) - M_PI;

                double xx1 = std::round((x * radius) / 2.0) + xc;
                double yy1 = (y1 * radius) / 2.0 + yc;
                double yy2 = (y2 * radius) / 2.0 + yc;

                if (radius >= 0) {
                    buffer.SetPixel(xx1, std::round(yy1), color);
                    buffer.SetPixel(xx1, std::round(yy2), color);

                    if (x + xincr > 2.0 || x == -2.0 + xincr) {

                        if (yy1 > yy2)
                            std::swap(yy1, yy2);

                        for (double z = yy1; z < yy2; z += 0.5) {
                            buffer.SetPixel(xx1, std::round(z), color);
                        }
                    }
                } else {
                    break;
                }
            }
        } else {
            break;
        }
    }
}

void RippleEffect::Drawtree(RenderBuffer& buffer, int Movement, int xc, int yc, double radius, HSVValue& hsv, int Ripple_Thickness, int CheckBox_Ripple3D)
{
    struct line {
        wxPoint start;
        wxPoint end;

        line(const wxPoint s, const wxPoint e)
        {
            start = s;
            end = e;
        }
    };

    const line points[] = { line(wxPoint(3, 0), wxPoint(5, 0)),
                            line(wxPoint(5, 0), wxPoint(5, 3)),
                            line(wxPoint(3, 0), wxPoint(3, 3)),
                            line(wxPoint(0, 3), wxPoint(8, 3)),
                            line(wxPoint(0, 3), wxPoint(2, 6)),
                            line(wxPoint(8, 3), wxPoint(6, 6)),
                            line(wxPoint(1, 6), wxPoint(2, 6)),
                            line(wxPoint(6, 6), wxPoint(7, 6)),
                            line(wxPoint(1, 6), wxPoint(3, 9)),
                            line(wxPoint(7, 6), wxPoint(5, 9)),
                            line(wxPoint(2, 9), wxPoint(3, 9)),
                            line(wxPoint(5, 9), wxPoint(6, 9)),
                            line(wxPoint(6, 9), wxPoint(4, 11)),
                            line(wxPoint(2, 9), wxPoint(4, 11)) };
    int count = sizeof(points) / sizeof(line);

    xlColor color(hsv);

    for (float i = 0; i < Ripple_Thickness; i += .5) {
        if (CheckBox_Ripple3D) {
            if (buffer.allowAlpha) {
                color.alpha = 255.0 * (1.0 - (float(i) / float(Ripple_Thickness)));
            } else {
                hsv.value *= 1.0 - (float(i) / float(Ripple_Thickness)); // we multiply by 1.0 when steps=0
                color = hsv;
            }
        }
        if (Movement == MOVEMENT_EXPLODE) {
            radius = radius + i;
        } else {
            radius = radius - i;
        }
        if (radius >= 0) {
            for (int j = 0; j < count; ++j) {
                int x1 = std::round(((double)points[j].start.x - 4.0) / 11.0 * radius);
                int y1 = std::round(((double)points[j].start.y - 4.0) / 11.0 * radius);
                int x2 = std::round(((double)points[j].end.x - 4.0) / 11.0 * radius);
                int y2 = std::round(((double)points[j].end.y - 4.0) / 11.0 * radius);
                buffer.DrawLine(xc + x1, yc + y1, xc + x2, yc + y2, color);
            }
        } else {
            break;
        }
    }
}

void RippleEffect::Drawcrucifix(RenderBuffer& buffer, int Movement, int xc, int yc, double radius, HSVValue& hsv, int Ripple_Thickness, int CheckBox_Ripple3D)
{
    struct line {
        wxPoint start;
        wxPoint end;

        line(const wxPoint s, const wxPoint e)
        {
            start = s;
            end = e;
        }
    };

    const line points[] = { line(wxPoint(2, 0), wxPoint(2, 6)),
                            line(wxPoint(2, 6), wxPoint(0, 6)),
                            line(wxPoint(0, 6), wxPoint(0, 7)),
                            line(wxPoint(0, 7), wxPoint(2, 7)),
                            line(wxPoint(2, 7), wxPoint(2, 10)),
                            line(wxPoint(2, 10), wxPoint(3, 10)),
                            line(wxPoint(3, 10), wxPoint(3, 7)),
                            line(wxPoint(3, 7), wxPoint(5, 7)),
                            line(wxPoint(5, 7), wxPoint(5, 6)),
                            line(wxPoint(5, 6), wxPoint(3, 6)),
                            line(wxPoint(3, 6), wxPoint(3, 0)),
                            line(wxPoint(3, 0), wxPoint(2, 0)) };
    int count = sizeof(points) / sizeof(line);

    xlColor color(hsv);

    for (float i = 0; i < Ripple_Thickness; i += .5) {
        if (CheckBox_Ripple3D) {
            if (buffer.allowAlpha) {
                color.alpha = 255.0 * (1.0 - (float(i) / float(Ripple_Thickness)));
            } else {
                hsv.value *= 1.0 - (float(i) / float(Ripple_Thickness)); // we multiply by 1.0 when steps=0
                color = hsv;
            }
        }
        if (Movement == MOVEMENT_EXPLODE) {
            radius = radius + i;
        } else {
            radius = radius - i;
        }
        if (radius >= 0) {
            for (int j = 0; j < count; ++j) {
                int x1 = std::round(((double)points[j].start.x - 2.5) / 7.0 * radius);
                int y1 = std::round(((double)points[j].start.y - 6.5) / 10.0 * radius);
                int x2 = std::round(((double)points[j].end.x - 2.5) / 7.0 * radius);
                int y2 = std::round(((double)points[j].end.y - 6.5) / 10.0 * radius);
                buffer.DrawLine(xc + x1, yc + y1, xc + x2, yc + y2, color);
            }
        } else {
            break;
        }
    }
}

void RippleEffect::Drawpresent(RenderBuffer& buffer, int Movement, int xc, int yc, double radius, HSVValue& hsv, int Ripple_Thickness, int CheckBox_Ripple3D)
{
    struct line {
        wxPoint start;
        wxPoint end;

        line(const wxPoint s, const wxPoint e)
        {
            start = s;
            end = e;
        }
    };

    const line points[] = { line(wxPoint(0, 0), wxPoint(0, 9)),
                            line(wxPoint(0, 9), wxPoint(10, 9)),
                            line(wxPoint(10, 9), wxPoint(10, 0)),
                            line(wxPoint(10, 0), wxPoint(0, 0)),
                            line(wxPoint(5, 0), wxPoint(5, 9)),
                            line(wxPoint(5, 9), wxPoint(2, 11)),
                            line(wxPoint(2, 11), wxPoint(2, 9)),
                            line(wxPoint(5, 9), wxPoint(8, 11)),
                            line(wxPoint(8, 11), wxPoint(8, 9)) };
    int count = sizeof(points) / sizeof(line);

    xlColor color(hsv);

    for (float i = 0; i < Ripple_Thickness; i += .5) {
        if (CheckBox_Ripple3D) {
            if (buffer.allowAlpha) {
                color.alpha = 255.0 * (1.0 - (float(i) / float(Ripple_Thickness)));
            } else {
                hsv.value *= 1.0 - (float(i) / float(Ripple_Thickness)); // we multiply by 1.0 when steps=0
                color = hsv;
            }
        }
        if (Movement == MOVEMENT_EXPLODE) {
            radius = radius + i;
        } else {
            radius = radius - i;
        }
        if (radius >= 0) {
            for (int j = 0; j < count; ++j) {
                int x1 = std::round(((double)points[j].start.x - 5) / 7.0 * radius);
                int y1 = std::round(((double)points[j].start.y - 5.5) / 10.0 * radius);
                int x2 = std::round(((double)points[j].end.x - 5) / 7.0 * radius);
                int y2 = std::round(((double)points[j].end.y - 5.5) / 10.0 * radius);
                buffer.DrawLine(xc + x1, yc + y1, xc + x2, yc + y2, color);
            }
        } else {
            break;
        }
    }
}

void RippleEffect::Drawcandycane(RenderBuffer& buffer, int Movement, int xc, int yc, double radius, HSVValue& hsv, int Ripple_Thickness, int CheckBox_Ripple3D)
{
    double originalRadius = radius;
    xlColor color(hsv);

    for (float i = 0; i < Ripple_Thickness; i += .5) {
        if (CheckBox_Ripple3D) {
            if (buffer.allowAlpha) {
                color.alpha = 255.0 * (1.0 - (float(i) / float(Ripple_Thickness)));
            } else {
                hsv.value *= 1.0 - (float(i) / float(Ripple_Thickness)); // we multiply by 1.0 when steps=0
                color = hsv;
            }
        }
        if (Movement == MOVEMENT_EXPLODE) {
            radius = radius + i;
        } else {
            radius = radius - i;
        }
        if (radius >= 0) {
            // draw the stick
            int y1 = std::round((double)yc + originalRadius / 6.0);
            int y2 = std::round((double)yc - originalRadius / 2.0);
            int x = std::round((double)xc + radius / 2.0);
            buffer.DrawLine(x, y1, x, y2, color);

            // draw the hook
            double r = radius / 3.0;
            for (double degrees = 0.0; degrees < 180; degrees += 1.0) {
                double radian = degrees * (M_PI / 180.0);
                x = std::round((r)*buffer.cos(radian) + xc + originalRadius / 6.0);
                int y = std::round((r)*buffer.sin(radian) + y1);
                buffer.SetPixel(x, y, color);
            }
        } else {
            break;
        }
    }
}

