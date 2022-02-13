#include "CSprite.h"
#include "CWorld.h"
#include "Functions.h"
#include "Derived.h"
#include "CText.h"
#include "CAlien.h"
#include "CAlienShip.h"
#include "CGameObject.h"
#include <vector>
#include <Windows.h>
#include <gdiplus.h>
#include <typeinfo>

using namespace std;
using namespace Gdiplus;	// GDI+

extern Bitmap *gpBitmap;
CWorld *g_world;

/**************************************************
loadActors
Description:Loads all the actors in the game and adds
them to g_world.
Arguments:  Void
Return: Void
Date: 2015/04/11
Author: Alvaro Chavez Mixco
***************************************************/
void loadActors()//Actors=objects in the game
{
    //Creates player and set his image
    CPlayer *player;
    player = new CPlayer("..\\Images\\player.png");
    //Uses player radius for positions so can't be set immediately in constructor
    int playerX = g_world->getRight() / 2;//Place character on middle of screen
    int playerY = g_world->getBottom() - (player->getRadius() + 20);//Ensures that there is a small distance between the player and bottom of the screen

    //Set player's position
    player->setXPos(playerX);
    player->setYPos(playerY);

    //Add player to the world and save its pointer
    g_world->setPlayer(player);
    g_world->addObject(player);

    //Add background objects
    CAlienShip *alienShip;
    alienShip = new CAlienShip("..\\Images\\AlienShip.png", 0, 20);
    g_world->addObject(alienShip);

    //Get how many enemies will be in the game world
    int numEnemies = g_world->getNumEnemies();

    //Object pooling of enemies
    //Create enemies and add them to enemies list.
    for (int i = 0; i < numEnemies; i++)
    {

        CAlien *alien = g_world->pickRandomEnemyType();//Choose a random type of enemy and creates it

        //Add everything to game world
        g_world->addObject(alien);//Add to game world
        g_world->addEnemy(alien);//Add to list of enemies
        //Aliens are by default set as inactive in their constructor

    }
}

/**************************************************
IniGame
Description: Generate random numbers seed, creates g_world,
,load the actors and do every start up code here
Arguments:  Void
Return: Void
Date: 2015/04/15
Author: Alvaro Chavez Mixco
***************************************************/
void initGame()
{
    srand(GetTickCount());//Get random number seeds (This is only initialized once, since there is no reset game option).
    g_world = new CWorld(0, 0, gpBitmap->GetWidth(), gpBitmap->GetHeight(), 20, 15);//Assigns the world size as the size of the bitmap
    loadActors();
}

/**************************************************
updateGameObjects
Description: Updates every object in g_world, if they are active.
Arguments:  unsigned long elapsed-Milliseconds elapsed in a frame
Return: Void
Date: 2015/04/10
Author: Alvaro Chavez Mixco
***************************************************/
bool updateGameObjects(unsigned long elapsed)
{

    g_world->placeEnemies(elapsed);//Place the enemies (set them to active and their position)
    if (g_world->getPlayer()->getActiveStatus() == true && g_world->getBaseHealth() > 0)//Checks if the player is still alive and the base is still alive
    {
        for (auto &i : g_world->getActors())//Update every object in the world
        {
            if (i->getActiveStatus() == true)//Checks if the object is active
            {
                i->update(elapsed);

                if (i->getActiveStatus() == false)//Check for inactive objects after the update
                {
                    if (i->getType() == TYPE_ALIEN)//Checks if i is an alien
                    {
                        delete i;//Delete the current enemy
                        i = NULL;
                        i = g_world->pickRandomEnemyType();//Randomly pick another type of enemy
                    }
                }

            }
        }
        return false;
    }
    else
    {
        return true;
    }

}

/**************************************************
gameObjectCollision
Description: Collides the objects in the game.
Arguments:  unsigned long elapsed-Milliseconds elapsed in a frame
Return: Void
Date: 2015/04/16
Author: Alvaro Chavez Mixco
***************************************************/
void gameObjectCollision(unsigned long elapsed)
{
    //Enemies don't collide between themselves.
    //Game balance issues-Strong alien is too big and slow, and if they collide it would make the game too slow

    CPlayer *p = g_world->getPlayer();//Save player for easier user

    //Since only the aliens and the players are collideable, don't bother checking for collision for background objects(AlienShip)
    for (auto i : g_world->getEnemies())//Go through all enemies
    {

        if (p->circleCollision(i) == true)//Check if an enemy is colliding with the player
        {
            p->setActiveStatus(false);//Kill the player
        }

        if (p->getPlayerShooting() == true)//Check if player is shooting
        {

            if (i->rectangleCollision(p->getXPos() + p->getRadius(), p->getYPos(), p->getLaserEndX(), p->getLaserEndY()))//Check if laser is hitting enemy
            {
                //Rectangle Collision only works when objects are at 90 degree angle, like in this game

                i->applyDamage(elapsed);//Damage the enemy according to how long the laser has been hitting him.
            }
        }
    }

}

/**************************************************
drawGameObjectsGDIPlus
Description: Using double buffering, draws all the objects in g_world,
if they are active.
Arguments:  HDC hdc-Hardware device context used for drawing
Return: Void
Date: 2015/04/10
Author: Alvaro Chavez Mixco
***************************************************/
void drawGameObjectsGDIPlus(HDC hdc)//Double buffering to eliminate flickering graphics
{
    Bitmap buffer(gpBitmap->GetWidth(), gpBitmap->GetHeight(), PixelFormat32bppPARGB);//Create buffer
    Graphics memGraphics(&buffer);     // graphics object for memory bitmap

    //Draw everything offscreen using the second buffer
    // graphics object for window
    Graphics graphics(hdc);

    // Draw images in memory
    Rect rect(0, 0, gpBitmap->GetWidth(), gpBitmap->GetHeight());
    memGraphics.DrawImage(gpBitmap, rect);

    CPlayer *p = g_world->getPlayer();//Save the address of the player for easier use

    for (auto i : g_world->getActors())//Draw every object in the world
    {
        if (i->getActiveStatus() == true)//Checks if the object is active
        {
            i->draw(memGraphics);

            //Player shooting
            if (i == p)//See if the object is the player
            {

                if (p->getPlayerShooting() == true)//Check if the player is shooting
                {
                    //Draw the laser(line) according to the player position                
                    CLine laser(p->getXPos() + p->getRadius(), p->getYPos(), p->getLaserLength(), -90, p->getLaserThickness(), RGB(255, 0, 0));
                    laser.draw(memGraphics);

                    //Save laser end points (later used for collision).
                    p->setLaserEndX(laser.getEndX());
                    p->setLaserEndY(laser.getEndY());
                }

            }

        }
    }

    if ((p->getActiveStatus() == false) || (g_world->getBaseHealth() <= 0))//if the player lost the game (player death or base health at 0)
    {
        //Draw a red screen to indicate the player is death
        CRectangle deathScreen(0, 0, g_world->getRight(), g_world->getBottom(), RGB(0, 0, 0), 1, 150, RGB(255, 20, 20));
        deathScreen.draw(memGraphics);

        CText deathText;//Game Over text
        deathText.showText(hdc, memGraphics, L"GAME OVER", g_world->getRight() / 2 - 170, g_world->getBottom() / 2 - 50, L"Impact", 50, RGB(0, 0, 0));
    }
    else//If the game is still active
    {
        //Draw a semitransparent rectangle where the score will be displayed
        CRectangle scoreDisplay(0, 0, g_world->getRight(), 35, 2, RGB(14, 165, 100), 100, RGB(26, 216, 134));
        scoreDisplay.draw(memGraphics);

    }

    //Score and health is displayed even when the game is over
    //Display player score
    CText scoreText;
    wstring scoreT;
    string currentPlayerScore = "Score: " + to_string(g_world->getPlayerScore());//Convert score to string
    scoreT.assign(currentPlayerScore.begin(), currentPlayerScore.end());//Convert string to W_Chars
    scoreText.showText(hdc, memGraphics, (WCHAR*)scoreT.c_str(), 20, 0, L"Helvetica", 24, RGB(255, 255, 255));

    //Display the health of the base
    CText healthBaseText;
    wstring healthT;
    string currentBaseHealth = "Health: " + to_string(g_world->getBaseHealth());//Convert health to string
    healthT.assign(currentBaseHealth.begin(), currentBaseHealth.end());//Convert string to W_Chars
    healthBaseText.showText(hdc, memGraphics, (WCHAR*)healthT.c_str(), g_world->getRight() / 2 + 20, 0, L"Helvetica", 24, RGB(255, 255, 255));

    // Draw memory bitmap onto windows graphics object
    graphics.DrawImage(&buffer, 0, 0);//Bleed, and draw the image.

}

/**************************************************
cleanupGame
Description: All of the clean up code in the game. Including
deleting everything in g_world, and g_world itself.
Arguments:  Void
Return: Void
Date: 2015/04/10
Author: Alvaro Chavez Mixco
***************************************************/
void cleanupGame()
{
	//Actors deleted in world destructor
    delete g_world;//Delete the game world
    g_world = NULL;

}

/**************************************************
calculateDistance
Description: Using the pythagorean theorem calculates the
distance between two vectors
Arguments:  float x- X position of the first vector
float y- Y position of the first vector
float x2- X position of the second vector
float y2-Y position of the second vector
Return: float distance-Distance between the two vectors
Date: 2015/04/10
Author: Alvaro Chavez Mixco
***************************************************/
float calculateDistance(float x, float y, float x2, float y2)
{
    //calculate distance using Pythagorean Theorem 
    //distance=Sqrt(x^2+y^2)
    //distance=sqrt((x-x2)^2+(y-y2)^2)
    float distance = sqrtf(((x - x2)*(x - x2)) + ((y - y2)*(y - y2)));

    return distance;

}