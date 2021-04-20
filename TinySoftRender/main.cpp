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
	flip_vertically�����ã����ѧ������ͼ����bmp�ļ��Ķ�ȡ�����ص�ԭ�㣨��ʼ�㣩����Ϊ���Ͻǣ�
	���ߴ�ֱ��תͼƬ��Ϊ�˷�����ߵĹ���ϰ�ߣ���ԭ�������½�
	*/
	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("Lesson1Line/line.tga");
	return 0;
}

/*
ȷ������,
������仭��,
ȡ��С��ʱ�䲽��dt,ÿ����dt���ڴ�����Ϳɫ
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
