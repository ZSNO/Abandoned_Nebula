#include "enemies.h"

bool Enemy::initialize(Image *sp, Character *pl)
{
	player = pl;
	sprite = sp;
	dungeon = pl->getDungeon();
	sprite->setCurrentFrame(0);
	health = (int)((player->getDunLevel() * player->getDunLevel()) + 10);
	damage = (int)((player->getDunLevel() * player->getDunLevel()) + 4);
	return 1;
}

void Enemy::update(float frameTime)
{
	float offX = 0, offY = 0;
	if(moving && direction != -1)
	{
		curFrametime += frameTime;
		if(direction == characterNS::RIGHT)
			offX = frameTime * MOVEMENTSPEED;
		if(direction == characterNS::LEFT)
			offX = frameTime * -MOVEMENTSPEED;
		if(direction == characterNS::UP)
			offY = frameTime * -MOVEMENTSPEED;
		if(direction == characterNS::DOWN)
			offY = frameTime * MOVEMENTSPEED;
		sOffX += offX/(sprite->getWidth()*TEX_SCALE);
		sOffY += offY/(sprite->getHeight()*TEX_SCALE);
		if((direction == characterNS::UP && sOffY < curY) || (direction == characterNS::DOWN && sOffY > curY) || (direction == characterNS::LEFT && sOffX < curX) || (direction == characterNS::RIGHT && sOffX > curX))//  || (!player->isMoving() && isMoving())) 
		{
			sOffY = (float)curY;
			sOffX = (float)curX;
			moving = 0;
			sprite->setCurrentFrame(direction * 4);
			doneMoving = 1;
		}
	}
	if(direction == -1)
	{
		moving = 0;
		doneMoving = 1;
	}
	sprite->update(frameTime);
}

void Enemy::draw()
{
	if(moving && curFrametime > characterNS::TIMEPERFRAME)
	{
		if(!(sprite->getCurrentFrame() % 4))
			sprite->setCurrentFrame(sprite->getCurrentFrame() + 1 + rlAnim);
		else
		{
			sprite->setCurrentFrame(sprite->getCurrentFrame() - 1 - rlAnim);
			rlAnim=!rlAnim;
		}
		curFrametime = 0;
	}
	sprite->setX(sOffX * sprite->getWidth() * sprite->getScale() + dungeon->getOffX());
	sprite->setY(sOffY * sprite->getHeight() * sprite->getScale() + dungeon->getOffY() - sprite->getHeight()*sprite->getScale()/2);
	sprite->draw();
}

void Enemy::move()
{
	//Add enemy to enemy col, detect when can attack player
	std::random_device rd;
	int sx = 0, sy = 0;
	if(player->getFov()->getFov(curX,curY) == 2) //if enemy is visible to player, move towards player
	{
		int pX = player->getCurX(), pY = player->getCurY();
		int difX = pX - curX, difY = pY - curY;

		std::ostringstream dd;
		dd << pX << ":" << pY << ":" << difX << ":" << difY << ":" << curX << ":" << curY;
		debug = dd.str();

		if((difX == 0 && std::abs(difY) <= 1) || (difY == 0 && std::abs(difX) <= 1))
		{
			//throw(GameError(gameErrorNS::WARNING, "pass"));
			int dam = damage + (rd() % 6) - 2;
			if(dam < 0)
				dam = 0;
			player->addHealth(-dam);
			direction = -1;
		}
		else if(std::abs(difX) > std::abs(difY))
		{
			//horiz diff is greater, move this direction first
			if(curX < pX && dungeon->getColCell(curX + 1, curY) && player->getEnemyCon()->getEnemy(curX + 1, curY) == -1) //If enemy is to left of player and can move toward player
				direction = characterNS::RIGHT;
			else if(curX > pX && dungeon->getColCell(curX - 1, curY) && player->getEnemyCon()->getEnemy(curX - 1, curY) == -1)
				direction = characterNS::LEFT;
			else //if can't move either left or right, try moving up or down
			{
				if(curY < pY && dungeon->getColCell(curX, curY + 1) && player->getEnemyCon()->getEnemy(curX, curY + 1) == -1)
					direction = characterNS::DOWN;
				else if(curY > pY && dungeon->getColCell(curX, curY - 1) && player->getEnemyCon()->getEnemy(curX, curY - 1) == -1)
					direction = characterNS::UP;
				else
					direction = -1; //can't move :c
			}
		}
		else
		{
			//vert diff
			if(curY < pY && dungeon->getColCell(curX, curY + 1) && player->getEnemyCon()->getEnemy(curX, curY + 1) == -1)
				direction = characterNS::DOWN;
			else if(curY > pY && dungeon->getColCell(curX, curY - 1) && player->getEnemyCon()->getEnemy(curX, curY - 1) == -1)
				direction = characterNS::UP;
			else
			{
				if(curX < pX && dungeon->getColCell(curX + 1, curY) && player->getEnemyCon()->getEnemy(curX + 1, curY) == -1)
					direction = characterNS::RIGHT;
				else if(curX > pX && dungeon->getColCell(curX - 1, curY) && player->getEnemyCon()->getEnemy(curX - 1, curY) == -1)
					direction = characterNS::LEFT;
				else
					direction = -1;
			}
		}
	}
	else //wander
	{
		direction = rd() % 4;
		int tries = 0;
		switch(direction)
		{
		case characterNS::DOWN:
			sx=0;sy=1;break;
		case characterNS::UP:
			sx=0;sy=-1;break;
		case characterNS::RIGHT:
			sx=1;sy=0;break;
		case characterNS::LEFT:
			sx=-1;sy=0;
		}
		while((!dungeon->getColCell(curX + sx, curY + sy) || player->getEnemyCon()->getEnemy(curX + sx, curY + sy) != -1 || (player->getCurX() == curX + sx && player->getCurY() == curY + sy)) && tries < 100)
		{
			direction = rd() % 4;
			switch(direction)
			{
			case characterNS::DOWN:
				sx=0;sy=1;break;
			case characterNS::UP:
				sx=0;sy=-1;break;
			case characterNS::RIGHT:
				sx=1;sy=0;break;
			case characterNS::LEFT:
				sx=-1;sy=0;
			}
			tries++;
		}
		if(tries == 100)
			direction = -1;
	}
	switch(direction)
	{
	case characterNS::DOWN:
		sx=0;sy=1;break;
	case characterNS::UP:
		sx=0;sy=-1;break;
	case characterNS::RIGHT:
		sx=1;sy=0;break;
	case characterNS::LEFT:
		sx=-1;sy=0;break;
	default:
		sx=0;sy=0;
	}
	moving = 1;
	curX = curX + sx;
	curY = curY + sy;
}

void Enemy::setCoord(int x, int y)
{
	curX = x;
	curY = y;
	sOffX = (float)x;
	sOffY = (float)y;
}

void Enemy::addDamage(int dam)
{
	health -= dam;
	if(health < 0)
	{
		health=0;
		ded=1;
		player->getEnemyCon()->setEnemy(curX,curY,-1);
		player->addExp(damage);
		std::random_device rd;
		if(rd() % 100 < 10)
			player->getItems()->genNewItem(player->getLevel(),curX,curY);
	}
}

void EnemyCon::initialize(Graphics *g, int w, int h, Character *pl)
{
	graphics = g;
	width = w;
	height = h;
	enemyMap = new char[width*height];
	for(int i=0;i<w*h;i++)
		enemyMap[i] = -1;
	player = pl;
}

bool EnemyCon::newEnemyType(int width, int height, int ncols, TextureManager *textureM)
{
	Image newEn;
	bool ok = newEn.initialize(graphics, width, height, ncols, textureM);
	if(ok)
		newEn.setScale(TEX_SCALE);
	types.push_back(newEn);
	return ok;
}

void EnemyCon::newEnemy(int x, int y)
{
	Enemy newEn;
	std::random_device rd;
	if(types.size() == 0)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Can't create enemy without making an enemy type first!"));
	newEn.initialize(&types.at(rd() % types.size()), player);
	newEn.setCoord(x,y);
	enemies.push_back(newEn);
}

void EnemyCon::newEnemy(int x, int y, int enemyType)
{
	Enemy newEn;
	if(enemyType > (int)types.size() - 1)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Can't create enemy with non existant enemy type!"));
	newEn.initialize(&types.at(enemyType), player);
	newEn.setCoord(x,y);
	enemies.push_back(newEn);
}

void EnemyCon::update(float frameTime)
{
	for(int i=0;i<(int)enemies.size();i++)
	{
		Enemy *en = &enemies.at(i);
		if(player->isMoving() && !en->isMoving() && !en->isDoneMoving() && !en->isDed())
		{
			setEnemy(en->getCurX(), en->getCurY(), -1);
			en->move();
			setEnemy(en->getCurX(), en->getCurY(), i);
		}
		en->update(frameTime);
		if(!player->isMoving() && en->isDoneMoving())
			en->setDoneMoving(0);
	}
}

void EnemyCon::draw()
{
	for(int i=0;i<(int)enemies.size();i++)
	{
		Enemy *en = &enemies.at(i);
		if(en->isDed() || player->getFov()->getFov(en->getCurX(),en->getCurY()) != 2)
			continue;
		en->draw();
	}
}

void EnemyCon::cleanUp()
{
	enemies.clear();
}

void EnemyCon::reset(int w, int h)
{
	if(enemyMap != NULL)
		delete[] enemyMap;
	width = w;
	height = h;
	enemyMap = new char[width*height];
	for(int i=0;i<w*h;i++)
		enemyMap[i] = -1;
	cleanUp();
}