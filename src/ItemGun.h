#pragma once
#include "Item.h"
#include "World.h"
#include "EntityBullet.h"
#include "SDL_mixer.h"
#include <functional>

class ItemGun : public Item {
public:
	int lastTime = -1;
	Mix_Chunk* s_zap;
	ItemGun(Game* game) : Item(game, 1001) {
		s_zap = game->getResources().s_zap;
	}


	bool action() override {
	//	std::cout << game->updates << std::endl;

		if (game->updates - lastTime > 6 || lastTime == -1) { // 3 bullets per sec
			// this process could be simplified
			Mix_PlayChannel(-1, s_zap, 0);
	
			EntityBullet* bullet = new EntityBullet(game->world, 10);
			bullet->setActive(true);
			//Point vel = { (game->mouseX - (game->getWidth() / 2)) / 10.0f, (game->mouseY - (game->getHeight() / 2)) / 10.0f };
			Point vel = {( game->world->getLookVector().x / 10.0f), (game->world->getLookVector().y / 10.0f)};
			bullet->setVelocity(vel.x, vel.y);
		
			//std::cout << (bullet->getVelocityX()) << "," << bullet->getVelocityY() << std::endl;
			bullet->setRotation(atan(vel.y / vel.x) * 180.0f / 3.14f);
			bullet->setPosition((game->world->getPlayer()->getX() + (game->world->getPlayer()->getWidth() / 2) - bullet->getWidth() / 2) + vel.normalized().x * 40, -14 + (game->world->getPlayer()->getY() + (game->world->getPlayer()->getHeight() / 2) - (bullet->getHeight() / 2)) + vel.normalized().y * 40);
			game->world->createParticleEffect({ 85, 1.0f, 1.0f, 1.0f }, 4, bullet->getX(), bullet->getY(), vel.x / 8, vel.x / 8, 1);
		//	game->world->createParticleEffect({ 255, 152/255.0f, 0.0f, 1.0f }, 4, bullet->getX(), bullet->getY(), vel.x / 8, vel.x / 8, 1);
			//bullet->setPosition((game->world->getPlayer()->getX() + game->world->getPlayer()->getWidth() / 2) - 6, (game->world->getPlayer()->getY() + game->world->getPlayer()->getHeight() / 2) + 45);

			lastTime = game->updates;
		}
		return true;
	}
};
