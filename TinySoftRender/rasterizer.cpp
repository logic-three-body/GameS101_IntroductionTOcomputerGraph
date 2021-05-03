#include "rasterizer.h"
#include"compute.h"
void rasterizer::InitZBuffer()
{
	ZBuffer = new float[width*height];
	for (int i = width * height; i--; ZBuffer[i] = -std::numeric_limits<float>::max());
}
void rasterizer::lineBresenham(int x0, int y0, int x1, int y1, const TGAColor & color)
{
	bool steep = false;
	if (std::abs(x0 - x1) < std::abs(y0 - y1))
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0 > x1)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	for (int x = x0; x <= x1; x++)
	{
		float t = (x - x0) / float(x1 - x0);
		int y = y0 * (1.0f - t) + y1 * t;
		if (steep)
		{
			frameBuffer.setpixel(y, x, color);
		}
		else
		{
			frameBuffer.setpixel(x, y, color);
		}
	}
}

void rasterizer::lineBresenham(Vec2i p0, Vec2i p1, const TGAColor & color)
{
	lineBresenham(p0.x, p0.y, p1.x, p1.y,color);
}

void rasterizer::DrawWireTrangile(Trianglei & t, const TGAColor & color)
{
	DrawWireTrangile(t.p0, t.p1, t.p2, color);
}

void rasterizer::DrawWireTrangile(Vec2i p0, Vec2i p1, Vec2i p2, const TGAColor & color)
{
	lineBresenham(p0, p1, color);
	lineBresenham(p1, p2, color);
	lineBresenham(p2, p0, color);
}

void rasterizer::Draw2DFillTrangile(Trianglei & t, const TGAColor & color)
{
	Draw2DFillTrangile(t.p0, t.p1, t.p2, color);
}

void rasterizer::Draw2DFillTrangile(Vec2i p0, Vec2i p1, Vec2i p2, const TGAColor & color)
{
	// sort the vertices, t0, t1, t2 lower−to−upper (bubblesort yay!) 
	if (p0.y > p1.y) std::swap(p0, p1);
	if (p0.y > p2.y) std::swap(p0, p2);
	if (p1.y > p2.y) std::swap(p1, p2);
	int total_height = p2.y - p0.y;//计算高度
	//画p0->p1
	for (int y = p0.y; y <= p1.y; y++) {
		int segment_height = p1.y - p0.y + 1;
		float alpha = (float)(y - p0.y) / total_height;
		float beta = (float)(y - p0.y) / segment_height; // be careful with divisions by zero 
		Vec2i A = p0 + (p2 - p0)*alpha;
		Vec2i B = p0 + (p1 - p0)*beta;
		if (A.x > B.x) std::swap(A, B);//水平扫描
		for (int j = A.x; j <= B.x; j++) {
			frameBuffer.setpixel(j, y, color); // attention, due to int casts t0.y+i != A.y 
		}
	}
	//画p1->p2
	for (int y = p1.y; y <= p2.y; y++) {
		int segment_height = p2.y - p1.y + 1;
		float alpha = (float)(y - p0.y) / total_height;
		float beta = (float)(y - p1.y) / segment_height; // be careful with divisions by zero 
		Vec2i A = p0 + (p2 - p0)*alpha;
		Vec2i B = p1 + (p2 - p1)*beta;
		if (A.x > B.x) std::swap(A, B);//水平扫描
		for (int j = A.x; j <= B.x; j++) {
			frameBuffer.setpixel(j, y, color); // attention, due to int casts t0.y+i != A.y 
		}
	}
}

void rasterizer::Draw2DInterpolateTrangile(Trianglei&t, const TGAColor&color)
{
	Draw2DInterpolateTrangile(t.p0, t.p1, t.p2, color);
}

void rasterizer::Draw2DInterpolateTrangile(Vec2i p0, Vec2i p1, Vec2i p2, const TGAColor & color)
{
	Vec2i* pts = new Vec2i[3];
	pts[0] = p0;
	pts[1] = p1;
	pts[2] = p2;
	Vec2i boundingbox_min(frameBuffer.get_width()-1,frameBuffer.get_height()-1);
	Vec2i boundingbox_max(0,0);
	Vec2i clamp(frameBuffer.get_width() - 1, frameBuffer.get_height() - 1);
	for (size_t i = 0; i < 3; i++)//设置包围盒
	{
		for (size_t j = 0; j < 2; j++)
		{
			boundingbox_min[j] = std::max(0, std::min(boundingbox_min[j], pts[i][j]));
			boundingbox_max[j] = std::min(clamp[j],std::max(boundingbox_max[j],pts[i][j]));
		}
	}
	Vec2i P;
	for (P.x = boundingbox_min.x; P.x <= boundingbox_max.x; P.x++)
	{
		for (P.y = boundingbox_min.y; P.y <= boundingbox_max.y; P.y++)
		{
			Vec3f bc_screen = barycentric(pts, P);
			if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) 
				continue;
			frameBuffer.setpixel(P.x,P.y,color);
		}
	}

	delete[] pts;
}

void rasterizer::Draw2DWireFrame(Model & model, int width, int height, const TGAColor & color)
{
	for (int i = 0; i < model.nfaces(); i++) {
		std::vector<int> face = model.face(i);
		for (int j = 0; j < 3; j++) {
			Vec3f v0 = model.vert(face[j]);
			Vec3f v1 = model.vert(face[(j + 1) % 3]);
			int x0 = (v0.x + 1.)*width / 2.;
			int y0 = (v0.y + 1.)*height / 2.;
			int x1 = (v1.x + 1.)*width / 2.;
			int y1 = (v1.y + 1.)*height / 2.;
			lineBresenham(x0, y0, x1, y1,color);
		}
	}
}

void rasterizer::Draw2DWireFrame(Model & model, const TGAColor & color)
{
	Draw2DWireFrame(model, width, height,color);
}

void rasterizer::Draw2DFlatFrame(Model & model)
{
	Draw2DFlatFrame(model, width, height);
}

void rasterizer::Draw2DFlatFrame(Model & model, int width, int height)
{
	for (int i = 0; i < model.nfaces(); i++) {
		std::vector<int> face = model.face(i);
		Vec2i screen_coords[3];
		for (int j = 0; j < 3; j++) {
			Vec3f world_coords = model.vert(face[j]);
			screen_coords[j] = Vec2i((world_coords.x + 1.)*width / 2., (world_coords.y + 1.)*height / 2.);
		}
		Draw2DInterpolateTrangile(screen_coords[0], screen_coords[1], screen_coords[2], TGAColor(rand() % 255, rand() % 255, rand() % 255, 255));
	}
}

void rasterizer::DrawGray2DFrame(Model & model, Vec3f light_dir = Vec3f(0,0,0))
{
	DrawGray2DFrame(model, width, height,light_dir);
}

void rasterizer::DrawGray2DFrame(Model & model, int width, int height, Vec3f light_dir = Vec3f(0,0,0))
{
	for (int i = 0; i < model.nfaces(); i++) {
		std::vector<int> face = model.face(i);
		Vec2i screen_coords[3];
		Vec3f world_coords[3];
		for (int j = 0; j < 3; j++) {
			Vec3f v = model.vert(face[j]);
			screen_coords[j] = Vec2i((v.x + 1.)*width / 2., (v.y + 1.)*height / 2.);
			world_coords[j] = v;
		}
		Vec3f n = cross((world_coords[2] - world_coords[0]), (world_coords[1] - world_coords[0]));
		n.normalize();
		float intensity = n * light_dir;
		if (intensity > 0) {
			Draw2DInterpolateTrangile(screen_coords[0], screen_coords[1], screen_coords[2], TGAColor(intensity * 255, intensity * 255, intensity * 255, 255));
		}
	}
}

void rasterizer::DrawFillTrangile(Triangle3i & t, const TGAColor & color)
{
	DrawFillTrangile(t.p0, t.p1, t.p2, color);
}

void rasterizer::DrawFillTrangile(Vec3i p0, Vec3i p1, Vec3i p2, const TGAColor & color)
{
	if (p0.y == p1.y && p0.y == p2.y) return; // i dont care about degenerate triangles
	if (p0.y > p1.y) std::swap(p0, p1);
	if (p0.y > p2.y) std::swap(p0, p2);
	if (p1.y > p2.y) std::swap(p1, p2);
	int total_height = p2.y - p0.y;
	for (int i = 0; i < total_height; i++) {
		bool second_half = i > p1.y - p0.y || p1.y == p0.y;
		int segment_height = second_half ? p2.y - p1.y : p1.y - p0.y;
		float alpha = (float)i / total_height;
		float beta = (float)(i - (second_half ? p1.y - p0.y : 0)) / segment_height; // be careful: with above conditions no division by zero here
		Vec3i A = p0 + (p2 - p0)*alpha;
		Vec3i B = second_half ? p1 + (p2 - p1)*beta : p0 + (p1 - p0)*beta;
		if (A.x > B.x) std::swap(A, B);
		for (int j = A.x; j <= B.x; j++) {
			float phi = B.x == A.x ? 1. : (float)(j - A.x) / (float)(B.x - A.x);
			Vec3i P = A + (B - A)*phi;
			P.x = j; P.y = p0.y + i; // a hack to fill holes (due to int cast precision problems)
			int idx = j + (p0.y + i)*width;
			if (ZBuffer[idx] < P.z) {//Z值小的会进来
				ZBuffer[idx] = P.z;
				frameBuffer.setpixel(P.x, P.y, color); // attention, due to int casts t0.y+i != A.y
			}
		}
	}
}

void rasterizer::DrawFillTrangile(Triangle3f & t, const TGAColor & color)
{
	DrawFillTrangile(t.p0, t.p1, t.p2, color);
}

void rasterizer::DrawFillTrangile(Vec3f p0, Vec3f p1, Vec3f p2, const TGAColor & color)
{
	if (p0.y == p1.y && p0.y == p2.y) return; // i dont care about degenerate triangles
	if (p0.y > p1.y) std::swap(p0, p1);
	if (p0.y > p2.y) std::swap(p0, p2);
	if (p1.y > p2.y) std::swap(p1, p2);
	int total_height = p2.y - p0.y;
	for (int i = 0; i < total_height; i++) {
		bool second_half = i > p1.y - p0.y || p1.y == p0.y;
		int segment_height = second_half ? p2.y - p1.y : p1.y - p0.y;
		float alpha = (float)i / total_height;
		float beta = (float)(i - (second_half ? p1.y - p0.y : 0)) / segment_height; // be careful: with above conditions no division by zero here
		Vec3f A = p0 + (p2 - p0)*alpha;
		Vec3f B = second_half ? p1 + (p2 - p1)*beta : p0 + (p1 - p0)*beta;
		if (A.x > B.x) std::swap(A, B);
		for (int j = A.x; j <= B.x; j++) {
			float phi = B.x == A.x ? 1. : (float)(j - A.x) / (float)(B.x - A.x);
			Vec3f P = A + (B - A)*phi;
			P.x = j; P.y = p0.y + i; // a hack to fill holes (due to int cast precision problems)
			int idx = j + (p0.y + i)*width;
			if (ZBuffer[idx] < P.z) {//Z值大的会进来(模型空间)
				ZBuffer[idx] = P.z;
				frameBuffer.setpixel(P.x, P.y, color); // attention, due to int casts t0.y+i != A.y
			}
		}
	}
}

void rasterizer::DrawInterpolateTrangile(Triangle3i & t, const TGAColor & color)
{
	DrawInterpolateTrangile(t.p0, t.p1, t.p2, color);
}

void rasterizer::DrawInterpolateTrangile(Vec3i p0, Vec3i p1, Vec3i p2, const TGAColor & color)
{
	Vec3i* pts = new Vec3i[3];
	pts[0] = p0;
	pts[1] = p1;
	pts[2] = p2;
	Vec2i bboxmin(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
	Vec2i bboxmax(-std::numeric_limits<int>::max(), -std::numeric_limits<int>::max());
	Vec2i clamp(frameBuffer.get_width() - 1, frameBuffer.get_height() - 1);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			bboxmin[j] = std::max(0, std::min(bboxmin[j], pts[i][j]));
			bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], pts[i][j]));
		}
	}
	Vec3i P;
	for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) {
		for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) {
			Vec3i bc_screen = barycentric(pts[0], pts[1], pts[2], P);
			if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;
			P.z = 0;
			for (int i = 0; i < 3; i++) P.z += pts[i][2] * bc_screen[i];
			if (ZBuffer[int(P.x + P.y*width)] > P.z) {
				ZBuffer[int(P.x + P.y*width)] = P.z;
				frameBuffer.setpixel(P.x, P.y, color);
			}
		}
	}
}

void rasterizer::DrawInterpolateTrangile(Triangle3f & t, const TGAColor & color)
{
	DrawInterpolateTrangile(t.p0, t.p1, t.p2, color);
}

void rasterizer::DrawInterpolateTrangile(Vec3f p0, Vec3f p1, Vec3f p2, const TGAColor & color)
{
	Vec3f* pts = new Vec3f[3];
	pts[0] = p0;
	pts[1] = p1;
	pts[2] = p2;
	Vec2f bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
	Vec2f clamp(frameBuffer.get_width() - 1, frameBuffer.get_height() - 1);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			bboxmin[j] = std::max(0.f, std::min(bboxmin[j], pts[i][j]));
			bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], pts[i][j]));
		}
	}
	Vec3f P;
	for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) {
		for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) {
			Vec3f bc_screen = barycentric(pts[0], pts[1], pts[2], P);
			if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;
			P.z = 0;
			for (int i = 0; i < 3; i++) P.z += pts[i][2] * bc_screen[i];
			if (ZBuffer[int(P.x + P.y*width)] < P.z) {
				ZBuffer[int(P.x + P.y*width)] = P.z;
				frameBuffer.setpixel(P.x, P.y, color);
			}
		}
	}
}

void rasterizer::DrawGrayFrame(Model & model, Vec3f light_dir)
{
	DrawGrayFrame(model, width, height, light_dir);
}

void rasterizer::DrawGrayFrame(Model & model, int width, int height, Vec3f light_dir)
{
	for (int i = 0; i < model.nfaces(); i++) {
		std::vector<int> face = model.face(i);
		Vec3f screen_coords[3];
		Vec3f world_coords[3];
		for (int i = 0; i < 3; i++) screen_coords[i] = world2screen(model.vert(face[i]));
		Triangle3f t(screen_coords);
		for (int j = 0; j < 3; j++) {
			Vec3f v = model.vert(face[j]);
			world_coords[j] = v;
		}
		Vec3f n = cross((world_coords[2] - world_coords[0]), (world_coords[1] - world_coords[0])).normalize();
		float intensity = n * light_dir.normalize();
		if (0 < intensity)
		{
			DrawInterpolateTrangile(t, TGAColor(int(intensity * 255) % 255, int(intensity * 255) % 255, int(intensity * 255) % 255, 255));
		}
	}
}

void rasterizer::viewport(int x, int y, int w, int h)
{
	Viewport = Matrix::identity();
	Viewport[0][3] = x + w / 2.f;
	Viewport[1][3] = y + h / 2.f;
	Viewport[2][3] = 1.f;
	Viewport[0][0] = w / 2.f;
	Viewport[1][1] = h / 2.f;
	Viewport[2][2] = 0;
}

void rasterizer::Prespect_projection(float coeff)
{
	Projection = Matrix::identity();
	Projection[3][2] = coeff;
}

void rasterizer::projection()
{
	Projection = Matrix::identity();
}
