#pragma once
#include"model.h"
#include"Value.h"
#include"geometry.h"
#include"TGAImage.h"



class rasterizer
{
private:
	int width;//��դ������
	int height;//��դ������
	TGAImage frameBuffer;
	float* ZBuffer;//��ȼ��
	Matrix ModelView;//ģ���ӽǱ任����
	Matrix Projection;//͸�ӱ任����
	Matrix Viewport;//�ӿڱ任����
	
public:
	//���캯��
	rasterizer() :width(0), height(0) { frameBuffer.resize(width,height,TGAImage::RGB); };
	rasterizer(int _width, int _height):width(_width),height(_height) { frameBuffer.resize(width, height, TGAImage::RGB); };

	//���ܺ���
	void Resize(int _width, int _height) { width = _width; height = _height; };//�ض����դ������С
	void GetFrameBuffer(TGAImage&img) { frameBuffer = img; };
	TGAImage GiveBuffer() { return frameBuffer; };
	void WriteBuffer(const char* path) { frameBuffer.write_tga_file(path); };
	void ReadBuffer(const char* path) { frameBuffer.read_tga_file(path); };
	void ClearBuffer() { frameBuffer.clear(); };
	void InitZBuffer();
	Vec3f world2screen(Vec3f v) {//��������ת��Ļ����
		return Vec3f(int((v.x + 1.)*width / 2. + .5), int((v.y + 1.)*height / 2. + .5), v.z);
	}

	//Bresenham������
	void lineBresenham(int x0, int y0, int x1, int y1,const TGAColor&color);
	void lineBresenham(Vec2i p0, Vec2i p1,const TGAColor&color);
	void DrawWireTrangile(Trianglei&t,const TGAColor&color);
	void DrawWireTrangile(Vec2i p0, Vec2i p1, Vec2i p2,const TGAColor&color);
	//2D
	void Draw2DFillTrangile(Trianglei&t,const TGAColor&color);
	void Draw2DFillTrangile(Vec2i p0, Vec2i p1, Vec2i p2,const TGAColor&color);
	void Draw2DInterpolateTrangile(Trianglei&t, const TGAColor&color);
	void Draw2DInterpolateTrangile(Vec2i p0, Vec2i p1, Vec2i p2, const TGAColor&color);
	void Draw2DWireFrame(Model & model, int width, int height,const TGAColor&color);
	void Draw2DWireFrame(Model & model, const TGAColor&color);
	void Draw2DFlatFrame(Model & model);
	void Draw2DFlatFrame(Model & model, int width, int height);
	void DrawGray2DFrame(Model & model, Vec3f light_dir);
	void DrawGray2DFrame(Model & model, int width, int height, Vec3f light_dir);
	//3D
	void DrawFillTrangile(Triangle3i&t, const TGAColor&color);
	void DrawFillTrangile(Vec3i p0, Vec3i p1, Vec3i p2, const TGAColor&color);
	void DrawFillTrangile(Triangle3f&t, const TGAColor&color);
	void DrawFillTrangile(Vec3f p0, Vec3f p1, Vec3f p2, const TGAColor&color);
	void DrawInterpolateTrangile(Triangle3i&t, const TGAColor&color);
	void DrawInterpolateTrangile(Vec3i p0, Vec3i p1, Vec3i p2, const TGAColor&color);
	void DrawInterpolateTrangile(Triangle3f&t, const TGAColor&color);
	void DrawInterpolateTrangile(Vec3f p0, Vec3f p1, Vec3f p2, const TGAColor&color);
	void DrawGrayFrame(Model & model, Vec3f light_dir);
	void DrawGrayFrame(Model & model, int width, int height, Vec3f light_dir);
	void DrawColorfulFrame(Model & model);
	void DrawColorfulFrame(Model & model, int width, int height);


	//�任
	void viewport(int x, int y, int w, int h);
	void Prespect_projection(float coeff = 0.f); // coeff = -1/c ͸��ͶӰ
	void projection();//����ͶӰ
	Matrix GetModelView() { return ModelView; }
	Matrix GetProjection() { return Projection; }
	Matrix GetViewPort() { return Viewport; }
};

