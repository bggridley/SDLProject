#include "Entity.h"
#include <iostream>

Entity::Entity(World* wr) : world(wr) {
	//std::cout << "created world object!" << std::endl;

	wr->addObject(this);
}

void Entity::setPosition(float xpos, float ypos) {
	this->x = xpos;
	this->y = ypos;


	previousState.x = xpos;
	previousState.y = ypos;
	currentState.x = xpos;
	currentState.y = ypos;
}

void Entity::setRotation(float degs) {
	this->rotation = degs;
}

void Entity::setVelocity(float x, float y) {
	this->vx = x;
	this->vy = y;
}

void Entity::updateFade(float delta) {
	if (fading == 0 || fading == 1) {
		color.a += alphaPerTick;
		if (color.a >= 1.0f) {
			color.a = 1.0f;
			fading = -1;
		}
		else if (color.a <= 0.0f) {
			color.a = 0.0f;
			fading = -1;
		}
	}
}

void Entity::update(float delta, int updateCount) {
	updates++;
	previousState = currentState;

	animation.update(updateCount);

	if (gravity && active) {
		vy += 1.0 / 60.0f * 9.8f * 32 * delta; // 9.8 pixels per SECOND
	}

	float maxHeight = (world->height * 32.0f * world->tileSize);

	if (y + height > maxHeight) {
		y = maxHeight - height;

		vy = 0;
	}

	//if (vy > 0) falling = true;


	if (vy > maxSpeed) {
		vx *= maxSpeed / vy;
		vy = maxSpeed;
	}

	if (vy < -maxSpeed) {
		vx *= -maxSpeed / vy;
		vy = -maxSpeed;
	}

	if (vx > maxSpeed) {
		vy *= maxSpeed / vx;
		vx = maxSpeed;
	}

	if (vx < -maxSpeed) {
		vy *= -maxSpeed / vx;
		vx = -maxSpeed;
	}

	if (collisions) {
		checkCollisions();
	}

	x += vx;
	y += vy;

	currentState.x = x;
	currentState.y = y;
	//vy = 0;

	if (vx > 0) {
		animation.setFlipped(false);
	}
	else if (vx < 0) {
		animation.setFlipped(true);
	}
}


void Entity::checkCollisions() {
	falling = true;
	for (Entity* e : world->entities) {
		if (!active || !collisions || !e->active || !e->collisions || this->id == e->id) continue;

		//entityCollision(e); // maybe make an actual response thingie but that's ok for now.

	}

	for (int i = 0; i <= 5; i++) {
		float checkvx = (vx / 5) * i;
		float checkvy = (vy / 5) * i;



		//std::cout << checkvy << "/" << vy << " iter:" << i << std::endl;
		//std::cout << checkvx << "/" << vx << std::endl;
		std::vector<Collision> list = checkCollisions(checkvx, checkvy); // check the collision and call the reponse.
		//std::cout << "initial collision: " << c.tileX << ", " << c.tileY << std::endl;
		if (!list.empty()) {
			float colX = checkvx;
			float colY = checkvy;

			if (vx > 0) {
				colX += maxX;
			}

			if (vy > 0) {
				colY += maxY;
			}
			tileCollision(x + colX, y + colY);
			Collision biggestX = { -1, -1, -1, {0, 0} };
			Collision biggestY = { -1, -1, -1, {0, 0} };
			for (Collision c : list) {
				if (std::abs(c.axis.x) > std::abs(biggestX.axis.x)) {
					if ((c.axis.x < 0 && !world->getMaterial(c.tileX - 1, c.tileY).solid) || (c.axis.x > 0 && !world->getMaterial(c.tileX + 1, c.tileY).solid)) {
						biggestX = c;
					}
				}
				/*else {
					if (std::abs(c.axis.x) == std::abs(biggestX.axis.x)) {
						std::cout << c.axis.x << ", " << biggestX.axis.x << std::endl;
						if (vx < 0) {
							biggestX = (c.axis.x > 0) ? c : biggestX;
						}
						else if (vx > 0) {
							biggestX = (c.axis.x < 0) ? c : biggestX;
						}
					}
				}*/

				if (std::abs(c.axis.y) > std::abs(biggestY.axis.y)) {
					if ((c.axis.y < 0 && !world->getMaterial(c.tileX, c.tileY - 1).solid) || (c.axis.y > 0 && !world->getMaterial(c.tileX, c.tileY + 1).solid)) {
						biggestY = c;
					}
				}
				//else {
				//	if (std::abs(c.axis.y) == std::abs(biggestY.axis.y)) { // ths statemen talong with the x are to ensure that -1 is not confused with positive 1 :)
				//		
				//		if (vy < 0) {
				//			biggestY = (c.axis.y > 0) ? c : biggestY;
				//		}
				//		else if (vy > 0) {
				//			biggestY = (c.axis.y < 0) ? c : biggestY;
				//		}
				//	}
				//}
			}


		/*	if (biggestX.axis.x < 0 && vx < 0) {
				biggestX.axis.x = -biggestX.axis.x;
			}


			if (biggestX.axis.x > 0 && vx > 0) {
				biggestX.axis.x = -biggestX.axis.x;
			}


			if (biggestY.axis.y < 0 && vy < 0) {
				biggestY.axis.y = -biggestY.axis.y;
			}


			if (biggestY.axis.y > 0 && vy > 0) {
				biggestY.axis.y = -biggestY.axis.y;
			}*/


			//	std::cout << biggestX.axis.x << ", " << biggestY.axis.y << std::endl;
			if (biggestX.axis.x * biggestX.overlap != 0) {
				//	if ((biggestX.axis.x < 0 && !world->getMaterial(biggestX.tileX - 1, biggestX.tileY).solid) || (biggestX.axis.x > 0 && !world->getMaterial(biggestX.tileX + 1, biggestX.tileY).solid)) {
				x += checkvx + (biggestX.axis.x * biggestX.overlap);

				//	if (biggestX.axis.x * biggestX.overlap != 0.0f || biggestY.axis.y * biggestY.overlap != 0.0f) {
				vx = 0.0f;

				//}
			}

			if (biggestY.axis.y * biggestY.overlap != 0) {

		
					y += checkvy + (biggestY.axis.y * biggestY.overlap);

					if (biggestY.axis.y * biggestY.overlap < 0) {
						falling = false;
					}

					//std::cout << "passed the y" << std::endl;
					vy = 0.0f;
					vx *= 0.9f;
	 
			}
			//std::cout << biggestX.axis.x << ",, " << biggestY.axis.y << std::endl;


			return;// break from the outer for loop
		}
	}


	
}

std::vector<Entity::Collision> Entity::checkCollisions(float vx, float vy) {
	std::vector<Point> tilepoints = world->getTilePoints();
	std::vector<Collision> currentCollisions;


	std::vector<Point> entitypoints = getRotatedPoints();
	std::vector<Point> axes1;
	getNormals(&axes1, entitypoints);

	std::vector<Point> axes2;
	getNormals(&axes2, tilepoints);

	//std::cout << "TILE POINTS: " << std::endl;
	//for (Point p : axes2) {
	//	
	//	std::cout << p.x << ", " << p.y<< std::endl;


	//}

	//std::cout << "ENTITY POINTS: " << std::endl;
	// project the entitys points onto an axis

	// proceed with finding nearby tiles and checking each individual tile.

	minX = INT_MAX;
	minY = INT_MAX;
	maxX = INT_MIN;
	maxY = INT_MIN;

	for (Point p : entitypoints) {
		if (p.x < minX + vx) minX = p.x;
		if (p.y < minY + vy) minY = p.y;
		if (p.x > maxX + vx) maxX = p.x;
		if (p.y > maxY + vy) maxY = p.y;
	}


	float startX = (x + minX + vx);
	float startY = (y + minY + vy);

	float endX = (x + maxX + vx);
	float endY = (y + maxY + vy);


	float overlap = INT_MAX;
	Point smallest = { 0, 0 };
	for (int i = startX / world->tileSize; i < endX / world->tileSize; i++) {
		for (int j = startY / world->tileSize; j < endY / world->tileSize; j++) {
			if (i < 0 || i >= world->getWidth() * 32 || j < 0 || j >= world->getHeight() * 32) continue;
			if (world->materialData[world->tiles[i][j].id].solid) {
				// a VIABLE COLLISION

				bool collision = true;

				for (Point axis : axes1) {

					Projection p1 = getProjection({ this->x + vx, this->y + vy }, entitypoints, axis);
					Projection p2 = getProjection({ i * 32.0f, j * 32.0f }, tilepoints, axis);
					if (!p1.overlap(p2)) {
						//std::cout << "no collision";
						collision = false;
						goto noCollision; // no collision
					}
					else {
						float o = p1.getOverlap(p2);
						// check for minimum
						if (o < overlap) {
							// then set this one as the smallest
							overlap = o;
							smallest = axis;
						}
					}
				}

				for (Point axis : axes2) {

					Projection p1 = getProjection({ this->x + vx, this->y + vy }, entitypoints, axis);
					Projection p2 = getProjection({ i * 32.0f, j * 32.0f }, tilepoints, axis);

					if (!p1.overlap(p2)) {
						//std::cout << "no collision:" << std::endl;
						//std::cout << p1._min << "-- > " << p1._max << std::endl;
						//std::cout << p2._min << "-- > " << p2._max << std::endl;
						collision = false;
						goto noCollision;
					}
					else {
						float o = p1.getOverlap(p2);
						// check for minimum
						if (o < overlap) {

							overlap = o;
							smallest = axis;
						}
					}
				}

			noCollision:

				if (collision) {

					Point smallestFixed = smallest.normalized();


					//float pr = (smallest.y * overlap);


					Point mtv = Point{ x + vx, y + vy } -Point(i * 32.0f, j * 32.0f);
					if (smallest.dot(mtv) < 0) { // aka vectors not in a similar direction (within 90 degrees)
						smallestFixed.x = -smallestFixed.x;
						smallestFixed.y = -smallestFixed.y;
						// this could be problematic
					}
					currentCollisions.push_back(Collision{ i, j, overlap, smallestFixed });
				}
			}
		}
	}

	return currentCollisions;
}

bool Entity::collisionResponse(Collision c, float vx, float vy) {
	//std::cout << "collision" << std::endl;
	Point norm = c.axis;
	float overlap = c.overlap;

	int i = c.tileX;
	int j = c.tileY;


	//float pr = (smallest.y * overlap);


	Point mtv = Point{ x + vx, y + vy } -Point(i * 32.0f, j * 32.0f);
	if (norm.dot(mtv) < 0) { // aka vectors not in a similar direction (within 90 degrees)
		norm.x = -norm.x;
		norm.y = -norm.y;
	}

	//std::cout << norm.x << ", " << norm.y << std::endl;

	//std::cout << norm
	//if((norm.x > 0 && !world->getMaterial(i + 1, j).solid) || (norm.x < 0 && !world->getMaterial(i - 1, j).solid))
	//x += vx + (norm.x * overlap);

	if ((norm.y > 0 && !world->getMaterial(i, j + 1).solid) || (norm.y < 0 && !world->getMaterial(i, j - 1).solid)) {
		y += vy + (norm.y * overlap);
		this->vy = 0;
	}

	if ((norm.x > 0 && !world->getMaterial(i + 1, j).solid) || (norm.x < 0 && !world->getMaterial(i - 1, j).solid)) {
		x += vy + (norm.y * overlap);
		this->vy = 0;
	}

	if ((norm.y < 0 && vy > 0)) {
		falling = false;
	}


	//	if (!(std::abs(norm.x) > 0.0f && std::abs(norm.x) < 1.0f && std::abs(norm.y) > 0.0f && std::abs(norm.y) < 1.0f)) {
	//		this->vx *= 0.8f;
	//	}
	//	else {
	//		this->vx *= 0.5f;
	//	}
	//}



	//if (std::abs(norm.x) > 0.0f && std::abs(norm.x) <= 1.0f && std::abs(norm.y) > 0.0f && std::abs(norm.y) <= 1.0f) {
	//	if (vy > 0 && !world->getMaterial(i, j - 1).solid) {
	//		y += (norm.y * overlap);
	//	}
	//	else if (vy < 0 && !world->getMaterial(i, j + 1).solid) {
	//		y += (norm.y * overlap);
	//	}

	//	x += (norm.x * overlap);

	//	std::cout << "corner" << std::endl;
	//}
	//else {

	//	// moving to the right
	//	if (vx > 0 && !world->getMaterial(i - 1, j).solid) {
	//		std::cout << "right" << std::endl;
	//		x += vx + (norm.x * overlap);
	//		vx = 0;
	//	}
	//	else if (vx < 0 && !world->getMaterial(i + 1, j).solid) {
	//		x += norm.x * overlap;
	//		std::cout << "left" << std::endl;
	//		vx = 0;
	//	}

	//	if (vy > 0 && !world->getMaterial(i, j - 1).solid) {
	//		std::cout << "down" << norm.y * overlap << norm.x * overlap << std::endl;
	//		y += vy + (norm.y * overlap);
	//		vy = 0;


	//	}
	//	else if (vy < 0 && !world->getMaterial(i, j + 1).solid) {
	//		std::cout << "up" << std::endl;
	//		vy = 0;
	//		y += norm.y * overlap;
	//	}
	//}


	return true;
}

void Entity::getNormals(std::vector<Point>* axes, std::vector<Point> points) {
	axes->clear();

	for (int i = 0; i < points.size(); i++) {
		Point vec1 = points.at(i);
		Point vec2 = points.at((i + 1) == points.size() ? 0 : i + 1);

		//std::cout << (0 / 32) << std::endl;

		axes->push_back(((vec2 - vec1)).perpendicular().normalized());
	}


}

// for a tile, you would pass the x and y of the position in tileX and tile Y and then the coords for the position (0,0) (0,32) etc.
Entity::Projection Entity::getProjection(Point position, std::vector<Point> positions, Point norm) {
	float min = norm.dot(positions.at(0) + position); // add the world X to the positions
	float max = min;
	for (int j = 1; j < positions.size(); j++) {
		Point pos = (positions.at(j) + position);
		//			float x = (dot(pos, norm) * norm.x());
			//		float y = (dot(pos, norm) * norm.y());
					//Vector2 projection = Vector2{ x, y };

		float p = norm.dot(pos);
		//Debug{} << p << ", SIZE: " << obj->getPositions().size();
		if (p < min) {
			min = p;
		}
		else if (p > max) {
			max = p;
		}

		if (std::isnan(min)) {
			//std::cout << "min IS NAN" << norm.x << ", " << norm.y << ", " << std::endl;
		}

		//Vector2 projection = dot(pos, norm) * norm;

		//Debug{} << "AXIS OF" << vec1 << "---> " << vec2 << "|||| " << pos << "PROJECTED ONTO: " << x << ", " << y;

	}
	//Debug{} << "_____ MINS / MAX'S OF THOSE: " << min << ", " << max;
	return Projection(min, max);
}

bool isCollision(int x, int y) {
	return false;
}

std::vector<Point> Entity::getRotatedPoints() {
	std::vector<Point> points;

	Point center = { width / 2, height / 2 };

	float angle = rotation * 3.14f / 180.0f;

	Point topLeft = rotatePoint({ 0.0f, 0.0f }, center, angle);
	Point topRight = rotatePoint({ width, 0.0f }, center, angle);
	Point bottomLeft = rotatePoint({ 0.0f, height }, center, angle);
	Point bottomRight = rotatePoint({ width, height }, center, angle);

	/*std::cout << "topLEFT X: " << topLeft.x << " y:" << topLeft.y << std::endl;
	std::cout << "topRIGHT X: " << topRight.x << " y:" << topRight.y << std::endl;
	std::cout << "BOTTOMLEFT X: " << bottomLeft.x << " y:" << bottomLeft.y << std::endl;
	std::cout << "BOTTOMRIGHT X: " << bottomRight.x << " y:" << bottomRight.y << std::endl;*/

	points.push_back(bottomLeft);
	points.push_back(topLeft);
	points.push_back(topRight);
	points.push_back(bottomRight);

	return points;
}

Point Entity::rotatePoint(Point topLeft, Point center, float angle) {
	float rotatedX = std::cos(angle) * (topLeft.x - center.x) - std::sin(angle) * (topLeft.y - center.y) + center.x;
	float rotatedY = (std::sin(angle) * (topLeft.x - center.x) + std::cos(angle) * (topLeft.y - center.y) + center.y);

	return { rotatedX, rotatedY };
}

bool Entity::collides(float x, float y, float width, float height) {
	return(this->x + vx + this->width > x && this->x + vx < x + width && this->y + vy + this->height > y && this->y + vy < y + height);
}

float Entity::getLerpX(float alpha) {
	return (currentState.x * alpha) +
		previousState.x * (1.0f - alpha);
}

float Entity::getLerpY(float alpha) {
	return (currentState.y * alpha) +
		previousState.y * (1.0f - alpha);
}

void Entity::lerpTranslation(float alpha, float scale) {
	float lerpx = getLerpX(alpha);

	float lerpy = getLerpY(alpha);


	glTranslatef((-lerpx * scale + ((world->game->getWidth() / 2) - (width / 2))), ((-lerpy * scale + ((world->game->getHeight() / 2) - (height / 2)))), 0.0f);
	//glRotatef(10, 0, 1.0, 0);

	glScalef(scale, scale, 0.0f);
	//glTranslatef(-(-lerpx + ((world->game->getWidth() / 2) - (width / 2))) * scale, (-(-lerpy + ((world->game->getHeight() / 2) - (height / 2)))) *scale, 0.0f);

}

// remember to render the lerp X not the non lerp X
// this function can be expanded upon to include animations, among other things...
// idea: maybe create an Animation for every entity (which each animation will contain a STATIC textureatlas to avoid recreating 12381238 textures every frame/per entity)
// then just render the current animation step... for still textures it will just be a 1x1 atlas with an animation step that never changes
// it will be controlled by the update function of each respective entity..... very versatile!
void Entity::render(float alpha) {
	float lerpx = getLerpX(alpha);

	float lerpy = getLerpY(alpha);

	if (color.a > 0.0f) {

		//glTranslatef(lerpx, lerpy, 0.0f);

		float difX = lerpx + width / 2;
		float difY = lerpy + height / 2;
		//glTranslatef(-lerpx, -lerpy, 0.0f);
		//glLoadIdentity();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//glPushMatrix();

		glTranslatef(difX, difY, 0.0f);

		/*	glBegin(GL_QUADS);
			for (Point p : getRotatedPoints()) {
				glVertex2f(-width / 2 + p.x, -height / 2 + p.y);
			}

			glEnd();*/

		glRotatef(rotation, 0.0, 0.0, 1.0);

		if (animation.isInitialized()) {
			animation.render(-(animation.getAtlas().getTextureWidth()) / 2, (-(animation.getAtlas().getTextureHeight()) / 2) - yOffset, animation.getAtlas().getTextureWidth(), animation.getAtlas().getTextureHeight());
		}
		else {

			world->getGame()->getRenderer().setScale(1.0f);
			world->getGame()->getRenderer().setWorld(true);
			world->getGame()->getRenderer().setColor(color);
			world->getGame()->getRenderer().drawQuad(-width / 2, -height / 2, width, height, false);
			world->getGame()->getRenderer().setWorld(false);
		}




		//glPopMatrix();
		glLoadIdentity();
		glMatrixMode(GL_PROJECTION);



	}
}

void Entity::fadeIn(int ticks) {
	fading = 1;
	alphaPerTick = (1.0f - color.a) / ticks;
}

void Entity::fadeOut(int ticks) {
	fading = 0;
	alphaPerTick = -color.a / ticks;
}


Renderer::Color Entity::getColor() {
	return color;
}

void Entity::setColor(Renderer::Color c) {
	this->color = c;
}

bool Entity::isActive() {
	//std::cout << active << std::endl;
	return active;
}

void Entity::setActive(bool act) {
	if (!act) {
		this->fadeOut(10);
	}

	vx = 0.0f;
	vy = 0.0f;

	this->active = act;
}

void Entity::setSize(float width, float height) {
	this->width = width;
	this->height = height;
}

void Entity::setID(int id) {
	this->id = id;
}

int Entity::getType() {
	return type;
}