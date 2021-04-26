#pragma once
#include"geometry.h"
typedef struct Pointi
{
	int x;
	int y;

public:
	Pointi() {};
	Pointi(int _x, int _y) :x(_x), y(_y) {};
};

typedef struct Trianglei
{
	Pointi p1;
	Pointi p2;
	Pointi p3;
public:
	Trianglei() {};
	Trianglei(Pointi _p1,Pointi _p2,Pointi _p3):p1(_p1),p2(_p2),p3(_p3) {};
	Trianglei(Vec2i _p1,Vec2i _p2,Vec2i _p3) 
	{
		p1.x = _p1.x;
		p1.y = _p1.y;
		p2.x = _p2.x;
		p2.y = _p2.y;
		p3.x = _p3.x;
		p1.y = _p3.y;
	};
};


