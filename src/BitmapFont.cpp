#include "BitmapFont.h"

// this whole function was adapted from https://github.com/iBecameALoaf/Trilandia/blob/master/src/com/loafy/game/gfx/Font.java
// ^ i made that in 8th grade?

float BitmapFont::scale;

void BitmapFont::init(std::string fileName, GLuint texID) {
	this->texID = texID;
	float imageWidth = 256;
	float imageHeight = 256; //theoretically, i could just put this in the file too but whatever... maybe later

	std::ifstream input(fileName);
	for (std::string line; getline(input, line);)
	{
		Glyph g; // one glyph per line
		std::vector<std::string> tokens = tokenizer(line, ' ');
		for (std::string str : tokens) {
			// remove extra space
			str.erase(remove_if(str.begin(), str.end(), isspace), str.end());

			if (str == "") {
				continue;
			}
			
			std::vector<std::string> pairs = tokenizer(str, '=');

			
			std::string key = pairs[0];
			int value = std::stoi(pairs[1]);
		

			// i HATE doing this... why does c++ make me do this...... why no switch statement
			// it feels so hardcoded
			if (key == "id") {
				g.id = value;
			}
			else if (key == "x") {
				g.x = value;
			}
			else if (key == "y") {
				g.y = value;
			}
			else if (key == "width") {
				g.width = value;
			}
			else if (key == "height") {
				g.height = value;
			}
			else if (key == "xoffset") {
				g.xoffset = value;
			}
			else if (key == "yoffset") {
				g.yoffset = value;
			}
			else if (key == "xadvance") {
				g.xadvance = value;
			}
		}
		
		g.u = g.x / imageWidth;
		g.v = g.y / imageHeight;
		g.u2 = g.u + (float)g.width / imageWidth;
		g.v2 = g.v + (float) g.height / imageHeight;
		glyphs[g.id] = g;
		//std::cout << g.id << ", " << g.x << ", " << g.y << std::endl;
	}
}

void BitmapFont::renderString(float x, float y, std::string string) {
	glBindTexture(GL_TEXTURE_2D, texID);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);

	for (int i = 0; i < string.length(); i++) {
		char c = string.at(i);


		int a = c; // char as an ID, which can be got from the list of glyphs
		Glyph g = glyphs[a];
		

		float ww = g.width * scale;
		float hh = g.height * scale;

		float xx = x + g.xoffset * scale;
		float yy = y + g.yoffset * scale;

		
		glColor4f(color.r, color.g, color.b, color.a);

		glTexCoord2f(g.u, g.v);
		glVertex2f(xx, yy);

		//std::cout << g.id << ", " << g.u << ", " << g.v << ", " << g.u2 << ", " << g.v2 << ", " << std::endl;
		glTexCoord2f(g.u2, g.v);
		glVertex2f(xx + ww, yy);

		glTexCoord2f(g.u2, g.v2);
		glVertex2f(xx + ww, yy + hh);

		glTexCoord2f(g.u, g.v2);
		glVertex2f(xx, yy + hh);

		glColor4f(1.0, 1.0, 1.0, 1.0f); // could make it something else maybe glPushAttrib to push/pop color just need to find out what's more efficient

		x += g.xadvance * scale;
	}

	glEnd();

	// we could use VBO for static strings most likely
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	//glPopMatrix(); // return to the old matrix that is translated
}

void BitmapFont::renderCenteredString(float x, float y, std::string string) {
	x -= getWidth(string) / 2;
	y -= getHeight(string) / 2;
	//std::cout << "subtracting" << getHeight(string) / 2;;
	renderString(x, y, string);
}

// depreacated, probably dose more harm than good
void BitmapFont::renderStaticString(float x, float y, std::string string, bool worldCoords) {
	if (staticStrings.find(string) == staticStrings.end()) {

		StaticString s;
		s.length = string.length();
		s.originalX = x;
		s.originalY = y;

		if (s.vID == 0) {
			glGenBuffers(1, &s.vID);
		}

		if (s.tID == 0) {
			glGenBuffers(1, &s.tID);
		}
	
		std::cout << "this should only happen once" << std::endl;


		for (int i = 0; i < string.length(); i++) {
			char c = string.at(i);


			int a = c; // char as an ID, which can be got from the list of glyphs
			Glyph g = glyphs[a];


			float ww = g.width;
			float hh = g.height;

			float xx = x + g.xoffset;
			float yy = y + g.yoffset;

			// 12 vertices per each singular glyph/char
			s.vertexData.push_back(xx + ww); // top right
			s.vertexData.push_back(yy + hh);

			s.textureData.push_back(g.u2);
			s.textureData.push_back(g.v2);

			s.vertexData.push_back(xx); // top left
			s.vertexData.push_back(yy + hh);

			s.textureData.push_back(g.u);
			s.textureData.push_back(g.v2);

			s.vertexData.push_back(xx); // bottom left
			s.vertexData.push_back(yy);

			s.textureData.push_back(g.u);
			s.textureData.push_back(g.v);

			s.vertexData.push_back(xx); // bottom left
			s.vertexData.push_back(yy);

			s.textureData.push_back(g.u);
			s.textureData.push_back(g.v);

			s.vertexData.push_back(xx + ww); // bottom right
			s.vertexData.push_back(yy);

			s.textureData.push_back(g.u2);
			s.textureData.push_back(g.v);

			s.vertexData.push_back(xx + ww); // top right again
			s.vertexData.push_back(yy + hh);

			s.textureData.push_back(g.u2);
			s.textureData.push_back(g.v2);

			x += g.xadvance;
		}

		// 6 vertices per string thing.
		
			glBindBuffer(GL_ARRAY_BUFFER, s.vID);
			glBufferData(GL_ARRAY_BUFFER, s.vertexData.size() * sizeof(float) * 2, &s.vertexData.front(), GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, s.tID);
			glBufferData(GL_ARRAY_BUFFER, s.textureData.size() * sizeof(float) * 2, &s.textureData.front(), GL_DYNAMIC_DRAW);
		

			staticStrings.emplace(string, s);

			glBindBuffer(GL_ARRAY_BUFFER, 0);

			renderStaticString(s.originalX, s.originalY, string, worldCoords);
	}
	else {
		// render and find the shit

		StaticString s = staticStrings.at(string);

		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		if (!worldCoords)
			glPopMatrix();

		glBindBuffer(GL_ARRAY_BUFFER, s.tID);

		glBindTexture(GL_TEXTURE_2D, texID);
		glTexCoordPointer(2, GL_FLOAT, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, s.vID);
		//glEnableVertexAttribArray(0); // attribute 1 aka position
		glVertexPointer(2, GL_FLOAT, 0, 0);

		//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0); this also works perfectly fine

		//glBindBuffer(GL_ARRAY_BUFFER, vID);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glTranslatef(x - s.originalX, y - s.originalY, 0);

		//glColor3f(0.1f, 0.1f, 0.1f);
		glDrawArrays(GL_TRIANGLES, 0, 6 * s.length);

		glTranslatef(-(x - s.originalX), -(y - s.originalY), 0);
		//glDisableVertexAttribArray(0); // attribute

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		if (!worldCoords) {
			glPushMatrix();
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
	}
}

float BitmapFont::getWidth(std::string string) {
	float width = 0.0f;

	for (int i = 0; i < string.length(); i++) {
		char c = string.at(i);


		int a = c; // char as an ID, which can be got from the list of glyphs
		Glyph g = glyphs[a];

		width += g.xadvance * scale;
	}

	return width;
}

/*
Make sure font scale is set properly when calling this method
*/
float BitmapFont::getHeight(std::string string) {
	float smallestyo = 500; // the smallest y offset of the entire group of glyphs
	float biggesttotal = -500; // the smallest y offset of the entire group of glyphs

	for (int i = 0; i < string.length(); i++) {
		char c = string.at(i);


		int a = c; // char as an ID, which can be got from the list of glyphs
		Glyph g = glyphs[a];
		if (g.height + g.yoffset > biggesttotal) {
			biggesttotal = g.height + g.yoffset;
		}

		if (g.yoffset < smallestyo) {
			smallestyo = g.yoffset;
		}
	}

	return (smallestyo + biggesttotal) * scale;
}


//copy pa sted from stack exchagne; why is c++ so complicated?
std::vector<std::string> BitmapFont::tokenizer(const std::string& p_pcstStr, char delim) {
	std::vector<std::string> tokens;
	std::stringstream   mySstream(p_pcstStr);
	std::string         temp;

	while (getline(mySstream, temp, delim)) {
		tokens.push_back(temp);
	}

	return tokens;
}

BitmapFont::~BitmapFont() {
	for (const auto& values : staticStrings) {
		if (values.second.vID != 0) {
			glDeleteBuffers(1, &values.second.vID);
		}

		if (values.second.tID != 0) {
			glDeleteBuffers(1, &values.second.tID);
		}
	}
}