# GAMES101->assignment5 光线与三角形相交

项目地址：[here](https://github.com/logic-three-body/GameS101_IntroductionTOcomputerGraph/tree/%E4%BD%9C%E4%B8%9A1/Whitted%20Style%20Ray%20Tracing)

## 实现

### 生成光线：

```c++
// [comment]
// The main render function. This where we iterate over all pixels in the image, generate
// primary rays and cast these rays into the scene. The content of the framebuffer is
// saved to a file.
// [/comment]
void Renderer::Render(const Scene& scene)
{
    std::vector<Vector3f> framebuffer(scene.width * scene.height);

    float scale = std::tan(deg2rad(scene.fov * 0.5f));
    float imageAspectRatio = scene.width / (float)scene.height;

    // Use this variable as the eye position to start your rays.
    Vector3f eye_pos(0);
    int m = 0;
    for (int j = 0; j < scene.height; ++j)
    {
        for (int i = 0; i < scene.width; ++i)
        {
            // generate primary ray direction

			// Screen space to NDC space
			float nx = (i + 0.5f) * 2 / scene.width - 1.0f;
			float ny = (j + 0.5f) * 2 / scene.height - 1.0f;
			// NDC space to world space

			// Project matrix
			/*
			*   [ n/r ,0   ,0       ,0      ]
			*   [ 0   ,n/t ,0       ,0      ]
			*   [ 0   ,0   ,n+f/n-f ,2nf/f-n]
			*   [ 0   ,0   ,1       ,0      ]
			*/
			//  
// 在投影矩阵中 x 的系数为 n/r
// 在投影矩阵中 y 的系数为 n/t
// 现在要做一个逆操作，所以我们用 NDC空间的坐标分别除以投影矩阵中的系数
// x = nx / n / r
// y = ny / n / t
// 其中 n(相机到近投影面距离为 默认情况下为1）
// => 
// x = nx * r
// y = ny * t
// 其中 r = tan(fov/2)*aspect * |n|， t=tan(fov/2) * |n| , |n| = 1
// 所以可得,世界空间中坐标为
// x = nx * tan(fov/2)*aspect
// y = ny * tan(fov/2)*aspect
			float x = nx * scale * imageAspectRatio;
			float y = -ny * scale;
            // TODO: Find the x and y positions of the current pixel to get the direction
            // vector that passes through it.
            // Also, don't forget to multiply both of them with the variable *scale*, and
            // x (horizontal) variable with the *imageAspectRatio*            

            Vector3f dir = Vector3f(x, y, -1); // Don't forget to normalize this direction!
			dir = normalize(dir);//normalize this direction!
            framebuffer[m++] = castRay(eye_pos, dir, scene, 0);
        }
        UpdateProgress(j / (float)scene.height);
    }

    // save framebuffer to file
    FILE* fp = fopen("binary.ppm", "wb");
    (void)fprintf(fp, "P6\n%d %d\n255\n", scene.width, scene.height);
    for (auto i = 0; i < scene.height * scene.width; ++i) {
        static unsigned char color[3];
        color[0] = (char)(255 * clamp(0, 1, framebuffer[i].x));
        color[1] = (char)(255 * clamp(0, 1, framebuffer[i].y));
        color[2] = (char)(255 * clamp(0, 1, framebuffer[i].z));
        fwrite(color, 1, 3, fp);
    }
    fclose(fp);    
}
```



### 光线与三角形求交：

```C++
bool rayTriangleIntersect(const Vector3f& v0, const Vector3f& v1, const Vector3f& v2, const Vector3f& orig,
                          const Vector3f& dir, float& tnear, float& u, float& v)
{
    // TODO: Implement this function that tests whether the triangle
    // that's specified bt v0, v1 and v2 intersects with the ray (whose
    // origin is *orig* and direction is *dir*)
    // Also don't forget to update tnear, u and v.

	Vector3f e1 = v1 - v0;
	Vector3f e2 = v2 - v0;
	Vector3f s0 = orig - v0;
	Vector3f s1 = crossProduct(dir, e2);
	Vector3f s2 = crossProduct(s0, e1);

	Vector3f s = Vector3f(dotProduct(s2, e2), dotProduct(s1, s0), dotProduct(s2, dir)) / dotProduct(s1, e1);
	tnear = s.x;
	u = s.y;
	v = s.z;

	if (tnear >= 0 && u >= 0 && v >= 0 && (u + v) <= 1)
	{
		return true;
	}

    return false;
}
```

## 结果

![image-20210726124752051](https://i.loli.net/2021/07/26/KyHpxYeNL86nGMk.png)