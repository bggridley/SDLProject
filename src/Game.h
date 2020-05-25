#pragma once
#include <SDL.h>
#include <glew.h>
#include <SDL_image.h>
#include <iostream>
#include "World.h"
#include "BitmapFont.h"
#include "Texture.h"
#include "Renderer.h"
#include "Resources.h"

//class Item; // we're not going to save explicit different items so we don't need multiple forward declarations like with GuiPaused et
class Item;
class Inventory;
class World;
class GuiMainMenu;
class GuiPaused;
class Gui;

class Game {
public:
	Game();
	~Game();
	
	GuiMainMenu* mainMenu;
	GuiPaused* guiPaused;

	Texture loadTexture(std::string path, GLint param);
	SDL_Window* window;
	World* world;
	Gui* currentGui = nullptr;
	Resources resources;


	void init(); // this is necessary so that textures can be created
	void input(SDL_Event* event);
	void update(float delta, int updateCount, int frameCount);
	int updates;
	void render(float alpha);
	void setDimensions(float width, float height);
	void setFullscreenDimensions(float width, float height);
	void setState(int state);
	void setCurrentGui(Gui* gu);
	float getWidth();
	float getHeight();
	float getScale();
	BitmapFont getFont();
	Renderer getRenderer();
	Resources getResources();
	Inventory* getInventory();

	enum GameState {
		MAIN_MENU,
		INGAME
	};


	float mouseGameX;
	float mouseGameY;
	int mouseX;
	int mouseY;
	bool zoomIn = false;
	bool zoomOut = false;
	bool mousePressed = false;
private:
	Inventory* inventory;
	Renderer renderer;
	BitmapFont font;
	int state;
	int fps;


	float scale = 1.0f;
	float width;
	float height;
	float fullscreenWidth;
	float fullscreenHeight;

	

	
	bool vsync = true;
	bool fullscreen = false;

};