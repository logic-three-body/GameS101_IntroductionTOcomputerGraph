#include"TGAImage.h"
#include"line.h"
#include"global.h"
#include"model.h"
#include<cmath>




int main()
{
	Model *model = nullptr;
	model = new Model("obj/african_head/african_head.obj");
	TGAImage img(width, height, TGAImage::RGB);
	for (int i = 0; i < model->nfaces(); i++)
	{
		std::vector<int> face = model->face(i);
		for (int j = 0; j < 3; j++)
		{
			Vec3f v0 = model->vert(face[j]);
			Vec3f v1 = model->vert(face[(j+1)%3]);
			int x0 = (v0.x+1.0)*width/2.0;
			int y0 = (v0.y + 1.0)*height / 2.0;
			int x1 = (v1.x + 1.0)*width / 2.0;
			int y1 = (v1.y + 1.0)*height / 2.0;
			lineBresenham(x0,y0,x1,y1,img,white);
		}
	}
	img.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	img.write_tga_file("Lesson1Line/Wireframe/output.tga");
	delete model;
	return 0;
}


