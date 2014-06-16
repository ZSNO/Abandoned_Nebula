#ifndef an_char
#define an_char
#define WIN32_LEAN_AND_MEAN

#include "image.h"
#include "input.h"
#include "dungeon.h"
#include "fov.h"
#include "items.h"
//#include "enemies.h"
#include "audio.h"
#include "textDX.h"
#include <string>
#include <sstream>

class EnemyCon;
class AbandonedNebula;

namespace characterNS
{
	const int WALKFRAMES = 3;
	const int DOWN = 0;
	const int UP = 1;
	const int RIGHT  = 2;
	const int LEFT = 3;
	const int ATTACKFRAME = 3;
	const float TIMEPERFRAME = 0.125f;
}

//Character attributes:
//0 EXP
//1 Current Health
//2 Max Health
//3 Armor
//4 Damage
//5 Speed
//6 Fire Chance
//7 Poison Chance
//8 Freeze Chance
//9 Paralyze Chance

class Character
{
private:
	Image texture;
	int direction;
	bool moving;
	int framesInWalk;
	DungeonGen *dungeon;
	float curFrametime;
	bool rlAnim;
	int curX, curY;
	float mapOffX, mapOffY;
	Input *input;
	float controlLockTimeout;
	FOV fov;
	short att[10];
	char level;
	AN_ItemCon *items;
	bool visible;
	int monies;
	bool ded;
	short expLevels[18];
	EnemyCon *enemies;
	float attackAnim;
	int dunLevel;
	bool won;
	int healthRegen;
	int paralyzeTicks;
	Audio *sound;
	std::string message;
	float messageTimeout;
	TextDX *mesText;

public:
	Character();
	bool initialize(AbandonedNebula *game, int width, int height, int ncols, TextureManager *textureM, TextureManager *shadow, TextDX *txt);
	void update(float frameTime);
	void draw();
	void move(int dir);
	void setCoord(int x, int y);
	int isMoving()
	{return moving;}
	double getMapOffX()
	{return mapOffX;}
	double getMapOffY()
	{return mapOffY;}
	int getCurX()
	{return curX;}
	int getCurY()
	{return curY;}
	FOV* getFov()
	{return &fov;}
	bool getVisible()
	{return visible;}
	void setVisible(bool i)
	{visible = i;}
	void setLevel(char l)
	{level = l;}
	char getLevel()
	{return level;}
	void addMoney(int i)
	{monies += i;}
	int getMoney()
	{return monies;}

	void addExp(int i) {
		att[0] += i;
		if(att[0] >= expLevels[level])
		{
			att[0] = att[0] - expLevels[level];
			level++;
			att[1] += 5;
			att[2] += 5;
			sound->playCue(LEVELUP);
			showMessage("Level up!");
		}
	}
	short getExp()
	{return att[0];}
	void addHealth(int i)
	{
		if(i < 0)
			att[1] += (i + att[3]);
		else
			att[1] += i;
		if(att[1] >= att[2]){att[1] = att[2];}
		if(att[1] <= 0){ded = 1;controlLockTimeout = 600;sound->playCue(TERMINATED);}
	}
	short getHealth()
	{return att[1];}
	short getMaxHealth()
	{return att[2];}
	short getExpLevel()
	{return expLevels[level];}
	void equipItem(int itemNum);
	void unEquipItem(int itemNum);
	DungeonGen *getDungeon()
	{return dungeon;}
	void setEnemyCon(EnemyCon *moo)
	{enemies = moo;}
	EnemyCon *getEnemyCon()
	{return enemies;}
	AN_ItemCon *getItems()
	{return items;}
	int getDunLevel()
	{return dunLevel;}
	bool isDed()
	{return ded;}
	bool isWinner()
	{return won;}
	void setWinner(bool b)
	{won = b;}
	void setMaxHealth(short i)
	{att[2] = i;}
	void setParalyzeTicks(int ticks)
	{paralyzeTicks = ticks;}
	void showMessage(std::string mes)
	{message = mes;messageTimeout=2.0f;}
	void reset()
	{
		moving=0;
		controlLockTimeout=1;
		dunLevel=0;
		won=0;
		ded=0;
		texture.setCurrentFrame(0);
		fov.setDimensions(25,25);
		fov.resetFov();
		direction = 0;
		curFrametime = 0;
		curX = 0;
		curY = 0;
		mapOffX = 0;
		mapOffY = 0;
		level = 0;
		monies = 0;
		for(int i=0;i<10;i++)
			att[i]=0;
		att[1] = 20;
		att[2] = 20;
		att[4] = 5;
		dunLevel = 0;
		healthRegen = 5;
		paralyzeTicks = 0;
		messageTimeout = 0.0f;
	}
};

#endif