#include"TGAImage.h"
#include "line.h"
/*
确定两点,
在两点间画线,
取极小的时间步长dt,每增长dt就在此像素涂色
*/
void lineNaiveSegment(int x0, int y0, int x1, int y1, TGAImage&img, const TGAColor&color)
{
	float length = 1.0f;
	for (float dt = 0.0f; dt < length; dt += 0.01f)
	{
		int x = x0 + (x1 - x0)*dt;
		int y = y0 + (y1 - y0)*dt;
		img.set(x, y, color);
	}
}

//无法画出斜率为负的线
void lineBresenhambad(int x0, int y0, int x1, int y1, TGAImage & img, const TGAColor & color)
{
	for (int x = x0; x <= x1; ++x)
	{
		float dt = float(x - x0) / float(x1 - x0);
		int y = y0 * (1.0f - dt) + y1 * dt;
		img.set(x, y, color);
	}
}
/*
Bresenham算法：
*/
void lineBresenham(int x0, int y0, int x1, int y1, TGAImage & img, const TGAColor & color)
{
	bool steep = false;
	if (std::abs(x0 - x1) < std::abs(y0 - y1))
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0 > x1)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	for (int x = 0; x <= x1; x++)
	{
		float t = (x - x0) / float(x1 - x0);
		int y = y0 * (1.0f - t) + y1 * t;
		if (steep)
		{
			img.set(y, x, color);
		}
		else
		{
			img.set(x, y, color);
		}
	}
}


void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) 
{
	bool steep = false;
	if (std::abs(x0 - x1) < std::abs(y0 - y1)) 
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0 > x1) 
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	for (int x = x0; x <= x1; x++) 
	{
		float t = (x - x0) / float(x1 - x0);
		int y = y0 * (1.0f - t) + y1 * t;
		if (steep) {
			image.set(y, x, color);
		}
		else {
			image.set(x, y, color);
		}
	}
}
