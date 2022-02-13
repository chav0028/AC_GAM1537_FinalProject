#include "CShape.h"
#include <Windows.h>
#include <gdiplus.h>

using namespace std;
using namespace Gdiplus;

CShape::CShape(int x, int y, int s, float t, COLORREF colStroke, int alphaFill, COLORREF colFill)
{
    m_x = x;
    m_y = y;
    m_sides = s;
    m_thick = t;
    m_colorStroke = colStroke;
    m_colorFill = colFill;
    m_alphaFill = alphaFill;
    m_pen = new Pen(Color(255, GetRValue(m_colorStroke), GetGValue(m_colorStroke), GetBValue(m_colorStroke)), m_thick);//Set pen properties-stroke
    m_brush = new SolidBrush(Color(m_alphaFill, GetRValue(m_colorFill), GetGValue(m_colorFill), GetBValue(m_colorFill)));//Set brush properties-fill
}

CShape::~CShape()
{
    //Delete pointers
    delete m_pen;
    delete m_brush;
    m_brush = NULL;
    m_pen = NULL;
}