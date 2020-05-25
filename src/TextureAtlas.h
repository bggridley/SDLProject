#pragma once
#include "Texture.h"
#include <map>
class TextureAtlas
{
public:
	/*
	* width as in HOW MANY TEXTURES WIDTH WISE, height as in how many textures HEIGHT wise.
	* textureWidth as in PIXEL width of a texture, textureHeight as in pixel HEIGHT of an individual texture
	*/
	TextureAtlas() {

	}

	void init(Texture tex, int width, int height, int textureWidth, int textureHeight, float offset);

	struct UVTexCoords {
		float xmin;
		float xmax;
		float ymin;
		float ymax;
	};

	Texture getTexture() const;
	bool isInitialized();
	int getWidth();
	int getHeight(); // i don't think this is even needed anyway
	int getTextureWidth(); // i don't think this is even needed anyway
	int getTextureHeight(); // i don't think this is even needed anyway
	UVTexCoords getCoords(int material);
private:
	int width;
	int height;
	int textureWidth;
	int textureHeight;
	Texture tex; // allows to check if the atlas has been intiialized
	bool initialized = false;

	std::map <int, UVTexCoords> texCoords;
};

