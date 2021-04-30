#pragma once
#include"model.h"
#include"Value.h"
#include"geometry.h"
#include"TGAImage.h"



class rasterizer
{
private:
	int width;//光栅化器宽
	int height;//光栅化器长
	TGAImage frameBuffer;
	int* ZBuffer;//深度检测
public:
	//构造函数
	rasterizer() :width(0), height(0) { frameBuffer.resize(width,height,TGAImage::RGB); };
	rasterizer(int _width, int _height):width(_width),height(_height) { frameBuffer.resize(width, height, TGAImage::RGB); };

	//功能函数
	void Resize(int _width, int _height) { width = _width; height = _height; };//重定义光栅化器大小
	void GetFrameBuffer(TGAImage&img) { frameBuffer = img; };
	TGAImage GiveBuffer() { return frameBuffer; };
	void WriteBuffer(const char* path) { frameBuffer.write_tga_file(path); };
	void ReadBuffer(const char* path) { frameBuffer.read_tga_file(path); };
	void ClearBuffer() { frameBuffer.clear(); };
	void InitZBuffer();
	//Bresenham绘制线
	void lineBresenham(int x0, int y0, int x1, int y1,const TGAColor&color);
	void lineBresenham(Vec2i p0, Vec2i p1,const TGAColor&color);
	void DrawWireTrangile(Trianglei&t,const TGAColor&color);
	void DrawWireTrangile(Vec2i p0, Vec2i p1, Vec2i p2,const TGAColor&color);
	//2D
	void DrawFillTrangile(Trianglei&t,const TGAColor&color);
	void DrawFillTrangile(Vec2i p0, Vec2i p1, Vec2i p2,const TGAColor&color);
	void DrawInterpolateTrangile(Trianglei&t, const TGAColor&color);
	void DrawInterpolateTrangile(Vec2i p0, Vec2i p1, Vec2i p2, const TGAColor&color);
	void DrawWireFrame(Model & model, int width, int height,const TGAColor&color);
	void DrawWireFrame(Model & model, const TGAColor&color);
	void DrawFlatFrame(Model & model);
	void DrawFlatFrame(Model & model, int width, int height);
	void DrawGrayFrame(Model & model, Vec3f light_dir);
	void DrawGrayFrame(Model & model, int width, int height, Vec3f light_dir);
	//3D
	void DrawFillTrangile(Triangle3i&t, const TGAColor&color);
	void DrawFillTrangile(Vec3i p0, Vec3i p1, Vec3i p2, const TGAColor&color);
};

