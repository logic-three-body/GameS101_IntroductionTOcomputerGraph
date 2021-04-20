#include"TGAImage.h"
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);

//draw line
void line(int x0, int y0, int x1, int y1, TGAImage&img, const TGAColor&color);


int main()
{
	TGAImage image(100, 100, TGAImage::RGB);
	line(13, 20, 80, 40, image, white);
	/*
	flip_vertically的作用，如果学过数字图像处理bmp文件的读取，像素的原点（起始点）可能为左上角，
	作者垂直反转图片是为了方便读者的固有习惯，即原点在左下角
	*/
	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("Lesson1Line/line.tga");
	return 0;
}

/*
确定两点,
在两点间画线,
取极小的时间步长dt,每增长dt就在此像素涂色
*/
void line(int x0, int y0, int x1, int y1, TGAImage&img, const TGAColor&color)
{
	float length = 1.0f;
	for (float dt = 0.0f; dt < length; dt += 0.01f)
	{
		int x = x0 + (x1 - x0)*dt;
		int y = y0 + (y1 - y0)*dt;
		img.set(x, y, color);
	}
}
