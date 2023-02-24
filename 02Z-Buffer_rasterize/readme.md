# GAMES101->assignment2 Triangles and Z-buffering

项目地址：[here](https://github.com/logic-three-body/GameS101_IntroductionTOcomputerGraph/tree/%E4%BD%9C%E4%B8%9A1/02Z-Buffer_rasterize)

## 包围盒

```c++
// TODO : Find out the bounding box of current triangle.
// iterate through the pixel and find if the current pixel is inside the triangle
int left = MIN(v[0].x(), MIN(v[1].x(), v[2].x())) - 1;
int right = MAX(v[0].x(), MAX(v[1].x(), v[2].x())) + 1;
int bottom = MIN(v[0].y(), MIN(v[1].y(), v[2].y())) - 1;
int top = MAX(v[0].y(), MAX(v[1].y(), v[2].y())) + 1;
```

## 三角形栅格化算法

```c++
for (int x = left; x <= right; x++)
{
    for (int y = bottom; y <= top; y++)
    {
        if (insideTriangle(x, y, t.v))
        {
            std::tuple<float, float, float> alpha = computeBarycentric2D(x, y, t.v);
            float w_reciprocal = 1.0 / (std::get<0>(alpha) / v[0].w() + std::get<1>(alpha) / v[1].w() + std::get<2>(alpha) / v[2].w());
            float z_interpolated = std::get<0>(alpha) * v[0].z() / v[0].w() + std::get<1>(alpha) * v[1].z() / v[1].w() + std::get<2>(alpha) * v[2].z() / v[2].w();
            z_interpolated *= w_reciprocal;
            if (z_interpolated < depth_buf[get_index(x, y)])
            {
                depth_buf[get_index(x, y)] = z_interpolated;
                set_pixel(Vector3f(x, y, z_interpolated), t.getColor());
            }
        }
    }
}
```

## 点是否在三角形内

```c++
static bool insideTriangle(float x, float y, const Vector3f* _v)
{
	// TODO : Implement this function to check if the point (x, y) is inside the triangle represented by _v[0], _v[1], _v[2]
	float a = _v[0].x(), b = _v[0].y(), c = _v[1].x(), d = _v[1].y(), e = _v[2].x(), f = _v[2].y();
	float t1 = (c - a)*(y - b) - (d - b)*(x - a);
	float t2 = (e - c)*(y - d) - (f - d)*(x - c);
	float t3 = (a - e)*(y - f) - (b - f)*(x - e);
	bool x1 = true ? t1 > 0.0:false, x2 = true ? t2 > 0.0:false, x3 = true ? t3 > 0.0:false;
	bool ret = false;
	if ((x1 &x2&x3) || (!x1)&(!x2)&(!x3)) ret = true;
	return ret;
}
```

## z-buffer 算法

```c++
//... some code
if (z_interpolated < depth_buf[get_index(x, y)])
{
    depth_buf[get_index(x, y)] = z_interpolated;
    set_pixel(Vector3f(x, y, z_interpolated), t.getColor());
}
```

## Anti-aliasing

```c++
//4x4 SuperSampling
const int sam_num16 = 16;
const int xy = 2;
const int x_coord = 0;
const int y_coord = 1;
float dir4x4[sam_num16][xy] = {
    {0.125, 0.125},
    {0.125, 0.375},
    {0.125, 0.625},
    {0.125, 0.875},
    {0.375, 0.125},
    {0.375, 0.375},
    {0.375, 0.625},
    {0.375, 0.875},
    {0.625, 0.125},
    {0.625, 0.375},
    {0.625, 0.625},
    {0.625, 0.875},
    {0.875, 0.125},
    {0.875, 0.375},
    {0.875, 0.625},
    {0.875, 0.875},
};
//2x2 SuperSampling
const int sam_num4 = 4;
float dir2x2[sam_num4][xy] = {
    {0.25, 0.25},
    {0.25, 0.75},
    {0.75, 0.25},
    {0.75, 0.75},
};

for (int x = left; x <= right; x++)
{
    for (int y = bottom; y <= top; y++)
    {
        float min_dep = INFINITY;
        int count = 0;
        for (int i = 0; i < sam_num4; i++)
        {
            if (insideTriangle(x + dir2x2[i][x_coord], y + dir2x2[i][y_coord], t.v))
            {
                count++;
                std::tuple<float, float, float> alpha = computeBarycentric2D(x, y, t.v);
                float w_reciprocal = 1.0 / (std::get<0>(alpha) / v[0].w() + std::get<1>(alpha) / v[1].w() + std::get<2>(alpha) / v[2].w());
                float z_interpolated = std::get<0>(alpha) * v[0].z() / v[0].w() + std::get<1>(alpha) * v[1].z() / v[1].w() + std::get<2>(alpha) * v[2].z() / v[2].w();
                z_interpolated *= w_reciprocal;
                min_dep = MIN(min_dep, z_interpolated);
            }
        }
        if (count == 0)
            continue;
        if (min_dep < depth_buf[get_index(x, y)])
        {
            depth_buf[get_index(x, y)] = min_dep;
            set_pixel(Vector3f(x, y, min_dep), t.getColor() * count / sam_num4);
        }
    }
}
```

## 结果图

### 未抗锯齿

![result](https://i.loli.net/2021/06/24/U5l6M1RWhAxNXrV.png)

### 2x2 MSAA 

![result2x2](https://i.loli.net/2021/06/24/Nq7f9n1pWi5QK2L.png)

### 4X4 MSAA

![result4x4](https://i.loli.net/2021/06/24/yPCYjwNZpR6dKlB.png)

## 参考：

[pcl运行无法解析的外部符号 __imp__calloc_dbg](https://blog.csdn.net/weixin_53110214/article/details/114456911)