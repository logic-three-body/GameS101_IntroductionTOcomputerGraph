#include"FrameWork.h"
#include"global.h"
int main(int argc, char** argv) {
	auto path1 = "Lesson3Zbuffer/ZBuffer/Test/test4.tga";
	auto path2 = "Lesson3Zbuffer/ZBuffer/Test/test1.tga";
	rasterizer r(width,height);
	r.InitZBuffer();
	Vec3f light_dir(0, 0, -1);
	for (int i = 0; i < model.nfaces(); i++) {
		std::vector<int> face = model.face(i);
		Vec3f pts[3];
		for (int i = 0; i < 3; i++) pts[i] = r.world2screen(model.vert(face[i]));
		Triangle3f t(pts);
		r.DrawInterpolateTrangile(t,TGAColor(rand() % 255, rand() % 255, rand() % 255, 255));
	}

	r.WriteBuffer(path1);
	//r.ClearBuffer();
	//r.DrawFlatFrame(model);
	//r.WriteBuffer(path2);

	return 0;
}