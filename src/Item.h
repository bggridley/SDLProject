#pragma once
#include "TextureAtlas.h"
#include "Resources.h"
#include "Renderer.h"

class Game;

class Item {

public:
	static TextureAtlas items;
	static TextureAtlas blocks;
	// id 0 to 1000 represents block items
	//id 1001 and past are just regular items
	int id = -1;
	Game* game;
	Item(Game* game, int itemID);
	void render(Renderer::Color color, float x, float y, float scale);
	virtual bool action();
	int width = 32;
	int height = 32;
	int amount = 0;

	static enum ID {
		GUN = 1001,
		TEST_ITEM = 100
	};
};