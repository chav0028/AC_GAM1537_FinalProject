#include "CText.h"
#include <Windows.h>
#include <gdiplus.h>

using namespace Gdiplus;

//Class provided, no header file
void CText::showText(HDC hdc, Graphics &graphics, WCHAR text[], int x, int y, WCHAR fontName[], int size, COLORREF col)
{
    Font font(fontName, (float)size);
    SolidBrush  brush(Color(255, GetRValue(col), GetGValue(col), GetBValue(col)));
    PointF p((float)x, (float)y);
    graphics.DrawString(text, -1, &font, p, &brush);
}