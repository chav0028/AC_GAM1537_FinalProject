#include "CWorld.h"
#include "CAlien.h"
#include "CAlienRandom.h"
#include <vector>

#include "Windows.h"
#include "mmsystem.h"

using namespace std;

CWorld::CWorld(int l, int t, int r, int b, int bh, int numberEnemies, int playerScore) : m_left(l), m_top(t), m_right(r), m_bottom(b), m_timeToSpawn(0), m_baseHealth(bh), m_numEnemies(numberEnemies), m_playerScore(playerScore)
{
    //Set spawn to 0 in the constructor so that enemies appear at the beginning of game
}

CWorld::~CWorld()
{
	for (auto i : getActors())//delete all elements in the world vector
	{
		delete i;
		i = NULL;
	}
}
/**************************************************
CWorld::addObject
Description: add game object to the game world
Arguments:  CGameObject *obj-game object being added
Return: Void
Date: 2015/03/31
Author: Alvaro Chavez Mixco
***************************************************/
void CWorld::addObject(CGameObject *obj)
{
    m_actors.push_back(obj);
}

/**************************************************
CWorld::addEnemy
Description: add an enemy to the game world list of enemies
Arguments:  CAlien *enemy-enemy (alien) being added to the list of enemies
Return: Void
Date: 2015/03/31
Author: Alvaro Chavez Mixco
***************************************************/
void CWorld::addEnemy(CAlien *enemy)
{
    m_enemies.push_back(enemy);
}

/**************************************************
CWorld::placeEnemies
Description: Spawns enemies at a random position.
Arguments:  unsigned long elapsedTime-Used to calculate if it is already
time to spawn more enemies
Return: Void
Date: 2015/04/17
Author: Alvaro Chavez Mixco
***************************************************/
void CWorld::placeEnemies(unsigned long elapsedTime)
{
    m_timeToSpawn -= elapsedTime;//Reduce timer

    if (m_timeToSpawn <= 0)//Place the enemies after the timer has passed
    {
        for (int i = 0; i < m_numEnemies; i++)//Check the list of enemies
        {
            if (m_enemies[i]->getActiveStatus() == false)//If there is any inactive enemy
            {
                m_enemies[i]->setActiveStatus(true);//Set enemy as active

                int maxX = (getRight() - (m_enemies[i]->getRadius() * 2));//The max x value, while still displaying the enemy completely
                int maxY = (getBottom() - (int(m_enemies[i]->getRadius() * 2)));
                //Get random positions
                int randomY = (rand() % maxY) - (getBottom());//Set enemy randomly above screen to better simulate that they appear at different times.
                //Get a random coordinate in y, then substract the size of the screen, to ensure that the object will be offscreen.
                int randomX = (rand() % maxX);//Spawn the enemies randomly on the x position, but considering the radius of enemy

                //Place Enemies
                m_enemies[i]->setXPos(randomX);
                m_enemies[i]->setYPos(randomY);

                m_timeToSpawn = m_enemiesSpawnTime;//Resets timer
            }

        }

    }

}

/**************************************************
CWorld::pickRandomEnemyType
Description: Creates a random type of enemy, by creating
a pointer and allocating a type in it.
Arguments: Void
Return: CAlien *alien-A pointer to the alien that was
randomly created.
Date: 2015/04/17
Author: Alvaro Chavez Mixco
***************************************************/
CAlien* CWorld::pickRandomEnemyType()
{

    CAlien *alien;
    int randTypeEnemy = rand() % 11;//Divide by a bigger amount than order of enemies in order to make certain types of enemies more common

    do
    {

        //Determine which type of enemy will be
        if (randTypeEnemy >=0&&randTypeEnemy<=3)
        {
			//Normal Alien
            alien = new CAlien("..\\Images\\AlienNormal.png", ALIEN_TYPE_NORMAL,-100, 100,2,500,0,60,10);//Create alien offscreen
        }
        else if (randTypeEnemy >= 4 && randTypeEnemy <= 7)
        {
            int randDirection = rand() % 2;//Fast alien is the only one with a X direction, others move straight  down
            if (randDirection == 0)
            {
                randDirection = -1;//Set direction as left
                //The other random value is 1, so it doesn't need to be modified. It will mean right
            }
			//Fast Alien
            alien = new CAlien("..\\Images\\AlienFast.png",ALIEN_TYPE_FAST, -100, -100, 1,60,35*randDirection,60,20);
        }
        else if (randTypeEnemy >= 8 && randTypeEnemy <= 9)
        {
			//Strong Alien
            alien = new CAlien("..\\Images\\AlienStrong.png",ALIEN_TYPE_STRONG, -100, -100,4,1200,0,15,30);
        }
        else
        {
			//Random Alien
            alien = new CAlienRandom("..\\Images\\AlienRandom.png", -100, -100);
        }

    } while (alien == NULL);//Checks that the new operator hasn't failed

    return alien;
}