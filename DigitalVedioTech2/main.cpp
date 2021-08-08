#include <opencv2/opencv.hpp>
#include<string>
//#define CV_CAP_PROP_FRAME_COUNT CAP_PROP_FRAME_COUNT 
/*
comment
我的opencv版本是4.5.1 一些名称有所变化
*/
#if(CV_MAJOR_VERSION >= 3)
#define  CV_CAP_PROP_POS_FRAMES CAP_PROP_POS_FRAMES
#define CV_WINDOW_FREERATIO WINDOW_FREERATIO
#else
#define  CV_CAP_PROP_POS_FRAMES 
#define CV_WINDOW_FREERATIO 
#endif

using namespace cv;
using namespace std;

const char* windowName = "视频播放";
int stop = 1;// 播放、暂停
int rateOfProgress = 0;// 播放进度 0-100
int contrastRatio = 50;// 对比度
int light = 255;//0-255;
int histogramEqualization = 0;
int filter = 1;
int filter2 = 1;
int filter3 = 1;
bool IsDrag = false;
class videoProcess {
public:
	Mat frame;
	videoProcess(Mat f) {
		this->frame = f;
	}
	// 调节亮度
	void adjustLight(int light) {
		light = light - 255;
		for (int r = 0; r < frame.rows; r++) {
			for (int c = 0; c < frame.cols; c++) {
				int B = frame.at<Vec3b>(r, c)[0];
				int G = frame.at<Vec3b>(r, c)[1];
				int R = frame.at<Vec3b>(r, c)[2];

				B += light;
				G += light;
				R += light;

				if (B > 255)B = 255;
				if (G > 255)G = 255;
				if (R > 255)R = 255;

				if (B < 0)B = 0;
				if (G < 0)G = 0;
				if (R < 0)R = 0;


				frame.at<Vec3b>(r, c)[0] = B;
				frame.at<Vec3b>(r, c)[1] = G;
				frame.at<Vec3b>(r, c)[2] = R;

			}
		}
	}

	// 直方图均衡化
	void histgoramEqu()
	{
		// 三个通道分别进行均衡化处理
		float totalPixelNum = frame.rows * frame.cols;
		int hist[3][256] = { 0 };//BGR
		// 构造 3 通道的直方图
		for (int r = 0; r < frame.rows; r++) {
			for (int c = 0; c < frame.cols; c++) {
				hist[0][frame.at<Vec3b>(r, c)[0]]++;
				hist[1][frame.at<Vec3b>(r, c)[1]]++;
				hist[2][frame.at<Vec3b>(r, c)[2]]++;
			}
		}
		float CDF[3][256] = { 0 };//3 个通道的累计分布函数	//Cumulative distribution function
		// 计算 3 通道的所有累计分布函数的值
		for (int i = 0; i < 256; i++) {
			float sum[3] = { 0 };
			for (int k = 0; k <= i; k++) {
				sum[0] += hist[0][k];
				sum[1] += hist[1][k];
				sum[2] += hist[2][k];
			}
			CDF[0][i] = sum[0] / totalPixelNum;
			CDF[1][i] = sum[1] / totalPixelNum;
			CDF[2][i] = sum[2] / totalPixelNum;
		}
		for (int r = 0; r < frame.rows; r++) {
			for (int c = 0; c < frame.cols; c++) {
				for (int i = 0; i < 3; i++)
					frame.at<Vec3b>(r, c)[i] = 255 * CDF[i][frame.at<Vec3b>(r, c)[i]];
			}
		}
	}
	// 调节对比度，范围 50%-150%
	void adjustContrast(int t)
	{
		float k = 0;
		if (t - 50 == 0)// 不处理
			return;
		else if (t - 50 > 0) {
			k = 1 + (t - 50) / 100.0;
		}
		else
			k = 1 - (50 - t) / 100.0;
		for (int r = 0; r < frame.rows; r++) {
			for (int c = 0; c < frame.cols; c++) {
				int B = frame.at<Vec3b>(r, c)[0];
				int G = frame.at<Vec3b>(r, c)[1];
				int R = frame.at<Vec3b>(r, c)[2];
				B *= k;
				G *= k;
				R *= k;
				if (B > 255)B = 255;
				if (G > 255)G = 255;
				if (R > 255)R = 255;
				frame.at<Vec3b>(r, c)[0] = B;
				frame.at<Vec3b>(r, c)[1] = G;
				frame.at<Vec3b>(r, c)[2] = R;
			}
		}
	}
	// 均值滤波
	void filter(int d)  // 模板的边长
	{
		Mat temp = frame.clone();
		int num = d * d;
		for (int r = d / 2; r < frame.rows - d / 2; r++) {
			for (int c = d / 2; c < frame.cols - d / 2; c++) {
				for (int i = 0; i < 3; i++) {// 分别处理 3 个通道
					int sum = 0;
					for (int dr = -d / 2; dr <= d / 2; dr++) {
						for (int dc = -d / 2; dc <= d / 2; dc++) {
							sum += temp.at<Vec3b>(r + dr, c + dc)[i];
						}
					}
					frame.at<Vec3b>(r, c)[i] = (float)sum / num;
				}

			}
		}

	}
	void boxfilter(int d)
	{
		Mat temp = frame.clone();
		boxFilter(temp, frame, -1, Size(d, d));
	}
	void gaussianblur(int d)
	{
		Mat temp = frame.clone();
		GaussianBlur(temp, frame, Size(d, d), 0, 0);
	}
	// 返回最终的处理结果
	Mat getFrame()
	{
		return frame;
	}
};

static void onTrackbar(int, void*)
{
	if (filter % 2 == 0)
		filter++;
	if (filter2 % 2 == 0)
		filter2++;
	if (filter3 % 2 == 0)
		filter3++;
}

static void onTrackbar1(int, void*)
{
	IsDrag = true;
}

// 创建滑动条
void createBar() {
	createTrackbar("暂停", windowName, &stop, 1, onTrackbar);
	createTrackbar("进度", windowName, &rateOfProgress, 100, onTrackbar1);
	createTrackbar("对比度", windowName, &contrastRatio, 100, onTrackbar);
	createTrackbar("亮度", windowName, &light, 510, onTrackbar);
	createTrackbar("直方图均衡化", windowName, &histogramEqualization, 1, onTrackbar);
	createTrackbar("均值滤波", windowName, &filter, 21, onTrackbar);
	createTrackbar("方框滤波", windowName, &filter2, 21, onTrackbar);
	createTrackbar("高斯滤波", windowName, &filter3, 21, onTrackbar);
}

void main() {
	VideoCapture capture;
	capture.open("exp2.avi");
	int totalNumOfFrame = capture.get(CAP_PROP_FRAME_COUNT);// 视频总帧数
	int currentNumOfFrame = 0;

	while (1)
	{
		Mat frame;
		capture >> frame;
		if (frame.empty())break;// 结束，退出
		// 创建滑动条
		createBar();

		// 调节视频进度
		if (!IsDrag) {	// 进度条没动，根据当前是多少帧，更新进度条的位置
			currentNumOfFrame++;// 当前是第多少帧
			rateOfProgress = (float)currentNumOfFrame / totalNumOfFrame * 100;// 根据当前帧数，计算并更新滑动条位置
		}
		else {
			// 进度条动了，根据进度条位置，计算当前应该是多少帧，并更新画面
			currentNumOfFrame = rateOfProgress / 100.0 * totalNumOfFrame;
			capture.set(CV_CAP_PROP_POS_FRAMES, currentNumOfFrame - 1);
			IsDrag = false;
		}

		videoProcess vp = videoProcess(frame);
		// 调节视频亮度
		vp.adjustLight(light);
		// 直方图均衡化
		if (histogramEqualization == 1)
			vp.histgoramEqu();
		// 调节对比度
		vp.adjustContrast(contrastRatio);
		// 均值滤波
		vp.filter(filter);
		vp.boxfilter(filter2);
		vp.gaussianblur(filter3);
		// 获得最终结果
		frame = vp.getFrame();

		// 控制视频的播放 \ 暂停
		while (stop == 0)// 重复显示这一帧，看起来，，暂停了
		{
			imshow(windowName, frame);
			waitKey(30);
			continue;
		}

		namedWindow(windowName, CV_WINDOW_FREERATIO);

		imshow(windowName, frame);
		waitKey(30);

	}
	capture.release();
}