#include"FrameWork.h"
#include"global.h"
int main(int argc, char** argv) {
	auto path1 = "Lesson3Zbuffer/ZBuffer/Test/test10.tga";
	auto path2 = "Lesson3Zbuffer/ZBuffer/Test/test1.tga";
	rasterizer r(width,height);
	r.InitZBuffer();
	Vec3f light_dir(0, 0, -1);
	r.DrawGrayFrame(model,light_dir);
	//for (int i = 0; i < model.nfaces(); i++) {
	//	std::vector<int> face = model.face(i);
	//	Vec3f screen_coords[3];
	//	Vec3f world_coords[3];
	//	for (int i = 0; i < 3; i++) screen_coords[i] = r.world2screen(model.vert(face[i]));
	//	Triangle3f t(screen_coords);
	//	for (int j = 0; j < 3; j++) {
	//		Vec3f v = model.vert(face[j]);
	//		world_coords[j] = v;
	//	}
	//	Vec3f n = cross((world_coords[2] - world_coords[0]), (world_coords[1] - world_coords[0])).normalize();
	//	float intensity = n * light_dir.normalize();	
	//	if (0<intensity)
	//	{
	//		r.DrawInterpolateTrangile(t, TGAColor(int(intensity * 255)%255, int(intensity * 255)%255, int(intensity * 255)%255, 255));
	//	}
	//}

	r.WriteBuffer(path1);
	//r.ClearBuffer();
	//r.DrawFlatFrame(model);
	//r.WriteBuffer(path2);

	return 0;
}