#include"TGAImage.h"
#include "line.h"
/*
ȷ������,
������仭��,
ȡ��С��ʱ�䲽��dt,ÿ����dt���ڴ�����Ϳɫ
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

//�޷�����б��Ϊ������
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
Bresenham�㷨��
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

void DrawWireframe(Model & model)
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
			lineBresenham(x0, y0, x1, y1, image, white);
		}
	}
}



