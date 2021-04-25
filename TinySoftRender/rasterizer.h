#pragma once
#include"model.h"
#include"Value.h"
class rasterizer
{
public:
	//BresenhamªÊ÷∆œﬂ
	void lineBresenham(int x0, int y0, int x1, int y1, TGAImage&img, const TGAColor&color);
	void lineBresenham(Point p0,Point p1, TGAImage&img, const TGAColor&color);
	void DrawTrangile(Triangle&t, TGAImage&img, const TGAColor&color);
	void DrawWireFrame(Model & model,int width,int height, TGAImage&img, const TGAColor&color);
};

