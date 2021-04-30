#include"FrameWork.h"
#include"global.h"
int main(int argc, char** argv) {
	auto path1 = "Lesson3Zbuffer/ZBuffer/Test/test.tga";
	auto path2 = "Lesson3Zbuffer/ZBuffer/Test/test1.tga";
	rasterizer r(width,height);
	r.InitZBuffer();
	Vec3f light_dir(0, 0, -1);
	for (int i = 0; i < model.nfaces(); i++) {
		std::vector<int> face = model.face(i);
		Vec3i screen_coords[3];
		Vec3f world_coords[3];
		for (int j = 0; j < 3; j++) {
			Vec3f v = model.vert(face[j]);
			screen_coords[j] = Vec3i((v.x + 1.)*width / 2., (v.y + 1.)*height / 2., -(v.z + 1.)*depth / 2.);//转化到标准坐标系[0,1] 可以观察到obj文件顶点坐标有负值
			world_coords[j] = v;
		}
		Vec3f n = cross((world_coords[2] - world_coords[0]), (world_coords[1] - world_coords[0]));
		n.normalize();
		float intensity = n * light_dir;
		if (intensity > 0) {
			r.DrawFillTrangile(screen_coords[0], screen_coords[1], screen_coords[2], TGAColor(intensity * 255, intensity * 255, intensity * 255, 255));
		}
	}

	r.WriteBuffer(path1);
	//r.ClearBuffer();
	//r.DrawFlatFrame(model);
	//r.WriteBuffer(path2);

	return 0;
}