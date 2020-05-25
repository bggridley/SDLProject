#pragma once
#include <cmath>
class Point {

public:
	float x;
	float y;
	Point(float x, float y) {
		this->x = x;
		this->y = y;
	}

	Point operator-(const Point& rhs) const
	{
		return { this->x - rhs.x, this->y - rhs.y };
		// do your comparison
	}

	Point operator+(const Point& rhs) const
	{
		return { this->x + rhs.x, this->y + rhs.y };
		// do your comparison
	}

	float dot(Point point) { // DOT PRODUCT YAY
		return (this->x * point.x) + (this->y * point.y);
	}

	Point perpendicular() {
		float finalX = -y;
		float finalY = x;

		if (std::isinf(finalX)) finalX = 0;
		if (std::isinf(finalY)) finalY = 0;
		return { finalX, finalY };
	}

	Point normalized() { // AKA " unit vector " 
		float magnitude = std::sqrt((x * x) + (y * y));
		//std::cout << x << ", " << y << " -- > " << x / magnitude << ", " << y / magnitude << std::endl;
		return Point{ x / magnitude, y / magnitude };
	}
};