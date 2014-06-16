#include "dungeon.h"

//Get the char in the selected cell
char DungeonGen::getCell(int x, int y)
{
	return map[x + width * y];
}

//Sets the char in the selected cell
void DungeonGen::setCell(int x, int y, char fill)
{
	dug++;
	map[x + width * y] = fill;
}

void DungeonGen::setColCell(int x, int y, bool f)
{
	colMap[x + width * y] = f;
}

void DungeonGen::setItemCell(int x, int y, char f)
{
	itemMap[x + width * y] = f;
}

char DungeonGen::getItemCell(int x, int y)
{
	return itemMap[x + width * y];
}

bool DungeonGen::getColCell(int x, int y)
{
	return colMap[x + width * y];
}

//Room types
//0 = normal
//1 = secret
//2 = shop
//3 = exit/entrance

//Create a door from (sx,sy) to (ex,ey) with a door at (dx,dy), ns tells it not to create a secret room
//returns false on fail
bool DungeonGen::createRoom(int sx, int sy, int ex, int ey, int dx, int dy, bool ns)
{
	if (sx <= 0 || sy <= 0 || sy >= height || sx >= width)
		return 0;
	if (ex <= 0 || ey <= 0 || ey >= height || ex >= width)
		return 0;
	if (checkArea(sx, sy, ex, ey))
		return 0;
	DungeonCoord wall;
	DungeonRoom room;
	room.sx = sx;
	room.sy = sy;
	room.ex = ex;
	room.ey = ey;
	if (cRandom(0, 100) < 5 && !ns)
	{
		room.type = 1; //Secret room
	}
	else if (cRandom(0, 100) < 3 && !ns)
	{
		room.type = 2; //shop room
	}
	else
	{
		room.type = 0; //normal room
	}
	for (int i = sx; i < ex; i++)
	{
		for (int j = sy; j < ey; j++)
		{
			setCell(i, j, AREA);
			setColCell(i, j, 1);
		}			
	}
	//Add room's walls to avaiable list to pick from
	if (room.type == 0)
	{
		for (int i = sx - 1; i <= ex; i++)
		{
			for (int j = sy - 1; j <= ey; j++)
			{
				if ((j == sy - 1) || (j == ey) || (i == sx - 1) || (i == ex))
				{
					//Do not include corners, that could get weird.
					if ((j == sy - 1 && i == sx - 1) || (j == ey && i == ex))
						continue;
					if ((j == sy - 1 && i == ex) || (j == ey && i == sx - 1))
						continue;
					wall.x = i;
					wall.y = j;
					if (j == sy - 1)
						wall.dir = 0;
					else if (j == ey)
						wall.dir = 2;
					else if (i == ex)
						wall.dir = 1;
					else
						wall.dir = 3;
					walls.push_back(wall);
				}
			}
		}
	}
	//Set door char, do not put a door at start room
	if (dx != -1 && dy != -1)
	{
		if (room.type == 1)
			setCell(dx, dy, SECRET);
		else
			setCell(dx, dy, DOOR);
		setColCell(dx, dy, 1);
	}
	rooms.push_back(room);
	return 1;
}

//Checks area for existing room
//Accepts starting x, y and ending x, y
//Returns true for existing room, false for clear
//Checks 1 unit larger than room so there are walls between rooms
bool DungeonGen::checkArea(int sx, int sy, int ex, int ey)
{
	int carea = 0;
	for (int i = sx-1; i <= ex; i++)
	{
		for (int j = sy-1; j <= ey; j++)
		{
			carea++;
			if (getCell(i, j) != WALL)
			{
				return 1;
			}
		}
	}
	return 0;
}

//Random that excepts starting and end range.
int DungeonGen::cRandom(int s, int e)
{
	int range = e - s;
	std::random_device rd;
	if (range < 1)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Range less than 1!"));
	return (rd() % range) + s;
}

//Weighted random, accepts probablility of each a and b
bool DungeonGen::wRandom(int a, int b)
{
	int tot = a + b;
	std::random_device rd;
	if (rd() % tot <= (unsigned)a)
		return 0;
	else
		return 1;
}

//Fill the whole map with a char.
void DungeonGen::fillMap(char filler)
{
	for (int i = 0; i < width*height; i++)
		map[i] = filler;
}

//Main generation code, only this should be called on the class.
void DungeonGen::generate()
{
	map = new char[width*height];
	colMap = new bool[width*height];
	itemMap = new char[width*height];

	int rWidth, rHeight, sx, sy, tries = 0, wallNum, dx, dy, dir;

	//Make map all walls
	fillMap(WALL);
	for (int i = 0; i < width*height; i++)
		colMap[i] = 0;
	for (int i = 0; i < width*height; i++)
		itemMap[i] = 0;

	//Create first room on dungeon
	rWidth = cRandom(minRoom[0], maxRoom[0]);
	rHeight = cRandom(minRoom[1], maxRoom[1]);
	sx = (width / 2) - rWidth/2;
	sy = (height / 2) - rHeight/2;

	//If createRoom returns false, room failed to create, throw exception
	if (!createRoom(sx, sy, sx + rWidth, sy + rHeight, -1, -1, 1))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Could not create room!"));

	//Start room maker
	while (dug / area < dugPercentage && tries < 1000 && walls.size())
	{
		tries++;
		if (walls.size() != 1)
			wallNum = cRandom(0, walls.size() - 1);
		else
			wallNum = 0;
		dx = walls.at(wallNum).x;
		dy = walls.at(wallNum).y;
		dir = walls.at(wallNum).dir;
		walls.erase(walls.begin() + wallNum);
		rWidth = cRandom(minRoom[0], maxRoom[0]);
		rHeight = cRandom(minRoom[1], maxRoom[1]);
		switch (dir)
		{
		case 0:
			sx = dx - rWidth / 2;
			sy = dy - rHeight;
			break;
		case 1:
			sx = dx + 1;
			sy = dy - rHeight / 2;
			break;
		case 2:
			sx = dx - rWidth / 2;
			sy = dy + 1;
			break;
		case 3:
			sx = dx - rWidth;
			sy = dy - rHeight / 2;
		}
		createRoom(sx, sy, sx + rWidth, sy + rHeight, dx, dy, 0);
	}
	//std::cout << dug / area << " : " << tries << " : " << walls.size() << "\n";
	//Pick random start and exit rooms
	bool exit = 0, ent = 0;
	while (!exit || !ent)
	{
		int random;
		try
		{
			random = cRandom(0, rooms.size() - 1);
		}
		catch (char *str)
		{
			throw str;
		}
		DungeonRoom *temp;
		temp = &(rooms.at(random));
		if (temp->type == 0)
		{
			if (!exit)
			{
				temp->type = 3;
				setCell(cRandom(temp->sx + 1, temp->ex - 1), cRandom(temp->sy + 1, temp->ey - 1), EXIT);
				exit = 1;
			}
			else
			{
				temp->type = 3;
				int rx = cRandom(temp->sx + 1, temp->ex - 1), ry = cRandom(temp->sy + 1, temp->ey - 1);
				setCell(rx, ry, ENTRANCE);
				entX = rx;
				entY = ry;
				ent = 1;
			}
		}
	}
	for (int i = 0; i < (int)rooms.size(); i++)
	{
		populateRoom(rooms.at(i));
	}
	createTilemap();
}

//Populates the given room with chests and traps
void DungeonGen::populateRoom(DungeonRoom rr)
{
	if (rr.type == 2)
	{
		setItemCell(cRandom(rr.sx + 1, rr.ex - 1), cRandom(rr.sy + 1, rr.ey - 1), 3); //store
		return;
	}
	for (int i = rr.sy; i < rr.ey; i++)
	{
		for (int j = rr.sx; j < rr.ex; j++)
		{
			if (rr.type == 0)
			{
				if (cRandom(0, 99) < 5 && !rr.hasChest)
				{
					setItemCell(j, i, 1); //chest
					rr.hasChest = 1;
				}
				else if (cRandom(0, 99) < 5)
					setItemCell(j, i, 2); //trap
			}
			if (rr.type == 1)
			{
				if (cRandom(0, 99) < 10)
					setItemCell(j, i, 1); //chest
			}
		}
	}
}

//Sets the tile to texture number
void DungeonGen::setTile(int x, int y, char t)
{
	tileMap[x + width * y] = t;
}

//Get the char in the selected cell
char DungeonGen::getTile(int x, int y)
{
	return tileMap[x + width * y];
}

//Creates the tilemap that is used in the game
void DungeonGen::createTilemap()
{
	tileMap = new char[width*height];
	for(int i = 0;i < height;i++)
	{
		for(int j = 0;j < width;j++)
		{
			bool n, s, e, w;
			char cell;
			cell = getCell(j,i);
			n = (getCell(j,i-1) == WALL || getCell(j,i-1) == DOOR || getCell(j,i-1) == SECRET);
			s = (getCell(j,i+1) == WALL || getCell(j,i+1) == DOOR || getCell(j,i+1) == SECRET);
			w = (getCell(j-1,i) == WALL || getCell(j-1,i) == DOOR || getCell(j-1,i) == SECRET);
			e = (getCell(j+1,i) == WALL || getCell(j+1,i) == DOOR || getCell(j+1,i) == SECRET);
			switch(cell)
			{
			case DOOR:
				setTile(j,i,9);
				break;
			case ENTRANCE:
				setTile(j,i,11);
				break;
			case EXIT:
				setTile(j,i,10);
				break;
			case SECRET:
			case WALL:
				setTile(j,i,100);
				if(!s && i+1 < height)
				{
					if(getCell(j+1,i+1) == WALL || getCell(j+1,i+1) == DOOR || getCell(j+1,i+1) == SECRET)
						setTile(j,i,2);
					else if(getCell(j-1,i+1) == WALL || getCell(j-1,i+1) == DOOR || getCell(j-1,i+1) == SECRET)
						setTile(j,i,0);
					else if(getCell(j,i+1) != DOOR && getCell(j,i+1) != SECRET)
						setTile(j,i,1);
				}
				break;
			default:
				if(n && w)
					setTile(j,i,3);
				else if(n && e)
					setTile(j,i,5);
				else if(s && w)
					setTile(j,i,6);
				else if(s && e)
					setTile(j,i,8);
				else
					setTile(j,i,4);
			}
		}
	}
}

bool DungeonGen::initialize(Graphics *g, int width, int height, int ncols, TextureManager *textureM)
{
	bool res;
	res = texture.initialize(g,width,height,ncols,textureM);
	if(res)
		texture.setScale(TEX_SCALE);
	return res;
}

void DungeonGen::draw()
{
	if(!visible)
		return;
	for(int i = 0;i < height;i++)
	{
		for(int j = 0;j < width;j++)
		{
			int xoff = (int)(j * 24 * texture.getScale() + (int)offsetX);
			int yoff = (int)(i * 24 * texture.getScale() + (int)offsetY);
			if(xoff >= (int)GAME_WIDTH || xoff+(24*texture.getScale()) < 0)
				continue;
			if(yoff >= (int)GAME_HEIGHT || yoff+(24*texture.getScale()) < 0)
				continue;

			char tile = getTile(j,i);
			if(tile == 100)
				continue;

			texture.setCurrentFrame(tile);
			texture.setX((float)xoff);
			texture.setY((float)yoff);
			if(tile == 9)
				texture.draw(D3DCOLOR_ARGB(255,200,115,70));
			else
				texture.draw();

			if(getItemCell(j,i) == 3)
			{
				texture.setCurrentFrame(12);
				texture.draw();
			}
		}
	}
}

void DungeonGen::update(float frameTime)
{
	texture.update(frameTime);
}

void DungeonGen::updateOffset(float newx, float newy)
{
	offsetX += newx;
	offsetY += newy;
}

void DungeonGen::setOffset(float newx, float newy)
{
	offsetX = newx;
	offsetY = newy;
}

void DungeonGen::reset()
{
	if(map != NULL)
		delete[] map;
	if(tileMap != NULL)
		delete[] tileMap;
	if(colMap != NULL)
		delete[] colMap;
	if(itemMap != NULL)
		delete[] itemMap;

	dugPercentage = 0.5;
	dug = 0.0;
	offsetX = 0;
	offsetY = 0;
	map = NULL;
	tileMap = NULL;
	colMap = NULL;
	itemMap = NULL;
	walls.clear();
	rooms.clear();
}