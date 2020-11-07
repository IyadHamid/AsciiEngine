#pragma once
#include <iostream>
#include <vector>
#include "vec.h"

class AsciiRender {
public:
	vec2<int> res;

	AsciiRender(int resX, int resY);
	AsciiRender(vec2<int> res) : AsciiRender(res.x, res.y) {};

	char getPixel(int x, int y);
	char getPixel(vec2<int> point);
	void setPixel(int x, int y, char value);
	void setPixel(vec2<int> point, char value);

	void fillRectangle(vec2<int> point, vec2<int> other, char value);
	void fillEllipse(vec2<int> point, vec2<int> other, char value);
	void fillTriangle(vec2<int> v1, vec2<int> v2, vec2<int> v3, char value);
	void fillPolygon(std::vector<vec2<int>> points, char value);

	void setClear(bool value);
		
	friend std::ostream& operator<<(std::ostream& out, AsciiRender& ar);

protected:
	char* buffer;
	bool clear;

	void clearScreen();
};

