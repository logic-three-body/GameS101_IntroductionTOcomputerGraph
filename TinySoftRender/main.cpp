#include"FrameWork.h"
#include"global.h"
int main(int argc, char** argv) {
	auto path1 = "Lesson2rasterizer/Trangile/trangile2.tga";
	rasterizer r(width,height);
	Vec2i pts[3] = { Vec2i(10,10), Vec2i(100, 30), Vec2i(190, 160) };
	Vec2i t0[3] = { Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80) };
	Trianglei pt(t0);
	r.DrawFillTrangile(pt, white);
	r.WriteBuffer(path1);
	return 0;
}