#pragma once 
#include <string>
#include "Game.h"
#include "glew.h"
#include "Texture.h"
#include <functional>
#include "SDL_mixer.h"

class Game;

class Gui {
public:
	Gui(Game* g) : game(g) {
		guiButton = game->getResources().guiButton;
		clickSound = game->getResources().s_click;

	}
	// base class will define this} 
	void input(SDL_Event* event);
	virtual void update(float delta) = 0;
	virtual void render(float alpha) = 0;
	~Gui() {
		Mix_FreeChunk(clickSound);
	}
protected:
	Game* game;
	class GuiButton {
	public:
		bool selected = false;
		float scale;
		int x;
		int y;
		std::string text;
		bool centered;
		Renderer::Color normal;
		Renderer::Color highlighted;
		std::function<void()> action;
		std::function<void()> update;
		const int height = 32;
		const int width = 128;
	};

	Texture guiButton;
	std::vector<GuiButton*> guiButtons;
	Mix_Chunk* clickSound;
};