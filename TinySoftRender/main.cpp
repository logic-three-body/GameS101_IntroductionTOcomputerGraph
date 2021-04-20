#include"TGAImage.h"
#include"line.h"
#include"global.h"





int main()
{
	TGAImage image(100, 100, TGAImage::RGB);
	lineSegment(13, 20, 80, 40, image, white);
	/*
	flip_vertically的作用，如果学过数字图像处理bmp文件的读取，像素的原点（起始点）可能为左上角，
	作者垂直反转图片是为了方便读者的固有习惯，即原点在左下角
	*/
	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("Lesson1Line/BigStepline.tga");
	return 0;
}


