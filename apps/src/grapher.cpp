#include <iostream>
#include <functional>
#include "AsciiRender.h"

using namespace std;

int main() {
	//Note that the display will look steched vertically because characters are not 1:1
	double minX = -4.0, maxX = 4.0;
	double minY = -1.0, maxY = 1.0;

	function<double(double)> funcs[] = {
		[](double x) { return sin(2 * x) / (2 * x); },
		[](double x) { return pow(2.0, -pow(x, 2.0)) - .9; }
	};
	//Actual dimensions
	AsciiRender render(80, 20);


	//These for loop can be optimized, however this is written for readability
	double rangeX = maxX - minX, rangeY = maxY - minY;
	for (function<double(double)> func : funcs) {
		for (int relX = 0; relX < render.res.x; relX++) {
			//Translate from relative (from screen) coordinates to mathmatical coordinates
			double absX = relX * rangeX / (double)render.res.x + minX;
			//Get absolute y from function and translate it to relative coordinates
			double relY = (func(absX) - minY) * (double)render.res.y / rangeY;
			//Flip screen vertically
			relY = render.res.y - relY;
			render.setPixel(relX, (int)relY, '#');
		}
	}
	cout << render;

	return 0;
}