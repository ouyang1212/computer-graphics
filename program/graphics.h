#pragma once
#include <iostream>
#include <vector>
using namespace std;
struct point {
	float x, y;
	int color = 0;
	bool operator < (const point& a)const //ÖØÔØÔËËã·û
	{
		return x < a.x;
	}
};

struct Polygon1 {
	vector<point> points;
	int full = 0;
	int bezier = 0;
	int cutit = 0;
};

struct Circle {
	vector<point> points;
};


