#ifndef an_items
#define an_items
#define WIN32_LEAN_AND_MEAN

#include "image.h"
#include "constants.h"
#include "dungeon.h"
#include <string>
#include <vector>
#include <random>

class Character;

//Item attributes (8):
//[damage, speed, extra health, extra armor, fire chance, poison chance, freeze chance, paralyze chance]

//Item types
//0=potion 1=weapon 2=armor 3=currency 4=chest

//Item Specs:
//Potion: 0-7 = potions from randomized list
//Weapon: N/A
//Armor: 0=leggings 1=chest 2=helmet

class AN_Item
{
private:
	char itemType; //We don't have many itemtypes, so use a small amount of memory
	int textureNum;
	COLOR_ARGB colorShift;
	std::string name;
	int inventSlot;
	short attributes[8];
	int x, y;
	char itemSpec; //even more specific than itemType
public:
	//Constructor
	AN_Item()
	{
		itemType = 0;
		textureNum = 0;
		colorShift = graphicsNS::FILTER;
		name = "";
		inventSlot = -1;
		for(int i = 0;i<8;i++)
			attributes[i] = 0;
		itemSpec = 0;
	}
	//~AN_Item();
	//Get the type of item it is.
	//e.g. armor, weapon, currency, potion
	char getItemType()
	{return itemType;}
	//Set the type of item it is.
	void setItemType(char type)
	{itemType = type;}
	//Get what texture it is from the spritesheet
	int getTextureNum()
	{return textureNum;}
	//Set the textureNumber
	void setTextureNum(int num)
	{textureNum = num;}
	//Get the color filter of the item
	COLOR_ARGB getColorShift()
	{return colorShift;}
	//Set the color filter of the item
	void setColorShift(COLOR_ARGB col)
	{colorShift = col;}
	//Get the item's name
	std::string getName()
	{return name;}
	//Set the item's name
	void setName(std::string n)
	{name = n;}
	//Sees if item is in player's inventory
	int getInventSlot()
	{return inventSlot;}
	//Sets the next item in linked list
	void setInventSlot(int b)
	{inventSlot = b;}
	//Get attribute from array
	short getAttribute(int att)
	{return attributes[att];}
	//Set attribute in array
	void setAttribute(int att, short val)
	{attributes[att] = val;}
	//Set item spec type 
	void setItemSpec(char p)
	{itemSpec = p;}
	//get item spec type
	char getItemSpec()
	{return itemSpec;}
};

//Abandoned Nebula Item Container
class AN_ItemCon
{
private:
	std::vector<AN_Item> items;
	Image texture;
	bool potionsReveal[POTION_TYPES];
	DungeonGen *gen;
	int inventory[INVENTORYSLOTS];
	void remakeInventList();
	int potionColorOffset;
	Character *pl;
public:
	//basic initialize for the texture
	virtual bool initialize(Graphics *g, int width, int height, int ncols, TextureManager *textureM, DungeonGen *d, Character *p);
	//draw all items using the current x,y as a displacement
	virtual void draw(int x, int y, int itemNum, float scale, bool grid);
	//update texture
	virtual void update(float);
	//generate new item based on player's level with where to put said item
	bool genNewItem(int playerLevel, int x, int y);
	//clean up all items on the map not picked up
	void cleanUpItems();
	DWORD getPotionColor(int num);
	AN_Item* getItem(int it)
	{return &items.at(it);}
	void usePotion(int i);
	void deleteItem(int i)
	{items.erase(items.begin() + i);remakeInventList();}
	int nextInventSlot();
	int nextInventSlot(char potionNum, int ignore);
	void setInventSlot(int item, int slot);
	void removeInventSlot(int item, int slot);
	int getInventSlot(int slot);
	int getNumItems()
	{return (int)items.size();}
	int getTextureSize(bool wh)
	{if(!wh){return (int)(texture.getWidth()*texture.getScale());}else{return (int)(texture.getHeight()*texture.getScale());}}
	std::string getPotionName(int i);
	void clear();
};

#endif