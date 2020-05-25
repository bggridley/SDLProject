#pragma once
#include "TextureAtlas.h"

class Animation {

public:
	Animation();
	void update(int updates);
	void render(float x, float y, float width, float height);
	void init(TextureAtlas atlas, int startIndex, int endIndex, int interval);
	bool isInitialized();
	TextureAtlas getAtlas();
	void setFlipped(bool flip);
private:
	int interval;
	int startIndex;
	int endIndex;
	int currentIndex;
	TextureAtlas atlas;
	bool initialized = false;
	bool flipped = false;
};