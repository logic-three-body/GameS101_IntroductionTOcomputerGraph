#pragma once
#include"model.h"
#include"Value.h"
class rasterizer
{
private:
	int width;//光栅化器宽
	int height;//光栅化器长
public:
	//构造函数
	rasterizer() {};
	rasterizer(int _width, int _height):width(_width),height(_height) {};
	//功能函数
	void Resize(int _width, int _height) { width = _width; height = _height; };//重定义光栅化器大小
	//Bresenham绘制线
	void lineBresenham(int x0, int y0, int x1, int y1, TGAImage&img, const TGAColor&color);
	void lineBresenham(Pointi p0,Pointi p1, TGAImage&img, const TGAColor&color);
	void DrawTrangile(Trianglei&t, TGAImage&img, const TGAColor&color);
	void DrawTrangile(Pointi p0, Pointi p1, Pointi p2, TGAImage&img, const TGAColor&color);
	void DrawWireFrame(Model & model,int width,int height, TGAImage&img, const TGAColor&color);
	void DrawWireFrame(Model & model,TGAImage&img,const TGAColor&color);
};

