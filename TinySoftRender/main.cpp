#include <vector>
#include <cmath>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"
#include"line.h"
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
Model *model = NULL;
const int width = 800;
const int height = 800;



int main(int argc, char** argv) {
	if (2 == argc) {
		model = new Model(argv[1]);
	}
	else {
		model = new Model("obj/african_head/african_head.obj");
	}

	TGAImage image(width, height, TGAImage::RGB);
	for (int i = 0; i < model->nfaces(); i++) {
		std::vector<int> face = model->face(i);
		for (int j = 0; j < 3; j++) {
			Vec3f v0 = model->vert(face[j]);
			Vec3f v1 = model->vert(face[(j + 1) % 3]);
			int x0 = (v0.x + 1.)*width / 2.;
			int y0 = (v0.y + 1.)*height / 2.;
			int x1 = (v1.x + 1.)*width / 2.;
			int y1 = (v1.y + 1.)*height / 2.;
			//line(x0, y0, x1, y1, image, white);
			lineBresenham(x0, y0, x1, y1, image, white);
		}
	}
	//lineBresenham(13, 20, 80, 40, image, white);
	//image.write_tga_file("Lesson1Line/Third attempt/output4.tga");
	//lineBresenham(20, 13, 40, 80, image, red);
	//image.write_tga_file("Lesson1Line/Third attempt/output5.tga");
	//line(80, 40, 13, 20, image, green);
	//image.write_tga_file("Lesson1Line/Third attempt/output6.tga");

	image.write_tga_file("Lesson1Line/Wireframe/output2.tga");
	delete model;
	return 0;
}