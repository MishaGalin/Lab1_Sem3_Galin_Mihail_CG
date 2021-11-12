#include <windows.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

HWND console = GetConsoleWindow();
HDC hdc = GetDC(console);

COLORREF white = RGB(255, 255, 255);
COLORREF red = RGB(255, 0, 0);
COLORREF green = RGB(0, 255, 0);
COLORREF blue = RGB(0, 0, 255);

void line(int x1, int y1, int x2, int y2, COLORREF Color = RGB(255, 255, 255))
{
	int x = x1;
	int y = y1;
	while (x <= x2)
	{
		SetPixel(hdc, x, y, Color);
		y = y1 + (y2 - y1) / (x2 - x1) * (x - x1);
		x++;
	}
}

void drawLine(int x1, int y1, int x2, int y2, COLORREF Color = RGB(255, 255, 255)) {
	const int deltaX = abs(x2 - x1);
	const int deltaY = abs(y2 - y1);
	const int signX = x1 < x2 ? 1 : -1;
	const int signY = y1 < y2 ? 1 : -1;
	int error = deltaX - deltaY;
	SetPixel(hdc, x2, y2, Color);
	while (x1 != x2 || y1 != y2)
	{
		SetPixel(hdc, x1, y1, Color);
		int error2 = error * 2;
		if (error2 > -deltaY)
		{
			error -= deltaY;
			x1 += signX;
		}
		if (error2 < deltaX)
		{
			error += deltaX;
			y1 += signY;
		}
	}
}

void drawCircle(int x0, int y0, int radius, COLORREF BorderColor = RGB(255, 255, 255)) {
	int x = 0;
	int y = radius;
	int delta = 1 - 2 * radius;
	int error = 0;
	while (y >= 0) {
		SetPixel(hdc, x0 + x, y0 + y, BorderColor);
		SetPixel(hdc, x0 + x, y0 - y, BorderColor);
		SetPixel(hdc, x0 - x, y0 + y, BorderColor);
		SetPixel(hdc, x0 - x, y0 - y, BorderColor);
		error = 2 * (delta + y) - 1;
		if (delta < 0 && error <= 0) {
			++x;
			delta += 2 * x + 1;
			continue;
		}
		error = 2 * (delta - x) - 1;
		if (delta > 0 && error > 0) {
			--y;
			delta += 1 - 2 * y;
			continue;
		}
		++x;
		delta += 2 * (x - y);
		--y;
	}
}

int LineFill(int x, int y, int dir, int PrevXl, int PrevXr, COLORREF BorderColor, COLORREF Color)
{
	int xl = x, xr = x, c;
	do c = GetPixel(hdc, --xl, y); while ((c != BorderColor) && (c != Color));
	do c = GetPixel(hdc, ++xr, y); while ((c != BorderColor) && (c != Color));
	xl++; xr--;
	line(xl, y, xr, y, Color);
	for (x = xl; x <= xr; x++)
	{
		c = GetPixel(hdc, x, y + dir);
		if ((c != BorderColor) && (c != Color)) x = LineFill(x, y + dir, dir, xl, xr, BorderColor, Color);
	}
	for (x = xl; x < PrevXl; x++)
	{
		c = GetPixel(hdc, x, y - dir);
		if ((c != BorderColor) && (c != Color)) x = LineFill(x, y - dir, -dir, xl, xr, BorderColor, Color);
	}
	for (x = PrevXr; x < xr; x++)
	{
		c = GetPixel(hdc, x, y - dir);
		if ((c != BorderColor) && (c != Color)) x = LineFill(x, y - dir, -dir, xl, xr, BorderColor, Color);
	}
	return xr;
}

int main()
{
	srand(unsigned int(time(0)));
	int circle1X, circle1Y, circle2X, circle2Y, radius;
	double distance;

	while (true) {
		system("cls");
		circle1X = 150 + rand() % 200;
		circle1Y = 150 + rand() % 200;
		circle2X = 150 + rand() % 200;
		circle2Y = 150 + rand() % 200;
		radius = 80 + rand() % 20;
		distance = sqrt(pow(circle1X - circle2X, 2) + pow(circle1Y - circle2Y, 2));

		if (int(distance) < (2 * radius) && int(distance) > radius) {
			drawCircle(circle1X, circle1Y, radius);
			drawCircle(circle2X, circle2Y, radius);
			LineFill((circle1X + circle2X) / 2, (circle1Y + circle2Y) / 2, 1, (circle1X + circle2X) / 2, (circle1X + circle2X) / 2, white, red);
			LineFill(circle1X, circle1Y, 1, circle1X, circle1X, white, green);
			LineFill(circle2X, circle2Y, 1, circle2X, circle2X, white, blue);
		}
		else continue;
	}
}