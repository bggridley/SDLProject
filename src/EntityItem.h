#pragma once
#include "Entity.h"
#include "Item.h"

class EntityItem : public Entity
{
public:
	Item* getItem();
	EntityItem(World* world, Item* item);
	void update(float delta, int updateCount);
	void render(float alpha);
	void entityCollision(Entity* collider) {}
	void tileCollision(float x, float y) {}
private:
	Item* item;
};

