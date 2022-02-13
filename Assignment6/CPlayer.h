#ifndef __CPLAYER_H
#define __CPLAYER_H

#include "CGameObject.h"
#include "CSprite.h"
#include <Windows.h>
#include <gdiplus.h>
#include <string>

class CPlayer :public CGameObject
{
protected:
    CSprite m_sprite;
    float m_playerSpeedMagnitude;//Different than xDelta and yDelta because this value is used to set deltaX and deltaY when pressing a key.
    bool m_playerShooting;
    float m_laserLength;
    float m_laserEndX;
    float m_laserEndY;
    float m_laserThickness;

public:
    CPlayer(std::string nameSprite, int x = 0, int y = 0, int xs = 0, int ys = 0, bool currentStatus = true, float laserLength = 500, float playerSpeedMagnitude = 120, float laserT = 3);
    ~CPlayer();
    void draw(Gdiplus::Graphics &graphics);
    void update(unsigned long elapsed);

    //Getters and setters
    bool getPlayerShooting(){ return m_playerShooting; }
    float getLaserLength(){ return m_laserLength; }
    float getLaserEndX(){ return m_laserEndX; }
    float getLaserEndY(){ return m_laserEndY; }
    void setLaserEndX(float laserEndX){ m_laserEndX = laserEndX; }
    void setLaserEndY(float laserEndY){ m_laserEndY = laserEndY; }
    float getLaserThickness(){ return m_laserThickness; }
};

#endif