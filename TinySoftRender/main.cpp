#include"FrameWork.h"
#include"global.h"
int main(int argc, char** argv) {
	auto path1 = "Lesson3Zbuffer/ZBuffer/Test/test11.tga";
	auto path2 = "Lesson3Zbuffer/ZBuffer/Test/test1.tga";
	rasterizer r(width,height);
	r.InitZBuffer();
	Vec3f light_dir(0, 0, -1);
	r.DrawColorfulFrame(model);
	r.WriteBuffer(path1);
	//r.ClearBuffer();
	//r.DrawFlatFrame(model);
	//r.WriteBuffer(path2);

	return 0;
}