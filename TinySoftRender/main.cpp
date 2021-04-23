#include <vector>
#include <cmath>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"
#include"line.h"
#include"global.h"



int main(int argc, char** argv) {
	const char*new_path = "obj/diablo3_pose/diablo3_pose.obj";
	model.clear();
	model.Update(new_path);
	for (int i = 0; i < model.nfaces(); i++) {
		std::vector<int> face = model.face(i);
		for (int j = 0; j < 3; j++) {
			Vec3f v0 = model.vert(face[j]);
			Vec3f v1 = model.vert(face[(j + 1) % 3]);
			int x0 = (v0.x + 1.)*width / 2.;
			int y0 = (v0.y + 1.)*height / 2.;
			int x1 = (v1.x + 1.)*width / 2.;
			int y1 = (v1.y + 1.)*height / 2.;
			lineBresenham(x0, y0, x1, y1, image, white);
		}
	}
	image.write_tga_file("Lesson1Line/Wireframe/output8.tga");
	return 0;
}