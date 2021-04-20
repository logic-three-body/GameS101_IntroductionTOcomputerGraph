#include "Triangle.hpp"
#include "rasterizer.hpp"
#include <Eigen/Eigen>
#include <iostream>
#include <opencv2/opencv.hpp>

constexpr double MY_PI = 3.1415926;
constexpr float RotateAngle = 45;
inline double DEG2RAD(double deg) { return deg * MY_PI / 180; }

Eigen::Matrix4f get_model_matrix(float rotation_angle)
{
	Eigen::Matrix4f model = Eigen::Matrix4f::Identity();

	// TODO: Implement this function
	// Create the model matrix for rotating the triangle around the Z axis.
	// Then return it.
	Eigen::Matrix4f rotateZ, rotateY, rotateX, scaleK, translateX, translateZ;
	float angle = rotation_angle / 180 * MY_PI;
	//rotate with z
	rotateZ << std::cos(angle), -1 * std::sin(angle), 0, 0, \
		std::sin(angle), std::cos(angle), 0, 0, \
		0, 0, 1, 0, \
		0, 0, 0, 1;
	//rotate with x
	rotateX << 1, 0, 0, 0, \
		0, std::cos(angle), -std::sin(angle), 0, \
		0, std::sin(angle), std::cos(angle), 0, \
		0, 0, 0, 1;
	//rotate with y
	rotateY << std::cos(angle), 0, std::sin(angle), 0, \
		0, 1, 0, 0, \
		- std::sin(angle), 0, std::cos(angle), 0, \
		0, 0, 0, 1;

	//scale with k
	float k = .01;//scale factor
	scaleK << k , 0, 0, 0, \
		0, k, 0, 0, \
		0, 0, k , 0, \
		0, 0, 0, 1;

	//translate X OR Z for any step
	float step = -angle / 10.0;
	translateX << 1, 0, 0, step, \
		0, 1, 0, 0, \
		0, 0, 1, 0, \
		0, 0, 0, 1;

	translateZ << 1, 0, 0, 0, \
		0, 1, 0, 0, \
		0, 0, 1, step, \
		0, 0, 0, 1;
	model = rotateZ * model;
	//model = rotateX * model;
	//model = rotateY * model;
	//model = scaleK * model;
	//model *= translateX;
	//model *= translateZ;
	//model *= translateX*rotateX*scaleK;
	//model *= rotateZ * scaleK;
	//model *= rotateY * scaleK;
	return model;
}

Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos)
{
	Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

	Eigen::Matrix4f translate2orgin;
	translate2orgin << 1, 0, 0, -eye_pos[0],//move camera to world orgin
		0, 1, 0, -eye_pos[1],
		0, 0, 1, -eye_pos[2],
		0, 0, 0, 1;

	view = translate2orgin * view;

	return view;
}

Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio, float zNear, float zFar)
{
	Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();
	Eigen::Matrix4f M_persp2ortho(4, 4);
	Eigen::Matrix4f M_ortho_scale(4, 4);
	Eigen::Matrix4f M_ortho_trans(4, 4);

	float angle = eye_fov * MY_PI / 180.0; // half angle
	float height = zNear * tan(angle) * 2;
	float width = height * aspect_ratio;

	auto t = -zNear * tan(angle / 2);
	auto r = t * aspect_ratio;
	auto l = -r;
	auto b = -t;

	M_persp2ortho << zNear, 0, 0, 0,
		0, zNear, 0, 0,
		0, 0, zNear + zFar, -zNear * zFar,
		0, 0, 1, 0;

	M_ortho_scale << 2 / (r - l), 0, 0, 0,
		0, 2 / (t - b), 0, 0,
		0, 0, 2 / (zNear - zFar), 0,
		0, 0, 0, 1;

	M_ortho_trans << 1, 0, 0, -(r + l) / 2,
		0, 1, 0, -(t + b) / 2,
		0, 0, 1, -(zNear + zFar) / 2,
		0, 0, 0, 1;

	Eigen::Matrix4f M_ortho = M_ortho_scale * M_ortho_trans;
	//prespective
	projection = M_ortho * M_persp2ortho * projection;

	//orthogonal
	//projection *= M_ortho;

	return projection;
}

//Improved:得到绕任意过原点的轴的旋转变换矩阵 https://www.freesion.com/article/28641062948/
Eigen::Matrix4f get_model_matrix(Vector3f axis, float angle) {
	Eigen::Matrix4f RotateAxis = Eigen::Matrix4f::Identity();
	float radian = angle / 180 * MY_PI;
	float x = axis.x();
	float y = axis.y();
	float z = axis.z();
	float cos_angle = std::cos(radian);
	float sin_angle = std::sin(radian);

	RotateAxis << x * x + (1 - x * x)*cos_angle, x*y*(1 - cos_angle) + z * sin_angle, x*z*(1 - cos_angle) - y * sin_angle, 0, \
		x*y*(1 - cos_angle) - z * sin_angle, y*y + (1 - y * y)*cos_angle, y*z*(1 - cos_angle) + sin_angle, 0, \
		x*z*(1 - cos_angle) + y * sin_angle, y*z*(1 - cos_angle) - x * sin_angle, z*z + (1 - z * z)*cos_angle, 0, \
		0, 0, 0, 1;

	return RotateAxis;
}

//Improved2:得到绕任意过原点的轴的旋转变换矩阵 https://blog.csdn.net/qq_36242312/article/details/105742949
Eigen::Matrix4f get_model_matrix(float rotation_angle, Eigen::Vector3f axis_start, Eigen::Vector3f axis_end)
{
	// Eigen::Vector3f axis_start 为起点
	// Eigen::Vector3f axis_end 为终点
	Eigen::Matrix4f RotateAxis = Eigen::Matrix4f::Identity();
	// normalize axis
	Eigen::Vector3f axis;
	axis[0] = axis_end[0] - axis_start[0];
	axis[1] = axis_end[1] - axis_start[1];
	axis[2] = axis_end[2] - axis_start[2];
	float norm = sqrt(axis[0] * axis[0] + axis[1] * axis[1] + axis[2] * axis[2]);
	axis[0] /= norm;
	axis[1] /= norm;
	axis[2] /= norm;
	// compute radian
	float radian = rotation_angle / 180.0 * MY_PI;
	// compute component 计算轴角旋转矩阵分量
	Eigen::Matrix3f n(3, 3);
	n << 0, -axis[2], axis[1],
		axis[2], 0, -axis[0],
		-axis[1], axis[0], 0;

	Eigen::Matrix3f component1 = Eigen::Matrix3f::Identity() * cos(radian);
	Eigen::Matrix3f component2 = axis * axis.transpose() * (1 - cos(radian));
	Eigen::Matrix3f component3 = n * sin(radian);

	Eigen::Matrix3f rotate_m = component1 + component2 + component3;

	Eigen::Matrix4f rotate_martix = Eigen::Matrix4f::Identity();
	rotate_martix.block(0, 0, 3, 3) = rotate_m; // 前三个维度为旋转矩阵

	RotateAxis = rotate_martix * RotateAxis;
	return RotateAxis;
}


int main(int argc, const char** argv)
{
	float angle = 0;
	bool command_line = false;
	std::string filename = "output.png";

	if (argc >= 3) {
		command_line = true;
		angle = std::stof(argv[2]); // -r by default
		if (argc == 4) {
			filename = std::string(argv[3]);
		}
		else
			return 0;
	}

	rst::rasterizer r(700, 700);

	Eigen::Vector3f eye_pos = { 0, 0, 5 };

	std::vector<Eigen::Vector3f> pos{ {2, 0, -2}, {0, 2, -2}, {-2, 0, -2} };//给定点，操作此三点

	std::vector<Eigen::Vector3i> ind{ {0, 1, 2} };

	auto pos_id = r.load_positions(pos);
	auto ind_id = r.load_indices(ind);

	int key = 0;
	int frame_count = 0;

	if (command_line) {
		r.clear(rst::Buffers::Color | rst::Buffers::Depth);

		r.set_model(get_model_matrix(angle));
		r.set_view(get_view_matrix(eye_pos));
		r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

		r.draw(pos_id, ind_id, rst::Primitive::Triangle);
		cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
		image.convertTo(image, CV_8UC3, 1.0f);

		cv::imwrite(filename, image);

		return 0;
	}

	while (key != 27) {
		r.clear(rst::Buffers::Color | rst::Buffers::Depth);
		Vector3f Vs = { 1,1,1 },
			Ve = { 1,3,1 };		//Try example: {1,1,3} {3,1,1} {1,3,1} {3,3,3}
		//r.set_model(get_model_matrix(angle));//angle在变，键盘A+D控制的是angle
		//r.set_model(get_rotation(Vs, angle));//improve
		r.set_model(get_model_matrix(angle));//improve2
		r.set_view(get_view_matrix(eye_pos));//eye_pos不变
		r.set_projection(get_projection_matrix(45, 1, 0.1, 500));//projection参数不变

		r.draw(pos_id, ind_id, rst::Primitive::Triangle);

		cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
		image.convertTo(image, CV_8UC3, 1.0f);
		cv::imshow("image", image);
		key = cv::waitKey(10);
		//while (std::cin.get() != '\n')//清除缓冲区
			//continue;
		std::cout << "frame count: " << frame_count++ << '\n';
		std::string num = std::to_string(frame_count);
		std::string pathPZ = "img/Perspective/RotateZ/" + num + filename;
		std::string pathOY = "img/Orthogonal/RotateY/" + num + filename;
		std::string pathPY = "img/Perspective/RotateY/" + num + filename;

		if (key == 'a') {
			angle += RotateAngle;
			image.convertTo(image, CV_8UC3, 1.0f);
			//cv::imwrite(path3, image);
		}
		else if (key == 'd') {
			angle -= RotateAngle;
			image.convertTo(image, CV_8UC3, 1.0f);
			//cv::imwrite(path3, image);
		}
	}
	return 0;
}
