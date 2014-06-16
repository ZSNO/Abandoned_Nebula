// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// AbandonedNebulaClass.h

#ifndef _AbandonedNebula_H           // Prevent multiple definitions if this 
#define _AbandonedNebula_H           // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <string>
#include <sstream>
#include <cmath>
#include "game.h"
#include "textureManager.h"
#include "image.h"
#include "text.h"
#include "items.h"
#include "dungeon.h"
#include "dashboard.h"
#include "character.h"
#include "textDX.h"
#include "inventory.h"
#include "enemies.h"


//=============================================================================
// This class is the core of the game
//=============================================================================
class AbandonedNebula : public Game
{
private:
    //Textures
    TextureManager menuTexture;
	TextureManager tilesTexture;
	TextureManager noiseTexture;
	TextureManager menuButtonTexture;
	TextureManager menuGUITexture;
	TextureManager playerTexture;
	TextureManager shadowTexture;
	TextureManager itemsTexture;
	TextureManager inventoryTexture;
	TextureManager enemy1;
	TextureManager dedTexture;
	TextureManager winTexture;
	TextureManager barTexture;
	TextureManager creditsTexture;
	TextureManager menuStarTexture;
	TextureManager menuOverTexture;
	TextureManager enemy2;

	//Static images
    Image menu;
	Image menuStar;
	Image menuOver;
	Image noise;
	Image menuGUI;
	Image ded;
	Image won;
	Image creditsBG;

	//Text classes
	Text *titleText;
	TextDX *fpsCounter;
	TextDX *invText;
	TextDX *invTitle;

	//GUI Elements
	Bar health;
	Bar exp;
	PushButton startGame;
	PushButton quitGame;
	PushButton resumeGame;
	PushButton credits;

	//Custom Classes
	DungeonGen *dungGen;
	Character player;
	AN_ItemCon items;
	Inventory inventory;
	EnemyCon enemies;

	//misc
	bool hasPlayedAN;
	float timeBeforePlay;
	float resetTime;

public:
    // Constructor
    AbandonedNebula();
    // Destructor
    virtual ~AbandonedNebula();
    // Initialize the game
    void initialize(HWND hwnd);
    void update();      // must override pure virtual from Game
    void ai();          // "
    void collisions();  // "
    void render();      // "
    void releaseAll();
    void resetAll();
	void consoleCommand();
	void reset();
	DungeonGen* getDungeon()
	{return dungGen;}
	AN_ItemCon* getItemCon()
	{return &items;}
};

#endif
