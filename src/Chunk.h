#pragma once
#include <glew.h>
#include <map>
#include "World.h"
#include "TextureAtlas.h"
#include "Resources.h"

class World;

class Chunk
{
public:
	Chunk();
	~Chunk();

	// TODO: change texture and textureatlas to be initialized with new instead of the init function.... it feels weird.
	void init(World* world, float xOffset, float yOffset); // yes
	void render() const;
	/*
	Update a chunk's material at a given x and y coordinate (from 0 to 31)
	*/
	void setMaterial(int x, int y, int id);
	void setColor(int x, int y, float r, float g, float b);
	void pushColors();
	const int getX() const;
	const int getY() const;
private:
	TextureAtlas atlas;
	World* world;
	float tileSize = 32.0f;

	GLuint vID;
	GLuint tID;
	GLuint cID; // color array
	int x;
	int y;
	int width;
	int height;
	

	// this offset is the offset within the ARRAY given, not any other OpenGL offset
	void fillTexCoords(float* textureData, TextureAtlas::UVTexCoords coords, int offset);
	void fillColorCoords(float* colorData, float r, float g, float b, int offset);

};