#include "CSprite.h"
#include <Windows.h>
#include <gdiplus.h>
#include <string>

using namespace std;
using namespace Gdiplus;

CSprite::CSprite() :m_lastError(""), m_performanceBMP(NULL), m_x(0), m_y(0), m_w(0), m_h(0)
{
}

CSprite::~CSprite()
{
    if (m_performanceBMP)
    {
        delete m_performanceBMP;
        m_performanceBMP = NULL;
    }
}

/**************************************************
CSprite::loadBitmap
Description: Loads a bitmap, saving its width
and height
Arguments:  string fName-name of the file being opened
Return: Bool-If the image could be opened or not
Date: 2015/03/31
Author: Alvaro Chavez Mixco
***************************************************/
bool CSprite::loadBitmap(string fName)
{
    wstring file;
    int w, h;

    //Convert from string to wstring
    file.assign(fName.begin(), fName.end());

    //Load Bitmap
    Bitmap loadBmp(file.c_str(), FALSE);
    if (loadBmp.GetLastStatus() != Ok)//Check if bitmap was loaded
    {
        m_lastError = string(__FUNCTION__) + "Unable to load Bitmap";//Gives you name of the function that failed (string (__FUNCTION__) is visual studio specific
        return false;
    }

    //Set width and heigh
    w = loadBmp.GetWidth();
    h = loadBmp.GetHeight();

    if (m_performanceBMP)
    {
        delete m_performanceBMP;//prevent the program from allocating memory multiple times
    }

    //Slightly improve performance
    m_performanceBMP = new Bitmap(w, h, PixelFormat32bppPARGB);//Create empty bitmap

    Graphics *graphics = new Graphics(m_performanceBMP);//Create graphic objects for bitmap
    graphics->DrawImage(&loadBmp, 0, 0, w, h);//Draw bmp
    delete graphics;//Delete graphic used to draw

    //Set widht and height
    m_w = w;
    m_h = h;

    return true;
}

/**************************************************
CSprite::draw
Description: Draws an image using GDI+, setting the properties
of it for "top"? performance.
Arguments:  Graphics graphics-Graphics object used to draw bitmap.
Return: Void
Date: 2015/03/31
Author: Alvaro Chavez Mixco
***************************************************/
void CSprite::draw(Graphics &graphics)
{

    //GDI+ Graphic settings for optimal bitmap draw performance vs quality
    //Tries to improve performance of GDI+
    graphics.SetCompositingMode(CompositingModeSourceOver);
    graphics.SetCompositingQuality(CompositingQualityHighSpeed);
    graphics.SetSmoothingMode(SmoothingModeNone);
    graphics.SetPixelOffsetMode(PixelOffsetModeHalf);
    graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);

    graphics.DrawImage(m_performanceBMP, m_x, m_y);
    graphics.Flush();//Makes the bitmap appear in screen "immediately"
}

//Overloaded copy constructor(Done in class)
//Called when you use push back,
CSprite::CSprite(const CSprite &other) :m_lastError(other.m_lastError), m_x(other.m_x), m_y(other.m_y), m_w(other.m_w), m_h(other.m_h)
{
    //Copy everything in initializer list except pooint
    //Make a deep copy of the point
    int w = other.m_performanceBMP->GetWidth();
    int h = other.m_performanceBMP->GetHeight();
    m_performanceBMP = other.m_performanceBMP->Clone(0, 0, w, h, other.m_performanceBMP->GetPixelFormat());

}

//Overload assignment operator(Done in class)
CSprite &CSprite::operator=(const CSprite &rhs)
{
    ///Sanity check
    if (this != &rhs)//if I am not trying to copy to myself by accident
    {
        m_lastError = rhs.m_lastError;
        m_x = rhs.m_x;
        m_y = rhs.m_y;
        m_w = rhs.m_w;
        m_h = rhs.m_h;
        int w = rhs.m_performanceBMP->GetWidth();
        int h = rhs.m_performanceBMP->GetHeight();

        if (m_performanceBMP)
        {
            delete m_performanceBMP;//prevent the program from allocating memory multiple times  
        }
        m_performanceBMP = rhs.m_performanceBMP->Clone(0, 0, w, h, rhs.m_performanceBMP->GetPixelFormat());
    }
    return *this;
}