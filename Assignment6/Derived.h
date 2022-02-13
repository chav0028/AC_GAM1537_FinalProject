#ifndef __DERIVED_H
#define __DERIVED_H

#include "CShape.h"
#include <vector>
#include <gdiplus.h>
#include <windows.h>

class CLine :public CShape
{
private:
    int m_endX;
    int m_endY;
public:
    CLine(int x, int y, int len, float angle, float t = 3, COLORREF col = RGB(255, 255, 255));
    int CLine::getEndX();
    int CLine::getEndY();
    void draw(Gdiplus::Graphics &graphics);
};



class CParallelogram : public CShape
{
protected:
    int m_width;
    int m_height;
    float m_angle1;
    float m_angle2;
    std::vector<Gdiplus::Point>m_points;
    int m_numPoints;
public:
    CParallelogram(int x, int y, int w, int h, float topLeftAngleDeg = 135, float t = 3, COLORREF colStroke = RGB(255, 255, 255), int alphaFill = 0, COLORREF colFill = RGB(0, 0, 0));
    void draw(Gdiplus::Graphics &graphics);
};



class CRectangle :public CParallelogram
{
public:
    CRectangle(int x, int y, int w, int h, float t = 3, COLORREF colStroke = RGB(255, 255, 255), int alphaFill = 0, COLORREF colFill = RGB(0, 0, 0));
    void draw(Gdiplus::Graphics &graphics);
};



class CSquare :public CRectangle
{
public:
    CSquare(int x, int y, int l, float t = 3, COLORREF colStroke = RGB(255, 255, 255), int alphaFill = 0, COLORREF colFill = RGB(0, 0, 0));
    void draw(Gdiplus::Graphics &graphics);
};



class CTrapezoid : public CParallelogram//Isosceles trapezoid
{
public:
    CTrapezoid(int x, int y, int base, int h, int top, float t = 3, COLORREF colStroke = RGB(255, 255, 255), int alphaFill = 0, COLORREF colFill = RGB(0, 0, 0));
    void draw(Gdiplus::Graphics &graphics);
};



class CCube :public CSquare//Changed to inherit from CSquare instead of CRectangle
{
private:
    int m_depth;
    int m_secondRectangleX;
    int m_secondRectangleY;
    float m_inclinationAngle;
public:
    CCube(int x, int y, int l, int d, float angle = 45, float t = 3, COLORREF colStroke = RGB(255, 255, 255), int alphaFill = 0, COLORREF colFill = RGB(0, 0, 0));
    void draw(Gdiplus::Graphics &graphics);
};



class CTriangle :public CShape
{
protected:
    std::vector< Gdiplus::Point>m_points;
    int m_numPoints;
public:
    CTriangle(int x, int y, int lSide1, int angle1, int lSide2, int angle2, float t = 3, COLORREF colStroke = RGB(255, 255, 255), int alphaFill = 0, COLORREF colFill = RGB(0, 0, 0));
    void draw(Gdiplus::Graphics &graphics);
};



class CIsoTriangle :public CTriangle//Isosceles triangle
{
public:
    CIsoTriangle(int x, int y, int equalSidesLength, int angle, float t = 3, COLORREF colStroke = RGB(255, 255, 255), int alphaFill = 0, COLORREF colFill = RGB(0, 0, 0));
    void draw(Gdiplus::Graphics &graphics);
};



class CEquiTriangle :public CTriangle//Equilateral triangle
{
public:
    CEquiTriangle(int x, int y, int l, float t = 3, COLORREF colStroke = RGB(255, 255, 255), int alphaFill = 0, COLORREF colFill = RGB(0, 0, 0));
    void draw(Gdiplus::Graphics &graphics);
};



class CEllipse : public CShape
{
protected:
    int m_width;
    int m_height;
public:
    CEllipse(int x, int y, int w, int h, float t = 3, COLORREF colStroke = RGB(255, 255, 255), int alphaFill = 0, COLORREF colFill = RGB(0, 0, 0));
    void draw(Gdiplus::Graphics &graphics);

};



class CCircle :public CEllipse
{
public:
    CCircle(int x, int y, int diameter, float t = 3, COLORREF colStroke = RGB(255, 255, 255), int alphaFill = 0, COLORREF colFill = RGB(0, 0, 0));
    void draw(Gdiplus::Graphics &graphics);
};



#endif