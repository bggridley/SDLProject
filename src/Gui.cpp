#include "Gui.h"

void Gui::input(SDL_Event* event) {

	if (event->type == SDL_MOUSEBUTTONDOWN ) {
		for (GuiButton* button : guiButtons) {
			if (button->selected) {
				Mix_PlayChannel(-1, clickSound, 0);
				button->action();
				break;
			}
		}
	}
}

void Gui::update(float delta) {
	for (GuiButton* button : guiButtons) {
		if (button->centered) {
			button->x = game->getWidth() / 2;
		}


		float width = guiButton.getWidth();
		float height = guiButton.getHeight();
		button->selected = (game->mouseX >= button->x - (width * button->scale / 2) && game->mouseX <= button->x + (width * button->scale / 2) && game->mouseY >= button->y - (height * button->scale / 2) && game->mouseY <= button->y + (height * button->scale / 2));
	}
}

void Gui::render(float alpha) {
	for (GuiButton* button : guiButtons) {
		//float buttonWidth = game->getFont().getWidth(button.text) * button.scale

		float width = guiButton.getWidth();
		float height = guiButton.getHeight();
		//guiButton.render(alpha, button.x - guiButton.getWidth() / 2, button.y)
		game->getRenderer().setScale(button->scale);
		game->getRenderer().setColor(button->selected ? button->highlighted : button->normal);
		game->getRenderer().drawQuad(button->x - (width * button->scale / 2), button->y - (height * button->scale / 2), width, height, false);
		game->getRenderer().drawTexturedQuad(guiButton, button->x - (width * button->scale/ 2), button->y - (height * button->scale / 2));
		game->getFont().setScale(button->scale);
		game->getFont().renderCenteredString(button->x, button->y, button->text);

		//std::cout << button->y << std::endl;
		//std::cout << button->x << ", " << (button->x - (width * button->scale / 2)) << std::endl;
	}
}