#ifndef __CALIENRANDOM_H
#define __CALIENRANDOM_H

#include "CAlien.h"

class CAlienRandom :public CAlien
{
private:
    float m_randomDirectionTimer;
    float m_defaultTimer;
public:
    CAlienRandom(std::string nameSprite, int x = 0, int y = 0, float randTimer = 1000, int dam = 2, float hp = 700, int xs = 20, int ys = 60, int points = 50, bool currentStatus = false);
    ~CAlienRandom();
    virtual void update(unsigned long elapsed);
};

#endif
