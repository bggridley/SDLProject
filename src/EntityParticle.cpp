#include "EntityParticle.h"

EntityParticle::EntityParticle(World* world) : Entity(world) {
	this->type = Type::PARTICLE;
	this->destroyOnInactivity = false;
	collisions = false;
	width = 10;
	height = 10;
}

void EntityParticle::render(float alpha) {
	Entity::render(alpha);
}

void EntityParticle::update(float delta, int updateCount) {
	Entity::update(delta, updateCount);

	rotation = x * 8;

	if (color.a == 0.0f) {
		active = false;
		//std::cout << "deactivated particle in update" << std::endl;
	}
}

void EntityParticle::setSize(float size) {
	this->width = size;
	this->height = size;
}