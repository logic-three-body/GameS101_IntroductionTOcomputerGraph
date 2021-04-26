﻿#include"FrameWork.h"
#include"global.h"
void line(Vec2i p0, Vec2i p1, TGAImage &image, TGAColor color) {
	bool steep = false;
	if (std::abs(p0.x - p1.x) < std::abs(p0.y - p1.y)) {
		std::swap(p0.x, p0.y);
		std::swap(p1.x, p1.y);
		steep = true;
	}
	if (p0.x > p1.x) {
		std::swap(p0, p1);
	}

	for (int x = p0.x; x <= p1.x; x++) {
		float t = (x - p0.x) / (float)(p1.x - p0.x);
		int y = p0.y*(1. - t) + p1.y*t;
		if (steep) {
			image.set(y, x, color);
		}
		else {
			image.set(x, y, color);
		}
	}
}
void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color) {
	// sort the vertices, t0, t1, t2 lower−to−upper (bubblesort yay!) 
	if (t0.y > t1.y) std::swap(t0, t1);
	if (t0.y > t2.y) std::swap(t0, t2);
	if (t1.y > t2.y) std::swap(t1, t2);
	line(t0, t1, image, green);
	line(t1, t2, image, green);
	line(t2, t0, image, red);
}
void FilltriangleUpper(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color) {//draw horizontally
	// sort the vertices, t0, t1, t2 lower−to−upper (bubblesort yay!) 
	if (t0.y > t1.y) std::swap(t0, t1);
	if (t0.y > t2.y) std::swap(t0, t2);
	if (t1.y > t2.y) std::swap(t1, t2);
	int total_height = t2.y - t0.y;//计算高度
	for (int y = t0.y; y <= t1.y; y++) {
		int segment_height = t1.y - t0.y + 1;
		float alpha = (float)(y - t0.y) / total_height;
		float beta = (float)(y - t0.y) / segment_height; // be careful with divisions by zero 
		Vec2i A = t0 + (t2 - t0)*alpha;
		Vec2i B = t0 + (t1 - t0)*beta;
		image.set(A.x, y, red);
		image.set(B.x, y, green);
		if (A.x > B.x) std::swap(A, B);//水平扫描
		for (int j = A.x; j <= B.x; j++) {
			image.set(j, y, color); // attention, due to int casts t0.y+i != A.y 
		}
	}
}

void FilltriangleLower(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color)
{
	// sort the vertices, t0, t1, t2 lower−to−upper (bubblesort yay!) 
	if (t0.y > t1.y) std::swap(t0, t1);
	if (t0.y > t2.y) std::swap(t0, t2);
	if (t1.y > t2.y) std::swap(t1, t2);
	int total_height = t2.y - t0.y;
	for (int y = t1.y; y <= t2.y; y++) {
		int segment_height = t2.y - t1.y + 1;
		float alpha = (float)(y - t0.y) / total_height;
		float beta = (float)(y - t1.y) / segment_height; // be careful with divisions by zero 
		Vec2i A = t0 + (t2 - t0)*alpha;
		Vec2i B = t1 + (t2 - t1)*beta;
		if (A.x > B.x) std::swap(A, B);//水平扫描
		for (int j = A.x; j <= B.x; j++) {
			image.set(j, y, color); // attention, due to int casts t0.y+i != A.y 
		}
	}
}
int main(int argc, char** argv) {
	auto path1 = "Lesson2rasterizer/Trangile/test2.tga";
	rasterizer r(width,height);
	r.DrawWireFrame(model,white);
	r.WriteBuffer(path1);
	r.ReadBuffer(path1);
	return 0;
}