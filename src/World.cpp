#include "World.h"
#include "EntityPlayer.h" // avoid circular dependencies
#include "EntityParticle.h"

// width and height in amount of chunks (
World::World(Game* g, const int w, const int h) : game(g), width(w), height(h) {
	tilePoints.push_back({ 0.0f, 32.0f });
	tilePoints.push_back({ 0.0f, 0.0f });
	tilePoints.push_back({ 32.0f, 0.0f });
	tilePoints.push_back({ 32.0f, 32.0f });

	tiles = new Tile * [width * 32];
	for (int i = 0; i < width * 32; i++)
		tiles[i] = new Tile[height * 32];

	float rh = 8; // random height
	for (int i = 0; i < width * 32; i++) {
		rh += ((rand() % 255) - (255.0f / 2.0f)) / 255.0f / 3;

		if (rh > 30) {
			rh = 30;
		}

		if (rh < 12) {
			rh = 12;
		}

		int y = 32 + int((std::sin(i * 3.14f / (32))) * rh);


		for (int j = 0; j < height * 32; j++) {

			int materialID = 0;
			if (j == y) {
				materialID = Material::GRASS;
			}
			else {
				if (j < y) {
					materialID = Material::AIR;
				}

				if (j > y) {
					materialID = Material::DIRT;
				}

				if (j > y + 10) {
					materialID = Material::STONE;
				}

			}

			tiles[i][j].id = materialID;
			// in radians

		}
	}


	// width = width in chunks
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			// TODO change the init fucntion to just take normal 
		//	chunks[i][j].init(this, game->spritesheet, i * 32 * tileSize, j * 32 * tileSize);
		}
	}

	//setMaterial(31, 32, Material::AIR);

	// do this for every material

	materialData[Material::AIR] = { false };
	materialData[Material::STONE] = { true };
	materialData[Material::GRASS] = { true };
	materialData[Material::DIRT] = { true };
}

void World::init() {
	player = new EntityPlayer(this);
	player->setPosition(0, 30 * 32);

	for (int i = 0; i < 30; i++) {
		//EntityPlayer* testDummy = new EntityPlayer(this);
		//testDummy->setPosition(500 + (i * 32), 0);
	}

	for (int i = 0; i < 100; i++) {
		EntityParticle* particle = new EntityParticle(this);
		particle->setActive(false);
		particle->setColor({ 1.0f, 1.0f, 1.0f, 0.0f });
		particleEffects.push_back(particle);
	}
}

World::~World() {
	for (int i = 0; i < width; i++)
		delete[] tiles[i];
	delete[] tiles;

	for (Entity* obj : entities) {
		delete obj;
	}

	for (const auto& values : activeChunks) {
		delete values.second;
	}
}

void World::update(float delta, int updateCount) {
	// update previous state if theres anything to be added

	lookVector.x = (game->mouseX - (game->getWidth() / 2));
	lookVector.y = (game->mouseY - (game->getHeight() / 2) + 17);

	// this is only used when holding guns so subtract the y

	updateChunks();
	if (game->mousePressed) {
		//
	//	player->rotation += 0.4;
	}




	//player->getRotatedPoints();



	// TODO move this to EntityPlayer class and remove friend class world from Entity.h.
	if (left) {
		// move the player instead
		player->vx -= 50.0f * delta;
		//xOffset += delta * 500;
	}


	if (right) {
		player->vx += 50.0f * delta;
		//xOffset -= delta * 500;
	}

	if (player->vx > 250 * delta) player->vx = 250 * delta;
	if (player->vx < -250 * delta) player->vx = -250 * delta;

	if (player->vx > 250 * delta) player->vx = 250 * delta;
	if (player->vx < -250 * delta) player->vx = -250 * delta;

	if (up && !player->falling) {
		player->vy = -500.0f * delta;
	}


	for (auto it = entities.begin(); it != entities.end(); it++) {
		Entity* obj = *it;

		obj->updateFade(delta);
		obj->update(delta, updateCount);

		if (!obj->isActive()) {
			if (obj->destroyOnInactivity && obj->getColor().a <= 0.05f) {
				entities.erase(it--); //decrement after PASSING value i loev c++ so cool ( as opposed to --it)
				//std::cout << "destroying because inactive " << std::endl;
			}
		}
	}
}

void World::updateChunks() {
	float sx = float(player->getX() - (32 * 2) - (game->getWidth() / 2) / game->getScale());
	float sy = float(player->getY() - (32 * 2) - (game->getHeight() / 2) / game->getScale());
	float ex = float(player->getX() + (32 * 2) + (game->getWidth() / 2) / game->getScale());
	float ey = float(player->getY() + (32 * 4) + (game->getHeight() / 2) / game->getScale());


	//std::cout << sx <<", " << ex << std::endl;

	int startX = float(sx / 32.0f / 32.0f);
	int startY = float(sy / 32.0f / 32.0f);

	int endX = float(ex / 32.0f / 32.0f);
	int endY = float(ey / 32.0f / 32.0f);

	//std::cout << startX << "," << endX << std::endl;
	if (startX < 0) startX = 0;
	if (startY < 0) startY = 0;
	if (endX > width - 1) endX = width - 1;
	if (endY > height - 1) endY = height - 1;

	for (std::map<int, Chunk*>::iterator it = activeChunks.begin(); it != activeChunks.end(); ) {
		Chunk* chunk = it->second;
		if (chunk->getX() + (32 * 32) < sx || chunk->getX() > ex || chunk->getY() + (32 * 32) < sy || chunk->getY() > ey) {
			it = activeChunks.erase(it);
			//std::cout << "removed a chunk. active chunks:" << std::endl;
		}
		else {
			it++;
		}
	}


	bool newchunk = false;
	for (int i = startX; i <= endX; i++) {
		for (int j = startY; j <= endY; j++) {
			int index = i + (j * width);
			if (activeChunks.find(index) == activeChunks.end()) {
				Chunk* chunk = new Chunk();
				chunk->init(this, i * 32 * tileSize, j * 32 * tileSize);
				activeChunks.emplace(index, chunk);
				//std::cout << "made a chunk: " << chunk->getX() << ", " << chunk->getY() <<  "active: " << activeChunks.size() << std::endl;
				newchunk = true;
			}
		}
	}

	if (newchunk) {
		updateLighting();
	}
}

void World::render(float alpha) {
	for (const auto& values : activeChunks) {
		values.second->render();
		//std::cout << "rendering something" << std::endl;
	}

	for (Entity* obj : entities) {
		obj->render(alpha);
	}
}

void World::updateLighting() {
	// i guess we can update all of the active chunks......
//	std::cout << "?" << std::endl;
	int minX = INT_MAX;
	int maxX = INT_MIN;
	int minY = INT_MAX;
	int maxY = INT_MIN;
	for (std::map<int, Chunk*>::iterator it = activeChunks.begin(); it != activeChunks.end(); ) {

		int index = it->first;

		int x = (index % (width)) * 32;
		int y = (index / width) * 32;

		if (x < minX) {
			minX = x;
		}

		if (x + 32 > maxX) {
			maxX = x + 32;
		}

		if (y < minY) {
			minY = y;
		}

		if (y + 32 > maxY) {
			maxY = y + 32;
		}

		it++;
	}

	//std::cout << minX << "," << maxX << std::endl;
	for (int i = minX; i < maxX; i++) {
		for (int j = minY; j < maxY; j++) {
			tiles[i][j].lightLevel = 0.0f;
			tiles[i][j].r = 0.0f;
			tiles[i][j].g = 0.0f;
			tiles[i][j].b = 0.0f;
		}
	}

	for (int i = minX; i < maxX; i++) {
		for (int j = minY; j < maxY; j++) {
			if (tiles[i][j].id == Material::AIR) {
				if (getTile(i, j + 1).id == 0 && getTile(i, j - 1).id == 0 && getTile(i + 1, j).id == 0 && getTile(i - 1, j).id == 0) continue;
				updateLight(i, j, 1.0f, 1.0f, 1.0f, 1.0f, { true, true, true, true });
				//	std::cout << "some air blocks" << std::endl;
			}
		}
	}

	//updateLight(10, 45, 1.0f, 0.5f, 1.0f, 1.0f, { true, true, true, true });
	updateLight(player->getX() / 32, player->getY() / 32, 1.0f, 1.0f, 1.0f, 1.0f, { true, true, true, true });
	

	for (std::map<int, Chunk*>::iterator it = activeChunks.begin(); it != activeChunks.end(); ) {
		it->second->pushColors();
		it++;
	}
}

void World::updateLight(int currentX, int currentY, float lightLevel, float r, float g, float b, LightDirections directions) {
	if (currentX < 0 || currentX > width * 32 || currentY < 0 || currentY > height * 32) return;

	Tile current = getTile(currentX, currentY);

	float mr = r;
	float mg = g;
	float mb = b;
	if (current.lightLevel != 0.0f) {
		mr = (r + current.r) / 2.0f;
		mg = (g + current.g) / 2.0f;
		mb = (b + current.b) / 2.0f;
	}

	if (lightLevel <= getTile(currentX, currentY).lightLevel) return;

	tiles[currentX][currentY].r = mr;
	tiles[currentX][currentY].g = mg;
	tiles[currentX][currentY].b = mb;

	

	tiles[currentX][currentY].lightLevel = lightLevel;
	//setColor(currentX, currentY, r * lightLevel, g * lightLevel, b * lightLevel);

	lightLevel -= 0.1f;

	if (lightLevel < 0.0f) {
		lightLevel = 0.0f;
		return;
	}

	if (directions.left) {
		updateLight(currentX - 1, currentY, lightLevel, r, g, b, { true, false, true, true }); // l r up down
	}

	if (directions.right) {
		updateLight(currentX + 1, currentY, lightLevel, r, g, b, { false, true, true, true }); // l r up down
	}

	if (directions.up) {
		updateLight(currentX, currentY - 1, lightLevel, r, g, b, { true, true, true, false }); // l r up down
	}

	if (directions.down) {
		updateLight(currentX, currentY + 1, lightLevel, r, g, b, { true, true, false, true }); // l r up down
	}
}

bool World::setMaterialAtMouse(int materialID) {
	int tileX = floor(game->mouseGameX / tileSize);
	int tileY = floor(game->mouseGameY / tileSize);
	int material = getTile(tileX, tileY).id;
		//setColor(tileX, tileY, 0.0f, 1.0f, 0.0f);
		bool result = setMaterial(tileX, tileY, materialID);
		updateLighting();
		return result;
}

World::MaterialData World::getMaterial(int x, int y) {
	if (x > 0 && y > 0 && x < width * 32 && y < height * 32) {
		return materialData[tiles[x][y].id];
	}
	else {
		return materialData[0]; // return air // maybe return -1 instead?
	}
}

World::Tile World::getTile(int x, int y) {
	if (x >= 0 && x < width * 32 && y >= 0 && y < height * 32) {
		return tiles[x][y];
	}
	else {
		Tile tile;
		return tile;
	}
}

bool World::setMaterial(int x, int y, int materialID) {
	if (x >= 0 && x < width * 32 && y >= 0 && y < height * 32) {
		int chunkX = floor(x / 32); // should automatically be rounded down
		int chunkY = floor(y / 32);

		int index = chunkX + (chunkY * width);

		if (activeChunks.find(index) != activeChunks.end()) {
			activeChunks.at(index)->setMaterial(x % 32, y % 32, materialID);
		}
		else {
			tiles[x][y].id = materialID;
		}

		return true;
	}
	else {
		return false;
	}


}

bool World::setColor(int x, int y, float r, float g, float b) {
	if (x >= 0 && x < width * 32 && y >= 0 && y < height * 32) {
		int chunkX = floor(x / 32); // should automatically be rounded down
		int chunkY = floor(y / 32);

		int index = chunkX + (chunkY * width);

		if (activeChunks.find(index) != activeChunks.end()) {
			// calculate the RGB given the lightlevel (aka the V value of HSV)..
			activeChunks.at(index)->setColor(x % 32, y % 32, r, g, b);
		}
		else {
			tiles[x][y].r = r;
			tiles[x][y].g = g;
			tiles[x][y].b = b;
		}

		return true;
	}
	else {
		return false;
	}
}


void World::addObject(Entity* obj) {
	count++;
	obj->id = count;
	entities.push_back(obj);
}

void World::createParticleEffect(Renderer::Color color, int amount, float x, float y, float vx, float vy, float size) {
	int count = 0;
	for (Entity* ent : particleEffects) {
		EntityParticle* e = (EntityParticle*)ent;
		if (count >= amount) break;

		if (!e->isActive()) {
			int magnitude = sqrt((vx * vx) + (vy * vy));
			e->setActive(true);
			e->setColor(color);
			e->setPosition(x, y);
			e->setVelocity(vx / magnitude * ((rand() % 300) / 100.0f) + ((rand() % 100) / 100.0f), vy / magnitude * ((rand() % 300) / 100.0f) + ((rand() % 100) / 100.0f));
			e->fadeOut(60);
			e->setSize(size);
			count++;
		}
	}
}