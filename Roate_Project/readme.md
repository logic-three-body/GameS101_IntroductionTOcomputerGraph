# 作业1 旋转与投影

## 模型矩阵

```C++
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
```



## 透视投影矩阵

```C++
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
```

## 三角形旋转

### 绕Z轴

```c++
rotateZ << std::cos(angle), -1 * std::sin(angle), 0, 0,
    std::sin(angle), std::cos(angle), 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1;
```

![ZRotate](https://i.loli.net/2021/06/08/i46RI9kdg2f8OKH.gif)

### 绕任意轴

```c++
RotateAxis << x * x + (1 - x * x) * cos_angle, x *y *(1 - cos_angle) + z *sin_angle, x *z *(1 - cos_angle) - y *sin_angle, 0,
    x *y *(1 - cos_angle) - z *sin_angle, y *y + (1 - y * y) * cos_angle, y *z *(1 - cos_angle) + sin_angle, 0,
    x *z *(1 - cos_angle) + y *sin_angle, y *z *(1 - cos_angle) - x *sin_angle, z *z + (1 - z * z) * cos_angle, 0,
    0, 0, 0, 1;
```

```C++
Vector3f Vs = { 1,1,1 };
```

![Rotate1](https://i.loli.net/2021/06/08/KOYZfJGtSx75vad.gif)

```c++
Vector3f Ve = { 0,0,1 };
```

![Rotate2](https://i.loli.net/2021/06/08/7A34hnS8xVzfMce.gif)



## 绕任意轴旋转

```c++
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
```



## 其他结果

### 绕X旋转

```c++
rotateX << 1, 0, 0, 0,
    0, std::cos(angle), -std::sin(angle), 0,
    0, std::sin(angle), std::cos(angle), 0,
    0, 0, 0, 1;
```

![RotateX](https://i.loli.net/2021/06/08/NifPRGlWwjnJd2b.gif)

### 绕Y旋转

```c++
rotateY << std::cos(angle), 0, std::sin(angle), 0,
    0, 1, 0, 0,
    -std::sin(angle), 0, std::cos(angle), 0,
    0, 0, 0, 1;
```

![RotateY](https://i.loli.net/2021/06/08/GwD1bHmiSX5QM6q.gif)

### 平移 X轴为例

```C++
float step = -angle / 10.0;
translateX << 1, 0, 0, step,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1;
```

![TransX](https://i.loli.net/2021/06/08/hONaZsHByKJzFXk.gif)

### 缩放

```c++
//scale with k
float k = .1 * angle; //scale factor
scaleK << k, 0, 0, 0,
    0, k, 0, 0,
    0, 0, k, 0,
    0, 0, 0, 1;
```

![TransX](https://i.loli.net/2021/06/08/cEx8LbiaGXy6HqS.gif)