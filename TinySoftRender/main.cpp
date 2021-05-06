#include"FrameWork.h"
#include"global.h"
int main(int argc, char** argv) {
	auto path1 = "TriangleTransform/test.tga";
	auto path2 = "Lesson5Camera/test1.tga";
	rasterizer r(width,height);
	r.InitZBuffer();
	r.viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4, depth);
	float cof= -1.f / (eye - center).norm();
	r.Prespect_projection(cof);
	//r.projection();
	r.lookat(eye, center, Vec3f(0,0,1));



	Vec3f p1(0, 0, 1), p2(1, 0, 0), p3(0, 1, 0);
	Vec3f pt[3];
	pt[0] = r.world2screen(p1);
	pt[1] = r.world2screen(p2);
	pt[3] = r.world2screen(p3);
	Triangle3f t(pt);
	r.DrawInterpolateTrangile(t,white);
	//r.DrawModelFrame(model, light_dir);
	r.WriteBuffer(path1);
	//r.ClearBuffer();
	//r.WriteBuffer(path2);
	return 0;
}