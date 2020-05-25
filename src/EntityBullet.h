#pragma once
#include "Entity.h"

class World;
class Entity;

class EntityBullet : public Entity
{
public:
	EntityBullet(World* world, float damage);
	void update(float delta, int updateCount);
	void render(float alpha);
	void entityCollision(Entity* collider) {}
	void tileCollision(float x, float y) override;
	float staticRotation = 0.0f;
};

