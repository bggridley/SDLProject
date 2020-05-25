#pragma once
#include "Gui.h"
#include "Game.h"

class Game;

class GuiMainMenu : public Gui {
public:
	GuiMainMenu(Game* g);
	void render(float alpha);
	void update(float delta);
	GuiButton button;
};
