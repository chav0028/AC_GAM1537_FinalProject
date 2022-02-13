#include "CAlien.h"
#include "CWorld.h"
#include "CGameObject.h"
#include "CSprite.h"
#include <Windows.h>
#include <string>

using namespace std;
using namespace Gdiplus;

extern CWorld *g_world;

CAlien::CAlien(string nameSprite,AlienType type, int x, int y, int dam, float hp, int xs, int ys, int points, bool currentStatus) :CGameObject(TYPE_ALIEN,x, y, xs, ys, currentStatus),
m_alienType(type),m_damage(dam), m_defaultHealth(hp), m_pointsScored(points)
{
    m_sprite.loadBitmap(nameSprite);//Load image according to file name
    m_sprite.setPos(x, y);//Set it positions
    m_width = m_sprite.getWidth();//Sets the object as the same size as the sprite
    m_height = m_sprite.getHeight();
    m_radius = (m_width + m_height) / 2.0f;//Get diameter
    m_radius /= 2.0f;//Get radius from the diameter
}

CAlien::~CAlien()
{

}

/**************************************************
CAlien::update
Description: updates the alien game object (moves its position, guarantees
it doesn't go out of bounds, and if applicable damage the base.
Arguments:  unsigned long elapsed-Milliseconds elapsed in a frame
Return: Void
Date: 2015/04/10
Author: Alvaro Chavez Mixco
***************************************************/
void CAlien::update(unsigned long elapsed)
{
    float xStep = 0;
    float yStep = 0;

    movementSpeeds(elapsed, m_xDelta, m_yDelta, xStep, yStep);//Get how much alien moved in x and y

    m_x += xStep;//Horizontal movement
    if (isOutOfBoundsX(g_world) == true)//Check bounds.
    {
        m_x -= xStep;//Keeps sprite on screen
        m_xDelta *= -1;//Reverse direction
    }

    m_y += yStep;//Vertical movement
    if (isOutOfBoundsYDown(g_world) == true)//If enemy is below the screen
    {
        //Damage base
        g_world->setBaseHealth(g_world->getBaseHealth() - m_damage);
        m_active = false;
    }

    m_sprite.setPos(m_x, m_y);//Updates x and y positon
}

/**************************************************
CAlien::draw
Description: Draws the alien.
Arguments:  Graphics &graphics object used to draw the sprite
Return: Void
Date: 2015/04/10
Author: Alvaro Chavez Mixco
***************************************************/
void CAlien::draw(Graphics &graphics)
{
    m_sprite.draw(graphics);
}

/**************************************************
CAlien::applyDamage
Description: Apply damage to the enemy according to how long it has
been hit. If his health reaches 0 it dies (set inactive) and the
score is added to the player.
Arguments:  unsigned long elapsedTime-Used to calculate how much damage
it is applied.
Return: Void
Date: 2015/04/17
Author: Alvaro Chavez Mixco
***************************************************/
void  CAlien::applyDamage(unsigned long elapsedTime)
{
    //Done in a different function in case effects (flickering, change of color, etc) want to be added later.

    m_health -= elapsedTime;//Substract from health

    if (m_health <= 0)//If enemy is dead
    {
        setActiveStatus(false);//Set as inactive
        g_world->setPlayerScore(g_world->getPlayerScore() + m_pointsScored);//Ad to player score
        m_health = m_defaultHealth;//Ensures that if the enemy is used again (if a new type isn't picked), the enemy will start with full health.

    }

}