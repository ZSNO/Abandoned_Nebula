#include "items.h"
#include "character.h"

bool AN_ItemCon::initialize(Graphics *g, int width, int height, int ncols, TextureManager *textureM, DungeonGen *d, Character *p)
{
	for(int i=0;i<POTION_TYPES;i++)
		potionsReveal[i] = 0;
	for(int i=0;i<INVENTORYSLOTS;i++)
			inventory[i]=-1;
	gen = d;
	pl = p;

	//Default chest item
	AN_Item chest;
	chest.setItemType(4);
	chest.setTextureNum(0);
	chest.setInventSlot(INVENTORYSLOTS + 1);
	items.push_back(chest);

	//Default Trap item (doesn't need parameters!)
	AN_Item trap;
	trap.setInventSlot(INVENTORYSLOTS + 2);
	trap.setItemType(5);
	items.push_back(trap);

	//Default store item (doesn't need paramters!)
	AN_Item store;
	store.setInventSlot(INVENTORYSLOTS + 3);
	store.setItemType(5);
	items.push_back(store);

	std::random_device rd;
	potionColorOffset = rd() % POTION_TYPES;

	bool ok;
	ok = texture.initialize(g, width, height, ncols, textureM);
	if(ok)
		texture.setScale(TEX_SCALE);

	return ok;
}

void AN_ItemCon::draw(int x, int y, int itemNum, float scale, bool grid)
{
	if(itemNum == 2 || itemNum == 3)
		return;
	texture.setScale(scale);
	texture.setCurrentFrame(items.at(itemNum-1).getTextureNum());
	if(grid)
	{
		texture.setX(x + 4 * scale);
		texture.setY(y + 4 * scale);
	}
	else
	{
		texture.setX((float)x);
		texture.setY((float)y);
	}
	texture.draw(items.at(itemNum-1).getColorShift());
}

void AN_ItemCon::update(float frametime)
{
	texture.update(frametime);
}

void AN_ItemCon::cleanUpItems()
{
	for(int i=0;i<(int)items.size();i++)
	{
		if(items.at(i).getInventSlot() < 0)
			items.erase(items.begin() + i--);
	}
	//the i-- MAY crash the game!!!!!
	remakeInventList();
}

bool AN_ItemCon::genNewItem(int playerLevel, int x, int y)
{
	//Make new item and add to vector
	AN_Item newItem;

	//Create random device for MAXIMUM RANDOM!!!!!!!!!!!!!!!!!!
	//(random_device is a true random number generator!)
	std::random_device rd;

	//Determine colorshift of item
	COLOR_ARGB color;
	if(playerLevel < 2)
		color = graphicsNS::FILTER;
	else if(playerLevel < 4)
		color = (graphicsNS::YELLOW | graphicsNS::GRAY);
	else if(playerLevel < 6)
		color = graphicsNS::WASHGREEN;
	else if(playerLevel < 8)
		color = (graphicsNS::RED | graphicsNS::GRAY);
	else if(playerLevel < 10)
		color = (graphicsNS::BLUE | graphicsNS::GRAY);
	else if(playerLevel < 12)
		color = graphicsNS::WASHPURPLE;
	else if(playerLevel < 14)
		color = (graphicsNS::ORANGE | graphicsNS::GRAY);
	else if(playerLevel < 16)
		color = graphicsNS::LTGRAY;
	else
		color = graphicsNS::DKGRAY;
	if(playerLevel < 1)
		playerLevel = 1;

	//Scale factor for weapon stats, scales exponentially
	float scaleFactor = (playerLevel * playerLevel)/10.0f;

	//Determine item type and assign appropriate things
	char itemType = rd() % 4;
	newItem.setItemType(itemType);
	//Set item related things + texture
	switch(itemType)
	{
	case 0: //Item is a "potion" (syringe)
		newItem.setTextureNum(1);
		newItem.setItemType(0);
		newItem.setItemSpec(rd() % POTION_TYPES);
		newItem.setColorShift(this->getPotionColor(newItem.getItemSpec()));
		if(potionsReveal[newItem.getItemSpec()])
			newItem.setName(getPotionName(newItem.getItemSpec()));
		else
			newItem.setName("Mysterious Stimpak");
		newItem.setAttribute(0,1);
		break;
	case 1: //Item is a weapon
		newItem.setTextureNum(2);
		newItem.setItemType(1);
		newItem.setColorShift(color);
		newItem.setAttribute(0, rd() % (short)(scaleFactor * 10) + 1); //Set damage scale relative to player level
		//newItem.setAttribute(1, (rd() % 10 + 1) / 4); //Randomly set speed of weapon
		/*for(int i = 2;i < 8;i++) //no special attributes, scrapped
		{
			if(rd() % 50 < (unsigned int)playerLevel)
				newItem.setAttribute(i, (short)(rd() % 10 * scaleFactor));
		}*/

		//Create name reletive to attributes
		newItem.setName("Weapon");
		break;
	case 2: //Item is armor
		newItem.setItemType(2);
		newItem.setColorShift(color);
		//choose helmet, chest or leggings
		newItem.setItemSpec(rd() % 3);
		newItem.setTextureNum(newItem.getItemSpec() + 3);
		if(rd() % 50 < (unsigned)playerLevel)
			newItem.setAttribute(2, (short)(rd() % 10 * scaleFactor));
		newItem.setAttribute(3, (short)(rd() % 10 * scaleFactor/3) + 1);

		//Create name based on stats c:
		newItem.setName("Armor");
		break;
	case 3: //Item is currency
		newItem.setTextureNum(6);
		newItem.setItemType(3);
		newItem.setAttribute(0, (rd() % playerLevel)*10 + 10);
	}
	items.push_back(newItem);
	gen->setItemCell(x,y,items.size());
	return 1;
}

DWORD AN_ItemCon::getPotionColor(int num)
{
	num += potionColorOffset;
	if(num > 7)
		num -= 8;
	switch(num)
	{
	case 0:
		return D3DCOLOR_ARGB(255,128,128,255);
	case 1:
		return D3DCOLOR_ARGB(255,128,255,128);
	case 2:
		return D3DCOLOR_ARGB(255,255,128,128);
	case 3:
		return D3DCOLOR_ARGB(255,255,255,128);
	case 4:
		return D3DCOLOR_ARGB(255,255,128,255);
	case 5:
		return D3DCOLOR_ARGB(255,128,255,255);
	case 6:
		return D3DCOLOR_ARGB(255,255,192,128);
	case 7:
		return graphicsNS::FILTER;
	}
	return D3DCOLOR_ARGB(255,255,255,255);
}

int AN_ItemCon::nextInventSlot()
{
	int curMax = 0;
	for(int i=0;i<(int)items.size();i++) {
		if(items.at(i).getInventSlot() == curMax) {
			curMax++;
			i = 0;
		}
	}
	if(curMax >= INVENTORYSLOTS - 4)
		return -1;
	else
		return curMax;
}

int AN_ItemCon::nextInventSlot(char potionNum, int ignore)
{
	for(int i=0;i<(int)items.size();i++) {
		if(i == ignore || items.at(i).getInventSlot() == -1)
			continue;
		if(items.at(i).getItemType() == 0 && items.at(i).getItemSpec() == potionNum)
		{
			items.at(i).setAttribute(0, items.at(i).getAttribute(0) + 1);
			return -2;
		}
	}
	return nextInventSlot();
}

void AN_ItemCon::remakeInventList()
{
	for(int i=0;i<(int)items.size();i++) {
		if(items.at(i).getInventSlot() != -1 && !(items.at(i).getInventSlot() >= INVENTORYSLOTS))
			inventory[items.at(i).getInventSlot()] = i;
	}
}

void AN_ItemCon::setInventSlot(int item, int slot)
{
	if(item != -1)
		items.at(item).setInventSlot(slot);
	if(slot < 0)
		return;
	inventory[slot] = item;
}

void AN_ItemCon::removeInventSlot(int item, int slot)
{
	if(item != -1)
		items.at(item).setInventSlot(-1);
	if(slot < 0)
		return;
	inventory[slot] = -1;
}

int AN_ItemCon::getInventSlot(int slot)
{
	if(slot >= INVENTORYSLOTS)
		return -1;
	return inventory[slot];
}

void AN_ItemCon::usePotion(int i)
{
	potionsReveal[i] = 1;
	for(int j=0;j<(int)items.size();j++) {
		if(items.at(j).getItemType() == 0 && items.at(j).getItemSpec() == i)
			items.at(j).setName(getPotionName(i));
	}
	switch(i)
	{
	case 0: //reveal map
		pl->getFov()->fillFov(0,0,gen->getWidth(),gen->getHeight(),2);
		pl->getFov()->forceUpdate();
		pl->showMessage("Map revealed!");
		break;
	case 1: //add exp level
		pl->addExp(pl->getExpLevel());
		pl->showMessage("Level up!");
		break;
	case 2: //blind
		pl->getFov()->fillFov(0,0,gen->getWidth(),gen->getHeight(),3);
		pl->getFov()->forceUpdate();
		pl->showMessage("Blinded!");
		break;
	case 3: //heal
		pl->addHealth(pl->getMaxHealth()/2);
		pl->showMessage("Healed!");
		break;
	case 4: //freeze
		pl->setParalyzeTicks(1);
		pl->showMessage("Frozen!");
		break;
	case 5: //take away health from "fire"
		pl->addHealth(-pl->getMaxHealth()/6);
		pl->showMessage("On fire!");
		break;
	case 6: //take away health from "poison"
		pl->addHealth(-pl->getMaxHealth()/4);
		pl->showMessage("Poisoned!");
		break;
	case 7: //paralyze
		pl->setParalyzeTicks(3);
		pl->showMessage("Paralyzed!");
		break;
	}
}

std::string AN_ItemCon::getPotionName(int i)
{
	switch(i)
	{
	case 0:
		return "Reveal Map";
	case 1:
		return "Level Up";
	case 2:
		return "Blindess";
	case 3:
		return "Healing";
	case 4:
		return "Frozen";
	case 5:
		return "Fire";
	case 6:
		return "Poison";
	case 7:
		return "Paralyze";
	}
	return "YOU BROKE IT :C";
}

void AN_ItemCon::clear()
{
	for(int i=0;i<(int)items.size();)
			items.erase(items.begin() + i);
	for(int i=0;i<POTION_TYPES;i++)
		potionsReveal[i] = 0;
	for(int i=0;i<INVENTORYSLOTS;i++)
			inventory[i]=-1;
	//Default chest item
	AN_Item chest;
	chest.setItemType(4);
	chest.setTextureNum(0);
	chest.setInventSlot(INVENTORYSLOTS + 1);
	items.push_back(chest);

	//Default Trap item (doesn't need parameters!)
	AN_Item trap;
	trap.setInventSlot(INVENTORYSLOTS + 2);
	trap.setItemType(5);
	items.push_back(trap);

	//Default store item (doesn't need paramters!)
	AN_Item store;
	store.setInventSlot(INVENTORYSLOTS + 3);
	store.setItemType(5);
	items.push_back(store);

	std::random_device rd;
	potionColorOffset = rd() % POTION_TYPES;
}