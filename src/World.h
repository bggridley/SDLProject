#pragma once
#include "Game.h"
#include <SDL.h>
#include "Entity.h"
#include <vector>
#include "Chunk.h"
#include <cmath>
#include <algorithm>
#include <iterator>
#include <map>
#include "Point.h"

class Game;
class Entity;
class Chunk; // forward declarations

class World {
private:
	Point lookVector{0.0f, 0.0f};
	int count = 0;
	Entity* player;
	std::vector<Point> tilePoints; // used for collision detection, this is static because all tilepoints are the same
	std::vector<Entity*> entities;
	std::vector<Entity*> particleEffects;
	std::map<int, Chunk*> activeChunks; // int here is the chunk ID aka chunk index (in the world grid)

	int width; // world size in chunks
	int height; // world size in chunks
	
	//const int chunkSize = 32;
	float tileSize = 32.0f; // ensure these are the same as they are defined in Chunk::init

	struct MaterialData {
		bool solid = false;
	};


	struct Tile { // we will know the tileX/tileY based off of the array so no tileX/tileY is needed

		float lightLevel = 0.0f;
		float r = 0.0f; // between 0 and 255
		float g = 0.0f; // between 0 and 255
		float b = 0.0f; // between 0 and 255
		// R G AND B are the shader colors that will go over a texture
		short id; // replace this with materialData you know?  a struct inside of a struct
		// maybe hold a material data of some sort? which will hold the specific material data of the tile
	};

	
	MaterialData materialData[16 * 16]; // eventually the spritesheet will be 16 * 16 maybe even bigger
	 //initialize this array on world creation
	//Chunk** chunks;
public:
	Tile** tiles;
	int** lightmap;
	void init(); // spawn player, etc.
	World(Game* g, int width, int height);
	~World();
	Game* game; // this should be private
	struct LightDirections {
		bool left;
		bool right;
		bool up;
		bool down;
	};
	void updateLight(int currentX, int currentY, float lightLevel, float r, float g, float b, LightDirections directions);
	void updateLighting();
	void updateChunks();
	void update(float delta, int updateCount);
	void render(float alpha);
	void addObject(Entity* obj);
	void createParticleEffect(Renderer::Color c, int particles, float x, float y, float vx, float vy, float size);
	/* 
	x and y here are tile coordinates
	*/
	MaterialData getMaterial(int x, int y);
	bool setMaterialAtMouse(int materialID);
	bool setMaterial(int x, int y, int materialID);
	bool setColor(int x, int y, float r, float g, float b);
	Tile getTile(int x, int y);
	bool left;
	bool right;
	bool up;
	bool down;



	enum Material {
		AIR,
		STONE,
		GRASS,
		DIRT,
	};

	int getEntityCount() {
		return entities.size();
	}

	std::map<int, Chunk*> getActiveChunks() { return activeChunks; }
	Entity* getPlayer() { return player; }
	int getHeight() {
		return height;
	}

	int getWidth() {
		return width;
	}

	Game* getGame() {
		return game;
	}

	Point getLookVector() {
		return lookVector;
	}

	std::vector<Point> getTilePoints() {
		return tilePoints;
	}

	friend class Entity;
	friend class Chunk;
};