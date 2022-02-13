#ifndef __CROBOT_H
#define __CROBOT_H

#include "CGameObject.h"
#include "CSprite.h"
#include <Windows.h>
#include <gdiplus.h>
#include <string>

class CRobot :public CGameObject
{
protected:
    CSprite m_sprite;
public:
    CRobot(std::string fname, int x = 0, int y = 0, int xs = 0, int ys = 0);
    virtual ~CRobot();
    void draw(Gdiplus::Graphics &graphics);
    void update();
    
};

#endif