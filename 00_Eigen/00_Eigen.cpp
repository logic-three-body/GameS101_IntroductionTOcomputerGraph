

#include<cmath>
#include<Eigen/Core>
#include<Eigen/Dense>
#include<iostream>

#define M_PI 3.1415926f
#define DEG2RAD (M_PI/180.0f)

int main() {
	Eigen::Vector3f p(2.0f, 1.0f, 1.0f); //定义我们的p点 写成齐次坐标形式
	// 目标是先旋转后平移 所以我们在p的左边依次乘以旋转矩阵和平移矩阵、
	Eigen::Matrix3f rotation;   //定义旋转矩阵
	Eigen::Matrix3f transform;  //定义平移矩阵
	double theta = 45.0 / 180.0*M_PI; //转换成弧度数
	rotation << cos(theta), -sin(theta), 0,
		sin(theta), cos(theta), 0,
		0, 0, 1;   //旋转矩阵写成其次坐标形式
	transform << 1, 0, 1,
		0, 1, 2,
		0, 0, 1;  // 平移矩阵

	p = transform * rotation*p;  // 转换过程
	std::cout << p << std::endl;   // 输出结果

	return 0;
}