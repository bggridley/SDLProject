#include "EntityPlayer.h"
#include "EntityItem.h"
#include "Inventory.h"

EntityPlayer::EntityPlayer(World* world) : Entity(world) {
	this->type = Type::PLAYER;
	width = 28;
	height = 56;
	yOffset = 4.0f;
	rotation = 0.0f;

	// 28 x 56
	animation.init(world->game->getResources().playerAtlas, 1, 7, 7);
}

void EntityPlayer::render(float alpha) {
	Entity::render(alpha);

	Inventory* inv = world->getGame()->getInventory();
	std::map<int, Item*> items = inv->getItems();
	if (items.find(inv->getScrollSlot()) == items.end()) { std::cout << "found nothing " << std::endl;  return; }
	Item* holding = world->getGame()->getInventory()->getItemAt(world->getGame()->getInventory()->getScrollSlot());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	

	
	
	if (holding->id == Item::GUN) {
		float lookX = world->getLookVector().x;
		float lookY = world->getLookVector().y;


		float rot = atan(lookY / lookX) * 180.0f / 3.14f;

		if (lookY < 0 && lookX < 0) {
			rot = rot - 180;
		} else if (lookY > 0 && lookX < 0) {
			rot = rot - 180;
		}

		if (rot == -0.0f && lookX < 0) {
			rot = -180;
		}

		glTranslatef(getLerpX(alpha) + (width / 2), getLerpY(alpha) + 17.0f, 0.0f);
	
		//std::cout << rot << ", " << world->getLookVector().x << "," << world->getLookVector().y << std::endl;
		glRotatef(rot, 0.0, 0.0, 1.0);

		if (rot < -90) {
			glScalef(1.0f, -1.0, 1.0);
		}
	}

	holding->render({ 1.0f, 1.0f, 1.0f, 1.0f }, 0, -17.0f, 1.0f);

	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);

}

void EntityPlayer::update(float delta, int updateCount) {
	Entity::update(delta, updateCount);

	//rotation = ((atan(vy / vx)) * 180.0f / 3.14f) + 90;

	if (world->getGame()->mousePressed) {
	//	rotation += 1.0f;
	}
}


// TODO actualyl do add the enetityCollision thing so we can figure out from what side this player is getting hit from
void EntityPlayer::entityCollision(Entity* e) {
	//maybe make like an overarching "entityliving" or something that can take bullet hits
	if (e->getType() == Type::BULLET) {
		if (e->getUpdates() > 10) { // can thit the player when the bullet is coming out of his gun
			world->createParticleEffect(Renderer::Color{ 1.0f, 0.0f, 0.0f, 1.0f }, 7, e->getX(), e->getY(), -e->getVelocityX(), -e->getVelocityY(), 10);
			e->setActive(false);
		} 
	}
	else if (e->getType() == Type::ITEM) {

		//if can go in inventory
		EntityItem* item = (EntityItem*)e;
		world->getGame()->getInventory()->addItem(item->getItem());
		e->fadeOut(60.0f);
		e->setActive(false);
	}
}