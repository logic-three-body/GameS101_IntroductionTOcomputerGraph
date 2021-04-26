﻿#include"FrameWork.h"
#include"global.h"
int main(int argc, char** argv) {
	auto path1 = "Lesson2rasterizer/Flat/model7.tga";
	auto path2 = "Lesson2rasterizer/Flat/model8.tga";
	rasterizer r(width,height);
	Vec2i pts[3] = { Vec2i(10,10), Vec2i(100, 30), Vec2i(190, 160) };
	Trianglei pt(pts);
	Vec3f light_dir(0,0,-1);
	r.DrawGrayFrame(model, light_dir.normalize());
	r.WriteBuffer(path1);
	r.ClearBuffer();
	r.DrawFlatFrame(model);
	r.WriteBuffer(path2);

	return 0;
}