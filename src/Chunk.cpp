#include "Chunk.h"
#include <iostream>

Chunk::Chunk() {
	vID = 0;
	tID = 0;
	cID = 0;
}

Chunk::~Chunk() {
	if (vID != 0) {
		glDeleteBuffers(1, &vID); // delete buffer
	}

	if (tID != 0) {
		glDeleteBuffers(1, &tID); // delete buffer; if tihis is not done, weird things can happen
	}

	if (cID != 0) {
		glDeleteBuffers(1, &cID); // delete buffer; if tihis is not done, weird things can happen
	}
}

// xoffset and y offset are world coordinates
void Chunk::init(World* world, float xOffset, float yOffset) {
	this->x = xOffset;
	this->y = yOffset;
	this->width = width;
	this->height = height;
	this->world = world;

	atlas = world->getGame()->getResources().tileAtlas;
	// tex is going to be the "Texture atlas" in which i will 

	if (vID == 0) {
		glGenBuffers(1, &vID);
	}

	if (tID == 0) {
		glGenBuffers(1, &tID);
	}

	if (cID == 0) {
		glGenBuffers(1, &cID);
	}

	float vertexData[12 * 32 * 32]; // 12 per tile * amount of tiles
	float textureData[12 * 32 * 32]; // maybe i should index each tile to save the gpu some space and just put this on the cpu one time per chunk
	float colorData[18 * 32 * 32]; // 18 here cus there's 3 colors per vertice, not 2
	
	// do this up here so the individual sections can be set from within the for loop with setMaterial

	for (int i = 0; i < 32; i++) {
		for (int j = 0; j < 32; j++) {

			//int materialID = (rand() % 3) + 1; // so no air

			//if (i < 3 || j < 3) {
			//	materialID = 0;
			//}

			//if (materialID == 2) {
			//	materialID = 0;
			//}

			// index = NUM_COLS*x + y;

			int index = i + (j * 32);
			int offset = index * 12;

			float xo = i * tileSize;
			float yo = j * tileSize;
			vertexData[0 + (offset)] = xo + x + tileSize;
			vertexData[1 + offset] = yo + y + tileSize;
			vertexData[2 + offset] = xo + x;
			vertexData[3 + offset] = yo + y + tileSize;
			vertexData[4 + offset] = xo + x;
			vertexData[5 + offset] = yo + y;
			vertexData[6 + offset] = xo + x;
			vertexData[7 + offset] = yo + y;
			vertexData[8 + offset] = xo + x + tileSize;
			vertexData[9 + offset] = yo + y;
			vertexData[10 + offset] = xo + x + tileSize;
			vertexData[11 + offset] = yo + y + tileSize;

			// change material id into Xpos and Ypos of the TEXTURE

			int materialID = world->tiles[int(this->x / tileSize) + i][int(this->y / tileSize) + j].id;

			// hope this works lmfao?
			TextureAtlas::UVTexCoords coords = atlas.getCoords(materialID);// .at(materialID);

			//setMaterial(i, j, materialID);
			fillTexCoords(textureData, coords, offset);


			// the offset was 12 for the other two because 6 * 2 (Coordinates per vertice) = 12.
			// there are 3 color values (r,g,b) per vertice so it will be 18.
			offset = index * 18;

			fillColorCoords(colorData, 0.f, 0.f, 0.f, offset);


			//world->tiles[int(this->x / tileSize) + i][int(this->y / tileSize) + j].id = materialID;
			// draw a square
		}
	}
	

	// store UV coordinates in a map to change what tex will be mapped to erach vertex
	// 12 per tile




	glBindBuffer(GL_ARRAY_BUFFER, vID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, tID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureData), textureData, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, cID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_DYNAMIC_DRAW);


	glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind the buffer so it can be rebound later for drawing
}

void Chunk::fillColorCoords(float* colorData, float r, float g, float b, int offset) {
	for (int i = 0; i < 18 / 3; i++) {
		colorData[i * 3 + offset] = r; // or whatever the default lighting value should be.
		colorData[i * 3 + 1 + offset] = g; // or whatever the default lighting value should be.
		colorData[i * 3 + 2 + offset] = b; // or whatever the default lighting value should be.
	}
}

void Chunk::fillTexCoords(float* textureData, TextureAtlas::UVTexCoords coords, int offset) {

	textureData[0 + offset] = coords.xmax; // x max
	textureData[1 + offset] = coords.ymax; // y max
	textureData[2 + offset] = coords.xmin; // x min
	textureData[3 + offset] = coords.ymax; // y max
	textureData[4 + offset] = coords.xmin; // x min
	textureData[5 + offset] = coords.ymin; // y min
	textureData[6 + offset] = coords.xmin; // x min
	textureData[7 + offset] = coords.ymin; // y min
	textureData[8 + offset] = coords.xmax; // x max
	textureData[9 + offset] = coords.ymin; // y min
	textureData[10 + offset] = coords.xmax; // x max
	textureData[11 + offset] = coords.ymax; // y max
}

// x and y are between 0 and 31
void Chunk::setMaterial(int x, int y, int id) {
	float textureData[12]; // 12 vertices ya hurd
	int index = x + (y * 32);

	// 12 vertice points per tile
	// * sizeof(float)
	int offset = index * 12 * sizeof(float);

	fillTexCoords(textureData, atlas.getCoords(id), 0);

	glBindBuffer(GL_ARRAY_BUFFER, tID);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(textureData), textureData);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	world->tiles[int(this->x / tileSize) + x][int(this->y / tileSize) + y].id = id; // hope this works lmfao?
}

void Chunk::setColor(int x, int y, float r, float g, float b) {
	//float colorData[18]; // 18 vertices ya hurd
	int index = x + (y * 32);

	// 12 vertice points per tile
	// * sizeof(float)
	int offset = index * 18; //* sizeof(float);

	//fillColorCoords(colorData, r, g, b, offset);

	world->tiles[int(this->x / tileSize) + x][int(this->y / tileSize) + y].r = r; // hope this works lmfao?
	world->tiles[int(this->x / tileSize) + x][int(this->y / tileSize) + y].g = g; // hope this works lmfao?
	world->tiles[int(this->x / tileSize) + x][int(this->y / tileSize) + y].b = b; // hope this works lmfao?
}
 // push colors to the GPU
void Chunk::pushColors() {
	float colorData[18 * 32 * 32];
	for (int x = 0; x < 32; x++) {
		for (int y = 0; y < 32; y++) {
			int index = x + (y * 32);
			World::Tile tile = world->tiles[int(this->x / tileSize) + x][int(this->y / tileSize) + y];
			float r = tile.r * tile.lightLevel;
			float g = tile.g * tile.lightLevel;
			float b = tile.b * tile.lightLevel;
			fillColorCoords(colorData, r, g, b, index * 18);
			// fill up le data.
		}
	}

	//update the entire VBO one time.

	glBindBuffer(GL_ARRAY_BUFFER, cID);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(colorData), colorData);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Chunk::render() const {
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	glBindBuffer(GL_ARRAY_BUFFER, tID);

	glBindTexture(GL_TEXTURE_2D, atlas.getTexture().getTexID());
	glTexCoordPointer(2, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vID); 
	//glEnableVertexAttribArray(0); // attribute 1 aka position
	glVertexPointer(2, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, cID);
	glColorPointer(3, GL_FLOAT, 0, 0);

	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0); this also works perfectly fine

	//glBindBuffer(GL_ARRAY_BUFFER, vID);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glColor3f(0.1f, 0.1f, 0.1f);
	glDrawArrays(GL_TRIANGLES, 0, 6 * 32 * 32);
	//glDisableVertexAttribArray(0); // attribute

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}

const int Chunk::getX() const {
	return this->x;
}

const int Chunk::getY() const {
	return this->y;
}
