#pragma once
#include "Texture.h"
#include "TextureAtlas.h"
#include "glew.h"
#include "sdl.h"
#include "sdl_image.h"
#include "sdl_mixer.h"
#include <string>

class Resources {

public:
	Texture tiles;
	Texture player;
	Texture items;
	Texture font;
	Texture guiButton;
	Texture inventory;
	TextureAtlas tileAtlas; // these all have to be 
	TextureAtlas playerAtlas;
	TextureAtlas itemAtlas;
	Mix_Chunk* s_click;
	Mix_Chunk* s_zap;

	Resources() {

	}

	void init() {
		inventory = loadTexture("res/inventory.png", GL_NEAREST);
		tiles = loadTexture("res/stone.png", GL_LINEAR);
		player = loadTexture("res/player.png", GL_NEAREST); // might look bette rwith nearest
		items = loadTexture("res/items.png", GL_NEAREST);
		font = loadTexture("res/font.png", GL_NEAREST);
		guiButton = loadTexture("res/guibutton.png", GL_NEAREST);
		s_click = Mix_LoadWAV("res/click.ogg");;
		s_zap = Mix_LoadWAV("res/zaps.ogg");;


		tileAtlas.init(tiles, 16, 16, 32, 32, 2);
		itemAtlas.init(items, 16, 16, 32, 32, 2);
		playerAtlas.init(player, 4, 4, 64, 64, 0);
	}


private:
	Texture loadTexture(std::string path, GLint param) {
		GLuint texID = 0;
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);

		SDL_Surface* loadedSurface = IMG_Load(path.c_str());
		GLint mode = GL_RGBA;

		int width = loadedSurface->w;
		int height = loadedSurface->h;

		glEnable(GL_TEXTURE_2D);
		glTexImage2D(GL_TEXTURE_2D, 0, mode, loadedSurface->w, loadedSurface->h, 0, mode, GL_UNSIGNED_BYTE, loadedSurface->pixels);



		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//glGenerateMipmap(GL_TEXTURE_2D);



		glDisable(GL_TEXTURE_2D);

		SDL_FreeSurface(loadedSurface);
		Texture tex;
		tex.init(texID, width, height);
		return tex;
	}

};