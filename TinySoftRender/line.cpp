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

void lineInterpolateSegment(int x0, int y0, int x1, int y1, TGAImage & img, const TGAColor & color)
{
	for (int x = x0; x <= x1; ++x)
	{
		float dt = float(x - x0) / float(x1-x0);
		int y = y0 * (1.0f - dt) + y1 * dt;
		img.set(x, y, color);
	}
}
