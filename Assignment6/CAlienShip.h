#ifndef __CALIENSHIP_H
#define __CALIENSHIP_H

#include "CGameObject.h"
#include "CSprite.h"

class CAlienShip :public CGameObject
{
protected:
    CSprite m_sprite;
public:
    CAlienShip(std::string nameSprite, int x = 0, int y = 0, int xs = 30, bool currentStatus = true);//No Ys, ship only moves horizontally
    virtual ~CAlienShip();
    virtual void draw(Gdiplus::Graphics &graphics);//Virtual in case it it rewriten in a child class
    virtual void update(unsigned long elapsed);//Virtual in case it it rewriten in a child class
};


#endif