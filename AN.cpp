// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// AbandonedNebulaClass.cpp v1.1
// This class is the core of the game

#include "AN.h"

//=============================================================================
// Constructor
//=============================================================================
AbandonedNebula::AbandonedNebula()
{
    titleText = new Text();
	fpsCounter = new TextDX();
	invText = new TextDX();
	invTitle = new TextDX();
	dungGen = new DungeonGen();
	hasPlayedAN = 0;
	timeBeforePlay = 0;
	resetTime = 0;
}

//=============================================================================
// Destructor
//=============================================================================
AbandonedNebula::~AbandonedNebula()
{
    releaseAll();           // call onLostDevice() for every graphics item
    safeDelete(titleText);
	safeDelete(fpsCounter);
	safeDelete(invText);
	safeDelete(invTitle);
	safeDelete(dungGen);
}

//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void AbandonedNebula::initialize(HWND hwnd)
{
    Game::initialize(hwnd); // throws GameError

    // menu
	if (!menuTexture.initialize(graphics,MENU_BACK_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu texture"));
	if (!menu.initialize(graphics,0,0,0,&menuTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu"));
	if (!menuStarTexture.initialize(graphics,MENU_STAR_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu star texture"));
	if (!menuStar.initialize(graphics,0,0,0,&menuStarTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu star"));
	if (!menuOverTexture.initialize(graphics,MENU_OVER_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu over texture"));
	if (!menuOver.initialize(graphics,0,0,0,&menuOverTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu over"));

    // initialize fonts
	if(!titleText->initialize(graphics, TITLETEXT_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing title font"));
	if(!fpsCounter->initialize(graphics, 32, 1, 0, "Arial"))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing fps counter font"));
	if(!invText->initialize(graphics, 24, 1, 0, "Arial"))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing inventory text font"));
	if(!invTitle->initialize(graphics, 32, 1, 0, "Arial"))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing inventory title font"));

	//tiles texture
	if(!tilesTexture.initialize(graphics,TILES_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing tiles texture"));

	//dungeon generator and display
	if(!dungGen->initialize(graphics,24,24,13,&tilesTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initalizing dungeon generator"));

	//noise overlay
	if(!noiseTexture.initialize(graphics,NOISE_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing noise texture"));
	if(!noise.initialize(graphics,0,0,0,&noiseTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initalizing noise Image"));

	//Items container
	if(!itemsTexture.initialize(graphics,ITEMS_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initalizing item texture"));
	if(!items.initialize(graphics,16,16,4,&itemsTexture, dungGen, &player))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initalizing items container"));

	//menu gui
	if(!menuButtonTexture.initialize(graphics,MENUBUTTON_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu button texture"));
	if(!menuGUITexture.initialize(graphics,MENUBG_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu gui texture"));
	if(!menuGUI.initialize(graphics,0,0,0,&menuGUITexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu gui Image"));

	//credits
	if(!creditsTexture.initialize(graphics,CREDITS_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing credits texture"));
	if(!creditsBG.initialize(graphics,0,0,0,&creditsTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing creditsBG"));

	creditsBG.setX((float)((int)GAME_WIDTH/2 - creditsBG.getWidth()/2));
	creditsBG.setY((float)((int)GAME_HEIGHT/2 - creditsBG.getHeight()/2));
	creditsBG.setVisible(0);


	menuGUI.setX((float)((int)GAME_WIDTH/2 - menuGUI.getWidth()/2));
	menuGUI.setY((int)GAME_HEIGHT/2);

	if(!startGame.initialize(graphics,&menuButtonTexture,input,hwnd,(int)(menuGUI.getX() + 15),(int)(menuGUI.getY() + 15), 1))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initalizing start button"));
	if(!quitGame.initialize(graphics,&menuButtonTexture,input,hwnd,(int)(menuGUI.getX() + 15),(int)(menuGUI.getY() + 125), 1))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initalizing quit button"));
	if(!credits.initialize(graphics,&menuButtonTexture,input,hwnd,(int)(menuGUI.getX() + 15),(int)(menuGUI.getY() + 255), 1))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initalizing credits button"));

	//Escape menu button
	if(!resumeGame.initialize(graphics,&menuButtonTexture,input,hwnd,(int)(menuGUI.getX() + 15),(int)(menuGUI.getY() + 15), 1))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initalizing quit button"));
	resumeGame.setVisible(0);

	//shadow fov
	if(!shadowTexture.initialize(graphics,SHADOW_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing noise texture"));

	//Player
	if(!playerTexture.initialize(graphics,CHARACTER_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing character texture"));
	if(!player.initialize(this,24,24,4,&playerTexture,&shadowTexture,invTitle))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing player object"));

	//Inventory
	if(!inventoryTexture.initialize(graphics,INVENTORY_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing inventory texture"));
	if(!inventory.initialize(graphics,0,0,0,&inventoryTexture,input,&items,invText,invTitle,&menuGUITexture,&player))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing inventory object"));

	//Enemies
	if(!enemy1.initialize(graphics,ENEMY_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing enemy1 texture"));
	if(!enemy2.initialize(graphics,ENEMY2_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing enemy2 texture"));
	enemies.initialize(graphics,1,0,&player);
	enemies.newEnemyType(24,24,4,&enemy1);
	enemies.newEnemyType(24,24,4,&enemy2);
	player.setEnemyCon(&enemies);
	
	//Death image
	if (!dedTexture.initialize(graphics,DED_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ded texture"));
	if (!ded.initialize(graphics,0,0,0,&dedTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ded"));
	ded.setVisible(0);

	//win image
	if (!winTexture.initialize(graphics,WON_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing win texture"));
	if (!won.initialize(graphics,0,0,0,&winTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing won"));
	won.setVisible(0);

	//Bars
	if (!barTexture.initialize(graphics,BAR_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing bar texture"));
	if (!health.initialize(graphics,&barTexture,30,660,1.0f,graphicsNS::RED))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing hp bar"));
	if (!exp.initialize(graphics,&barTexture,30,720,1.0f,graphicsNS::GREEN))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing hp bar"));

    return;
}

void AbandonedNebula::reset()
{
	player.reset();
	dungGen->reset();
	dungGen->setDimensions(25,25);
	dungGen->generate();
	enemies.reset(25,25);
	std::random_device rd;
	for(int i=0;i<8;i++)
	{
		int x = rd() % 25;
		int y = rd() % 25;
		if(!dungGen->getColCell(x,y))
		{
			i--;continue;
		}
		enemies.newEnemy(x,y);
	}
	player.setCoord(dungGen->getEntX(),dungGen->getEntY());
	items.cleanUpItems();
	won.setVisible(0);
	ded.setVisible(0);
	items.clear();
}

//=============================================================================
// Update all game items
//=============================================================================
void AbandonedNebula::update()
{
	menu.update(frameTime);
	if(!hasPlayedAN && timeBeforePlay > 0.1)
	{
		audio->playCue(ABANDONED);
		audio->playCue(SPACE);
		hasPlayedAN = 1;
	}
	else
		timeBeforePlay += frameTime;

	if(resetTime > 0)
		resetTime -= frameTime;
	if(resetTime < 0)
	{
		resetTime = 0;
		reset();
	}

	resumeGame.update(frameTime);
	if(input->wasKeyPressed(VK_ESCAPE) && !startGame.getVisible())
	{
		if(resumeGame.getVisible())
		{
			menuGUI.setVisible(0);
			resumeGame.setVisible(0);
			quitGame.setVisible(0);
			titleText->setVisible(0);
		}
		else
		{
			menuGUI.setVisible(1);
			resumeGame.setVisible(1);
			quitGame.setVisible(1);
			titleText->setVisible(1);
		}
	}
	if(!startGame.getVisible() && menuGUI.getVisible() && !resumeGame.getVisible())
	{
		menuGUI.setVisible(0);
		resumeGame.setVisible(0);
		quitGame.setVisible(0);
		titleText->setVisible(0);
	}
	credits.update(frameTime);
	if(!credits.getVisible() && menuGUI.getVisible() && !resumeGame.getVisible())
	{
		input->setMouseLButton(0);
		menuGUI.setVisible(0);
		startGame.setVisible(0);
		quitGame.setVisible(0);
		creditsBG.setVisible(1);
	}
	if(input->getMouseLButton() && creditsBG.getVisible())
	{
		input->setMouseLButton(0);
		menuGUI.setVisible(1);
		startGame.setVisible(1);
		quitGame.setVisible(1);
		creditsBG.setVisible(0);
		credits.setVisible(1);
	}

	startGame.update(frameTime);
	if(!startGame.getVisible() && menu.getVisible() && credits.getVisible())
	{
		menu.setVisible(0);
		menuStar.setVisible(0);
		menuOver.setVisible(0);
		quitGame.setVisible(0);
		menuGUI.setVisible(0);
		credits.setVisible(0);
		dungGen->setDimensions(25,25);
		player.getFov()->setDimensions(25,25);
		player.getFov()->resetFov();
		dungGen->generate();
		dungGen->setVisible(1);
		player.getFov()->setVisible(1);
		titleText->setVisible(0);
		player.setCoord(dungGen->getEntX(),dungGen->getEntY());
		player.setVisible(1);
		enemies.reset(dungGen->getWidth(),dungGen->getHeight());
		std::random_device rd;
		for(int i=0;i<8;i++)
		{
			int x = rd() % 25;
			int y = rd() % 25;
			if(!dungGen->getColCell(x,y))
			{
				i--;continue;
			}
			enemies.newEnemy(x,y);
		}
		audio->stopCue(SPACE);
		audio->playCue(WATCH);
	}

	quitGame.update(frameTime);
	if(!quitGame.getVisible() && menuGUI.getVisible() && !creditsBG.getVisible())
	{
		PostQuitMessage(0);
	}

	player.update(frameTime);
	enemies.update(frameTime);
	inventory.update(frameTime);
    dungGen->update(frameTime);

	health.set((float)player.getHealth()/(float)player.getMaxHealth() * 100);
	exp.set((float)player.getExp()/(float)player.getExpLevel() * 100);

	noise.update(frameTime);
	noise.setX(noise.getX() + frameTime * 5000);
	noise.setY(noise.getY() + frameTime * 7000);
	if(noise.getX() > 0)
		noise.setX(noise.getX() - noise.getWidth());
	if(noise.getY() > 0)
		noise.setY(noise.getY() - noise.getHeight());

	menuStar.update(frameTime);
	menuStar.setX(menuStar.getX() + frameTime * 12);
	menuStar.setY(menuStar.getY() + frameTime * 8);
	if(menuStar.getX() > 0)
		menuStar.setX(menuStar.getX() - menuStar.getWidth());
	if(menuStar.getY() > 0)
		menuStar.setY(menuStar.getY() - menuStar.getHeight());

	if(player.isDed() && !ded.getVisible())
	{
		ded.setVisible(1);
		resetTime = 10.0f;
	}
	if(player.isWinner() && !won.getVisible())
	{
		won.setVisible(1);
		resetTime = 10.0f;
	}
}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void AbandonedNebula::ai()
{}

//=============================================================================
// Handle collisions
//=============================================================================
void AbandonedNebula::collisions()
{}

//=============================================================================
// Render game items
//=============================================================================
void AbandonedNebula::render()
{
    graphics->spriteBegin();                // begin drawing sprites
	graphics->get3Ddevice()->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_NONE);

	dungGen->draw();
	//enemies.draw();
	player.draw();

	inventory.draw();
	ded.draw();
	won.draw();
	invText->print("Health:",30,635);
	health.draw(graphicsNS::FILTER);
	exp.draw(graphicsNS::FILTER);
	std::ostringstream stat;
	stat << player.getHealth() << "/" << player.getMaxHealth();
	invText->print(stat.str(),35,665);
	stat.clear();
	stat.str("");
	stat << "EXP: Level " << (int)player.getLevel();
	invText->print(stat.str(),30,695);
	stat.clear();
	stat.str("");
	stat << player.getExp() << "/" << player.getExpLevel();
	invText->print(stat.str(),35,725);
	stat.clear();
	stat.str("");
	stat << "Station Level: " << player.getDunLevel();
	invText->print(stat.str(),30,30);
	stat.clear();
	stat.str("");
	stat << "Score/$$$: " << player.getMoney();
	invText->print(stat.str(),30,60);

	RECT region;
	region.top = 450;
	region.bottom = 600;
	region.left = 0;
	region.right = GAME_WIDTH;

	if(won.getVisible())
	{
		invTitle->setFontColor(D3DCOLOR_ARGB(255,41,137,0));
		std::ostringstream score;
		score << "Score: " << player.getMoney() * 2;
		score << "\nReseting in " << floor(resetTime*100)/100;
		invTitle->print(score.str(),region,DT_CENTER);
	}
	if(ded.getVisible())
	{
		invTitle->setFontColor(D3DCOLOR_ARGB(255,163,55,50));
		std::ostringstream score;
		score << "Score: " << player.getMoney();
		score << "\nReseting in " << floor(resetTime*100)/100;
		invTitle->print(score.str(),region,DT_CENTER);
	}
	invTitle->setFontColor(graphicsNS::WHITE);

	noise.draw();
	float x = noise.getX();
	float y = noise.getY();

	if(x < -noise.getWidth() * noise.getScale() + (int)GAME_WIDTH)
	{
		noise.setX(x + noise.getWidth() * noise.getScale());
		noise.draw();
	}
	if(y < -noise.getWidth() * noise.getScale() + (int)GAME_HEIGHT)
	{
		noise.setY(y + noise.getWidth() * noise.getScale());
		noise.draw();
		noise.setX(x);
		if(noise.getX() < -noise.getWidth() * noise.getScale() + (int)GAME_WIDTH)
			noise.draw();
	}
	noise.setY(y);

	menu.draw();

	menuStar.draw();
	x = menuStar.getX();
	y = menuStar.getY();

	if(x < -menuStar.getWidth() + (int)GAME_WIDTH)
	{
		menuStar.setX(x + menuStar.getWidth());
		menuStar.draw();
	}
	if(y < -menuStar.getHeight() + (int)GAME_HEIGHT)
	{
		menuStar.setY(y + menuStar.getHeight());
		menuStar.draw();
		menuStar.setX(x);
		if(menuStar.getX() < -menuStar.getWidth() + (int)GAME_WIDTH)
			menuStar.draw();
	}
	menuStar.setY(y);

	menuOver.draw();
	menuGUI.draw();
	startGame.draw();
	quitGame.draw();
	resumeGame.draw();
	credits.draw();
	creditsBG.draw();
	if(creditsBG.getVisible())
	{
		titleText->setProportional(1);
		titleText->setFontColor(graphicsNS::GRAY);
		titleText->setBackColor(graphicsNS::FILTER);
		titleText->setScale(0.5f);
		titleText->print("Zach Nofz\n\n\n\nTokiah Jade\n\n\n\nN0b0t\n\n\n\nChuck Kelly", (int)GAME_WIDTH/2, (int)creditsBG.getY() + 30, textNS::CENTER);
		titleText->setScale(0.3f);
		titleText->print("\n\nProgrammer, Designer\n\n\n\n\n\n\nVoice acting\n\n\n\n\n\n\nMusic\n\n\n\n\n\nGame engine",(int)GAME_WIDTH/2, (int)creditsBG.getY() + 30, textNS::CENTER);
	}

#if defined(_DEBUG)
		std::ostringstream fpsText;
		fpsText << "FPS: " << fps; 
		if(player.getVisible())
		{
			fpsText << "\nPlayerPos\nX" << player.getCurX() << "\nY" << player.getCurY();
			fpsText << "\nHealth: " << player.getHealth();
			int it = dungGen->getItemCell(player.getCurX(),player.getCurY());
			if(it != 0)
				fpsText << "\nItemNum: " << it << "\nItem texnum: " << items.getItem(it-1)->getTextureNum();

			fpsText << "\n" << inventory.checkMouseLoc();
			fpsText << "\nInventorySlots:\n";
			for(int i=0;i<INVENTORYSLOTS;i++)
				fpsText << items.getInventSlot(i) << ", ";
		}
		if(enemies.getNumber())
		{
			Enemy *meow = enemies.getEnemyObj(0);
			fpsText << "\nEnemy is moving: " << meow->isMoving() << " (" << player.isMoving() << " : " << meow->isDoneMoving() << ")";
			fpsText << "\noffX: " << meow->getCurX() << "(" << meow->getCurX() - player.getCurX() << ")";
			fpsText << "\noffY: " << meow->getCurY() << "(" << meow->getCurY() - player.getCurY() << ")";
			fpsText << "\nDir: " << meow->getDir();
			fpsText << "\nHealth: " << meow->getHealth();
			fpsText << "\nDebug: " << meow->debugM();
		}
		fpsCounter->print(fpsText.str(),10,10);
#endif

	//font on main menu
	if(menu.getVisible() && !creditsBG.getVisible())
	{
		titleText->setProportional(1);
		titleText->setFontColor(graphicsNS::GRAY);
		titleText->setBackColor(graphicsNS::FILTER);
		titleText->setScale(1);
		titleText->print("Abandoned",120,50);
		titleText->print("Nebula",270,120);
		titleText->setScale(0.5);
		titleText->print("Start",(int)(startGame.getX() + 35), (int)(startGame.getY() + 30));
		titleText->print("Credit",(int)(credits.getX() + 25), (int)(credits.getY() + 30));
		titleText->print("Quit",(int)(quitGame.getX() + 70), (int)(quitGame.getY() + 30));
	}
	if(resumeGame.getVisible())
	{
		titleText->setProportional(1);
		titleText->setFontColor(graphicsNS::GRAY);
		titleText->setBackColor(graphicsNS::FILTER);
		titleText->setScale(0.5);
		titleText->print("Resume",(int)(resumeGame.getX() + 12), (int)(resumeGame.getY() + 30));
		titleText->print("Quit",(int)(quitGame.getX() + 70), (int)(quitGame.getY() + 30));
	}

	
    graphics->spriteEnd();                  // end drawing sprites
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void AbandonedNebula::releaseAll()
{
	noiseTexture.onLostDevice();
	menuGUITexture.onLostDevice();
	menuButtonTexture.onLostDevice();
	menuTexture.onLostDevice();
	tilesTexture.onLostDevice();
	titleText->onLostDevice();
	playerTexture.onLostDevice();
	fpsCounter->onLostDevice();
	shadowTexture.onLostDevice();
	itemsTexture.onLostDevice();
	inventoryTexture.onLostDevice();
	invText->onLostDevice();
	invTitle->onLostDevice();
	enemy1.onLostDevice();
	dedTexture.onLostDevice();
	winTexture.onLostDevice();
	barTexture.onLostDevice();
	creditsTexture.onLostDevice();
	enemy2.onLostDevice();
    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void AbandonedNebula::resetAll()
{
	noiseTexture.onResetDevice();
	menuTexture.onResetDevice();
	menuGUITexture.onResetDevice();
	menuButtonTexture.onResetDevice();
	tilesTexture.onResetDevice();
	titleText->onResetDevice();
	playerTexture.onResetDevice();
	fpsCounter->onResetDevice();
	shadowTexture.onResetDevice();
	itemsTexture.onResetDevice();
	inventoryTexture.onResetDevice();
	invText->onResetDevice();
	invTitle->onResetDevice();
	enemy1.onResetDevice();
	dedTexture.onResetDevice();
	winTexture.onResetDevice();
	barTexture.onResetDevice();
	creditsTexture.onResetDevice();
	enemy2.onResetDevice();
    Game::resetAll();
    return;
}

void AbandonedNebula::consoleCommand()
{
	command = console->getCommand();    // get command from console

    if(command == "")                   // if no command
        return;

    if (command == "help")              // if "help" command
    {
        console->print("Console Commands:");
        console->print("fps - toggle display of frames per second");
		console->print("fov - toggles FOV for player");
		console->print("makeItem - creates new random item above player");
		console->print("incLevel - increments player level");
		console->print("decLevel - deincrements player level");
        return;
    }

    else if (command == "fps")
    {
        fpsOn = !fpsOn;                 // toggle display of fps
        if(fpsOn)
            console->print("fps On");
        else
            console->print("fps Off");
    }
	else if(command == "fov")
	{
		if(player.getFov()->getVisible())
			player.getFov()->setVisible(0);
		else
			player.getFov()->setVisible(1);
	}
	else if(command == "makeItem" || command == "m")
	{
		if(items.genNewItem((int)player.getLevel(),player.getCurX(),player.getCurY()-1))
			console->print("Item created.");
	}
	else if(command == "incLevel" || command == "i")
	{
		player.setLevel(player.getLevel()+1);
		std::ostringstream meow;
		meow << (int)player.getLevel();
		console->print(meow.str());
	}
	else if(command == "decLevel")
	{
		player.setLevel(player.getLevel()-1);
		std::ostringstream meow;
		meow << (int)player.getLevel();
		console->print(meow.str());
	}
	else if(command == "items")
	{
		std::ostringstream num;
		num << items.getNumItems();
		console->print(num.str());
	}
	else if(command == "makeEnemy" || command == "e")
	{
		enemies.newEnemy(player.getCurX()-2, player.getCurY()-3);
		console->print("New enemy created!");
	}
	else if(command == "win")
		player.setWinner(1);
	else if(command == "god")
	{
		player.setMaxHealth(32000);
		player.addHealth(32000);
	}
	else if(command == "para")
		player.setParalyzeTicks(2);
	else
		console->print("Invalid command entered!");

}