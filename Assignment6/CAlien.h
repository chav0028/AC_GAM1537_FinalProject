#ifndef __CALIEN_H
#define __CALIEN_H

#include "CGameObject.h"
#include "CSprite.h"
#include <Windows.h>
#include <gdiplus.h>
#include <string>

enum AlienType
{
	ALIEN_TYPE_NORMAL,
	ALIEN_TYPE_FAST,
	ALIEN_TYPE_STRONG,
	ALIEN_TYPE_RANDOM
};

class CAlien :public CGameObject
{
protected:
    CSprite m_sprite;
	AlienType m_alienType;
    int m_damage;
    float m_health;
    float m_defaultHealth;
    int m_pointsScored;//How much they are worth, when killed, to the score
    //Point score is usually (default values) in multiples of 10 for better appearance

public:
    CAlien(std::string nameSprite, AlienType type=ALIEN_TYPE_NORMAL,int x = 0, int y = 0, int dam = 1, float hp = 100, int xs = 0, int ys = 0, int points = 10, bool currentStatus = false);
    virtual ~CAlien();
    virtual void draw(Gdiplus::Graphics &graphics);//Virtual in case it it rewriten in a child class
    virtual void update(unsigned long elapsed);//Virtual in case it it rewriten in a child class
    void  applyDamage(unsigned long elapsedTime);
};

#endif