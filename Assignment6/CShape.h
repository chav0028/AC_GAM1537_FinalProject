#ifndef __CSHAPE_H
#define __CSHAPE_H

#include <Windows.h>
#include <windowsx.h>
#include <gdiplus.h>

class CShape
{
protected:
    int m_sides;
    int m_x;
    int m_y;

    //Windows Specific
    //GDI Plus-Pen to draw
    //        - Brush to paint
    float m_thick;//Line thickness
    Gdiplus::Pen *m_pen; //pointer to pen
    Gdiplus::SolidBrush *m_brush;//Pointer to brush

    //Members variables saved in case they are changed in the future.
    COLORREF m_colorStroke;
    COLORREF m_colorFill;
    int m_alphaFill;

    //Default values passed in each child class, except for fills (lines)
    CShape(int x, int y, int s, float t, COLORREF colStroke, int alphaFill = 0, COLORREF colFill = RGB(255, 255, 255));
public:
    virtual ~CShape();
    virtual void draw(Gdiplus::Graphics &graphics) = 0;//Pure virtual, HAS to be redefined in child class
};

#endif