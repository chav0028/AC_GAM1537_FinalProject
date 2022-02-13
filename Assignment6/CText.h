#ifndef __CTEXT_H
#define __CTEXT_H

#include <Windows.h>
#include <gdiplus.h>

class CText
{
public:
    void showText(HDC hdc, Gdiplus::Graphics &graphics, WCHAR text[], int x, int y, WCHAR fontName[] = L"Arial", int size = 18, COLORREF col = RGB(255, 255, 255));
};

#endif