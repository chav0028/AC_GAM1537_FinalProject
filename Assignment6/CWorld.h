#ifndef __CWORLD_H
#define __CWORLD_H

#include "CGameObject.h"
#include "CAlien.h"//Enemies
#include "CPlayer.h"
#include <vector>

class CWorld
{
private:
    float const m_enemiesSpawnTime = 10;//Constant so that this value can be used to reset timer (time to spawn), and just having to change this part in code to modify spawn time  
    int m_left;
    int m_top;
    int m_right;
    int m_bottom;
    std::vector <CGameObject *>m_actors;//Vector to pointers of game objects
    CPlayer *m_player;
    std::vector <CAlien *> m_enemies;
    float m_timeToSpawn; //Set to 0  at start to spawn enemies once game starts
    int m_baseHealth;
    int m_numEnemies;
    int m_playerScore;

public:
    CWorld(int l, int t, int r, int b, int bh = 20, int numberEnemies = 8, int playerScore = 0);//bh==base health
	~CWorld();
    void addObject(CGameObject *object);
    void addEnemy(CAlien *enemy);
    void placeEnemies(unsigned long elapsedTime);
    CAlien* pickRandomEnemyType();

    //Getters and setters
    int getLeft()const{ return m_left; }
    int getTop()const{ return m_top; }
    int getRight()const{ return m_right; }
    int getBottom()const{ return m_bottom; }
    std::vector<CGameObject *>&getActors(){ return m_actors; }
    std::vector<CAlien *>&getEnemies(){ return m_enemies; }
    void setPlayer(CPlayer *player){ m_player = player; }
    CPlayer *getPlayer(){ return m_player; }
    int getNumEnemies(){ return m_numEnemies; }
    int getPlayerScore(){ return m_playerScore; }
    void setPlayerScore(int score){ m_playerScore = score; }
    int getBaseHealth(){ return m_baseHealth; }
    void setBaseHealth(int health){ m_baseHealth = health; }
};

#endif