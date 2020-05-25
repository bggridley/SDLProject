#pragma once
#include "glew.h"
#include "Texture.h"
#

class Renderer {

public:
	Renderer() {
		scale = 1.0f;
		currentColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	}
	// direct rendering only, used mostly for GUI's
	//todo EASILY create gradients
	
	struct Color {
		float r;
		float g;
		float b;
		float a;
	};

	void setColor(Color c) {
		currentColor = c;
	}

	Color getColor() {
		return currentColor;
	}

	void setScale(float s) {
		this->scale = s;
	}

	float getScale() {
		return scale;
	}

	void setWorld(bool w) {
		this->world = w;
	}


	void drawQuad(float x, float y, float width, float height, bool wireframe) {
		if (!world) {
			glPopMatrix();
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(currentColor.r, currentColor.g, currentColor.b, currentColor.a);
		glBegin(wireframe ? GL_LINE_LOOP : GL_QUADS);
		glVertex2f(x, y);
		glVertex2f(x + (width * scale), y);
		glVertex2f(x + (width * scale), y + (height * scale));
		glVertex2f(x, y + (height * scale));
		glEnd();

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glDisable(GL_BLEND);

		if (!world) {
			glPushMatrix();
		}
	}

	void drawTexturedQuad(Texture t, float x, float y) {

		if (!world) {
			glPopMatrix();
		}

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, t.getTexID());
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);  glVertex2f(x, y);
		glTexCoord2f(1, 0); glVertex2f(x + (t.getWidth() * scale), y);
		glTexCoord2f(1.0, 1.0); glVertex2f(x + (t.getWidth() * scale), y + (t.getHeight() * scale));
		glTexCoord2f(0.0, 1.0);  glVertex2f(x, y + (t.getHeight() * scale));
		glEnd();
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		if (!world) {
			glPushMatrix();
		}
	}

	//cwidth and cheight are min 0 max textureWidth and textureHeight

	void drawTexturedQuad(Texture t, float x, float y, float cx, float cy, float cwidth, float cheight) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, t.getTexID());
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBegin(GL_QUADS);
		glTexCoord2f(cx/t.getWidth(), cy/t.getHeight());  glVertex2f(x, y);
		glTexCoord2f((cx + cwidth) / t.getWidth(), cy / t.getHeight()); glVertex2f(x + (cwidth * scale), y);
		glTexCoord2f((cx + cwidth) / t.getWidth(), (cy + cheight) / t.getHeight()); glVertex2f(x + (cwidth * scale), y + (cheight * scale));
		glTexCoord2f(cx / t.getWidth(), (cy + cheight) / t.getHeight());  glVertex2f(x, y + (cheight * scale));
		glEnd();
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

	}


private:
	static Color currentColor;
	static float scale;
	static bool world; // decides whether to pop/push
};
