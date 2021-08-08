#include <iostream>  
#include "opencv2/opencv.hpp"  
/*
https://blog.csdn.net/qq_42026651/article/details/108195943
*/
#include <opencv2\imgproc\types_c.h>
using namespace cv;
using namespace std;

/*
comment
我的opencv版本是4.5.1 一些名称有所变化
*/
#if(CV_MAJOR_VERSION >= 3)
#define  CV_CAP_PROP_POS_FRAMES CAP_PROP_POS_FRAMES
#define CV_WINDOW_FREERATIO WINDOW_FREERATIO
#define CV_CAP_PROP_FRAME_COUNT CAP_PROP_FRAME_COUNT
#define CV_CAP_PROP_FPS CAP_PROP_FPS
#else
#define  CV_CAP_PROP_POS_FRAMES 
#define CV_WINDOW_FREERATIO 
#define CV_CAP_PROP_FRAME_COUNT
#define CV_CAP_PROP_FPS
#endif

Mat MoveDetect(Mat temp, Mat frame)   // 运动物体检测
{
	Mat result = frame.clone();
	Mat gray1, gray2;
	cvtColor(temp, gray1, CV_BGR2GRAY);
	cvtColor(frame, gray2, CV_BGR2GRAY);   	// 将 background 和 frame 转为灰度图  
	Mat diff;
	absdiff(gray1, gray2, diff);   	// 将 background 和 frame 做差  
	imshow("diff", diff);
	Mat diff_thresh;
	threshold(diff, diff_thresh, 50, 255, CV_THRESH_BINARY);   	// 对差值图 diff_thresh 进行阈值化处理  
	imshow("diff_thresh", diff_thresh);
	Mat kernel_erode = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat kernel_dilate = getStructuringElement(MORPH_RECT, Size(18, 18));
	erode(diff_thresh, diff_thresh, kernel_erode);       	// 腐蚀  
	imshow("erode", diff_thresh);
	dilate(diff_thresh, diff_thresh, kernel_dilate);   			// 膨胀  
	imshow("dilate", diff_thresh);
	vector<vector<Point>> contours;
	findContours(diff_thresh, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);  	// 查找轮廓并绘制轮廓  
	//drawContours (result, contours, -1, Scalar (0, 0, 255), 2);  	// 在 result 上绘制轮廓
	vector<Rect> boundRect(contours.size());
	for (int i = 0; i < contours.size(); i++)   	// 查找正外接矩形  
	{
		boundRect[i] = boundingRect(contours[i]);
		rectangle(result, boundRect[i], Scalar(0, 255, 0), 2);   		// 在 result 上绘制正外接矩形
	}
	return result;
}

int main()
{
	VideoCapture video("exp3.avi");
	if (!video.isOpened())
	{
		cout << "video open error!" << endl;
		return 0;
	}
	int frameCount = video.get(CV_CAP_PROP_FRAME_COUNT); 	// 获取帧数
	double FPS = video.get(CV_CAP_PROP_FPS); 	// 获取 FPS
	Mat frame;
	Mat temp;  			// 存储前一帧图像
	Mat result;	  		// 存储结果图像
	for (int i = 0; i < frameCount; i++)
	{
		video >> frame;
		imshow("frame", frame);
		if (frame.empty())
		{
			cout << "frame is empty!" << endl;
			break;
		}
		int framePosition = video.get(CV_CAP_PROP_POS_FRAMES);  		// 获取帧位置
		cout << "framePosition: " << framePosition << endl;          	// 输出帧位置
		if (i == 0)  		// 如果为第一帧（temp 还为空）
		{
			result = MoveDetect(frame, frame); 			// 调用 MoveDetect () 进行运动物体检测，返回值存入 result
		}
		else   		// 若不是第一帧（temp 有值了）
		{
			result = MoveDetect(temp, frame);   			// 调用 MoveDetect () 进行运动物体检测，返回值存入 result
		}
		imshow("result", result);
		if (waitKey(1000.0 / FPS) == 27)
		{
			cout << "ESC退出!" << endl;
			break;
		}
		temp = frame.clone();
	}
	return 0;
}