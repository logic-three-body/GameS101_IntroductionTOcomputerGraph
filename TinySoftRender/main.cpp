#include"FrameWork.h"
#include"global.h"
int main(int argc, char** argv) {
	auto path1 = "Lesson4Perspective/Shark/test3.tga";
	auto path2 = "Lesson4Perspective/Test/test1.tga";
	rasterizer r(width,height);
	r.InitZBuffer();
	r.viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4);
	r.Prespect_projection(-1.f / camera.z);
	r.DrawModelFrame(model, light_dir);
	r.WriteBuffer(path1);
	//r.ClearBuffer();
	//r.WriteBuffer(path2);
	return 0;
}