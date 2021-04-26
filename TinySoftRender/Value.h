#pragma once
#include"geometry.h"
typedef struct Trianglei
{
	Vec2i p0;
	Vec2i p1;
	Vec2i p2;
public:
	Trianglei():p0(Vec2i(0,0)), p1(Vec2i(0, 0)), p2(Vec2i(0, 0)) {};
	Trianglei(Vec2i _p0, Vec2i _p1, Vec2i _p2) :p0(_p0), p1(_p1), p2(_p2) {};
	Trianglei(Vec2i*vi)
	{
		if (vi)
		{
			p0 = vi[0];
			p1 = vi[1];
			p2 = vi[2];
		}
		else
		{
			Trianglei();
		}
	};
};


