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
	flip_vertically�����ã����ѧ������ͼ����bmp�ļ��Ķ�ȡ�����ص�ԭ�㣨��ʼ�㣩����Ϊ���Ͻǣ�
	���ߴ�ֱ��תͼƬ��Ϊ�˷�����ߵĹ���ϰ�ߣ���ԭ�������½�
	*/
	//image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	//image.write_tga_file("Lesson1Line/Final/lineNaiveSegment3points.tga");
	return 0;
}


