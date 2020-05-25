#include "Item.h"
#include "Game.h"

TextureAtlas Item::items;
TextureAtlas Item::blocks;
//enum Item::ID;

bool Item::action() {
	return false;
	// by default nothing... it must be overrided.  i didn't use vritual bool because i wanted new Item(id.... other args) to be possible for items w/ like no action or litle data
}

Item::Item(Game* game, int itemID)  {
	this->id = itemID;
	this->game = game;
	if (!items.isInitialized()) {
		items = game->getResources().itemAtlas;
	}

	if (!blocks.isInitialized()) {
		blocks = game->getResources().tileAtlas;
	}
}

void Item::render(Renderer::Color color, float x, float y, float scale) {
	int width = this->width;
	int height = this->height;
	TextureAtlas currentAtlas;
	int index;
	if (id < 1000) {
		currentAtlas = blocks;
		index = id;
	}
	else {
		index = id - 1000;
		currentAtlas = items;
	}

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, currentAtlas.getTexture().getTexID());
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(color.r, color.g, color.b, color.a);
	glBegin(GL_QUADS);

	width *= scale;
	height *= scale;

	TextureAtlas::UVTexCoords coords = currentAtlas.getCoords(index);
	glTexCoord2f(coords.xmin, coords.ymin);  glVertex2f(x, y);
	glTexCoord2f(coords.xmax, coords.ymin); glVertex2f(x + width, y);
	glTexCoord2f(coords.xmax, coords.ymax); glVertex2f(x + width, y + height);
	glTexCoord2f(coords.xmin, coords.ymax);  glVertex2f(x, y + height);
	glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

}