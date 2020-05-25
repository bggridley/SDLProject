#pragma once
#include "Item.h"
#include "ItemTile.h"
#include "EntityItem.h"
#include "ItemGun.h"
#include <map>
#include <iterator>

class Game;

class Inventory {

private:
	int width = 8;
	int height = 4;
	int slotWidth = 34;
	Texture inventory;
	std::map<int, Item*> items;
	Game* game;
	bool opened = false;

	float xstart = 0;
	float ystart = 0;

	struct Slot {
		int x;
		int y;
	};

	//scrollslot only in playerinventory
	int scrollSlot = 0;

	Slot mouseSlot;

	static Item* pickedItem; // all sub classes will share this static item.. good!

public:
	Inventory(Game* game);
	~Inventory();
	void input(SDL_Event* event); 
	void render();
	void update();
	void toggle();
	bool isOpen();
	bool addItem(Item* item);
	std::map<int, Item*> getItems() { return items; }
	int getScrollSlot() { return scrollSlot; }
	Item* getItemAt(int index) { return items.at(index); }
};