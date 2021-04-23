#include"TGAImage.h"
#include"line.h"
#include"global.h"





int main()
{
	TGAImage image(100, 100, TGAImage::RGB);
	//lineNaiveSegment(100, 100, 0, 0, image, white);//first
	lineDDAbad(13, 20, 80, 40, image, white);//Second
	image.write_tga_file("Lesson1Line/Second attempt/lineNaiveSegment1s.tga");
	//lineDDAbad(20, 13, 40, 80, image, red);//Second
	//image.write_tga_file("Lesson1Line/Second attempt/lineNaiveSegment2.tga");
	lineDDAbad(80, 40, 13, 20, image, green);//Second
	image.write_tga_file("Lesson1Line/Second attempt/lineNaiveSegment23.tga");

	/*
	flip_vertically的作用，如果学过数字图像处理bmp文件的读取，像素的原点（起始点）可能为左上角，
	作者垂直反转图片是为了方便读者的固有习惯，即原点在左下角
	*/
	//image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	//image.write_tga_file("Lesson1Line/Final/lineNaiveSegment3points.tga");
	return 0;
}


