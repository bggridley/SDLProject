#pragma once
#include "glew.h"
class Texture
{
private:
	GLuint texID;
	int width = -1;
	int height = -1;// width and height in pixels
public:
	Texture() {} // stfu
	~Texture() {}
	void init(GLuint tID, int w, int h) {
		this->texID = tID;
		this->width = w;
		this->height = h;
	}//: texID(tID), width(w), height(h) {}

	GLuint getTexID() {
		return texID;
	}

	int getWidth() {
		return width;
	}

	int getHeight() {
		return height;
	}
};

