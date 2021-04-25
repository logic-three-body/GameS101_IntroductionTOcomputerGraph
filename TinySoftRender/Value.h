#pragma once
typedef struct Point
{
	int x;
	int y;

public:
	Point() {};
	Point(int _x, int _y) :x(_x), y(_y) {};
};

typedef struct Triangle
{
	Point p1;
	Point p2;
	Point p3;
public:
	Triangle() {};
	Triangle(Point _p1,Point _p2,Point _p3):p1(_p1),p2(_p2),p3(_p3) {};
};


