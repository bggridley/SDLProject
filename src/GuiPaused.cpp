#include "GuiPaused.h"

GuiPaused::GuiPaused(Game* g) : Gui(g) {
	back.text = "Back";
	back.centered = true;
	back.normal = { 1.0, 0.0, 0.0, 0.5f };
	back.highlighted = { 1.0, 1.0, 0.0, 0.5f };
	back.scale = 2.0f;
	back.y = game->getHeight() / 2;
	back.action = [&]() {
		

	game->setState(Game::GameState::MAIN_MENU);
	game->setCurrentGui((Gui*) game->mainMenu);
	};

	guiButtons.push_back(&back);
}
void GuiPaused::render(float alpha) {
	Gui::render(alpha);
}

void GuiPaused::update(float delta) {
	Gui::update(delta);
}