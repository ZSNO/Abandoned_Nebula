// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// dashboard.cpp v1.0

#include "dashboard.h"

//=============================================================================
// Initialize the Pushbutton
// Pre: *graphics = pointer to Graphics object
//      *textureM = pointer to TextureManager object
//      *in = pointer to Input object
//      hwnd = handle to window
//      left, top = screen location
//      scale = scaling (zoom) amount
// Post: returns true on success, false on error
//=============================================================================
bool PushButton::initialize(Graphics *graphics, TextureManager *textureM, Input *in, HWND h,
                    int left, int top, float scale)
{
    try {
        Image::initialize(graphics, 0, 0, 0, textureM);
        spriteData.x = (float)left;
        spriteData.y = (float)top;
        spriteData.scale = scale;
        hwnd = h;                       // handle to the window
        input = in;                     // the input object
        switchRect.left = left;
        switchRect.top  = top;
        switchRect.right = (long)(left + spriteData.width * spriteData.scale);
        switchRect.bottom = (long)(top + spriteData.height * spriteData.scale);

    }
    catch(...)
    {
        return false;
    }
    //return okay
    return true;
}

//=============================================================================
// Checks for mouse click on pushbutton
//=============================================================================
void PushButton::update(float frameTime)
{
    if (!initialized || !visible)
        return;

    // calculate screen ratios incase window was resized
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    float screenRatioX = (float)GAME_WIDTH / clientRect.right;
    float screenRatioY = (float)GAME_HEIGHT / clientRect.bottom;

    if (input->getMouseLButton())           // if mouse left button
    {
        // if mouse clicked inside switch
        if (input->getMouseX()*screenRatioX >= switchRect.left &&
            input->getMouseX()*screenRatioX <= switchRect.right &&
            input->getMouseY()*screenRatioY >= switchRect.top &&
            input->getMouseY()*screenRatioY <= switchRect.bottom)
			setVisible(0);
    }
}

//=============================================================================
// Initialize the Bar 
// Pre: *graphics = pointer to Graphics object
//      *textureM = pointer to TextureManager object
//      left, top = screen location
//      scale = scaling (zoom) amount
//      color = color of bar
// Post: returns true on success, false on error
//=============================================================================
bool Bar::initialize(Graphics *graphics, TextureManager *textureM, int left,
                     int top, float scale, COLOR_ARGB color)
{
    try {
        Image::initialize(graphics, 256, 32, 0, textureM);
        spriteData.x = (float)left;
        spriteData.y = (float)top;
        spriteData.scale = scale;
        colorFilter = color;
    }
    catch(...)
    {
        return false;
    }
    //return okay
    return true;
}

//=============================================================================
// set the Bar size
//=============================================================================
void Bar::set(float p)
{
    if (p < 0)
        p = 0;
    else if(p > 100)
        p = 100;
    spriteData.rect.right = spriteData.rect.left + (LONG)(spriteData.width*p/100);
}