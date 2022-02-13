#include "CPlayer.h"
#include <string>
#include <gdiplus.h>

using namespace std;
using namespace Gdiplus;

extern CWorld *g_world;

CPlayer::CPlayer(string nameSprite, int x, int y, int xs, int ys, bool currentStatus, float laserLength, float playerSpeedMagnitude, float laserT) :CGameObject(TYPE_PLAYER,x, y, xs, ys, currentStatus),
m_laserLength(laserLength), m_playerSpeedMagnitude(playerSpeedMagnitude), m_laserThickness(laserT)
{
    m_sprite.loadBitmap(nameSprite);//Load image according to file name
    m_sprite.setPos(x, y);//Set it positions
    m_width = m_sprite.getWidth();//Sets the object as the same size as the sprite, not necessary
    m_height = m_sprite.getHeight();
    m_radius = (m_width + m_height) / 2.0f;//Get diameter
    m_radius /= 2.0f;//Get radius from the diameter
}

CPlayer::~CPlayer()
{

}

/**************************************************
CPlayer::update
Description: updates the alien game object (moves its position, guarantees
it doesn't go out of bounds, and if applicable damage the base.
Arguments:  unsigned long elapsed-Milliseconds elapsed in a frame
Return: Void
Date: 2015/04/10
Author: Alvaro Chavez Mixco
***************************************************/
void CPlayer::update(unsigned long elapsed)
{
    m_playerShooting = false;

    if (GetAsyncKeyState(VK_SPACE))//Shoot
    {
        m_playerShooting = true;
    }
    else//If player is not shooting
    {
        //Ensure that the laser coordinates are reset, 
        //if not a bug where for the first frames of update when player shoots, the collision will use the last coordinates of the laser 
        //(before they are set to their new positions when the laser is drawn).
        setLaserEndX(m_x);
        setLaserEndY(m_y);
    }

    if (m_playerShooting == false)//Don't move if player is shooting
    {
        //Player Movement
        float xStep = 0;
        float yStep = 0;

        if (GetAsyncKeyState(VK_LEFT))
        {
            m_xDelta = -m_playerSpeedMagnitude;
        }
        else if (GetAsyncKeyState(VK_RIGHT))
        {
            m_xDelta = m_playerSpeedMagnitude;
        }

        movementSpeeds(elapsed, m_xDelta, m_yDelta, xStep, yStep);//Get how much alien moved in x and y

        m_x += xStep;//Horizontal movement
        if (isOutOfBoundsX(g_world) == true)//Check bounds.
        {
            if (m_x < 0)
            {
                m_x -= xStep - 2;//Keeps sprite on screen, -2 so that the player  bounces and isn't stuck when reaching edge of screen
            }
            else
            {
                m_x -= xStep + 2;//Keeps sprite on screen
            }
        }

        m_sprite.setPos(m_x, m_y);//Updates x and y positon

        //Reset speed before next update
        m_xDelta = 0;
    }
}

/**************************************************
CPlayer::draw
Description: Draws the player
Arguments:  Graphics &graphics object used to draw the sprite
Return: Void
Date: 2015/04/10
Author: Alvaro Chavez Mixco
***************************************************/
void CPlayer::draw(Graphics &graphics)
{
    m_sprite.draw(graphics);
}