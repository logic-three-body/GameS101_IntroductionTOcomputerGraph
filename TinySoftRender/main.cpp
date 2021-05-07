#include"FrameWork.h"
#include"global.h"
int main(int argc, char** argv) {
	auto path1 = "TriangleTransform/test29.tga";
	auto path2 = "Lesson5Camera/model5.tga";
	rasterizer r(width,height);
	r.InitZBuffer();
	r.viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4, depth);
	float cof= -1.f / (eye - center).norm();
	//r.Prespect_projection(cof);
	r.projection();
	float scale = 1.0;
	//r.lookat(eye, center*scale, up);
	r.lookatZ();
	r.DrawModelFrame(model, light_dir);
	r.WriteBuffer(path2);
	//r.ClearBuffer();
	//r.WriteBuffer(path2);
	return 0;
}