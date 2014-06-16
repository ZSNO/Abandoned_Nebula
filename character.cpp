#include "character.h"
#include "enemies.h"
#include "AN.h"

Character::Character()
{
	direction = 0;
	moving = 0;
	framesInWalk = 0;
	dungeon = NULL;
	curFrametime = 0;
	curX = 0;
	curY = 0;
	mapOffX = 0;
	mapOffY = 0;
	controlLockTimeout = 0;
	level = 0;
	visible = 0;
	monies = 0;
	ded = 0;
	for(int i=0;i<10;i++)
		att[i]=0;
	att[1] = 20;
	att[2] = 20;
	att[4] = 5;
	for(int i=0;i<17;i++)
		expLevels[i] = (i + 10) * (i + 1);
	expLevels[17] = 10000;
	dunLevel = 0;
	won = 0;
	healthRegen = 5;
	paralyzeTicks = 0;
	messageTimeout = 0.0f;
}

bool Character::initialize(AbandonedNebula *g, int width, int height, int ncols, TextureManager *textureM, TextureManager *shadow, TextDX *txt)
{
	dungeon = g->getDungeon();
	input = g->getInput();
	items = g->getItemCon();
	sound = g->getAudio();
	mesText = txt;
	bool ok = texture.initialize(g->getGraphics(), width, height, ncols, textureM);
	if(ok)
	{
		texture.setScale((float)TEX_SCALE);
		texture.setX(GAME_WIDTH/2 - texture.getWidth() * texture.getScale()/2);
		texture.setY(GAME_HEIGHT/2 - texture.getHeight() * texture.getScale());
	}
	return (ok && fov.initialize(g->getGraphics(),width,height,0,shadow,dungeon));
}

void Character::update(float frameTime)
{
	float offX = 0, offY = 0;
	if(messageTimeout > 0)
		messageTimeout -= frameTime;
	if(messageTimeout < 0)
	{
		messageTimeout = 0;
		message = "";
	}
	if(paralyzeTicks > 0 && !moving && !controlLockTimeout)
	{
		direction = -1;
		moving = 1;
		paralyzeTicks--;
		controlLockTimeout = 0.5f;
	}
	else if(!moving && !controlLockTimeout)
	{
		if(input->isKeyDown(VK_RIGHT))            // if move right
			move(characterNS::RIGHT);
		else if(input->isKeyDown(VK_LEFT))             // if move left
			move(characterNS::LEFT);
		else if(input->isKeyDown(VK_UP))               // if move up
			move(characterNS::UP);
		else if(input->isKeyDown(VK_DOWN))             // if move down
			move(characterNS::DOWN);
	}
	else if(moving && direction != -1)
	{
		curFrametime += frameTime;
		if(direction == characterNS::RIGHT)
			offX = frameTime * -MOVEMENTSPEED;
		if(direction == characterNS::LEFT)
			offX = frameTime * MOVEMENTSPEED;
		if(direction == characterNS::UP)
			offY = frameTime * MOVEMENTSPEED;
		if(direction == characterNS::DOWN)
			offY = frameTime * -MOVEMENTSPEED;
		dungeon->updateOffset(offX, offY);
		if((direction == characterNS::UP && dungeon->getOffY() > mapOffY) || (direction == characterNS::DOWN && dungeon->getOffY() < mapOffY))  
		{
			dungeon->setOffset(dungeon->getOffX(), mapOffY);
			moving = 0;
			texture.setCurrentFrame(direction * 4);
			controlLockTimeout = 0.05f;
		}
		if((direction == characterNS::LEFT && dungeon->getOffX() > mapOffX) || (direction == characterNS::RIGHT && dungeon->getOffX() < mapOffX))
		{
			dungeon->setOffset(mapOffX, dungeon->getOffY());
			moving = 0;
			texture.setCurrentFrame(direction * 4);
			controlLockTimeout = 0.05f;
		}
	}
	else if(direction == -1)
		moving = 0;
	if(controlLockTimeout > 0)
		controlLockTimeout -= frameTime;
	if(controlLockTimeout < 0)
		controlLockTimeout = 0;
	if(attackAnim > 0)
		attackAnim -= frameTime;
	if(attackAnim < 0)
	{
		texture.setCurrentFrame(direction * 4);
		attackAnim = 0;
	}

	texture.update(frameTime);
	fov.update(frameTime);
	fov.updateCharCoord(curX,curY);
	fov.updateOffset((int)dungeon->getOffX(), (int)dungeon->getOffY());
	items->update(frameTime);
}

void Character::draw()
{
	if(!visible)
		return;
	if(moving && curFrametime > characterNS::TIMEPERFRAME && direction != -1)
	{
		if(!(texture.getCurrentFrame() % 4))
			texture.setCurrentFrame(texture.getCurrentFrame() + 1 + rlAnim);
		else
		{
			texture.setCurrentFrame(texture.getCurrentFrame() - 1 - rlAnim);
			rlAnim?rlAnim=0:rlAnim=1;
		}
		curFrametime = 0;
	}

	for(int i = 0;i < dungeon->getHeight();i++) {
		for(int j = 0;j < dungeon->getWidth();j++) {
			char item = dungeon->getItemCell(j,i);
			if(item != 0 && fov.getFov(j,i) == 2)
				items->draw(
					(int)(j * 24 * texture.getScale() + dungeon->getOffX()),
					(int)(i * 24 * texture.getScale() + dungeon->getOffY()),
					item, TEX_SCALE, 1
				);
		}
	}

	fov.draw();
	enemies->draw();
	texture.draw();
	RECT region;
	region.top = GAME_HEIGHT/2 - 150;
	region.bottom = GAME_HEIGHT/2 - 100;
	region.left = 0;
	region.right = GAME_WIDTH;
	mesText->print(message,region,DT_CENTER);
}

void Character::move(int dir)
{
	if(moving)
		return;
	int sx, sy;
	std::random_device rd;
	switch(dir)
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
	int x = curX + sx, y = curY + sy;
	if(!dungeon->getColCell(x, y))
		return;

	int enNum = enemies->getEnemy(x,y);
	if(enNum != -1)
	{
		std::random_device rd;
		int damage = att[4] + (rd() % 6) - 2;
		if(damage < 0)
			damage = 0;
		enemies->getEnemyObj(enNum)->addDamage(damage);
		direction = dir;
		texture.setCurrentFrame(dir * 4 + 3);
		attackAnim = 0.2f;
		controlLockTimeout = 0.5f;
		moving = 1;
		direction = -1;
		sound->playCue(HIT);
		return;
	}

	switch(dungeon->getItemCell(x, y))
	{
	case 0:
		break;
	case 1:
		//open chest
		items->genNewItem(level,x,y);
		controlLockTimeout = 0.2f;
		return;
	case 2: //trap
		dungeon->setItemCell(x, y, 0);
		dungeon->setTile(x, y, 7);
		sound->playCue(STATUS);
		switch(rd() % 3)
		{
		case 0:
			addHealth((-level*2)-1-att[3]);
			showMessage("Damaged!");
			break;
		case 1:
			setParalyzeTicks(3);
			showMessage("Paralyzed!");
			break;
		case 2:
			//no trap
			break;
		}
		break;
	case 3:
		showMessage("The store seems to be broken.");
		sound->playCue(STORECUE);
		return;
	default:
		AN_Item *datItem;
		int itemNum = dungeon->getItemCell(x,y) - 1;
		datItem = items->getItem(itemNum);

		if(datItem->getItemType() == 3) //if item is currency
		{
			addMoney(datItem->getAttribute(0));
			dungeon->setItemCell(x,y,0);
			sound->playCue(PICKUP);
			break;
		}
		if(datItem->getItemType() == 0) //if item is stimpak, use stacking method
			items->setInventSlot(itemNum, items->nextInventSlot(datItem->getItemSpec(), itemNum));
		else //use regular slot method
			items->setInventSlot(dungeon->getItemCell(x,y) - 1, items->nextInventSlot());
		if(datItem->getInventSlot() == -1)
		{
			//notify inventory is full!
			showMessage("Inventory is full!");
			return;
		}
		sound->playCue(PICKUP);
		showMessage("Item picked up");
		dungeon->setItemCell(x,y,0);
		break;
	}

	if(dungeon->getCell(x,y) == EXIT)
	{
		int s = dunLevel + 25;
		//prompt user
		moving = 0;
		controlLockTimeout = 2;
		texture.setCurrentFrame(0);
		dungeon->reset();
		dungeon->setDimensions(s,s);
		dungeon->generate();
		enemies->reset(s,s);
		std::random_device rd;
		for(int i=0;i<(int)(s/4);i++)
		{
			int x = rd() % s;
			int y = rd() % s;
			if(!dungeon->getColCell(x,y))
			{
				i--;continue;
			}
			enemies->newEnemy(x,y);
		}
		setCoord(dungeon->getEntX(), dungeon->getEntY());
		fov.setDimensions(s,s);
		fov.resetFov();
		items->cleanUpItems();
		dunLevel++;
		if(dunLevel == 11)
		{
			won = 1;
			controlLockTimeout = 600;
			sound->playCue(CONGRATS);
		}
		else
			sound->playCue(FLOOR);
		return;
	}

	healthRegen--;
	if(healthRegen <= 0)
	{
		addHealth(1);
		healthRegen = 5;
	}

	moving = 1;
	direction = dir;
	mapOffX = (int)GAME_WIDTH/2.0f - x * 24 * dungeon->getTexture().getScale() - 12 * dungeon->getTexture().getScale();
	mapOffY = (int)GAME_HEIGHT/2.0f - y * 24 * dungeon->getTexture().getScale() - 12 * dungeon->getTexture().getScale();
	texture.setCurrentFrame(direction * 4 + 1);
	rlAnim = 0;
	curX += sx;
	curY += sy;
}

void Character::setCoord(int x, int y)
{
	dungeon->updateOffset(
		dungeon->getOffX() + (int)GAME_WIDTH/2 - x * 24 * dungeon->getTexture().getScale() - 12 * dungeon->getTexture().getScale(),
		dungeon->getOffY() + (int)GAME_HEIGHT/2 - y * 24 * dungeon->getTexture().getScale() - 12 * dungeon->getTexture().getScale()
	);
	curX = x;
	curY = y;
}

void Character::equipItem(int itemNum)
{
	if(itemNum < 0)
		return;
	AN_Item *meow = items->getItem(itemNum);
	att[2] += meow->getAttribute(2);
	att[3] += meow->getAttribute(3);
	att[4] += meow->getAttribute(0);
	att[5] += meow->getAttribute(1);
	att[6] += meow->getAttribute(4);
	att[7] += meow->getAttribute(5);
	att[8] += meow->getAttribute(6);
	att[9] += meow->getAttribute(7);
}
void Character::unEquipItem(int itemNum)
{
	if(itemNum < 0)
		return;
	AN_Item *meow = items->getItem(itemNum);
	att[2] -= meow->getAttribute(2);
	att[3] -= meow->getAttribute(3);
	att[4] -= meow->getAttribute(0);
	att[5] -= meow->getAttribute(1);
	att[6] -= meow->getAttribute(4);
	att[7] -= meow->getAttribute(5);
	att[8] -= meow->getAttribute(6);
	att[9] -= meow->getAttribute(7);
}