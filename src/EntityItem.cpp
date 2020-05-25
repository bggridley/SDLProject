#include "EntityItem.h"

EntityItem::EntityItem(World* world, Item* item) : Entity(world) {
	this->type = Type::ITEM;
	this->maxSpeed = 10;
	this->item = item;

	width = item->width;
	height = item->height;
}

void EntityItem::render(float alpha) {
	//Entity::render(alpha);
	item->render(this->color, getLerpX(alpha), getLerpY(alpha), 1.0f);
}

void EntityItem::update(float delta, int updateCount) {
	Entity::update(delta, updateCount);
}

Item* EntityItem::getItem() {
	return item;
}