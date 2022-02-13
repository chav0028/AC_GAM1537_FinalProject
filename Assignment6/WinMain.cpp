/***************************************************************************************
Title:    	Assignment 6
Course:   	GAM1537 Programming Fundamentals II
Section:  	4
Author:   	Alvaro Chavez Mixco
Date:     	2015/04/17
Description:  	In this assignment you will implement a simple Win32 game.
Game Description: In the game you control a soldier that has to defend a base against aliens (theme not final).
In the game the player will appear at the bottom of the screen. Aliens will appear at the top of the screen
(outside of view) and  move down. The player has to prevent the aliens from reaching the bottom of the screen
(base), by shooting them. If an alien reaches the bottom of the screen he will apply a certain amount of damage
to the base; if an alien collides with the player, the player dies. Depending on the health of the alien, that indicates
how much bullets he can take before dying. There is no way for the player to win the game, the game ends when
the player or the base die.
Game actors:
-Player
-Alien
-AlienFast-Movde diagonally down
-AlenNormal-Move straigh down
-AlienStrong-Move slowly down, but has more health
-AlienRanodm:After a certain time changes direction
-AlienShip:Just mvoe left and right
*****************************************************************************************/

#include"Functions.h"
#include "CSprite.h"
#include <Windows.h>
#include <gdiplus.h>
#include <vector>

// Store global handles to the main window, application and menu. 
using namespace std;
using namespace Gdiplus;	// GDI+

#pragma comment (lib,"Gdiplus.lib")	// GDI+

Bitmap *gpBitmap;//Background image

HWND windowHandle = 0;
HINSTANCE applicationInstance = 0;
HMENU mainMenuHandle = 0;

// Customize: change these values as needed

// Set the initial position of your window. 
int windowXPosition = 0;
int windowYPosition = 0;

// Set the width and height of your window.
int windowWidth = 800;
int windowHeight = 600;

int windowClientWidth = 800;
int windowClientHeight = 600;



LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:

        gpBitmap = new Bitmap(L"..\\Images\\background.jpg");//Loads background image
        if (gpBitmap == NULL)//If there is not image file
        {
            MessageBoxA(hWnd, "No file", "ERROR", MB_OK);
            PostQuitMessage(0);
        }
        else
        {
            initGame();
        }
        InvalidateRect(hWnd, 0, false);
        break;
    case WM_PAINT:
        HDC hdc;
        PAINTSTRUCT ps;
        hdc = BeginPaint(hWnd, &ps);
        if (gpBitmap)
        {
            //Window client width==border around window
            //Doesn't allow resizing
            SetWindowPos(hWnd, HWND_TOP, 0, 0, gpBitmap->GetWidth() + windowClientWidth, gpBitmap->GetHeight() + windowClientHeight, SWP_NOMOVE);
        }
        EndPaint(hWnd, &ps);
        break;
    case WM_KEYDOWN://Keyboard input (Only used for exiting program, moving player in CPlayer class
        switch (wParam)
        {
        case VK_ESCAPE://Quit program
            PostQuitMessage(0);
            break;
        }
        //InvalidateRect(hWnd, 0, false);//Makes program redraw image
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int showCmd)
{
    // Save handle to application instance global variable.
    applicationInstance = hInstance;
    ULONG_PTR           gdiplusToken;	// GDI+
    WNDCLASSEX wc = {};
    GdiplusStartupInput gdiplusStartupInput;	// GDI+
    // Initialize GDI+.
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);	// GDI+
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.hIconSm = 0;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = applicationInstance;
    wc.hCursor = LoadCursor(0, IDC_ARROW); // cursor to use for the class

    wc.lpszMenuName = 0;
    wc.lpszClassName = L"Alvaro Chavez Mixco Window Class";
    COLORREF r = RGB(255, 255, 255); // 0x00FFFFFF (BGRA, stored as ARGB (big endian)
    wc.hbrBackground = (HBRUSH)CreateSolidBrush(r);

    RegisterClassEx(&wc);

    windowHandle = CreateWindowEx(WS_EX_ACCEPTFILES,
        L"Alvaro Chavez Mixco Window Class",
        L"Alvaro Chavez Mixco Window Title",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        windowXPosition,
        windowYPosition,
        windowWidth,
        windowHeight,
        0,
        0,
        applicationInstance, 0);

    if (0 == windowHandle)
    {
        MessageBoxA(0, "CreateWindow Failed", "Application Error Message", 0);
        return false;
    }

    ShowWindow(windowHandle, showCmd);

    UpdateWindow(windowHandle);

    // Get the border and title bar dimensions so we can accurately resize our window in WM_PAINT
    RECT clientRectangle = {};
    RECT windowRectangle = {};

    GetClientRect(windowHandle, &clientRectangle);
    GetWindowRect(windowHandle, &windowRectangle);
    windowClientWidth = (windowRectangle.right - windowRectangle.left) - clientRectangle.right;
    windowClientHeight = (windowRectangle.bottom - windowRectangle.top) - clientRectangle.bottom;


    MSG msg;
    SecureZeroMemory(&msg, sizeof(MSG));

    int returnValue = 0; //  Used to check if GetMessage failed

    HDC hdc = GetDC(windowHandle);

    //Make program framre rate independent
    unsigned long lastTick = GetTickCount();
    unsigned long elapsed = lastTick;
    unsigned long currentTick = lastTick;
    bool gameOver = false;//Variable used to check if game is over.

    while (true)//Game loop, checks if thre is a message
    {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))//Checks if there is a message
        {
            if (msg.message == WM_QUIT)//Quit program if the message is quit
            {
                break;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else//If there are no messages update game
        {
            currentTick = GetTickCount();
            elapsed = min(currentTick - lastTick, 1000);//Milliseconds elapsed in this frame, it caps the time at 1 second so that the time isn't more than that.
            lastTick = currentTick;//Set last tick to current tick (next pass through loop will be the last tick);
            gameOver=updateGameObjects(elapsed);
            gameObjectCollision(elapsed);
            drawGameObjectsGDIPlus(hdc);//Draw

            if (gameOver == true)//If game is over exit the loop
            {
                break;
            }
        }

    }

    //Time for user to see score before closing
    Sleep(2500);

    //If there is no image loaded prevent program from crashing
    if (gpBitmap != NULL)
    {
        cleanupGame();//Clean all game code
    }

    //Delete global pointer of background
    delete gpBitmap;
    gpBitmap = NULL;

    GdiplusShutdown(gdiplusToken);	//GDI+
    return (int)msg.wParam;
}