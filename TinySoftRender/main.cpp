#include"TGAImage.h"
#include"line.h"
#include"global.h"





int main()
{
	TGAImage image(100, 100, TGAImage::RGB);
	lineSegment(13, 20, 80, 40, image, white);
	/*
	flip_vertically�����ã����ѧ������ͼ����bmp�ļ��Ķ�ȡ�����ص�ԭ�㣨��ʼ�㣩����Ϊ���Ͻǣ�
	���ߴ�ֱ��תͼƬ��Ϊ�˷�����ߵĹ���ϰ�ߣ���ԭ�������½�
	*/
	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("Lesson1Line/BigStepline.tga");
	return 0;
}


