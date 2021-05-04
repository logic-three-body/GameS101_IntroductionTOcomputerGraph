#include"FrameWork.h"
#include"global.h"
int main(int argc, char** argv) {
	auto path1 = "Lesson5Camera/test4.tga";
	auto path2 = "Lesson5Camera/test1.tga";
	rasterizer r(width,height);
	r.InitZBuffer();
	r.viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4, depth);
	float cof= -1.f / (eye - center).norm();
	r.Prespect_projection(cof);
	//r.projection();
	r.lookat(eye, center, Vec3f(-4.074089 ,1.102792 ,- 0.125490));
	r.DrawModelFrame(model, light_dir);
	r.WriteBuffer(path1);
	//r.ClearBuffer();
	//r.WriteBuffer(path2);
	return 0;
}