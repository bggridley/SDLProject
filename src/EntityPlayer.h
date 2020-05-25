#pragma once
#include "Entity.h"

class EntityPlayer : public Entity {
public:
	EntityPlayer(World* world);
	void update(float delta, int updateCount);
	void render(float alpha);
	void entityCollision(Entity* e);
	void tileCollision(float x, float y) {};
};

