#include"FrameWork.h"
#include"global.h"
int main(int argc, char** argv) {
	auto path1 = "Lesson4Perspective/Shark/test2.tga";
	auto path2 = "Lesson4Perspective/Test/test1.tga";
	rasterizer r(width,height);
	r.InitZBuffer();
	r.viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4);
	r.Prespect_projection(-1.f / camera.z);
	r.Draw2DWireFrame(model,white);
	//for (int i = 0; i < model.nfaces(); i++) {
	//	std::vector<int> face = model.face(i);
	//	Vec3f screen_coords[3];
	//	Vec3f world_coords[3];
	//	for (int j = 0; j < 3; j++) {
	//		Vec3f v = model.vert(face[j]);
	//		//viewport函数不如直接world2screen效果好
	//		//screen_coords[j] = m2v(r.GetViewPort()*r.GetProjection()*v2m(v));
	//		screen_coords[j] = r.world2screen(r.m2v(r.GetProjection()*r.v2m(v)));
	//		world_coords[j] = v;
	//	}
	//	Vec3f n = cross((world_coords[2] - world_coords[0]), (world_coords[1] - world_coords[0]));
	//	n.normalize();
	//	float intensity = n * light_dir;
	//	//r.DrawInterpolateTrangile(screen_coords[0], screen_coords[1], screen_coords[2], TGAColor(rand() % 255, rand() % 255, rand() % 255, 255));
	//	if (intensity > 0) {
	//		r.DrawInterpolateTrangile(screen_coords[0], screen_coords[1], screen_coords[2], TGAColor(int(intensity * 255) % 255, int(intensity * 255) % 255, int(intensity * 255) % 255, 255));
	//		//r.DrawInterpolateTrangile(screen_coords[0], screen_coords[1], screen_coords[2], TGAColor(rand() % 255, rand() % 255, rand() % 255, 255));
	//		//r.DrawFillTrangile(screen_coords[0], screen_coords[1], screen_coords[2], TGAColor(rand() % 255, rand() % 255, rand() % 255, 255));
	//	}
	//}


	r.WriteBuffer(path1);
	//r.ClearBuffer();
	//r.WriteBuffer(path2);

	return 0;
}