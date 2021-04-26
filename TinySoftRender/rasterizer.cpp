#include "rasterizer.h"
void rasterizer::lineBresenham(int x0, int y0, int x1, int y1, const TGAColor & color)
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
		if (steep)
		{
			frameBuffer.set(y, x, color);
		}
		else
		{
			frameBuffer.set(x, y, color);
		}
	}
}

void rasterizer::lineBresenham(Vec2i p0, Vec2i p1, const TGAColor & color)
{
	lineBresenham(p0.x, p0.y, p1.x, p1.y,color);
}

void rasterizer::DrawWireTrangile(Trianglei & t, const TGAColor & color)
{
	DrawWireTrangile(t.p0, t.p1, t.p2, color);
}

void rasterizer::DrawWireTrangile(Vec2i p0, Vec2i p1, Vec2i p2, const TGAColor & color)
{
	lineBresenham(p0, p1, color);
	lineBresenham(p1, p2, color);
	lineBresenham(p2, p0, color);
}

void rasterizer::DrawFillTrangile(Trianglei & t, const TGAColor & color)
{
	DrawFillTrangile(t.p0, t.p1, t.p2, color);
}

void rasterizer::DrawFillTrangile(Vec2i p0, Vec2i p1, Vec2i p2, const TGAColor & color)
{
	if (p0.y > p1.y)
	{
		std::swap(p0, p1);
	}
	if (p0.y > p2.y)
	{
		std::swap(p0, p2);
	}
	if (p1.y > p2.y)
	{
		std::swap(p1, p2);
	}
	lineBresenham(p0, p1, color);
	lineBresenham(p1, p2, color);
	lineBresenham(p2, p0, color);
}

void rasterizer::DrawWireFrame(Model & model, int width, int height, const TGAColor & color)
{
	for (int i = 0; i < model.nfaces(); i++) {
		std::vector<int> face = model.face(i);
		for (int j = 0; j < 3; j++) {
			Vec3f v0 = model.vert(face[j]);
			Vec3f v1 = model.vert(face[(j + 1) % 3]);
			int x0 = (v0.x + 1.)*width / 2.;
			int y0 = (v0.y + 1.)*height / 2.;
			int x1 = (v1.x + 1.)*width / 2.;
			int y1 = (v1.y + 1.)*height / 2.;
			lineBresenham(x0, y0, x1, y1,color);
		}
	}
}

void rasterizer::DrawWireFrame(Model & model, const TGAColor & color)
{
	DrawWireFrame(model, width, height,color);
}
