#include "AsciiRender.h"

#include <iostream>
#include <vector>
#ifdef _WIN32
	#include <windows.h>
#elif __linux__ || __APPLE__
	#include <unistd.h>
	#include <term.h>
#endif


AsciiRender::AsciiRender(int resX, int resY) : res{ resX, resY }, clear{ false } {
	const int size = (resX + 1) * resY;
	buffer = new char[size];
	for (int i = 0; i < size; i++) {
		buffer[i] = i % (resX + 1) == resX ? '\n' : ' ';
	}
	buffer[size - 1] = 0; //Add NUL character

}

char AsciiRender::getPixel(int x, int y) {
	if (x < res.x && y < res.y && x >= 0 && y >= 0) {
		return buffer[x + y * (res.x + 1)];
	}
	return ' ';
}

char AsciiRender::getPixel(vec2<int> point) {
	return getPixel(point.x, point.y);
}

void AsciiRender::setPixel(int x, int y, char value) {
	if (x < res.x && y < res.y && x >= 0 && y >= 0) {
		buffer[x + y * (res.x + 1)] = value;
	}
}

void AsciiRender::setPixel(vec2<int> point, char value) {
	setPixel(point.x, point.y, value);
}

void AsciiRender::fillRectangle(vec2<int> point, vec2<int> other, char value) {
	for (int i = point.x; i < other.x; i++) {
		for (int j = point.y; j < other.y; j++) {
			setPixel(i, j, value);
		}
	}
}

void AsciiRender::fillEllipse(vec2<int> point, vec2<int> other, char value) {
	//https://www.geeksforgeeks.org/check-if-a-point-is-inside-outside-or-on-the-ellipse/
	auto inEllipse = [](int x, int y, vec2<int> v1, vec2<int> v2)->bool {
		double h = ((double)v1.x + (double)v2.x) / 2.0;
		double k = ((double)v1.y + (double)v2.y) / 2.0;
		double a = h - v1.x;
		double b = k - v2.y;
		return pow((x - h) / a, 2.0) + pow((y - k) / b, 2.0) <= 1;
	};
	point.x = point.x < 0 ? 0 : point.x;
	point.y = point.y < 0 ? 0 : point.y;

	for (int i = point.x; i < other.x; i++) {
		for (int j = point.y; j < other.y; j++) {
			if (inEllipse(i, j, point, other)) {
				setPixel(i, j, value);
			}
		}
	}
}

void AsciiRender::fillTriangle(vec2<int> v1, vec2<int> v2, vec2<int> v3, char value) {
	//https://stackoverflow.com/questions/2049582/how-to-determine-if-a-point-is-in-a-2d-triangle
	auto sign = [](vec2<int> p1, vec2<int> p2, vec2<int> p3)->int {
		return ((int)p1.x - (int)p3.x) * ((int)p2.y - (int)p3.y) - 
			   ((int)p2.x - (int)p3.x) * ((int)p1.y - (int)p3.y);
	};
	auto inTriangle = [sign](vec2<int> pt, vec2<int> v1, vec2<int> v2, vec2<int> v3)->bool {
		int d1, d2, d3;
		bool has_neg, has_pos;
		d1 = sign(pt, v1, v2);
		d2 = sign(pt, v2, v3);
		d3 = sign(pt, v3, v1);
		has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
		has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

		return !(has_neg && has_pos);
	};

	vec2<int> point{ min(min(v1.x, v2.x), v3.x), min(min(v1.y, v2.y), v3.y) };
	vec2<int> other{ max(max(v1.x, v2.x), v3.x), max(max(v1.y, v2.y), v3.y) };
	point.x = point.x < 0 ? 0 : point.x;
	point.y = point.y < 0 ? 0 : point.y;

	for (int i = point.x; i < other.x; i++) {
		for (int j = point.y; j < other.y; j++) {
			if (inTriangle(vec2<int>{i, j}, v1, v2, v3)) {
				setPixel(i, j, value);
			}
		}
	}
}

void AsciiRender::fillPolygon(std::vector<vec2<int>> points, char value) {
	//https://en.wikipedia.org/wiki/Even%E2%80%93odd_rule
	unsigned int len = (unsigned int)points.size();
	vec2<int> min, max;

	for (unsigned int i = 0; i < len; i++) {
		points[i].x = points[i].x < 0 ? 0 : points[i].x;
		points[i].y = points[i].y < 0 ? 0 : points[i].y;
		min = { min(points[i].x, min.x), min(points[i].y, min.y) };
		max = { max(points[i].x, max.x), max(points[i].y, max.y) };
	}
	for (int y = min.y; y < max.y; y++) {
		for (int x = min.x; x < max.x; x++) {
			bool c = false;
			for (unsigned int i = 0, j = len - 1; i < len; j = i, i++) {
				if (((points[i].y > y) != (points[j].y > y)) &&
					(x < points[i].x + (points[j].x - points[i].x) * (y - points[i].y) /
									   (points[j].y - points[i].y)))
				{
					c = !c;
				}
			}
			if (c) {
				setPixel(x, y, value);
			}
		}
	}
}

void AsciiRender::setClear(bool value) {
	clear = value;
}

std::ostream& operator<<(std::ostream& out, AsciiRender& ar) {
	if (ar.clear) {
		ar.clearScreen();
	}
	out << ar.buffer;
	return out;
}


void AsciiRender::clearScreen() {
	//http://www.cplusplus.com/articles/4z18T05o/
	//Clearing the screen varies between OSs
#ifdef _WIN32
	COORD coordScreen = {0, 0};
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD dwConSize;
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	if (!GetConsoleScreenBufferInfo(hStdout, &csbi)) {
		return;
	}
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	if (!FillConsoleOutputCharacter(hStdout,
									(TCHAR)' ',
									dwConSize,
									coordScreen,
									&cCharsWritten))
	{
		return;
	}
	if (!GetConsoleScreenBufferInfo(hStdout, &csbi)) {
		return;
	}
	if (!FillConsoleOutputAttribute(hStdout,
									csbi.wAttributes,
									dwConSize,
									coordScreen,  
									&cCharsWritten)) 
	{
		return;
	}
	SetConsoleCursorPosition(hStdout, coordScreen);
#elif __linux__ || __APPLE__
	if (!cur_term) {
		int result;
		setupterm(NULL, STDOUT_FILENO, &result);
		if (result <= 0) {
			return;
		}
	}
	putp(tigetstr("clear"));
#endif
}
