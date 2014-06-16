#include "fov.h"

FOV::FOV()
{
	fovMap = NULL;
	dungeon = NULL;
	charX = 0;
	charY = 0;
	height = 0;
	width = 0;
	roomNum = -1;
	visible = 0;
}

FOV::~FOV()
{
	if(fovMap != NULL)
		delete[] fovMap;
}

bool FOV::initialize(Graphics *g, int width, int height, int ncols, TextureManager *textureM, DungeonGen *d)
{
	dungeon = d;
	bool ok = texture.initialize(g, width, height, ncols, textureM);
	if(ok)
		texture.setScale(TEX_SCALE);
	return ok;
}

void FOV::fillFov(int sx, int sy, int ex, int ey, char fill)
{
	for(int i = sy;i < ey;i++) {
		for(int j = sx;j < ex;j++) {
			setFov(j,i,fill);
		}
	}
	for(int i = sy-1;i <= ey+1;i++) {
		for(int j = sx-1;j < ex+1;j++) {
			if((i == sy - 1) || (i == ey) || (j == sx - 1) || (j == ex))
			{
				if ((i == sy - 1 && j == sx - 1) || (i == ey && j == ex))
					continue;
				if ((i == sy - 1 && j == ex) || (i == ey && j == sx - 1))
					continue;
				if(dungeon->getCell(j,i) == DOOR)
					setFov(j,i,2);
			}
		}
	}
}

void FOV::shadowFov()
{
	for(int i = 0;i < height;i++) {
		for(int j = 0;j < width;j++) {
			if(getFov(j,i) == 2)
				setFov(j,i,1);
		}
	}
}

void FOV::update(float frameTime)
{
	texture.update(frameTime);
	int tempRoomNum = getRoom(charX, charY);
	DungeonRoom *tempRoom;
	if(roomNum == tempRoomNum)
		return;
	roomNum = tempRoomNum;
	shadowFov();
	if(roomNum == -1)
	{
		if(getRoom(charX-1,charY) != -1)
		{
			//update lighting for rooms east and west of player
			tempRoom = dungeon->getRoom(getRoom(charX-1,charY));
			fillFov(tempRoom->sx,tempRoom->sy-1,tempRoom->ex,tempRoom->ey,2);

			tempRoom = dungeon->getRoom(getRoom(charX+2,charY));
			fillFov(tempRoom->sx,tempRoom->sy-1,tempRoom->ex,tempRoom->ey,2);
		}
		if(getRoom(charX,charY-1) != -1)
		{
			tempRoom = dungeon->getRoom(getRoom(charX,charY-1));
			fillFov(tempRoom->sx,tempRoom->sy-1,tempRoom->ex,tempRoom->ey,2);

			tempRoom = dungeon->getRoom(getRoom(charX,charY+1));
			fillFov(tempRoom->sx,tempRoom->sy-1,tempRoom->ex,tempRoom->ey,2);
		}
		return;
	}
	//light current room + walls + doors
	tempRoom = dungeon->getRoom(roomNum);
	fillFov(tempRoom->sx,tempRoom->sy-1,tempRoom->ex,tempRoom->ey,2);
}

int FOV::getRoom(int x, int y)
{
	DungeonRoom *curRoom;
	for(int i=0;i<dungeon->getRoomCount();i++)
	{
		curRoom = dungeon->getRoom(i);
		if(x >= curRoom->sx && x < curRoom->ex && y >= curRoom->sy && y < curRoom->ey)
			return i;
	}
	return -1;
}

void FOV::draw()
{
	if(!visible)
		return;
	for(int i = 0;i < height;i++) {
		for(int j = 0;j < width;j++) {
			int xoff = (int)(j * 24 * texture.getScale() + (int)mapX);
			int yoff = (int)(i * 24 * texture.getScale() + (int)mapY);
			if(xoff >= (int)GAME_WIDTH || xoff+(24*texture.getScale()) < 0)
				continue;
			if(yoff >= (int)GAME_HEIGHT || yoff+(24*texture.getScale()) < 0)
				continue;
			if(getFov(j,i) == 2)
				continue;
			texture.setX((float)xoff);
			texture.setY((float)yoff);
			if(getFov(j,i) == 3)
				texture.draw();
			if(getFov(j,i) == 1)
				texture.draw(graphicsNS::ALPHA50);
		}
	}
}

void FOV::resetFov()
{
	if(fovMap != NULL)
		delete[] fovMap;
	fovMap = new char[width*height];
	for(int i = 0;i < height;i++) {
		for(int j = 0;j < width;j++) {
			setFov(j,i,3);
		}
	}
	roomNum = -1;
}