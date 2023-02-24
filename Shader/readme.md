# GAMES101 assignment3 shader	

项目地址：[click](https://github.com/logic-three-body/GameS101_IntroductionTOcomputerGraph/tree/%E4%BD%9C%E4%B8%9A1/Shader)

渲染图像：[click](https://github.com/logic-three-body/GameS101_IntroductionTOcomputerGraph/tree/%E4%BD%9C%E4%B8%9A1/Shader/image)

模型支持在视图中**旋转**（利用[assignment1](https://github.com/logic-three-body/GameS101_IntroductionTOcomputerGraph/tree/%E4%BD%9C%E4%B8%9A1/Roate_Project)旋转三角形方法）

## MVP部分

model变换中 变量**coef**是模型的缩放因子

```c++
Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos)
{
	Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

	Eigen::Matrix4f translate;
	translate << 1, 0, 0, -eye_pos[0],
		0, 1, 0, -eye_pos[1],
		0, 0, 1, -eye_pos[2],
		0, 0, 0, 1;

	view = translate * view;

	return view;
}

Eigen::Matrix4f get_model_matrix(float angle)
{
	Eigen::Matrix4f rotation;
	angle = angle * MY_PI / 180.f;
	rotation << cos(angle), 0, sin(angle), 0,
		0, 1, 0, 0,
		-sin(angle), 0, cos(angle), 0,
		0, 0, 0, 1;

	Eigen::Matrix4f scale;
	float coef = 1.0f;//缩放因子
	coef = 2.5f;
	//coef = 5.0f;
	//coef = 7.0f;
	coef = 8.0f;
	scale << coef, 0, 0, 0,
		0, coef, 0, 0,
		0, 0, coef, 0,
		0, 0, 0, 1;

	Eigen::Matrix4f translate;
	translate << 1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1;

	return translate * rotation * scale;
}

Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio, float zNear, float zFar)
{
	Eigen::Matrix4f projection;
	float top = -tan(DEG2RAD(eye_fov / 2.0f) * abs(zNear));
	float right = top * aspect_ratio;

	projection << zNear / right, 0, 0, 0,
		0, zNear / top, 0, 0,
		0, 0, (zNear + zFar) / (zNear - zFar), (2 * zNear*zFar) / (zFar - zNear),
		0, 0, 1, 0;
	return projection;

}
```



## shader部分

### 纹理着色器

```C++
Eigen::Vector3f texture_fragment_shader(const fragment_shader_payload& payload)
{
	Eigen::Vector3f return_color = { 0, 0, 0 };
	if (payload.texture)
	{
		// TODO: Get the texture value at the texture coordinates of the current fragment
		return_color = payload.texture->getColor(payload.tex_coords.x(), payload.tex_coords.y());
	}
	Eigen::Vector3f texture_color;
	texture_color << return_color.x(), return_color.y(), return_color.z();

	Eigen::Vector3f ka = Eigen::Vector3f(0.005, 0.005, 0.005);
	Eigen::Vector3f kd = texture_color / 255.f;
	Eigen::Vector3f ks = Eigen::Vector3f(0.7937, 0.7937, 0.7937);

	auto l1 = light{ {20, 20, 20}, {500, 500, 500} };
	auto l2 = light{ {-20, 20, 0}, {500, 500, 500} };

	std::vector<light> lights = { l1, l2 };
	Eigen::Vector3f amb_light_intensity{ 10, 10, 10 };
	Eigen::Vector3f eye_pos{ 0, 0, 10 };

	float p = 150;

	Eigen::Vector3f color = texture_color;
	Eigen::Vector3f point = payload.view_pos;
	Eigen::Vector3f normal = payload.normal;

	Eigen::Vector3f result_color = { 0, 0, 0 };
	Vector3f view_dir = (eye_pos - point).normalized();
	for (auto& light : lights)
	{
		// TODO: For each light source in the code, calculate what the *ambient*, *diffuse*, and *specular* 
		// components are. Then, accumulate that result on the *result_color* object.
		float r2 = (light.position - point).squaredNorm();
		Vector3f diffsue(0, 0, 0);
		Vector3f specular(0, 0, 0);
		Vector3f ambient(0, 0, 0);
		Vector3f light_dir = (light.position - point).normalized();

		for (size_t i = 0; i < 3; i++)
		{
			Vector3f half_v = (view_dir + light_dir).normalized(); // half
			float intensity = light.intensity[i] / r2;
			diffsue[i] = kd[i] * intensity * std::max(0.0f, normal.dot(light_dir));
			specular[i] = ks[i] * intensity * std::pow(std::max(0.0f, normal.dot(half_v)), p);
			ambient[i] = amb_light_intensity[i] * ka[i];
		}
		result_color += diffsue;
		result_color += specular;
		result_color += ambient;
	}

	return result_color * 255.f;
}
```

### 法线贴图着色器

```C++
Eigen::Vector3f normal_fragment_shader(const fragment_shader_payload& payload)
{
	Eigen::Vector3f return_color = (payload.normal.head<3>().normalized() + Eigen::Vector3f(1.0f, 1.0f, 1.0f)) / 2.f;
	Eigen::Vector3f result;
	result << return_color.x() * 255, return_color.y() * 255, return_color.z() * 255;
	return result;
}
```



### 凹凸贴图着色器

```C++
Eigen::Vector3f bump_fragment_shader(const fragment_shader_payload& payload)
{

	Eigen::Vector3f ka = Eigen::Vector3f(0.005, 0.005, 0.005);
	Eigen::Vector3f kd = payload.color;
	Eigen::Vector3f ks = Eigen::Vector3f(0.7937, 0.7937, 0.7937);

	auto l1 = light{ {20, 20, 20}, {500, 500, 500} };
	auto l2 = light{ {-20, 20, 0}, {500, 500, 500} };

	std::vector<light> lights = { l1, l2 };
	Eigen::Vector3f amb_light_intensity{ 10, 10, 10 };
	Eigen::Vector3f eye_pos{ 0, 0, 10 };

	float p = 150;

	Eigen::Vector3f color = payload.color;
	Eigen::Vector3f point = payload.view_pos;
	Eigen::Vector3f normal = payload.normal.normalized();


	float kh = 0.2, kn = 0.1;

	// TODO: Implement bump mapping here
	// Let n = normal = (x, y, z)
	// Vector t = (x*y/sqrt(x*x+z*z),sqrt(x*x+z*z),z*y/sqrt(x*x+z*z))
	// Vector b = n cross product t
	// Matrix TBN = [t b n]
	// dU = kh * kn * (h(u+1/w,v)-h(u,v))
	// dV = kh * kn * (h(u,v+1/h)-h(u,v))
	// Vector ln = (-dU, -dV, 1)
	// Normal n = normalize(TBN * ln)

	float x = normal.x();
	float y = normal.y();
	float z = normal.z();
	Vector3f t(x*y / sqrt(x*x + z * z), sqrt(x*x + z * z), z*y / sqrt(x*x + z * z));
	Vector3f b = normal.cross(t);

	Matrix3f TBN;
	TBN.col(0) = t;
	TBN.col(1) = b;
	TBN.col(2) = normal;

	int w = payload.texture->width;
	int h = payload.texture->height;
	float u = payload.tex_coords.x();
	float v = payload.tex_coords.y();
	payload.texture->getColor(u, v);

	auto huv = payload.texture->getColor(u, v).norm();

	float dU = kh * kn * (payload.texture->getColor(u + 1.0f / w, v).norm() - huv);
	float dV = kh * kn * (payload.texture->getColor(u, v + 1.0f / h).norm() - huv);

	Vector3f ln(-dU, -dV, 1);
	Vector3f n = (TBN * ln).normalized();

	Eigen::Vector3f result_color = n;
	return result_color * 255.f;
}
```



### 置换贴图着色器

```C++
Eigen::Vector3f displacement_fragment_shader(const fragment_shader_payload& payload)
{

	Eigen::Vector3f ka = Eigen::Vector3f(0.005, 0.005, 0.005);
	Eigen::Vector3f kd = payload.color;
	Eigen::Vector3f ks = Eigen::Vector3f(0.7937, 0.7937, 0.7937);

	auto l1 = light{ {20, 20, 20}, {500, 500, 500} };
	auto l2 = light{ {-20, 20, 0}, {500, 500, 500} };

	std::vector<light> lights = { l1, l2 };
	Eigen::Vector3f amb_light_intensity{ 10, 10, 10 };
	Eigen::Vector3f eye_pos{ 0, 0, 10 };

	Eigen::Vector3f color = payload.color;
	Eigen::Vector3f point = payload.view_pos;
	Eigen::Vector3f normal = payload.normal;

	float kh = 0.2, kn = 0.1;

	// TODO: Implement displacement mapping here
	// Let n = normal = (x, y, z)
	// Vector t = (x*y/sqrt(x*x+z*z),sqrt(x*x+z*z),z*y/sqrt(x*x+z*z))
	// Vector b = n cross product t
	// Matrix TBN = [t b n]
	// dU = kh * kn * (h(u+1/w,v)-h(u,v))
	// dV = kh * kn * (h(u,v+1/h)-h(u,v))
	// Vector ln = (-dU, -dV, 1)
	// Position p = p + kn * n * h(u,v)
	// Normal n = normalize(TBN * ln)


	float x = normal.x();
	float y = normal.y();
	float z = normal.z();
	Vector3f t(x*y / sqrt(x*x + z * z), sqrt(x*x + z * z), z*y / sqrt(x*x + z * z));
	Vector3f b = normal.cross(t);

	Matrix3f TBN;
	TBN.col(0) = t.normalized();
	TBN.col(1) = b.normalized();
	TBN.col(2) = normal;

	int w = payload.texture->width;
	int h = payload.texture->height;
	float u = payload.tex_coords.x();
	float v = payload.tex_coords.y();
	payload.texture->getColor(u, v);

	auto huv = payload.texture->getColor(u, v).norm();

	float dU = kh * kn * (payload.texture->getColor(u + 1.0f / w, v).norm() - huv);
	float dV = kh * kn * (payload.texture->getColor(u, v + 1.0f / h).norm() - huv);

	Vector3f ln(-dU, -dV, 1);
	Vector3f n = (TBN * ln).normalized();
	Vector3f p = point + n * huv * kn;

	Eigen::Vector3f result_color = { 0, 0, 0 };
	Vector3f view_dir = (eye_pos - p).normalized();
	for (auto& light : lights)
	{
		// TODO: For each light source in the code, calculate what the *ambient*, *diffuse*, and *specular* 
		// components are. Then, accumulate that result on the *result_color* object.
		float r2 = (light.position - p).squaredNorm();
		Vector3f diffsue(0, 0, 0);
		Vector3f specular(0, 0, 0);
		Vector3f ambient(0, 0, 0);
		Vector3f light_dir = (light.position - p).normalized();

		for (size_t i = 0; i < 3; i++)
		{
			Vector3f h = (view_dir + light_dir).normalized(); // half
			float intensity = light.intensity[i] / r2;
			diffsue[i] = kd[i] * intensity * std::max(0.0f, normal.dot(light_dir));
			specular[i] = ks[i] * intensity * std::pow(std::max(0.0f, normal.dot(h)), 150);
			ambient[i] = amb_light_intensity[i] * ka[i];
		}
		result_color += diffsue;
		result_color += specular;
		result_color += ambient;
	}

	return result_color * 255.f;
}
```



### Blin-Phong模型着色器

```C++
Eigen::Vector3f texture_fragment_shader(const fragment_shader_payload& payload)
{
	Eigen::Vector3f return_color = { 0, 0, 0 };
	if (payload.texture)
	{
		// TODO: Get the texture value at the texture coordinates of the current fragment
		return_color = payload.texture->getColor(payload.tex_coords.x(), payload.tex_coords.y());
	}
	Eigen::Vector3f texture_color;
	texture_color << return_color.x(), return_color.y(), return_color.z();

	Eigen::Vector3f ka = Eigen::Vector3f(0.005, 0.005, 0.005);
	Eigen::Vector3f kd = texture_color / 255.f;
	Eigen::Vector3f ks = Eigen::Vector3f(0.7937, 0.7937, 0.7937);

	auto l1 = light{ {20, 20, 20}, {500, 500, 500} };
	auto l2 = light{ {-20, 20, 0}, {500, 500, 500} };

	std::vector<light> lights = { l1, l2 };
	Eigen::Vector3f amb_light_intensity{ 10, 10, 10 };
	Eigen::Vector3f eye_pos{ 0, 0, 10 };

	float p = 150;

	Eigen::Vector3f color = texture_color;
	Eigen::Vector3f point = payload.view_pos;
	Eigen::Vector3f normal = payload.normal;

	Eigen::Vector3f result_color = { 0, 0, 0 };
	Vector3f view_dir = (eye_pos - point).normalized();
	for (auto& light : lights)
	{
		// TODO: For each light source in the code, calculate what the *ambient*, *diffuse*, and *specular* 
		// components are. Then, accumulate that result on the *result_color* object.
		float r2 = (light.position - point).squaredNorm();
		Vector3f diffsue(0, 0, 0);
		Vector3f specular(0, 0, 0);
		Vector3f ambient(0, 0, 0);
		Vector3f light_dir = (light.position - point).normalized();

		for (size_t i = 0; i < 3; i++)
		{
			Vector3f half_v = (view_dir + light_dir).normalized(); // half
			float intensity = light.intensity[i] / r2;
			diffsue[i] = kd[i] * intensity * std::max(0.0f, normal.dot(light_dir));
			specular[i] = ks[i] * intensity * std::pow(std::max(0.0f, normal.dot(half_v)), p);
			ambient[i] = amb_light_intensity[i] * ka[i];
		}
		result_color += diffsue;
		result_color += specular;
		result_color += ambient;
	}

	return result_color * 255.f;
}
```



## 光栅化部分

```C++
void rst::rasterizer::rasterize_triangle(const Triangle& t, const std::array<Eigen::Vector3f, 3>& view_pos) 
{
	// TODO : Find out the bounding box of current triangle.
	float aabb_minx = 0;
	float aabb_miny = 0;
	float aabb_maxx = 0;
	float aabb_maxy = 0;
	for (size_t i = 0; i < 3; i++)
	{
		const Vector4f& p = t.v[i];
		if (i == 0)
		{
			aabb_minx = aabb_maxx = p.x();
			aabb_miny = aabb_maxy = p.y();
			continue;
		}

		aabb_minx = p.x() < aabb_minx ? p.x() : aabb_minx;
		aabb_miny = p.y() < aabb_miny ? p.y() : aabb_miny;

		aabb_maxx = p.x() > aabb_maxx ? p.x() : aabb_maxx;
		aabb_maxy = p.y() > aabb_maxy ? p.y() : aabb_maxy;
	}

	// iterate through the pixel and find if the current pixel is inside the triangle
	auto v = t.v;
	for (int x = (int)aabb_minx; x < aabb_maxx; x++)
	{
		for (int y = (int)aabb_miny; y < aabb_maxy; y++)
		{

			if (!insideTriangle(x, y, t.v)) continue;
			// TODO: Inside your rasterization loop:
			//    * v[i].w() is the vertex view space depth value z.
			//    * Z is interpolated view space depth for the current pixel
			//    * zp is depth between zNear and zFar, used for z-buffer
			auto[alpha, beta, gamma] = computeBarycentric2D(x, y, t.v);


			float Z = 1.0 / (alpha / v[0].w() + beta / v[1].w() + gamma / v[2].w());
			float zp = alpha * v[0].z() / v[0].w() + beta * v[1].z() / v[1].w() + gamma * v[2].z() / v[2].w();
			zp *= Z;

			int buf_index = get_index(x, y);
			if (zp >= depth_buf[buf_index]) continue;

			depth_buf[buf_index] = zp;

			// TODO: Interpolate the attributes:
			// auto interpolated_color
			// auto interpolated_normal
			// auto interpolated_texcoords
			// auto interpolated_shadingcoords

			auto interpolated_color = interpolate(alpha, beta, gamma, t.color[0], t.color[1], t.color[2], 1);
			auto interpolated_normal = interpolate(alpha, beta, gamma, t.normal[0], t.normal[1], t.normal[2], 1);
			auto interpolated_texcoords = interpolate(alpha, beta, gamma, t.tex_coords[0], t.tex_coords[1], t.tex_coords[2], 1);
			auto interpolated_viewpos = interpolate(alpha, beta, gamma, view_pos[0], view_pos[1], view_pos[2], 1);
            
   // Use: fragment_shader_payload payload( interpolated_color, interpolated_normal.normalized(), interpolated_texcoords, texture ? &*texture : nullptr);
    // Use: payload.view_pos = interpolated_shadingcoords;
    // Use: Instead of passing the triangle's color directly to the frame buffer, pass the color to the shaders first to get the final color;
    // Use: auto pixel_color = fragment_shader(payload);
            
			fragment_shader_payload payload(interpolated_color, interpolated_normal.normalized(), interpolated_texcoords, texture ? &*texture : nullptr);
			payload.view_pos = interpolated_viewpos;
			auto pixel_color = fragment_shader(payload);//注入shader
			set_pixel(Vector2i(x, y), pixel_color);
		}
	}	
}
```

## 渲染图

### 小牛spot

#### texture

![frame1.png](https://github.com/logic-three-body/GameS101_IntroductionTOcomputerGraph/blob/%E4%BD%9C%E4%B8%9A1/Shader/image/spot/snapshot/texture/frame1.png?raw=true)

![tex.gif](https://github.com/logic-three-body/GameS101_IntroductionTOcomputerGraph/blob/%E4%BD%9C%E4%B8%9A1/Shader/image/spot/snapshot/tex.gif?raw=true)

#### normal

![frame1.png](https://github.com/logic-three-body/GameS101_IntroductionTOcomputerGraph/blob/%E4%BD%9C%E4%B8%9A1/Shader/image/spot/snapshot/normal/frame1.png?raw=true)

![norm.gif](https://github.com/logic-three-body/GameS101_IntroductionTOcomputerGraph/blob/%E4%BD%9C%E4%B8%9A1/Shader/image/spot/snapshot/norm.gif?raw=true)

#### bump

![frame1.png](https://github.com/logic-three-body/GameS101_IntroductionTOcomputerGraph/blob/%E4%BD%9C%E4%B8%9A1/Shader/image/spot/snapshot/bump/frame1.png?raw=true)

![bump.gif](https://github.com/logic-three-body/GameS101_IntroductionTOcomputerGraph/blob/%E4%BD%9C%E4%B8%9A1/Shader/image/spot/snapshot/bump.gif?raw=true)

#### displacement

![frame1.png](https://github.com/logic-three-body/GameS101_IntroductionTOcomputerGraph/blob/%E4%BD%9C%E4%B8%9A1/Shader/image/spot/snapshot/displacement/frame1.png?raw=true)

![displacement.gif](https://github.com/logic-three-body/GameS101_IntroductionTOcomputerGraph/blob/%E4%BD%9C%E4%B8%9A1/Shader/image/spot/snapshot/displacement.gif?raw=true)

#### phong

![frame1.png](https://github.com/logic-three-body/GameS101_IntroductionTOcomputerGraph/blob/%E4%BD%9C%E4%B8%9A1/Shader/image/spot/snapshot/Blin-phong/frame1.png?raw=true)

![Blin-phong.gif](https://github.com/logic-three-body/GameS101_IntroductionTOcomputerGraph/blob/%E4%BD%9C%E4%B8%9A1/Shader/image/spot/snapshot/Blin-phong.gif?raw=true)

### bunny

#### texture

![frame171.png](https://github.com/logic-three-body/GameS101_IntroductionTOcomputerGraph/blob/%E4%BD%9C%E4%B8%9A1/Shader/image/bunny/snapshot/tex/frame171.png?raw=true)

![tex.gif](https://github.com/logic-three-body/GameS101_IntroductionTOcomputerGraph/blob/%E4%BD%9C%E4%B8%9A1/Shader/image/bunny/snapshot/tex.gif?raw=true)

#### normal

![frame4.png](https://github.com/logic-three-body/GameS101_IntroductionTOcomputerGraph/blob/%E4%BD%9C%E4%B8%9A1/Shader/image/bunny/snapshot/norm/frame4.png?raw=true)

![norm.gif](https://github.com/logic-three-body/GameS101_IntroductionTOcomputerGraph/blob/%E4%BD%9C%E4%B8%9A1/Shader/image/bunny/snapshot/norm.gif?raw=true)

### dragon

#### normal

##### 1K

![size1000.png](https://github.com/logic-three-body/GameS101_IntroductionTOcomputerGraph/blob/%E4%BD%9C%E4%B8%9A1/Shader/image/dragon/snapshot/size1000.png?raw=true)

##### 2K

![size2000.png](https://github.com/logic-three-body/GameS101_IntroductionTOcomputerGraph/blob/%E4%BD%9C%E4%B8%9A1/Shader/image/dragon/snapshot/size2000.png?raw=true)

##### 8K

![size8000.png](https://github.com/logic-three-body/GameS101_IntroductionTOcomputerGraph/blob/%E4%BD%9C%E4%B8%9A1/Shader/image/dragon/snapshot/size8000.png?raw=true)

![norm.gif](https://github.com/logic-three-body/GameS101_IntroductionTOcomputerGraph/blob/%E4%BD%9C%E4%B8%9A1/Shader/image/dragon/norm.gif?raw=true)

