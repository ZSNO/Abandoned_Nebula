#ifndef an_inventory
#define an_inventory
#define WIN32_LEAN_AND_MEAN

#include "image.h"
#include "items.h"
#include "input.h"
#include "textDX.h"
#include "character.h"
#include <sstream>

class Inventory
{
	Image background;
	Image itemDesc;
	AN_ItemCon *items;
	Input *input;
	TextDX *potionNum;
	TextDX *itemName;
	bool isDragging[INVENTORYSLOTS];
	int itemDragging, mousedOver;
	Character *player;
public:
	Inventory()
	{
		items = NULL;
		input = NULL;
		potionNum = NULL;
		for(int i=0;i<INVENTORYSLOTS;i++)
			isDragging[i] = 0;
		itemDragging = -1;
		mousedOver = -1;
	}
	bool initialize(Graphics *g, int width, int height, int ncols, TextureManager *textureM, Input *i, AN_ItemCon *it, TextDX *text, TextDX *text2, TextureManager *bg, Character *pl);
	void update(float frameTime);
	void draw();
	//Check to see what slot mouse is over
	int checkMouseLoc();
	//Check area for mouse
	bool checkArea(int sx, int sy, int ex, int ey);
};

#endif an_inventory