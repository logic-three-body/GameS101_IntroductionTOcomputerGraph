#include"FrameWork.h"
#include"global.h"
Vec3f m2v(mat<4, 1, float> m) {//矩阵至顶点
	return Vec3f(m[0][0] / m[3][0], m[1][0] / m[3][0], m[2][0] / m[3][0]);
}
mat<4, 1, float> v2m(Vec3f v) {//顶点至矩阵
	mat<4, 1, float> m;
	m[0][0] = v.x;
	m[1][0] = v.y;
	m[2][0] = v.z;
	m[3][0] = 1.f;
	return m;
}
int main(int argc, char** argv) {
	auto path1 = "Lesson3Zbuffer/ZBuffer/Test/test13.tga";
	auto path2 = "Lesson3Zbuffer/ZBuffer/Test/test1.tga";
	rasterizer r(width,height);
	r.InitZBuffer();
	r.viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4);
	r.Prespect_projection(-1.f / camera.z);

	for (int i = 0; i < model.nfaces(); i++) {
		std::vector<int> face = model.face(i);
		Vec3f screen_coords[3];
		Vec3f world_coords[3];
		for (int j = 0; j < 3; j++) {
			Vec3f v = model.vert(face[j]);
			screen_coords[j] = m2v(r.GetViewPort()*r.GetProjection()*v2m(v));
			world_coords[j] = v;
		}
		Vec3f n = cross((world_coords[2] - world_coords[0]), (world_coords[1] - world_coords[0]));
		n.normalize();
		float intensity = n * light_dir;
		if (intensity > 0) {
			r.DrawInterpolateTrangile(screen_coords[0], screen_coords[1], screen_coords[2], TGAColor(int(intensity * 255) % 255, int(intensity * 255) % 255, int(intensity * 255) % 255, 255));
		}
	}


	r.WriteBuffer(path1);
	//r.ClearBuffer();
	//r.DrawFlatFrame(model);
	//r.WriteBuffer(path2);

	return 0;
}