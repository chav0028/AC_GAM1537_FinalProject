#ifndef __CGAMEOBJECT_H
#define __CGAMEOBJECT_H

#include <Windows.h>
#include <gdiplus.h>

enum Type
{
    TYPE_PLAYER,
    TYPE_ALIEN,
    TYPE_BACKGROUNDOBJECT
};

//Forward declaration to solve circular reference
class CWorld;

class CGameObject
{
protected:
    Type m_type;
    float m_x;//Float not ints so that the update can be less than 1 pixel (even if that can't be drawn until it gets whole number)
    float m_y;
    int m_width;
    int m_height;
    float m_xDelta;//Speeds
    float m_yDelta;//Speeds
    float m_radius;
    bool m_active;//Determines if the object would be active (draw and update)

public:
    CGameObject(Type objectType,int x=0, int y=0, int xs=0, int ys=0, bool currentStatus = false);
    virtual ~CGameObject(){}
    virtual void draw(Gdiplus::Graphics &graphics){};
    virtual void update(unsigned long elapsed) = 0;//Pure virtual
    virtual bool isOutOfBoundsX(CWorld *w);//Compare position of object with the world bounds
    virtual bool isOutOfBoundsY(CWorld *w);
    bool isOutOfBoundsYDown(CWorld *w);//Only checks if object is offscreen only down Y.
    bool circleCollision(CGameObject *object);
    bool rectangleCollision(float aLeft, float aTop, float aRight, float aBottom);
    void movementSpeeds(unsigned long elapsed, float xDelta, float yDelta, float &xStep, float &yStep);

    //Getters and setters
    void setXPos(float x){ m_x = x; }
    void setYPos(float y){ m_y = y; }
    float getXPos(){ return m_x; }
    float getYPos(){ return m_y; }
    float getRadius(){ return m_radius; }
    bool getActiveStatus(){ return m_active; }
    void setActiveStatus(bool currentStatus){ m_active = currentStatus; };
    Type getType(){ return m_type; }
};

#endif