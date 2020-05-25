#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <map>
#include "glew.h"
#include "Texture.h"
#include "Renderer.h"

class BitmapFont
{
public:
	~BitmapFont();
	void init(std::string fileName, GLuint texId);
	// if worldcoords is true, then the font will be rendered with the world offset applied to it(so render in world coords)
	void renderString(float x, float y, std::string string);
	void renderCenteredString(float x, float y, std::string string);
	void setColor(float r, float g, float b, float a) { color.r = r; color.g = g; color.b = b; color.a = a; }
	void setScale(float s) { this->scale = s; }
	float getWidth(std::string string);
	float getHeight(std::string string);
private:
	static float scale;
	Renderer::Color color;

	//Color color;
	// depreacated; it was a good try and good use of mental power...
	// you learned how to use VBO's with not only arrays, but also Vectors
	// sometimes immediate draw mode is actually better, somehow, someway
	void renderStaticString(float x, float y, const std::string, bool worldCoords); // WARNING: if this isn't constant... then a new vbo will be made for EVERY SINGLE new string
	std::vector<std::string> tokenizer(const std::string& p_pcstStr, char delim);
	GLuint texID;
	struct StaticString {
		int length;
		GLuint vID;
		GLuint tID;
		std::vector<float> vertexData;
		std::vector<float> textureData;
		float originalX; // originals are stored, when it varies, translate the difference to avoid re-updating vertices every frame.
		float originalY;
	};

	struct Glyph {
		int id;
		float x;
		float y;
		int width;
		int height;
		int xoffset;
		int yoffset;
		int xadvance;

		float u; // the xmin/ max that will map to the texture
		float v;
		float u2;
		float v2;
	};
	std::map<std::string, StaticString> staticStrings; // the gluInt is the VBO ID
	std::map<int, Glyph> glyphs;


};

