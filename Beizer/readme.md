# GAMES101->assignment4 Bézier Curve

项目地址：[here](https://github.com/logic-three-body/GameS101_IntroductionTOcomputerGraph/tree/%E4%BD%9C%E4%B8%9A4/Bezier_curve)

## Bézier 曲线递归算法

```c++
cv::Point2f recursive_bezier(const std::vector<cv::Point2f> &control_points, float t)
{
	// TODO: Implement de Casteljau's algorithm
	if (control_points.size() == 1)
	{
		return control_points[0];
	}


	std::vector<cv::Point2f> lerp_points;
	for (size_t i = 1; i < control_points.size(); i++)
	{
		lerp_points.push_back(lerp_v2f(control_points[i - 1], control_points[i], t));
	}
	return recursive_bezier(lerp_points, t);

}
```

```c++
void bezier(const std::vector<cv::Point2f> &control_points, cv::Mat &window)
{
	// TODO: Iterate through all t = 0 to t = 1 with small steps, and call de Casteljau's 
	// recursive Bezier algorithm.
	for (double t = 0.0; t <= 1.0; t += 0.001)
	{
		auto point = recursive_bezier(control_points, t);
		window.at<cv::Vec3b>(point.y, point.x)[1] = 255;//控制颜色
	}
}
```

## 结果图

### naive_bezier off

![my_bezier_curve_off2](https://i.loli.net/2021/07/12/kcCB1FT478WwQzt.png)

![my_bezier_curve_off1](https://i.loli.net/2021/07/12/mlSePfR389CQ1uy.png)

### naive_bezier on

![my_bezier_curve_on2](https://i.loli.net/2021/07/12/82eLuRMAy36GICQ.png)![my_bezier_curve_on](https://i.loli.net/2021/07/12/aVtm9RI4Q2skrxY.png)