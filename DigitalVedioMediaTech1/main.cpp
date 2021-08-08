#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/ml/ml.hpp>
#include <string.h>
/*
https://blog.csdn.net/qq_42026651/article/details/108195943
*/
#include <opencv2\imgproc\types_c.h>

using namespace std;
using namespace cv;
using namespace std;

Vec3b RandomColor(int value)
{
	value = value % 255;  // ���� 0~255 �������  
	RNG rng;
	int aa = rng.uniform(0, value);
	int bb = rng.uniform(0, value);
	int cc = rng.uniform(0, value);
	return Vec3b(aa, bb, cc);
}

void watershed(Mat& the_image)
{
	Mat image = the_image;    // ���� RGB ��ɫͼ��  
//	imshow("Source Image", image);

	// �ҶȻ����˲���Canny ��Ե���  
	Mat imageGray;
	cvtColor(image, imageGray, CV_RGB2GRAY);// �Ҷ�ת��  
	GaussianBlur(imageGray, imageGray, Size(5, 5), 2);   // ��˹�˲�  
//	imshow("Gray Image", imageGray);
	Canny(imageGray, imageGray, 80, 150);
	//	imshow("Canny Image", imageGray);

		// ��������  
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(imageGray, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point());
	Mat imageContours = Mat::zeros(image.size(), CV_8UC1);  // ����     
	Mat marks(image.size(), CV_32S);   //Opencv ��ˮ��ڶ����������  
	marks = Scalar::all(0);
	int index = 0;
	int compCount = 0;
	for (; index >= 0; index = hierarchy[index][0], compCount++)
	{
		// �� marks ���б�ǣ��Բ�ͬ������������б�ţ��൱������עˮ�㣬�ж������������ж���עˮ��  
		drawContours(marks, contours, index, Scalar::all(compCount + 1), 1, 8, hierarchy);
		drawContours(imageContours, contours, index, Scalar(255), 1, 8, hierarchy);
	}

	// ����ľ��� marks ��
	Mat marksShows;
	convertScaleAbs(marks, marksShows);
	//	imshow("marksShow", marksShows);
	//	imshow ("����", imageContours);
	watershed(image, marks);

	// ��ˮ���㷨֮��ľ��� marks
	Mat afterWatershed;
	convertScaleAbs(marks, afterWatershed);
	imshow("After Watershed", afterWatershed);

	// ��ÿһ�����������ɫ���  
	Mat PerspectiveImage = Mat::zeros(image.size(), CV_8UC3);
	for (int i = 0; i < marks.rows; i++)
	{
		for (int j = 0; j < marks.cols; j++)
		{
			int index = marks.at<int>(i, j);
			if (marks.at<int>(i, j) == -1)
			{
				PerspectiveImage.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
			}
			else
			{
				PerspectiveImage.at<Vec3b>(i, j) = RandomColor(index);
			}
		}
	}
	imshow("After ColorFill", PerspectiveImage);

	// �ָ�����ɫ�Ľ����ԭʼͼ���ں�  
	Mat wshed;
	addWeighted(image, 0.4, PerspectiveImage, 0.6, 0, wshed);
	imshow("AddWeighted Image", wshed);

	waitKey(10);

}

int main()
{
	int loop;
	cout << "���������ѡ��0��ֵ�ָ1��ˮ���㷨��" << endl;
	cin >> loop;
	if (loop == 0)
	{
		long currentFrame = 1;
		float p;
		VideoCapture cap;
		// ���������Ҫ��ȡ�ؼ��ֵ���Ƶ
		cap.open("exp1.avi");
		if (!cap.isOpened())// �����Ƶ�����������򷵻�
		{
			cout << "cannot open video!" << endl;
			return 0;
		}
		Mat frame_key;
		cap >> frame_key;
		if (frame_key.empty())
			cout << "frame_key is empty!" << endl;
		//imshow("fram_1", frame_key);
		waitKey(10);

		Mat frame;
		Mat previousImage, currentImage, resultImage;
		while (1)
		{

			currentFrame++;
			Mat frame;
			cap >> frame;

			if (frame.empty())
			{
				cout << "frame is empty!" << endl;
				break;
			}
			imshow("��ǰ��Ƶ", frame);
			waitKey(10);

			Mat srcImage_base;
			Mat srcImage_test1;
			srcImage_base = frame_key;
			srcImage_test1 = frame;
			// ��ͼ��� BGR ɫ�ʿռ�ת���� HSV ɫ�ʿռ�
			cvtColor(srcImage_base, previousImage, CV_BGR2GRAY);
			cvtColor(srcImage_test1, currentImage, CV_BGR2GRAY);

			absdiff(currentImage, previousImage, resultImage);  // ֡������

			threshold(resultImage, resultImage, 10, 255.0, CV_THRESH_BINARY); // ��ֵ��������ֵ������ 20 ����Ϊ 255������Ϊ 0
			float counter = 0;
			float num = 0;
			// ͳ����֡�����ͼ����
			for (int i = 0; i < resultImage.rows; i++)
			{
				uchar* data = resultImage.ptr<uchar>(i); // ��ȡÿһ�е�ָ��
				for (int j = 0; j < resultImage.cols; j++)
				{
					num = num + 1;
					if (data[j] == 255) // ���ʵ�����ֵ
					{
						counter = counter + 1;
					}
				}
			}
			p = counter / num;

			if (p > 0.6) // ����ؼ�֡
			{
				frame_key = frame;
				imshow("�ؼ�֡��Ƶ", frame_key);
				waitKey(10);

				cvtColor(frame_key, previousImage, CV_BGR2GRAY);
				threshold(previousImage, previousImage, 80, 250, THRESH_BINARY);
				imshow("OTUS�����ֵ�ָ�", previousImage);
				adaptiveThreshold(currentImage, resultImage, 255, 0, 0, 7, 9);
				imshow("����Ӧ��ֵ�ָ�", previousImage);
				waitKey(10);
			}

		}
	}
	else if (loop == 1)
	{
		long currentFrame = 1;
		float p;
		VideoCapture cap;
		// ���������Ҫ��ȡ�ؼ��ֵ���Ƶ
		cap.open("exp1.avi");
		if (!cap.isOpened())// �����Ƶ�����������򷵻�
		{
			cout << "cannot open video!" << endl;
			return 0;
		}
		Mat frame_key;
		cap >> frame_key;
		if (frame_key.empty())
			cout << "frame_key is empty!" << endl;
		//imshow("fram_1", frame_key);
		waitKey(10);

		watershed(frame_key);

		Mat frame;
		Mat previousImage, currentImage, resultImage;
		while (1)
		{

			currentFrame++;
			Mat frame;
			cap >> frame;

			if (frame.empty())
			{
				cout << "frame is empty!" << endl;
				break;
			}
			imshow("��ǰ��Ƶ", frame);
			waitKey(10);

			Mat srcImage_base;
			Mat srcImage_test1;
			srcImage_base = frame_key;
			srcImage_test1 = frame;
			// ��ͼ��� BGR ɫ�ʿռ�ת���� HSV ɫ�ʿռ�
			cvtColor(srcImage_base, previousImage, CV_BGR2GRAY);
			cvtColor(srcImage_test1, currentImage, CV_BGR2GRAY);

			absdiff(currentImage, previousImage, resultImage);  // ֡������

			threshold(resultImage, resultImage, 10, 255.0, CV_THRESH_BINARY); // ��ֵ��������ֵ������ 20 ����Ϊ 255������Ϊ 0

			float counter = 0;
			float num = 0;
			// ͳ����֡�����ͼ����
			for (int i = 0; i < resultImage.rows; i++)
			{
				uchar* data = resultImage.ptr<uchar>(i); // ��ȡÿһ�е�ָ��
				for (int j = 0; j < resultImage.cols; j++)
				{
					num = num + 1;
					if (data[j] == 255) // ���ʵ�����ֵ
					{
						counter = counter + 1;
					}
				}
			}
			p = counter / num;

			if (p > 0.6) // ����ؼ�֡
			{
				frame_key = frame;
				//				imshow ("�ؼ�֡��Ƶ", frame_key);
				waitKey(10);
				watershed(frame_key);
				//				imshow ("�ؼ�֡��Ƶ", frame_key);
				waitKey(10);
			}
		}
	}
}