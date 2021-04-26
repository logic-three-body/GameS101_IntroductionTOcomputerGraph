#include"FrameWork.h"
#include"global.h"
int main(int argc, char** argv) {
	rasterizer r(width,height);
	Vec2i t0[3] = { Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80) };
	Vec2i t1[3] = { Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180) };
	Vec2i t2[3] = { Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180) };
	r.DrawWireTrangile(t0[0], t0[1], t0[2], image,red);
	r.DrawWireTrangile(t0[2], t0[1], t0[0], image,white);
	r.DrawWireTrangile(t2[0], t2[1], t2[2], image,green);
	image.write_tga_file("Lesson2rasterizer/Trangile/output2.tga");
	return 0;
}