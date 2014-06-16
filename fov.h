#ifndef an_fov
#define an_fov
#define WIN32_LEAN_AND_MEAN

#include "image.h"
#include "constants.h"
#include "dungeon.h"

class FOV
{
	Image texture;
	char *fovMap;
	DungeonGen *dungeon;
	int charX, charY;
	float mapX, mapY;
	int width, height;
	int roomNum;
	bool visible;

	void setFov(int x, int y, char val)
	{fovMap[x + width * y] = val;}

	int getRoom(int x, int y);
public:
	FOV();
	~FOV();
	bool initialize(Graphics *g, int width, int height, int ncols, TextureManager *textureM, DungeonGen *d);
	void update(float frameTime);
	void draw();
	void resetFov();
	void updateCharCoord(int x, int y)
	{charX = x;charY = y;}
	void updateOffset(int x, int y)
	{mapX = (float)x; mapY = (float)y;}
	void setDimensions(int x, int y)
	{width = x;height=y;}
	void setVisible(bool b)
	{visible = b;}
	int getRoomNum()
	{return roomNum;}
	bool getVisible()
	{return visible;}
	char getFov(int x, int y)
	{return fovMap[x + width * y];}
	void fillFov(int sx, int sy, int ex, int ey, char fill);
	void shadowFov();
	void forceUpdate()
	{roomNum = -1;}
};

#endif
