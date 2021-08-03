# GAMES101->assignment6 

项目地址：[GameS101_IntroductionTOcomputerGraph/Whitted Style Ray Tracing Accelerate at 作业1 · logic-three-body/GameS101_IntroductionTOcomputerGraph (github.com)](https://github.com/logic-three-body/GameS101_IntroductionTOcomputerGraph/tree/作业1/Whitted Style Ray Tracing Accelerate)

## 包围盒求交

```C++
inline bool Bounds3::IntersectP(const Ray& ray, const Vector3f& invDir,
                                const std::array<int, 3>& dirIsNeg) const
{
    // invDir: ray direction(x,y,z), invDir=(1.0/x,1.0/y,1.0/z), use this because Multiply is faster that Division
    // dirIsNeg: ray direction(x,y,z), dirIsNeg=[int(x>0),int(y>0),int(z>0)], use this to simplify your logic
    // TODO test if ray bound intersects
    
	const auto& origin = ray.origin;
	float tEnter = -std::numeric_limits<float>::infinity();
	float tExit = std::numeric_limits<float>::infinity();
	for (int i = 0; i < 3; i++)
	{
		float min = (pMin[i] - origin[i]) * invDir[i];
		float max = (pMax[i] - origin[i]) * invDir[i];
		if (!dirIsNeg[i])
		{
			std::swap(min, max);
		}
		tEnter = std::max(min, tEnter);
		tExit = std::min(max, tExit);
	}
	return tEnter < tExit && tExit >= 0;

}
```

## BVH加速光线与场景求交

```C++
Intersection BVHAccel::getIntersection(BVHBuildNode* node, const Ray& ray) const
{
    // TODO Traverse the BVH to find intersection
	Intersection isect;

	std::array<int, 3> dirIsNeg;
	dirIsNeg[0] = int(ray.direction.x >= 0);
	dirIsNeg[1] = int(ray.direction.y >= 0);
	dirIsNeg[2] = int(ray.direction.z >= 0);

	if (!node->bounds.IntersectP(ray, ray.direction_inv, dirIsNeg))
	{
		return isect;
	}

	// 叶子节点
	if (node->left == nullptr && node->right == nullptr)
	{
		isect = node->object->getIntersection(ray);
		return isect;
	}

	auto hit1 = getIntersection(node->left, ray);
	auto hit2 = getIntersection(node->right, ray);

	return hit1.distance < hit2.distance ? hit1 : hit2;
}
```

```C++
inline Intersection Triangle::getIntersection(Ray ray)
{
    // TODO find ray triangle intersection
	Intersection inter;

	Vector3f e1 = v1 - v0;
	Vector3f e2 = v2 - v0;
	Vector3f s0 = ray.origin - v0;
	Vector3f s1 = crossProduct(ray.direction, e2);
	Vector3f s2 = crossProduct(s0, e1);

	Vector3f s = Vector3f(dotProduct(s2, e2), dotProduct(s1, s0), dotProduct(s2, ray.direction)) / dotProduct(s1, e1);
	float tnear = s.x;
	float u = s.y;
	float v = s.z;

	if (tnear >= 0 && u >= 0 && v >= 0 && (u + v) <= 1)
	{

		inter.happened = true;
		inter.coords = Vector3f(tnear, u, v);
		inter.normal = normal;
		inter.m = m;
		inter.obj = this;
		inter.distance = tnear;
		return inter;
	}
    return inter;
}
```



## 结果图

![image-20210803124001019](https://i.loli.net/2021/08/03/qlGxRXJtf6yiLAu.png)

![image-20210803124014305](https://i.loli.net/2021/08/03/tMxp7jXZIo19UAS.png)

![image-20210803124032622](https://i.loli.net/2021/08/03/18VP3oIncfpRA5H.png)