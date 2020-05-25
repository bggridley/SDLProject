#pragma once
#include "World.h"
#include "Renderer.h"
#include <vector>
#include "Animation.h"
#include "TextureAtlas.h"
#include <algorithm>
#include "Point.h"

class World;

class Entity
{
private:
	struct Projection {
		float _min = 0.0f;
		float _max = 0.0f;

		Projection(float min, float max) {
			_min = min;
			_max = max;
		}

		bool overlap(Projection p2) {
			return !(this->_min > p2._max || p2._min > this->_max);
		}

		float getOverlap(Projection p2) {
			return std::min(_max, p2._max) - std::max(_min, p2._min);
		}
	};
private:
	int fading = -1;
	float alphaPerTick;
	std::vector<Point> getRotatedPoints();
	Point rotatePoint(Point point, Point center, float angle);
	void getNormals(std::vector<Point>* axes, std::vector<Point> entityPoints); // point class is essentailyl a vetor; this will fil the axes.
	Projection getProjection(Point position, std::vector<Point> positions, Point norm);

	bool isCollision(int x, int y);
	struct Collision {
		int tileX;
		int tileY;
		float overlap;
		Point axis{ 0.0f, 0.0f };
	};

public:
	enum Type {
		PLAYER,
		BULLET,
		PARTICLE,
		ITEM,
	};

	Entity(World* world);
	void setPosition(float x, float y);
	void setVelocity(float x, float y);
	void setRotation(float degs);
	float getX() { return x; }
	float getY() { return y; }
	float getVelocityX() { return vx; }
	float getVelocityY() { return vy; }
	float getWidth() { return width; }
	float getHeight() { return height; }
	int getUpdates() { return updates; }
	void setDestroyOnInactivity(bool a) {
		this->destroyOnInactivity = a;
	}
	void setColor(Renderer::Color c);
	Renderer::Color getColor();
	virtual void update(float delta, int updateCount) = 0;
	virtual void render(float alpha) = 0; // pure virtual functions so this class becomes abstract
	float getLerpX(float alpha);
	float getLerpY(float alpha); // these are for rendering purposes, helps to make smooth stuff.
	void lerpTranslation(float alpha, float scale);
	bool isPlayer() { return this->type == Type::PLAYER; }
	void checkCollisions();
	std::vector<Collision> checkCollisions(float vx, float vy);
	bool collisionResponse(Collision c, float vx, float vy);
	bool collides(float x, float y, float width, float height);
	bool collisions = true;
	friend class World;

	int getType();
	void updateFade(float delta);
	void fadeIn(int ticks);
	void fadeOut(int ticks);
	void setActive(bool act); // to control updating
	bool isActive();
	void setSize(float width, float height);
	void setID(int id);
	// for collisions
	float minX = INT_MAX;
	float minY = INT_MAX;
	float maxX = INT_MIN;
	float maxY = INT_MIN;
protected:
	Animation animation; // present in all classes which have animation u know...
	virtual void entityCollision(Entity* collider) = 0;
	virtual void tileCollision(float x, float y) = 0;
	enum EntityCollision {
		LEFT,
		RIGHT,
		TOP,
		BOTTOM
	};
	int id; // id is assigned by the world
	Renderer::Color color = { 1.0f, 1.0f, 1.0f, 1.0f };
	SDL_Rect rect;
	bool player;
	float maxSpeed = 15.0f;
	float x;
	float y;
	float vx;
	float vy;
	float rotation = 0.0f;

	float yOffset = 0.0f; // for the animation

	float movementSpeed; // aka acceleration
	float maxMovementSpeed;
	float width;
	float height;

	bool gravity = true;
	bool falling = false;
	bool active = true;
	bool destroyOnInactivity = true;
	World* world;
	int type; // is an ENUM but C++ is weird and will tell me i need enum class even when i do want it to convert to its underlying type (int).... weird
	int updates = 0;

	struct State {
		float x;
		float y;
	};

	State currentState;
	State previousState;

};