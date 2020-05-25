#pragma once
#include "Entity.h"

class World;
class Entity;

class EntityParticle : public Entity
{
public:
	EntityParticle(World* world);
	void update(float delta, int updateCount);
	void render(float alpha);
	void entityCollision(Entity* collider) {}
	void tileCollision(float x, float y) {};
	void setSize(float size);
};

