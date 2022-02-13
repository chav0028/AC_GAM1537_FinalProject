#ifndef __CSPRITE_H
#define __CSPRITE_H

#include <Windows.h>
#include <gdiplus.h>
#include <string>

class CSprite
{
private:
    std::string m_lastError;
    Gdiplus::Bitmap *m_performanceBMP;//Unique pointer gives problem
    int m_x;
    int m_y;
    int m_w;
    int m_h;

public:
    CSprite();
    ~CSprite();
    CSprite(const CSprite &other);//overload Copy constructor
    CSprite &operator=(const CSprite &rhs);//Overload assignment opeator
    bool loadBitmap(std::string fName);
    void draw(Gdiplus::Graphics &graphics);

    //Getters and setters
    int getXPos(){ return m_x; }
    int getYPos(){ return m_y; }
    int getWidth(){ return m_w; }
    int getHeight(){ return m_h; }
    void setPos(int x, int y){ m_x = x; m_y = y; };

};

#endif