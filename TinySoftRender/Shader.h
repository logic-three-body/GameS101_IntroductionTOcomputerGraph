#pragma once
#include"TGAImage.h"
#include"geometry.h"
#include<algorithm>
typedef struct IShader {
protected:
	Model* model;
	Vec3f light_dir;
	Matrix ModelView;
	Matrix Viewport;
	Matrix Projection;
public:
	virtual ~IShader() { model = nullptr; };
	virtual Vec4f vertex(int iface, int nthvert) = 0;
	virtual bool fragment(Vec3f bar, TGAColor &color) = 0;
	void GetTransformMat(Matrix modelview,Matrix viewport,Matrix projection)
	{
		ModelView = modelview;
		Viewport = viewport;
		Projection = projection;
	}
	void getLightdirect(Vec3f dir) { light_dir=dir; };
	void getModel(Model* m) { model = m; };
};

//高罗得着色
typedef struct GouraudShader : public IShader {


	Vec3f varying_intensity; // written by vertex shader, read by fragment shader
	virtual Vec4f vertex(int iface, int nthvert) {
		Vec4f gl_Vertex = embed<4>(model->vert(iface, nthvert)); // read the vertex from .obj file
		gl_Vertex = Viewport * Projection*ModelView*gl_Vertex;     // transform it to screen coordinates
		varying_intensity[nthvert] = std::max(0.f, model->normal(iface, nthvert)*light_dir); // get diffuse lighting intensity
		return gl_Vertex;
	}

	virtual bool fragment(Vec3f pixel, TGAColor &color) {
		float intensity = varying_intensity * pixel;   // interpolate intensity for the current pixel
		color = TGAColor(255, 255, 255)*intensity; // well duh
		return false;                              // no, we do not discard this pixel
	}
};