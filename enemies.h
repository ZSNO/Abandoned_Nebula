#ifndef an_enemies
#define an_enemies
#define WIN32_LEAN_AND_MEAN

#include "character.h"
#include <sstream>

class Enemy
{
	Image *sprite;
	int direction;
	bool moving;
	int framesInWalk;
	DungeonGen *dungeon;
	float curFrametime;
	bool rlAnim;
	int curX, curY;
	float sOffX, sOffY;
	int health, damage;
	bool ded;
	Character *player;
	bool doneMoving;

	std::string debug;
public:
	Enemy()
	{
		direction = 0;
		moving = 0;
		rlAnim = 0;
		curX = curY = 0;
		sOffX = sOffY = 0;
		ded = 0;
		dungeon = NULL;
		doneMoving = 0;
		curFrametime = 0;
	}
	bool initialize(Image *sp, Character *pl);
	void update(float frameTime);
	void draw();
	void move();
	void setCoord(int x, int y);
	int isMoving()
	{return moving;}
	int getCurX()
	{return curX;}
	int getCurY()
	{return curY;}
	float getOffX()
	{return sOffX;}
	float getOffY()
	{return sOffY;}
	bool isDed()
	{return ded;}
	Image *getSprite()
	{return sprite;}
	int getDir()
	{return direction;}
	bool isDoneMoving()
	{return doneMoving;}
	void setDoneMoving(bool d)
	{doneMoving = d;}
	void addDamage(int dam);
	int getHealth()
	{return health;}
	std::string debugM()
	{return debug;}
};

//Enenmy container
class EnemyCon
{
	std::vector<Enemy> enemies;
	std::vector<Image> types;
	char *enemyMap;
	Character *player;
	Graphics *graphics;
	int width, height;
public:
	EnemyCon()
	{
		enemyMap = NULL;
		graphics = NULL;
	}
	~EnemyCon()
	{
		delete[] enemyMap;
		cleanUp();
	}
	void initialize(Graphics *g, int w, int h, Character *pl);
	bool newEnemyType(int width, int height, int ncols, TextureManager *textureM);
	void newEnemy(int x, int y);
	void newEnemy(int x, int y, int enemyType);
	void update(float frameTime);
	void draw();
	int getEnemy(int x, int y)
	{return enemyMap[x + y * width];}
	void cleanUp();
	void reset(int w, int h);
	int getNumber()
	{return enemies.size();}
	Enemy *getEnemyObj(int i)
	{return &enemies.at(i);}
	void setEnemy(int x, int y, int en)
	{enemyMap[x + y * width] = en;}
};

#endif