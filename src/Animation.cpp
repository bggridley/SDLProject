#include "Animation.h"

Animation::Animation() {
	initialized = false;
}

void Animation::init(TextureAtlas atlas, int startIndex, int endIndex, int interval) {
	this->currentIndex = startIndex;
	this->atlas = atlas;
	this->startIndex = startIndex;
	this->endIndex = endIndex;
	this->interval = interval;
	this->initialized = true;
}

void Animation::update(int updates) {
	if (!initialized) return;
	if (updates % interval == 0) {
		if (++currentIndex > endIndex) {
			currentIndex = startIndex;
		}
	}
}
void Animation::render(float x, float y, float width, float height) {
	if (!initialized) return;
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, atlas.getTexture().getTexID());
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_QUADS);


	TextureAtlas::UVTexCoords coords = atlas.getCoords(currentIndex);

	float xmin = flipped ? coords.xmax : coords.xmin;
	float xmax = flipped ? coords.xmin : coords.xmax;

	glTexCoord2f(xmin, coords.ymin);  glVertex2f(x, y);
	glTexCoord2f(xmax, coords.ymin); glVertex2f(x + width, y);
	glTexCoord2f(xmax, coords.ymax); glVertex2f(x + width, y + height);
	glTexCoord2f(xmin, coords.ymax);  glVertex2f(x, y + height);
	glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

bool Animation::isInitialized() {
	return initialized;
}

TextureAtlas Animation::getAtlas() {
	return atlas;
}

void Animation::setFlipped(bool flip) {
	flipped = flip;
}