#include "Inventory.h"
#include "Game.h"

Item* Inventory::pickedItem;

Inventory::Inventory(Game* game) {
	this->game = game;
	this->inventory = game->getResources().inventory;

	//this->items.emplace(0, new Item(game, 1000));
	this->items.emplace(0, new ItemGun(game));
	this->items.emplace(4, new ItemTile(game, 2));
	this->items.emplace(3, new ItemTile(game, 0));
	this->items.emplace(9, new Item(game, 1000));
}

void Inventory::input(SDL_Event* event) {
	if (event->type == SDL_MOUSEBUTTONDOWN) {

		if (mouseSlot.x >= 0 && mouseSlot.x < width && mouseSlot.y >= 0 && mouseSlot.y < (opened ? height : 1)) {

			if (opened || mouseSlot.y == 0) {
				int index = mouseSlot.x + (width * mouseSlot.y);

				if (items.find(index) == items.end()) {
					// there is NO item in the clicked slot
					if (pickedItem != nullptr) {
						items.emplace(index, pickedItem);
						pickedItem = nullptr;
					}
				}
				else {
					if (pickedItem == nullptr) {
						pickedItem = items.at(index);
						items.erase(index);
					}

					else {
						// they are both existing... so switch them

						Item* oldPickedItem = pickedItem;
						pickedItem = items.at(index);
						items.erase(index);
						items.emplace(index, oldPickedItem);
					}
				} 
			}
		}
		else {
			// so we clicked outside of the inventory...
			// DROP
			if (pickedItem != nullptr) {
				//std::cout << "drop" << std::endl;
				EntityItem* item = new EntityItem(game->world, pickedItem);
				item->setPosition(game->mouseGameX, game->mouseGameY);
				item->setColor({ 1.0f, 1.0f, 1.0f, 0.0f });
				item->fadeIn(15);
				pickedItem = nullptr;
			}
		}
	}

	if (event->type == SDL_KEYDOWN) {
		if (event->key.keysym.sym >= 49 && event->key.keysym.sym <= 56) {
			scrollSlot = event->key.keysym.sym - 49;
		}
	}
}

void Inventory::update() {
	float mouseX = game->mouseX;
	float mouseY = game->mouseY;

	if (game->mousePressed) {
		if (!(mouseSlot.x >= 0 && mouseSlot.x < width && mouseSlot.y >= 0 && mouseSlot.y < (opened ? height : 1))) {
			if (items.find(scrollSlot) != items.end()) {
				if (items.at(scrollSlot)->action()) {
					items.at(scrollSlot)->amount--;
				}
			}
		}
	}

	mouseSlot.y = -1;
	mouseSlot.y = -1;
	//std::cout << "update" << xstart << ", " << mouseX << std::endl;

	float x = (mouseX);
	float y = (mouseY);

	// implement inventroyContainer bullshit that's gonna suck ass.
	if (x >= xstart && x < xstart + (inventory.getWidth() * 2.0f)) {
		if (y >= ystart && y < ystart + ((inventory.getWidth() / width) * height * 2.0f)) {
			mouseSlot.x = ((x - xstart) / (slotWidth * 2));
			mouseSlot.y = ((y - ystart) / (slotWidth * 2));
			//	std::cout << "inside of inventroy" << std::endl;
		}
	}
}

void Inventory::render() {
	game->getRenderer().setScale(2.0f);

	float xOffset = ((game->getWidth() - (inventory.getWidth() * game->getRenderer().getScale()))) / 2.0f;

	game->getRenderer().drawTexturedQuad(inventory, xOffset - game->getRenderer().getScale(), game->getRenderer().getScale() * 2, 0, 0, inventory.getWidth(), (inventory.getWidth() / width) * (opened ? 4.0f : 1.0f));
	//int spacingAdd = (34 - 16); // chagne this per item... block : 16  .. other : 24
	int count = 0;

	if (mouseSlot.x != -1 && mouseSlot.y != -1) {
		//std::cout << "ahh" << mouseSlot.x << std::endl;

		if (mouseSlot.y == 0 || opened) {

			game->getRenderer().drawTexturedQuad(inventory, this->xstart + ((mouseSlot.x) * game->getRenderer().getScale() * slotWidth), this->ystart + ((mouseSlot.y) * game->getRenderer().getScale() * slotWidth), 1 * slotWidth, 4 * slotWidth, slotWidth, slotWidth);
		}
	}

	game->getRenderer().drawTexturedQuad(inventory, this->xstart + ((scrollSlot) * game->getRenderer().getScale() * slotWidth), this->ystart, 0 * slotWidth, 4 * slotWidth, slotWidth, slotWidth);

	for (std::map<int, Item*>::iterator it = items.begin(); it != items.end(); ) {
		Item* item = it->second;
		int spacingAdd = slotWidth - item->width;
	

		int x = it->first % width;
		int y = it->first / width;

		if (!opened && it->first >= width) {
			it++;
			continue;
		}

		float xrender = xOffset - (game->getRenderer().getScale()); //+ spacingAdd;
		float yrender = (game->getRenderer().getScale() * 2); //+ spacingAdd;
		this->xstart = xrender;
		this->ystart = yrender;


		xrender += (x * game->getRenderer().getScale() * slotWidth) + spacingAdd;
		yrender += (y * game->getRenderer().getScale() * slotWidth) + spacingAdd;

		item->render({1.0f, 1.0f, 1.0f, 1.0f}, xrender, yrender, game->getRenderer().getScale());
		game->getFont().setScale(1.0f);
		game->getFont().renderString(xrender, yrender, std::to_string(item->amount));
		//count++;

		count++;
		it++;
	}

	if (pickedItem != nullptr) {
		pickedItem->render({1.0f, 1.0f, 1.0f, 1.0f}, game->mouseX, game->mouseY, game->getRenderer().getScale());
	}
}

Inventory::~Inventory() {
	for (const auto& values : items) {
		delete values.second;
	}
}

void Inventory::toggle() {
	opened = !opened;
}

bool Inventory::isOpen() {
	return opened;
}

bool Inventory::addItem(Item* item) {
	for (int j = 0; j < height; j++) {
	for (int i = 0; i < width; i++) {
			int index = i + (width * j);
			if (items.find(index) == items.end()) {
				items.emplace(index, item);
				return true;
			}
		}
	}

	return false;
}

