#ifndef __FUNCTIONS_H
#define __FUNCTIONS_H

#include <Windows.h>

void loadActors();
void initGame();
bool updateGameObjects(unsigned long elapsed);
void gameObjectCollision(unsigned long elapsed);
void drawGameObjectsGDIPlus(HDC hdc);
void cleanupGame();
float calculateDistance(float x, float y, float x2, float y2);//May not be reused in this particular program, but made into a function for future reuse.

#endif