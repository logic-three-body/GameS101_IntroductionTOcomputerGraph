#include"FrameWork.h"
#include"global.h"
int main(int argc, char** argv) {
	auto path1 = "TriangleTransform/test20.tga";
	auto path2 = "Lesson5Camera/test1.tga";
	rasterizer r(width,height);
	r.InitZBuffer();
	r.viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4, depth);
	float cof= -1.f / (eye - center).norm();
	//r.Prespect_projection(cof);
	r.projection();
	r.lookat(eye, center, Vec3f(0,0,1));



	Vec3f p0(0, 0, 1), p1(1, 0, 0), p2(0, 1, 0);
	Vec3f p3(0, 1, 1), p4(1, 1, 0), p5(1, 0, 1);
	Vec3f pt[3],pt1[3];
	//pt[0] = r.world2screen(p0);
	//pt[1] = r.world2screen(p1);
	//pt[3] = r.world2screen(p2);
	Matrix Transform = r.GetViewPort()*r.GetProjection()*r.GetModelView();
	//Matrix Transform = r.GetViewPort();
	pt[0] = r.m2v(Transform*r.v2m(p0));
	pt[1] = r.m2v(Transform*r.v2m(p1));
	pt[2] = r.m2v(Transform*r.v2m(p2));
	Triangle3f t(pt);
	pt1[0] = r.m2v(Transform*r.v2m(p3));
	pt1[1] = r.m2v(Transform*r.v2m(p4));
	pt1[2] = r.m2v(Transform*r.v2m(p5));
	Triangle3f t1(pt1);
	r.DrawInterpolateTrangile(t,white);
	r.DrawInterpolateTrangile(t1, red);
	//r.DrawModelFrame(model, light_dir);
	r.WriteBuffer(path1);
	//r.ClearBuffer();
	//r.WriteBuffer(path2);
	return 0;
}