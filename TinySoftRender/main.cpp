﻿#include"FrameWork.h"
#include"global.h"
int main(int argc, char** argv) {
	auto path1 = "Lesson2rasterizer/Flat/model0.tga";
	rasterizer r(width,height);
	Vec2i pts[3] = { Vec2i(10,10), Vec2i(100, 30), Vec2i(190, 160) };
	Trianglei pt(pts);
	r.DrawFlatFrame(model);
	r.WriteBuffer(path1);
	return 0;
}