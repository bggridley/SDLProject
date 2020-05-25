#include "EntityBullet.h"

EntityBullet::EntityBullet(World* world, float damage) : Entity(world) {
	this->type = Type::BULLET;
	this->maxSpeed = 30.0f;
	this->color = { 0.0, 1.0f, 0.0f, 1.0f };
	width = 14;
	height = 4;
}

void EntityBullet::render(float alpha) {
	//std::cout << "etnitybulletrender";
	Entity::render(alpha);
}

void EntityBullet::update(float delta, int updateCount) {
	Entity::update(delta, updateCount);
	// set rotation to match velocity

	rotation = staticRotation == 0.0 ? (atan(vy / vx)) * 180.0f / 3.14f : staticRotation;
	if (!falling && vx <= 0.01 && vy <= 0.01 && this->active) {
		//std::cout << "starting" << std::endl;
		this->setActive(false);
		//this->fadeOut(60);
	}

	//std::cout << vx << ", " << vy << "| " << x << "," << y << std::endl;
}

void EntityBullet::tileCollision(float x, float y) {
	staticRotation = rotation;
	active = false;
	vx = 0.0f;
	vy = 0.0f;
	//this->setActive(false);
	//this->setColor({ 1.0f, 1.0f, 1.0f, 0.0f });
	//world->createParticleEffect({ 0.0f, 1.0f, 0.0f, 1.0f }, 2, x, y, -vx, -vy, 1);
}