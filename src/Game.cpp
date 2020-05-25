#include "Game.h"
#include "EntityBullet.h"
#include "Gui.h"
#include "GuiMainMenu.h"
#include "GuiPaused.h"
#include "Inventory.h"
#include "Item.h"

Game::Game() {
}

void Game::init() {
	resources.init();
	font.init("res/font.fnt", resources.font.getTexID());
	world = new World(this, 80, 30);
	world->init(); // spawn in player, etc.
	setState(GameState::MAIN_MENU);
	mainMenu = new GuiMainMenu(this);
	guiPaused = new GuiPaused(this);
	this->currentGui = mainMenu;
	this->inventory = new Inventory(this);
}

void Game::input(SDL_Event* event) {
	if (event->type == SDL_KEYDOWN) {
		switch (event->key.keysym.sym) {
		case SDLK_a:
			world->left = true;
			break;
		case SDLK_d:
			world->right = true;
			break;
		case SDLK_w:
			world->up = true;
			break;
		case SDLK_s:
			world->down = true;
			break;
		case SDLK_EQUALS:
			zoomIn = true;
			break;
		case SDLK_MINUS:
			zoomOut = true;
			break;
		}
	}

	if (event->type == SDL_KEYUP) {
		switch (event->key.keysym.sym) {
		case SDLK_a:
			world->left = false;
			break;
		case SDLK_d:
			world->right = false;
			break;
		case SDLK_w:
			world->up = false;
			break;
		case SDLK_s:
			world->down = false;
			break;
		case SDLK_v:
			vsync = !vsync;
			SDL_GL_SetSwapInterval(vsync);
			break;
		case SDLK_f:
			fullscreen = !fullscreen;
			if (fullscreen) {
				SDL_SetWindowSize(window, fullscreenWidth, fullscreenHeight);
				SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
			}
			else {
				SDL_SetWindowSize(window, width, height);
				SDL_SetWindowFullscreen(window, 0);
			}

			glLoadIdentity();
			glMatrixMode(GL_PROJECTION);
			glViewport(0, 0, getWidth(), getHeight());
			//glViewport(0, 0, getWidth(), getHeight());
			glOrtho(0, getWidth(), getHeight(), 0, -1, 1);

			break;
		case SDLK_EQUALS:
			zoomIn = false;
			break;
		case SDLK_MINUS:
			zoomOut = false;
			break;
		case SDLK_ESCAPE:
			if (state != GameState::INGAME) break;
			if (this->currentGui != guiPaused) {
				this->setCurrentGui(guiPaused);
			}
			else {
				this->currentGui = nullptr;
			}
			break;
		case SDLK_TAB:
			inventory->toggle();
			break;

		}
	}

	if (event->type == SDL_MOUSEBUTTONDOWN) {
		mousePressed = true;


		int tileX = mouseGameX / 32;
		int tileY = mouseGameY / 32;
		//	std::cout << tileX << ", " << tileY << std::endl;

		//	std::cout << mouseGameX
	}

	if (event->type == SDL_MOUSEMOTION) {
		int mouseX;
		int mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);

		this->mouseX = mouseX;
		this->mouseY = mouseY;
	}

	if (event->type == SDL_MOUSEBUTTONUP) {
		mousePressed = false;

		// really should just add a world call to input
		if (false) {
			EntityBullet* launch = new EntityBullet(this->world, 200.0f);
			launch->setPosition(world->getPlayer()->getX() + world->getPlayer()->getWidth() / 2, world->getPlayer()->getY() - 10);

			float vx = (mouseX - (getWidth() / 2)) / 10.0f;
			float vy = (mouseY - ((getHeight() / 2) - (world->getPlayer()->getHeight() / 2) - 10)) / 10.0f;

			//std::cout << vx << ", " << vy << std::endl;


			launch->setVelocity(world->getPlayer()->getVelocityX() + vx, world->getPlayer()->getVelocityY() + vy);

			//world->getPlayer()->fadeOut(120);
		}
	}

	if (state == GameState::INGAME) {
		inventory->input(event);
	}


	if (currentGui != nullptr) {
		currentGui->input(event);
	}
}

void Game::update(float delta, int updateCount, int fps) {
	this->fps = fps;
	this->updates = updateCount;
	if (currentGui != nullptr) {
		currentGui->update(delta);
	}

	if (state == GameState::MAIN_MENU) {



	}

	else if (state == GameState::INGAME) {
		int midX = getWidth() / 2;
		int midY = getHeight() / 2;

		if (zoomOut) {
			scale -= 0.1 * delta;

			if (scale < 0.85f) {
				scale = 0.85f;
			}
		}

		if (zoomIn) {
			scale += 0.1 * delta;

			if (scale > 2.0f) {
				scale = 2.0f;
			}

		}

		this->mouseGameX = world->getPlayer()->getX() + world->getPlayer()->getWidth() / scale / 2 - (midX - mouseX) / scale;// (world->getPlayer()->getX() + (world->getPlayer()->getWidth() / 2));
		this->mouseGameY = world->getPlayer()->getY() + world->getPlayer()->getHeight() / scale / 2 - (midY - mouseY) / scale;// (world->getPlayer()->getX() + (world->getPlayer()->getWidth() / 2));
		world->update(delta, updateCount);
		inventory->update();
	}
}



void Game::render(float alpha) {
	std::string framespersec = "FPS:" + std::to_string(fps);



	if (state == GameState::MAIN_MENU) {
		font.setScale(1.0f);
		font.setColor(1.0f, 1.0f, 1.0f, 1.0f);
		font.renderString(0, 0, framespersec);
	}
	else if (state == GameState::INGAME) {
		world->getPlayer()->lerpTranslation(alpha, getScale()); // still in the projection matrix
		world->render(alpha);

		font.setScale(1.0f);
		font.setColor((rand() % 255) / 255.0f, (rand() % 255) / 255.0f, (rand() % 255) / 255.0f, 1.0f);

		font.renderCenteredString(world->getPlayer()->getLerpX(alpha) + world->getPlayer()->getWidth() / 2, world->getPlayer()->getLerpY(alpha) - 30.0f, "hello");

		std::string activechunks = "CHUNKS: " + std::to_string(world->getActiveChunks().size());
		std::string coords = "XY: " + std::to_string((int)world->getPlayer()->getX() / 32) + ", " + std::to_string(((world->getHeight() * 32) - (int)world->getPlayer()->getY() / 32));
		std::string entities = "Entities: " + std::to_string(world->getEntityCount());
		glPopMatrix();
		font.setScale(1.0f);
		font.setColor(1.0f, 1.0f, 1.0f, 1.0f);
		font.renderString(0, 0, framespersec);
		//std::string string = "hey " + std::to_string((int)testX);
	// this render string could be used to display anything above any player in the world
		font.renderString(0, 32, activechunks);
		font.renderString(0, 64, coords);
		font.renderString(0, 96, entities);




		

		inventory->render();
		glPushMatrix();
	}

	if (currentGui != nullptr) {
		//glPushMatrix();
		currentGui->render(alpha);
		//	glPopMatrix(); // so the world rendering isn't fucked up
	}
}

void Game::setDimensions(float width, float height) {
	this->height = height;
	this->width = width;
}

void Game::setFullscreenDimensions(float width, float height) {
	this->fullscreenWidth = width;
	this->fullscreenHeight = height;
}

float Game::getWidth() {
	return fullscreen ? fullscreenWidth : width;
}

float Game::getHeight() {
	return fullscreen ? fullscreenHeight : height;
}

float Game::getScale() {
	return scale;
}

BitmapFont Game::getFont() {
	return font;
}

Renderer Game::getRenderer() {
	return renderer;
}

void Game::setState(int state) {
	this->state = state;
	this->currentGui = nullptr;
}

Resources Game::getResources() {
	return resources;
}

void Game::setCurrentGui(Gui* gui) {
	this->currentGui = gui;
}

Inventory* Game::getInventory() {
	return inventory;
}