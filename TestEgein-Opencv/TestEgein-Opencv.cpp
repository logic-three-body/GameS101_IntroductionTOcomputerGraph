//#include<cmath>
//#include<Eigen/Core>
//#include<Eigen/Dense>
//#include<iostream>
//#include<optional>
//int main() {
//
//	// Basic Example of cpp
//	std::cout << "Example of cpp \n";
//	float a = 1.0, b = 2.0;
//	std::cout << a << std::endl;
//	std::cout << a / b << std::endl;
//	std::cout << std::sqrt(b) << std::endl;
//	std::cout << std::acos(-1) << std::endl;
//	std::cout << std::sin(30.0 / 180.0*acos(-1)) << std::endl;
//
//	// Example of vector
//	std::cout << "Example of vector \n";
//	// vector definition
//	Eigen::Vector3f v(1.0f, 2.0f, 3.0f);
//	Eigen::Vector3f w(1.0f, 0.0f, 0.0f);
//	// vector output
//	std::cout << "Example of output \n";
//	std::cout << v << std::endl;
//	// vector add
//	std::cout << "Example of add \n";
//	std::cout << v + w << std::endl;
//	// vector scalar multiply
//	std::cout << "Example of scalar multiply \n";
//	std::cout << v * 3.0f << std::endl;
//	std::cout << 2.0f * v << std::endl;
//
//	// Example of matrix
//	std::cout << "Example of matrix \n";
//	// matrix definition
//	Eigen::Matrix3f i, j;
//	i << 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0;
//	j << 2.0, 3.0, 1.0, 4.0, 6.0, 5.0, 9.0, 7.0, 8.0;
//	// matrix output
//	std::cout << "Example of output \n";
//	std::cout << i << std::endl;
//	// matrix add i + j
//	// matrix scalar multiply i * 2.0
//	// matrix multiply i * j
//	// matrix multiply vector i * v
//
//	return 0;
//}

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

static void help(char** argv)
{
	cout << "\nThis sample program demonstrates the use of the convexHull() function\n"
		<< "Call:\n"
		<< argv[0] << endl;
}

int main(int argc, char** argv)
{
	CommandLineParser parser(argc, argv, "{help h||}");
	if (parser.has("help"))
	{
		help(argv);
		return 0;
	}
	Mat img(500, 500, CV_8UC3);
	RNG& rng = theRNG();

	for (;;)
	{
		int i, count = (unsigned)rng % 100 + 1;

		vector<Point> points;

		for (i = 0; i < count; i++)
		{
			Point pt;
			pt.x = rng.uniform(img.cols / 4, img.cols * 3 / 4);
			pt.y = rng.uniform(img.rows / 4, img.rows * 3 / 4);

			points.push_back(pt);
		}

		vector<int> hull;
		convexHull(Mat(points), hull, true);

		img = Scalar::all(0);
		for (i = 0; i < count; i++)
			circle(img, points[i], 3, Scalar(0, 0, 255), FILLED, LINE_AA);

		int hullcount = (int)hull.size();
		Point pt0 = points[hull[hullcount - 1]];

		for (i = 0; i < hullcount; i++)
		{
			Point pt = points[hull[i]];
			line(img, pt0, pt, Scalar(0, 255, 0), 1, LINE_AA);
			pt0 = pt;
		}

		imshow("hull", img);

		char key = (char)waitKey();
		if (key == 27 || key == 'q' || key == 'Q') // 'ESC'
			break;
	}

	return 0;
}
