#include "CAlienShip.h"
#include "CGameObject.h"
#include <string>
#include <gdiplus.h>

using namespace std;
using namespace Gdiplus;

extern CWorld *g_world;

CAlienShip::CAlienShip(string nameSprite, int x, int y, int xs, bool currentStatus) :CGameObject(TYPE_BACKGROUNDOBJECT, x, y, xs, 0, currentStatus)
{
    m_sprite.loadBitmap(nameSprite);//Load image according to file name
    m_sprite.setPos(x, y);//Set it positions

    //Data saved in case collision want to be made later (would require change in the type of gameobject (Enum))
    m_width = m_sprite.getWidth();//Sets the object as the same size as the sprite, not necessary
    m_height = m_sprite.getHeight();
    m_radius = (m_width + m_height) / 2.0f;//Get diameter
    m_radius /= 2.0f;//Get radius from the diameter
}

CAlienShip::~CAlienShip()
{
}

/**************************************************
CAlienShip::update
Description: Moves the alien ship from side to side, "bouncing" from the sides of the screen
Arguments:  unsigned long elapsed-Milliseconds elapsed in a frame
Return: Void
Date: 2015/04/17
Author: Alvaro Chavez Mixco
***************************************************/
void CAlienShip::update(unsigned long elapsed)
{
    float xStep = 0;
    float yStep = 0;

    movementSpeeds(elapsed, m_xDelta, m_yDelta, xStep, yStep);//Get how much object moved

    m_x += xStep;//Horizontal movement
    if (isOutOfBoundsX(g_world) == true)//Check bounds.
    {
        m_x -= xStep;//Keeps sprite on screen
        m_xDelta *= -1;//Reverse direction
    }

    m_sprite.setPos(m_x, m_y);//Updates x and y positon
    //Y position doesn't change from the position it was set when object was created
}

/**************************************************
CAlien::draw
Description: Draw the alien ship
Arguments:  Graphics &graphics object used to draw the sprite
Return: Void
Date: 2015/04/10
Author: Alvaro Chavez Mixco
***************************************************/
void CAlienShip::draw(Graphics &graphics)
{
    m_sprite.draw(graphics);
}
