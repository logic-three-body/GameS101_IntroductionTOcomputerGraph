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
	value = value % 255;  // 生成 0~255 的随机数  
	RNG rng;
	int aa = rng.uniform(0, value);
	int bb = rng.uniform(0, value);
	int cc = rng.uniform(0, value);
	return Vec3b(aa, bb, cc);
}

void watershed(Mat& the_image)
{
	Mat image = the_image;    // 载入 RGB 彩色图像  
//	imshow("Source Image", image);

	// 灰度化，滤波，Canny 边缘检测  
	Mat imageGray;
	cvtColor(image, imageGray, CV_RGB2GRAY);// 灰度转换  
	GaussianBlur(imageGray, imageGray, Size(5, 5), 2);   // 高斯滤波  
//	imshow("Gray Image", imageGray);
	Canny(imageGray, imageGray, 80, 150);
	//	imshow("Canny Image", imageGray);

		// 查找轮廓  
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(imageGray, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point());
	Mat imageContours = Mat::zeros(image.size(), CV_8UC1);  // 轮廓     
	Mat marks(image.size(), CV_32S);   //Opencv 分水岭第二个矩阵参数  
	marks = Scalar::all(0);
	int index = 0;
	int compCount = 0;
	for (; index >= 0; index = hierarchy[index][0], compCount++)
	{
		// 对 marks 进行标记，对不同区域的轮廓进行编号，相当于设置注水点，有多少轮廓，就有多少注水点  
		drawContours(marks, contours, index, Scalar::all(compCount + 1), 1, 8, hierarchy);
		drawContours(imageContours, contours, index, Scalar(255), 1, 8, hierarchy);
	}

	// 传入的矩阵 marks 里
	Mat marksShows;
	convertScaleAbs(marks, marksShows);
	//	imshow("marksShow", marksShows);
	//	imshow ("轮廓", imageContours);
	watershed(image, marks);

	// 分水岭算法之后的矩阵 marks
	Mat afterWatershed;
	convertScaleAbs(marks, afterWatershed);
	imshow("After Watershed", afterWatershed);

	// 对每一个区域进行颜色填充  
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

	// 分割并填充颜色的结果跟原始图像融合  
	Mat wshed;
	addWeighted(image, 0.4, PerspectiveImage, 0.6, 0, wshed);
	imshow("AddWeighted Image", wshed);

	waitKey(10);

}

int main()
{
	int loop;
	cout << "请输入你的选择，0阈值分割，1分水岭算法：" << endl;
	cin >> loop;
	if (loop == 0)
	{
		long currentFrame = 1;
		float p;
		VideoCapture cap;
		// 这里放置需要提取关键字的视频
		cap.open("exp1.avi");
		if (!cap.isOpened())// 如果视频不能正常打开则返回
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
			imshow("当前视频", frame);
			waitKey(10);

			Mat srcImage_base;
			Mat srcImage_test1;
			srcImage_base = frame_key;
			srcImage_test1 = frame;
			// 将图像从 BGR 色彩空间转换到 HSV 色彩空间
			cvtColor(srcImage_base, previousImage, CV_BGR2GRAY);
			cvtColor(srcImage_test1, currentImage, CV_BGR2GRAY);

			absdiff(currentImage, previousImage, resultImage);  // 帧差法，相减

			threshold(resultImage, resultImage, 10, 255.0, CV_THRESH_BINARY); // 二值化，像素值相差大于 20 则置为 255，其余为 0
			float counter = 0;
			float num = 0;
			// 统计两帧相减后图像素
			for (int i = 0; i < resultImage.rows; i++)
			{
				uchar* data = resultImage.ptr<uchar>(i); // 获取每一行的指针
				for (int j = 0; j < resultImage.cols; j++)
				{
					num = num + 1;
					if (data[j] == 255) // 访问到像素值
					{
						counter = counter + 1;
					}
				}
			}
			p = counter / num;

			if (p > 0.6) // 输出关键帧
			{
				frame_key = frame;
				imshow("关键帧视频", frame_key);
				waitKey(10);

				cvtColor(frame_key, previousImage, CV_BGR2GRAY);
				threshold(previousImage, previousImage, 80, 250, THRESH_BINARY);
				imshow("OTUS大津法阈值分割", previousImage);
				adaptiveThreshold(currentImage, resultImage, 255, 0, 0, 7, 9);
				imshow("自适应阈值分割", previousImage);
				waitKey(10);
			}

		}
	}
	else if (loop == 1)
	{
		long currentFrame = 1;
		float p;
		VideoCapture cap;
		// 这里放置需要提取关键字的视频
		cap.open("exp1.avi");
		if (!cap.isOpened())// 如果视频不能正常打开则返回
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
			imshow("当前视频", frame);
			waitKey(10);

			Mat srcImage_base;
			Mat srcImage_test1;
			srcImage_base = frame_key;
			srcImage_test1 = frame;
			// 将图像从 BGR 色彩空间转换到 HSV 色彩空间
			cvtColor(srcImage_base, previousImage, CV_BGR2GRAY);
			cvtColor(srcImage_test1, currentImage, CV_BGR2GRAY);

			absdiff(currentImage, previousImage, resultImage);  // 帧差法，相减

			threshold(resultImage, resultImage, 10, 255.0, CV_THRESH_BINARY); // 二值化，像素值相差大于 20 则置为 255，其余为 0

			float counter = 0;
			float num = 0;
			// 统计两帧相减后图像素
			for (int i = 0; i < resultImage.rows; i++)
			{
				uchar* data = resultImage.ptr<uchar>(i); // 获取每一行的指针
				for (int j = 0; j < resultImage.cols; j++)
				{
					num = num + 1;
					if (data[j] == 255) // 访问到像素值
					{
						counter = counter + 1;
					}
				}
			}
			p = counter / num;

			if (p > 0.6) // 输出关键帧
			{
				frame_key = frame;
				//				imshow ("关键帧视频", frame_key);
				waitKey(10);
				watershed(frame_key);
				//				imshow ("关键帧视频", frame_key);
				waitKey(10);
			}
		}
	}
}