#include "CGameObject.h"
#include "CWorld.h"
#include "Functions.h"

using namespace std;

CGameObject::CGameObject(Type objectType,int x, int y, int xs, int ys, bool currentStatus) :m_type(objectType),m_x(x), m_y(y), m_xDelta(xs), m_yDelta(ys), m_active(currentStatus)
{
}

/**************************************************
CGameObject::isOutOfBoundsX
Description: Checks if the GameObject is currently outside
of the screen horizontally
Arguments: CWorld *w-Pointer to the game world used to get
the size of the world
Return: Bool-true if object outside of screen, false if he is not
Date: 2015/04/10
Author: Alvaro Chavez Mixco
***************************************************/
bool CGameObject::isOutOfBoundsX(CWorld *w)
{
    if (m_x< w->getLeft() || m_x + m_width>w->getRight())//Checks if object is outside of  the world bounds
    {
        return true;
    }

    //If object is inside the world bounds
    return false;
}

/**************************************************
CGameObject::isOutOfBoundsY
Description: Checks if the GameObject is currently outside
of the screen vertically
Arguments: CWorld *w-Pointer to the game world used to get
the size of the world
Return: Bool-true if object outside of screen, false if he is not
Date: 2015/04/10
Author: Alvaro Chavez Mixco
***************************************************/
bool CGameObject::isOutOfBoundsY(CWorld *w)
{
    if (m_y< w->getTop() || m_y + m_height>w->getBottom())//Checks if object is outside of  the world bounds
    {
        return true;
    }

    //If object is inside the world bounds
    return false;
}

/**************************************************
CGameObject::isOutOfBoundsY
Description: Only checks if the player is "below" the screen
Arguments: CWorld *w-Pointer to the game world used to get
the size of the world
Return: Bool-true if object outside of screen, false if he is not
Date: 2015/04/10
Author: Alvaro Chavez Mixco
***************************************************/
bool CGameObject::isOutOfBoundsYDown(CWorld *w)
{
    if (m_y + m_height > w->getBottom())//Checks if object is "below" the world bounds
    {
        return true;
    }

    //If object is inside the world bounds
    return false;
}

/**************************************************
CGameObject::circleCollision
Description: Checks if two game objects are colliding with each
other using simple circle collision
Arguments: CGameObject *object-Pointer to one of the
objects colliding
Return: Bool-true if objects are colliding, false if they are not
Date: 2015/04/10
Author: Alvaro Chavez Mixco
***************************************************/
bool CGameObject::circleCollision(CGameObject *object)
{
    //the other objects position
    float x2 = object->getXPos();
    float y2 = object->getYPos();

    //if distance<=(radius1+radius2)^2 there is a collision
    float distance = calculateDistance(m_x, m_y, x2, y2);//Calculate distance using pythagorean theorem
    float sumRadius = m_radius + object->getRadius();//Add radius of both objects

    if (distance < sumRadius)//check if they collide
    {
        return true;
    }

    return false;//If they don't collide
}

/**************************************************
CGameObject::movementSpeeds
Description: Calculates how much an object moves in both
the x and y direction.
Arguments: unsigned long elapsed-Milliseconds elapsed in a frame
float xDelta-Speed on the X direction
float yDelta-Speed on the Y direction
float xStep(passed by reference)-The distance the object moved in the
x direction
float yStep(passed by reference)-The distance the object moved in the
y direction
Return: Void, xStep and yStep passed by reference. The distance moved in
both the x and y  direction.
Date: 2015/04/10
Author: Alvaro Chavez Mixco
***************************************************/
void CGameObject::movementSpeeds(unsigned long elapsed, float xDelta, float yDelta, float &xStep, float &yStep)
{
    //Bounding checking not done in here since some objects may appear offscreen
    xStep = m_xDelta*((float)elapsed) / 1000.0f;//Movement pixels per second
    yStep = m_yDelta*((float)elapsed) / 1000.0f;//Movement pixels per second

}

/**************************************************
CGameObject::rectangleCollision
Description: Checks if 2 objects, in bounding boxes at a 90 degree angle, are colliding.
Arguments: float aLeft-The left side (x) of the rectangle
float aTop-The top y position of the object
float aRight-The right side (x) of the rectangle
float aBottom-The bottom y position of the object
Return: Bool-true if objects are colliding, false if they are not
Date: 2015/04/17
Author: Alvaro Chavez Mixco
***************************************************/
bool CGameObject::rectangleCollision(float aLeft, float aTop, float aRight, float aBottom)
{
    //Save the coordinates of this object bounding box for easier use
    float bLeft = m_x;
    float bTop = m_y;
    float bRight = m_x + m_width;
    float bBottom = m_y + m_height;

    //Check if the rectangles are colliding
    //If any of these conditions is true the rectangles are not colliding
    if (aTop < bBottom)
    {
        return false;
    }
    else if (aBottom > bTop)
    {
        return false;
    }
    else if (aLeft > bRight)
    {
        return false;
    }
    else if (aRight < bLeft)
    {
        return false;
    }

    return true;
}