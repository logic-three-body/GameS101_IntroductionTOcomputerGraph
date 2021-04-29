#include <vector>
#include <cmath>
#include <limits>
#include "tgaimage.h"
#include "geometry.h"
#include<string>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);
const int width = 800;
const int height = 500;

void line(Vec2i p0, Vec2i p1, TGAImage &image, TGAColor color) {
	bool steep = false;
	if (std::abs(p0.x - p1.x) < std::abs(p0.y - p1.y)) {
		std::swap(p0.x, p0.y);
		std::swap(p1.x, p1.y);
		steep = true;
	}
	if (p0.x > p1.x) {
		std::swap(p0, p1);
	}

	for (int x = p0.x; x <= p1.x; x++) {
		float t = (x - p0.x) / (float)(p1.x - p0.x);
		int y = p0.y*(1. - t) + p1.y*t + .5;
		if (steep) {
			image.set(y, x, color);
		}
		else {
			image.set(x, y, color);
		}
	}
}

void rasterize(Vec2i p0, Vec2i p1, TGAImage &image, TGAColor color, int ybuffer[]) {
	if (p0.x > p1.x) {
		std::swap(p0, p1);
	}
	for (int x = p0.x; x <= p1.x; x++) {
		float t = (x - p0.x) / (float)(p1.x - p0.x);
		int y = p0.y*(1. - t) + p1.y*t + .5;//插值
		if (ybuffer[x] > y) {
			ybuffer[x] = y;//永远保留最小的
			image.set(x, 0, color);
		}
	}
}

int main(int argc, char** argv) {
	{ // just dumping the 2d scene (yay we have enough dimensions!)
		TGAImage scene(width, height, TGAImage::RGB);

		// scene "2d mesh"
		line(Vec2i(20, 34), Vec2i(700, 30), scene, red);
		line(Vec2i(20, 34), Vec2i(730, 80), scene, green);
		line(Vec2i(20, 34), Vec2i(750, 110), scene, blue);

		// screen line
		line(Vec2i(10, 10), Vec2i(790, 10), scene, white);

		//scene.flip_vertically(); // i want to have the origin at the left bottom corner of the image
		scene.write_tga_file("Lesson3Zbuffer/YBuffer/scene1.tga");
	}

	{
		TGAImage render(width, 16, TGAImage::RGB);
		int ybuffer[width];
		for (int i = 0; i < width; i++) {
			ybuffer[i] = std::numeric_limits<int>::max();
		}
		rasterize(Vec2i(20, 34), Vec2i(744, 400), render, red, ybuffer);
		rasterize(Vec2i(120, 434), Vec2i(444, 400), render, green, ybuffer);
		rasterize(Vec2i(330, 463), Vec2i(594, 200), render, blue, ybuffer);


		// 1-pixel wide image is bad for eyes, lets widen it
		for (int i = 0; i < width; i++) {
			for (int j = 1; j < 16; j++) {
				render.set(i, j, render.getColor(i, 0));
			}
		}
		//render.flip_vertically(); // i want to have the origin at the left bottom corner of the image
		render.write_tga_file("Lesson3Zbuffer/YBuffer/renderF.tga");
	}
	return 0;
}