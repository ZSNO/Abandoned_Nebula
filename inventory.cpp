#include "inventory.h"

bool Inventory::initialize(Graphics *g, int width, int height, int ncols, TextureManager *textureM, Input *i, AN_ItemCon *it, TextDX *text, TextDX *text2, TextureManager *bg, Character *pl)
{
	input = i;
	items = it;
	potionNum = text;
	itemName = text2;
	player = pl;
	background.setX(624);
	background.setY(0);
	background.setVisible(0);
	itemDesc.setVisible(0);
	return (background.initialize(g,width,height,ncols,textureM) && itemDesc.initialize(g,width,height,ncols,bg));
}

void Inventory::update(float frameTime)
{
	background.update(frameTime);
	if(input->getCharIn() == 'i')
	{
		input->clearCharIn();
		if(background.getVisible())
			background.setVisible(0);
		else
			background.setVisible(1);
	}
	//let user see stats and drag items
	if(background.getVisible())
	{
		if(!(input->getMouseX() > 674 && input->getMouseX() < 994 && input->getMouseY() < 731 && input->getMouseY() > 50) && itemDragging != -1)
			return;
		mousedOver = checkMouseLoc();
		if(mousedOver == -1 && itemDragging < 0)
			return;

		//if holding mouse button check to drag item
		if(input->getMouseLButton() && itemDragging == -1)
		{
			itemDragging = mousedOver;
			//do dragging stuff
		}
		else if(!(input->getMouseLButton()) && itemDragging != -1)
		{
			int tempSlot = items->getInventSlot(mousedOver); //Get item num from transfer slot
			//put item in slot, if no slot or put in incompatible slot, return to pos
			if(mousedOver == -1)
			{
				itemDragging = -1;
				return;
			}
			int itemDragged = items->getInventSlot(itemDragging);
			if(mousedOver == 16) //trash
			{
				items->removeInventSlot(itemDragged,itemDragging);
				itemDragging = -1;
				return;
			}
			else if(mousedOver == 17 && items->getItem(itemDragged)->getItemType() == 0) //use
			{
				short num = items->getItem(itemDragged)->getAttribute(0);
				if(num > 1)
					items->getItem(itemDragged)->setAttribute(0,num-1);
				else
					items->removeInventSlot(itemDragged,itemDragging);
				itemDragging = -1;
				items->usePotion(items->getItem(itemDragged)->getItemSpec());
				return;
			}
			else if(mousedOver > 11)
			{
				int itemType = items->getItem(itemDragged)->getItemType();
				if(itemType == 2)
				{
					int itemSpec = items->getItem(itemDragged)->getItemSpec();
					if((itemSpec == 0 && mousedOver != 14) || (itemSpec == 1 && mousedOver != 13) || (itemSpec == 2 && mousedOver != 12))
					{
						itemDragging = -1;
						return;
					}
				}
				else if((itemType == 1 && mousedOver != 15) || itemType != 1)
				{
					itemDragging = -1;
					return;
				}
				player->unEquipItem(tempSlot);
				player->equipItem(itemDragged);
			}

			items->setInventSlot(itemDragged,mousedOver); //Set transfer slot to dragged item
			items->setInventSlot(tempSlot,itemDragging); //Set original slot to transfer slot

			//add character equipt stuff

			itemDragging = -1;
		}
		//if not holding mouse button check to see if mouse over item
		else if(itemDragging == -1)
		{
			itemDesc.setX(input->getMouseX() - (float)itemDesc.getWidth());
			itemDesc.setY(input->getMouseY() - (float)itemDesc.getHeight()/2);
			itemDesc.setVisible(1);
		}
	}
}

void Inventory::draw()
{
	if(!background.getVisible())
		return;
	background.draw();
	int x, y;
	x = (int)background.getX() + 68;
	y = 318;
	for(int i=0;i<INVENTORYSLOTS;i++)
	{
		if(i == 12)
		{x = (int)background.getX() + 68;y = 68;}
		if(i == 15)
		{x = (int)background.getX() + 178;y = 178;}
		if(items->getInventSlot(i) == -1)
		{
			x += 110;
			if(x > 296 + background.getX())
			{x=(int)background.getX() + 68;y+=110;}
			continue;
		}
		if(i == itemDragging)
			items->draw(input->getMouseX() - items->getTextureSize(0)/2,input->getMouseY() - items->getTextureSize(1)/2,items->getInventSlot(i) + 1,4,0);
		else
			items->draw(x,y,items->getInventSlot(i) + 1,4,0);
		if(items->getItem(items->getInventSlot(i))->getItemType() == 0)
		{
			std::ostringstream out;
			out << items->getItem(items->getInventSlot(i))->getAttribute(0);
			potionNum->print(out.str(), x + items->getTextureSize(0) - 12, y + items->getTextureSize(1) - 18);
		}

		x += 110;
		if(x > 296 + background.getX())
		{x=(int)background.getX() + 68;y+=110;}
	}

	int itemNum = items->getInventSlot(mousedOver);
	if(mousedOver != -1 && itemNum != -1 && itemDesc.getVisible())
	{
		itemDesc.draw(graphicsNS::ALPHA50);
		itemName->print(items->getItem(itemNum)->getName(),(int)itemDesc.getX()+25,(int)itemDesc.getY()+15);
		if(items->getItem(itemNum)->getItemType() != 0)
		{
			std::ostringstream out;
			for(int i=0;i<8;i++)
			{
				short att = items->getItem(itemNum)->getAttribute(i);
				if(att == 0)
					continue;
				std::string attStr;
				switch(i){
				case 0:
					attStr="Damage: ";break;
				case 1:
					attStr="Speed: ";break;
				case 2:
					attStr="Extra Health: ";break;
				case 3:
					attStr="Extra Armor: ";break;
				case 4:
					attStr="Fire Chance: ";break;
				case 5:
					attStr="Poison Chance: ";break;
				case 6:
					attStr="Freeze Chance: ";break;
				case 7:
					attStr="Paralyze Chance: ";break;
				}
				out << attStr << att << "\n";
			}
			potionNum->print(out.str(),(int)itemDesc.getX()+35,(int)itemDesc.getY()+40);
		}
		itemDesc.setVisible(0);
	}
}

int Inventory::checkMouseLoc()
{
	int x = (int)background.getX();
	
	//slot 0
	if(checkArea(50 + x, 300, 150 + x, 400))
		return 0;
	//slot 1
	if(checkArea(160 + x, 300, 260 + x, 400))
		return 1;
	//slot 2
	if(checkArea(270 + x, 300, 370 + x, 400))
		return 2;

	//slot 3
	if(checkArea(50 + x, 410, 150 + x, 510))
		return 3;
	//slot 4
	if(checkArea(160 + x, 410, 260 + x, 510))
		return 4;
	//slot 5
	if(checkArea(270 + x, 410, 370 + x, 510))
		return 5;

	//slot 6
	if(checkArea(50 + x, 520, 150 + x, 620))
		return 6;
	//slot 7
	if(checkArea(160 + x, 520, 260 + x, 620))
		return 7;
	//slot 8
	if(checkArea(270 + x, 520, 370 + x, 620))
		return 8;

	//slot 9
	if(checkArea(50 + x, 630, 150 + x, 730))
		return 9;
	//slot 10
	if(checkArea(160 + x, 630, 260 + x, 730))
		return 10;
	//slot 11
	if(checkArea(270 + x, 630, 370 + x, 730))
		return 11;

	//slot 12
	if(checkArea(50 + x, 50, 150 + x, 150))
		return 12;
	//slot 13
	if(checkArea(160 + x, 50, 260 + x, 150))
		return 13;
	//slot 14
	if(checkArea(270 + x, 50, 370 + x, 150))
		return 14;
	//slot 15
	if(checkArea(160 + x, 160, 260 + x, 260))
		return 15;

	//slot 16 trash
	if(checkArea(50 + x, 160, 150 + x, 260))
		return 16;
	//slot 17 use
	if(checkArea(270 + x, 160, 370 + x, 260))
		return 17;

	return -1;
}

bool Inventory::checkArea(int sx, int sy, int ex, int ey)
{
	if(input->getMouseX() > sx && input->getMouseX() < ex && input->getMouseY() < ey && input->getMouseY() > sy)
		return 1;
	else
		return 0;
}