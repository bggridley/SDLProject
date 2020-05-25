#pragma once
#include "Gui.h"
#include "Game.h"

class Game;

class GuiPaused : public Gui {
public:
	GuiButton back;
	GuiButton mainMenu;
	GuiPaused(Game* g);
	void render(float alpha);
	void update(float delta);
};
