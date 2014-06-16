#ifndef an_dungeon
#define an_dungeon
#define WIN32_LEAN_AND_MEAN

#include "gameError.h"
#include "constants.h"
#include "image.h"
#include <iostream>
#include <random>
#include <vector>

//Sturcture used for walls, includes the position and direction from the room
struct DungeonCoord
{
	int x;
	int y;
	int dir;
	DungeonCoord()
	{};
	DungeonCoord(int a, int b, int c) : x(a), y(b), dir(c)
	{};
};

//Dungeon Room struct, startx,y and endx,y plus room type
struct DungeonRoom
{
	int sx, sy, ex, ey, type;
	bool hasChest;
	DungeonRoom()
	{hasChest = 0;}
};

//Main Dungeon class
class DungeonGen
{
private:
	char *map;
	char *tileMap;
	bool *colMap;
	char *itemMap;

	int width, height, area;
	int minRoom[2], maxRoom[2];
	float dugPercentage, dug;
	std::vector<DungeonCoord> walls;
	std::vector<DungeonRoom> rooms;
	float offsetX, offsetY;
	Image texture;
	bool visible;
	int entX, entY;

	bool checkArea(int, int, int, int);
	int cRandom(int, int);
	bool wRandom(int, int);
	void fillMap(char);
	void setCell(int, int, char);
	bool createRoom(int, int, int, int, int, int, bool);
	void populateRoom(DungeonRoom);
	void createTilemap();
	void setColCell(int, int, bool);

public:
	DungeonGen() : width(25), height(25), area(25 * 25)
	{
		minRoom[0] = MINROOMX;
		minRoom[1] = MINROOMY;
		maxRoom[0] = MAXROOMX;
		maxRoom[1] = MAXROOMY;
		dugPercentage = 0.5;
		dug = 0.0;
		offsetX = 0;
		offsetY = 0;
		visible = 0;
		map = NULL;
		tileMap = NULL;
		colMap = NULL;
		itemMap = NULL;
	};
	DungeonGen(int w, int h) : width(w), height(h), area(w*h)
	{
		minRoom[0] = MINROOMX;
		minRoom[1] = MINROOMY;
		maxRoom[0] = MAXROOMX;
		maxRoom[1] = MAXROOMY;
		dugPercentage = 0.5;
		dug = 0.0;
		offsetX = 0;
		offsetY = 0;
		visible = 0;
		map = NULL;
		tileMap = NULL;
		colMap = NULL;
		itemMap = NULL;
	};
	~DungeonGen()
	{
		if(map != NULL)
			delete[] map;
		if(tileMap != NULL)
			delete[] tileMap;
		if(colMap != NULL)
			delete[] colMap;
		if(itemMap != NULL)
			delete[] itemMap;
	};
	void generate();
	void setDimensions(int x, int y)
	{width=x;height=y;area=x*y;}

	bool initialize(Graphics *g, int width, int height, int ncols, TextureManager *textureM);
	void updateOffset(float x, float y);
	void draw();
	void update(float frameTime);
	void setVisible(bool vis)
	{visible = vis;}
	bool getVisible()
	{return visible;}
	char getCell(int, int);
	bool getColCell(int, int);
	char getItemCell(int, int);
	Image getTexture()
	{return texture;}
	float getOffX()
	{return offsetX;}
	float getOffY()
	{return offsetY;}
	int getEntX()
	{return entX;}
	int getEntY()
	{return entY;}
	void setItemCell(int, int, char);
	void setTile(int, int, char);
	char getTile(int, int);
	void reset();
	int getRoomCount()
	{return rooms.size();}
	DungeonRoom* getRoom(int r)
	{return &(rooms.at(r));}
	int getWidth()
	{return width;}
	int getHeight()
	{return height;}
	void setOffset(float, float);
};

#endif