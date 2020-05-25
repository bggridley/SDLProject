#include "TextureAtlas.h"
#include <iostream>

void TextureAtlas::init(Texture t, int width, int height, int textureWidth, int textureHeight, float offset) {
	this->tex = t;
	this->width = width;
	this->height = height;
	this->textureWidth = textureWidth;
	this->textureHeight = textureHeight;
		//atlasWidth * astlasWidth gives amount of materials in the atlas
		for (int i = 0; i < width * height; i++) {
			// in amount of textures
			int xpos = i % (width);
			int ypos = i / (width);

			UVTexCoords coords;
			coords.xmin = ((float)xpos / (float)width) + (offset / t.getWidth()); // add half a pixel to avoid texture bleeding
			coords.xmax = coords.xmin + ((float)textureWidth - (offset * 2)) / (float)t.getWidth();

			
			coords.ymin = ((float)ypos / (float)height) + (offset / t.getWidth()); // fix so theres also atlas height
			coords.ymax = coords.ymin + ((float)textureHeight - (offset * 2))/ (float)t.getHeight();

		//	std::cout << i << "x: " << coords.xmin << ", " << coords.xmax << std::endl;
		//	std::cout << i << "y: " << coords.ymin << ", " << coords.ymax << std::endl;


			texCoords.emplace(std::pair<int, UVTexCoords>(i, coords));
			//texCoords[i] = coords;
		}

		initialized = true;
}

Texture TextureAtlas::getTexture() const {
	return tex;
}

bool TextureAtlas::isInitialized() {
	return initialized;
}

int TextureAtlas::getWidth() {
	return width;
}

int TextureAtlas::getHeight() {
	return height;
}

int TextureAtlas::getTextureWidth() {
	return textureWidth;		
}

int TextureAtlas::getTextureHeight() {
	return textureHeight;
}

TextureAtlas::UVTexCoords TextureAtlas::getCoords(int material) {
	return texCoords.at(material);
}