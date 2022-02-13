#include "CRobot.h"
#include "CWorld.h"
#include "CGameObject.h"
#include "CSprite.h"
#include <Windows.h>
#include <gdiplus.h>
#include <string>

using namespace std;
using namespace Gdiplus;

extern CWorld *g_world;

CRobot::CRobot(string fname, int x, int y, int xs, int ys) :CGameObject(x, y, xs, ys)
{
    m_sprite.loadBitmap(fname);//Load image according to file name
    m_sprite.setPos(x, y);//Set it positions
    m_width = m_sprite.getWidth();//Sets the object as the same size as the sprite, not necessary
    m_height = m_sprite.getHeight();
    m_Radius = (m_width + m_height) / 2.0f;//Get diameter
    m_Radius /= 2.0f;//Get radius from the diameter
}

CRobot::~CRobot()
{

}

void CRobot::update()
{
    m_x += m_xDelta;//Horizontal movement
    if (isOutOfBoundsX(g_world)==true)//Check bounds.
    {
        m_x -= m_xDelta;//Keeps sprite on screen
        m_xDelta *= -1;//Reverse direction
    }

    m_y += m_yDelta;//Vertical movement
    if (isOutOfBoundsY(g_world) == true)//Check bounds.
    {
        m_y -= m_yDelta;//Keeps sprite on screen
        m_yDelta *= -1;//Reverse direction
    }

    m_sprite.setPos(m_x, m_y);//Updates x and y positon

}

void CRobot::draw(Graphics &graphics)
{
    m_sprite.draw(graphics);
}