#pragma once
#include"TGAImage.h"
#include"geometry.h"

typedef struct IShader
{
	virtual~ IShader();
	virtual Vec3i vertex(int iface, int nthvert) = 0;
	virtual bool fragment(Vec3f bar, TGAColor &color) = 0;
};