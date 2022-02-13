#include "CAlienRandom.h"
#include "CAlien.h"
#include "CWorld.h"
#include <string>

using namespace std;

extern CWorld *g_world;

CAlienRandom::CAlienRandom(string nameSprite, int x, int  y, float randTimer, int dam, float hp, int xs, int ys, int points, bool currentStatus) :CAlien(nameSprite,ALIEN_TYPE_RANDOM, x, y, dam, hp, xs, ys, points, currentStatus)
, m_defaultTimer(randTimer), m_randomDirectionTimer(randTimer)
{
}

CAlienRandom::~CAlienRandom()
{
}

/**************************************************
CAlienRandom::update
Description: updates the alien game, doing what it parents class does, plus
after a certain time change its x direction and increases by a random value its
y value.
increasing its down speed)
Arguments:  unsigned long elapsed-Milliseconds elapsed in a frame
Return: Void
Date: 2015/04/10
Author: Alvaro Chavez Mixco
***************************************************/
void CAlienRandom::update(unsigned long elapsed)
{

    m_randomDirectionTimer -= elapsed;//Substract from timer

    if (m_randomDirectionTimer <= 0)//If time is over
    {
        m_xDelta *= -1;//Change X direction
        m_randomDirectionTimer = m_defaultTimer;//Reset timer

        int randomSpeedIncrease = rand() % 5;//Get a random number between 0 and 4
        m_yDelta += randomSpeedIncrease;//Increase speed
    }

    //Do the same as the parent class
    CAlien::update(elapsed);
}