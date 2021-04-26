#include "rasterizer.h"

void rasterizer::lineBresenham(int x0, int y0, int x1, int y1, TGAImage & img, const TGAColor & color)
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
			img.set(y, x, color);
		}
		else
		{
			img.set(x, y, color);
		}
	}
}

void rasterizer::lineBresenham(Pointi p0, Pointi p1, TGAImage&img, const TGAColor&color)
{
	lineBresenham(p0.x,p0.y,p1.x,p1.y,img,color);
}

void rasterizer::DrawTrangile(Trianglei & t, TGAImage&img, const TGAColor&color)
{
	DrawTrangile(t.p0,t.p1,t.p2,img,color);
}

void rasterizer::DrawTrangile(Pointi p0, Pointi p1, Pointi p2, TGAImage & img, const TGAColor & color)
{
	lineBresenham(p0, p1, img, color);
	lineBresenham(p1, p2, img, color);
	lineBresenham(p2, p0, img, color);
}

void rasterizer::DrawWireFrame(Model & model, int width, int height, TGAImage & img, const TGAColor & color)
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
			lineBresenham(x0, y0, x1, y1, img, color);
		}
	}
}

void rasterizer::DrawWireFrame(Model & model, TGAImage & img, const TGAColor & color)
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
			lineBresenham(x0, y0, x1, y1, img, color);
		}
	}
}
