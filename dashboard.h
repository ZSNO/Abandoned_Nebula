// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// dashboard.h v1.1

#ifndef _DASHBOARD_H            // Prevent multiple definitions if this 
#define _DASHBOARD_H            // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

class Dashboard;

#include "image.h"
#include "constants.h"
#include "textureManager.h"
#include "input.h"

class PushButton : public Image
{
    private:
    Input   *input;
    HWND    hwnd;
    RECT    switchRect;         // mouse click region
    public:
    // Initialize the Pushbutton
    // Pre: *graphics = pointer to Graphics object
    //      *textureM = pointer to TextureManager object
    //      *in = pointer to Input object
    //      hwnd = handle to window
    //      left, top = screen location
    //      scale = scaling (zoom) amount
    //      momentary = true for momentary, false for toggle
    bool initialize(Graphics *graphics, TextureManager *textureM, Input *in, HWND hwnd,
                    int left, int top, float scale);
    // Override update
    virtual void update(float frameTime);
};

class Bar : public Image
{
    public:
    // Initialize the Bar 
    // Pre: *graphics = pointer to Graphics object
    //      *textureM = pointer to TextureManager object
    //      left, top = screen location
    //      scale = scaling (zoom) amount
    //      color = color of bar
    bool initialize(Graphics *graphics, TextureManager *textureM, int left, int top,
                    float scale, COLOR_ARGB color);
    // Set Bar Size
    void set(float percentOn);
    // Override update so setRect() is not called.
    virtual void update(float frameTime)    {}
};

#endif

