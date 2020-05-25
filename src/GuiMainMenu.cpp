#include "GuiMainMenu.h"

GuiMainMenu::GuiMainMenu(Game* g) : Gui(g) {
	button.text = "Hello";
	button.centered = true;
	button.normal = { 1.0, 0.0, 0.0, 0.5f };
	button.highlighted = { 1.0, 1.0, 0.0, 0.5f };
	button.scale = 2.0f;
	button.action = [&]() {
		

	game->currentGui = game->mainMenu;
	game->setState(Game::GameState::INGAME);
	};

	guiButtons.push_back(&button);
}
void GuiMainMenu::render(float alpha) {
	Gui::render(alpha);
}

void GuiMainMenu::update(float delta) {
	Gui::update(delta);

	button.y = (game->getHeight() / 2);
}