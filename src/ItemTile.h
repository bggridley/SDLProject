#pragma once
#include "Item.h"
#include "World.h"
#include <functional>

class ItemTile : public Item {
public:
	ItemTile(Game* game, int material) : Item(game, material) {
		width = 16;
		height = 16;
	}

	bool action() override {
		return game->world->setMaterialAtMouse(this->id); // this is because itemTile < 1000 id;
	}
};
