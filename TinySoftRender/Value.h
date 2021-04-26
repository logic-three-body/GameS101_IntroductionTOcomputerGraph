#pragma once
#include"geometry.h"
typedef struct Pointi
{
	int x;
	int y;

public:
	Pointi():x(0),y(0) {};
	Pointi(int _x, int _y) :x(_x), y(_y) {};
	Pointi(Vec2i v):x(v.x),y(v.y) {};
};

typedef struct Trianglei
{
	Pointi p0;
	Pointi p1;
	Pointi p2;
public:
	Trianglei():p0(Vec2i(0,0)), p1(Vec2i(0, 0)), p2(Vec2i(0, 0)) {};
	Trianglei(Pointi _p1,Pointi _p2,Pointi _p3):p0(_p1),p1(_p2),p2(_p3) {};
	Trianglei(Vec2i _p1,Vec2i _p2,Vec2i _p3) 
	{
		p0.x = _p1.x;
		p0.y = _p1.y;
		p1.x = _p2.x;
		p1.y = _p2.y;
		p2.x = _p3.x;
		p0.y = _p3.y;
	};
};


