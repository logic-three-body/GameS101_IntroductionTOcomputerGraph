#include"FrameWork.h"
#include"global.h"
int main(int argc, char** argv) {
	auto path1 = "Lesson3Zbuffer/YBuffer/test.tga";
	auto path2 = "Lesson3Zbuffer/YBuffer/test1.tga";
	rasterizer r(width,height);
	r.lineBresenham(Vec2i(20, 34), Vec2i(744, 400), red);
	r.lineBresenham(Vec2i(120, 434), Vec2i(444, 400), green);
	r.lineBresenham(Vec2i(330, 463), Vec2i(594, 200), blue);

	r.lineBresenham(Vec2i(10, 10), Vec2i(790, 10), white);

	r.WriteBuffer(path1);
	//r.ClearBuffer();
	//r.DrawFlatFrame(model);
	//r.WriteBuffer(path2);

	return 0;
}