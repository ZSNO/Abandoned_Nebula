// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Game Engine constants.h v3.1
// Last modification: Dec-24-2013

#pragma once
#ifndef _CONSTANTS_H            // Prevent multiple definitions if this 
#define _CONSTANTS_H            // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

//=============================================================================
//                  Constants
//=============================================================================

// window
const char CLASS_NAME[] = "AbandonedNebula";
const char GAME_TITLE[] = "Abandoned Nebula";
const bool FULLSCREEN = false;              // windowed or fullscreen
const UINT GAME_WIDTH =  1024;               // width of game in pixels
const UINT GAME_HEIGHT = 768;               // height of game in pixels
 
// game
const bool VSYNC = false;                   // true locks display to vertical sync rate
const double PI = 3.14159265;
const float FRAME_RATE = 120.0f;                // the target frame rate (frames/sec)
const float MIN_FRAME_RATE = 10.0f;             // the minimum frame rate
const float MIN_FRAME_TIME = 1.0f/FRAME_RATE;   // minimum desired time for 1 frame
const float MAX_FRAME_TIME = 1.0f/MIN_FRAME_RATE; // maximum time used in calculations
const float MOVEMENTSPEED = 200.0f;

// graphic images
const char MENU_IMAGE[] =		"pictures\\menu.png";      // menu texture
const char CHARACTER_IMAGE[] =	"pictures\\char.png";//Character spritesheet
const char TILES_IMAGE[] =		"pictures\\update_tiles.png";//map tiles
const char ITEMS_IMAGE[] =		"pictures\\items.png";//item spritesheet
const char NOISE_IMAGE[] =		"pictures\\noise.png";//2048x2048 noise to scroll over screen
const char MENUBUTTON_IMAGE[] =		"pictures\\menu_button.png";
const char MENUBG_IMAGE[] =			"pictures\\menu_gui.png";
const char TITLETEXT_IMAGE[] =		"pictures\\font.png";
const char SHADOW_IMAGE[] =			"pictures\\shadow.png";
const char INVENTORY_IMAGE[] =		"pictures\\inventory.png";
const char ENEMY_IMAGE[] =			"pictures\\enemy.png";
const char DED_IMAGE[] =			"pictures\\ded.png";
const char WON_IMAGE[] =			"pictures\\win.png";
const char BAR_IMAGE[] =			"pictures\\bar.png";
const char CREDITS_IMAGE[] =		"pictures\\credits.png";
const char MENU_BACK_IMAGE[] =		"pictures\\menu_back.png";
const char MENU_STAR_IMAGE[] =		"pictures\\menu_star.png";
const char MENU_OVER_IMAGE[] =		"pictures\\menu_over.png";
const char ENEMY2_IMAGE[] =			"pictures\\enemy2.png";

const float TEX_SCALE = 4;
const int POTION_TYPES = 8;
const int INVENTORYSLOTS = 16;

// audio files required by audio.cpp
// WAVE_BANK must be location of .xwb file.
const char WAVE_BANK[]  = "audio\\sounds_all_up_in_this.xwb";
// SOUND_BANK must be location of .xsb file.
const char SOUND_BANK[] = "audio\\meow.xsb";

// audio cues
const char HIT[] = "hit";
const char HIT2[] = "hit2";
const char PICKUP[] = "pickup";
const char STATUS[] = "status";
const char ABANDONED[] = "abandoned";
const char CONGRATS[] = "congrats";
const char FLOOR[] = "floor";
const char LEVELUP[] = "levelup";
const char TERMINATED[] = "terminated";
const char STORECUE[] = "store";
const char SPACE[] = "spaceStation";
const char WATCH[] = "watchful";

// key mappings
// In this game simple constants are used for key mappings. If variables were used
// it would be possible to save and restore key mappings from a data file.
const UCHAR CONSOLE_KEY  = '`';         // ` key
const UCHAR ESC_KEY      = VK_ESCAPE;   // escape key
const UCHAR ALT_KEY      = VK_MENU;     // Alt key
const UCHAR ENTER_KEY    = VK_RETURN;   // Enter key

//=============================================================================
// Function templates for safely dealing with pointer referenced items.
// The functions defined by these templates may be called using a normal
// function call syntax. The compiler will create a function that replaces T
// with the type of the calling parameter.
//=============================================================================
// Safely release pointer referenced item
template <typename T>
inline void safeRelease(T& ptr)
{
    if (ptr)
    { 
        ptr->Release(); 
        ptr = NULL;
    }
}
#define SAFE_RELEASE safeRelease            // for backward compatiblility

// Safely delete pointer referenced item
template <typename T>
inline void safeDelete(T& ptr)
{
    if (ptr)
    { 
        delete ptr; 
        ptr = NULL;
    }
}
#define SAFE_DELETE safeDelete              // for backward compatiblility

// Safely delete pointer referenced array
template <typename T>
inline void safeDeleteArray(T& ptr)
{
    if (ptr)
    { 
        delete[] ptr; 
        ptr = NULL;
    }
}
#define SAFE_DELETE_ARRAY safeDeleteArray   // for backward compatiblility

// Safely call onLostDevice
template <typename T>
inline void safeOnLostDevice(T& ptr)
{
    if (ptr)
        ptr->onLostDevice(); 
}
#define SAFE_ON_LOST_DEVICE safeOnLostDevice    // for backward compatiblility

// Safely call onResetDevice
template <typename T>
inline void safeOnResetDevice(T& ptr)
{
    if (ptr)
        ptr->onResetDevice(); 
}
#define SAFE_ON_RESET_DEVICE safeOnResetDevice  // for backward compatiblility

//Dungeon room
const char WALL = '#';
const char DOOR = 'D';
const char AREA = ' ';
const char CHEST = 'C';
const char SECRET = 'S';
const char HTRAP = 'T';
const char TRAP = 'Y';
const int MINROOMX = 3;
const int MINROOMY = 3;
const int MAXROOMX = 9;
const int MAXROOMY = 9;
const char ENTRANCE = 'E';
const char EXIT = 'F';
const char STORE = 'H';

#endif
