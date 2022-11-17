#pragma once
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cmath>
#include "menu.h"
#include"graphics.h"
#include"algorithms.h"
using namespace std;

vector<Polygon1> polygons;
vector<Circle> circles;
Polygon1 p,q;
Circle c;
point tem{ -1,-1 };
point cut1{ -1,-1 },cut2{ -1,-1 },cutneed;//记录裁剪的图形
bool first = true;
bool c_first = true;
int tran_x, tran_y;
int _xtmp, _ytmp;//作为缩放变量用
int cut_point;

void myDisplay() {
	glClearColor(0.0, 0.0, 0.0, 0.0);//设置背景色为黑色
	glClear(GL_COLOR_BUFFER_BIT);//清除颜色缓冲区
	glColor3f(1.0, 1.0, 1.0);// 内容显示颜色
	cout << menu_num;
	cout << "!!" << cut_point << "!!" << cut1.x ;
	if (cut_point) {
		setPixel_point_red(cut1.x, cut1.y);
		setPixel_point_red(cut2.x, cut2.y);
	}

	for (unsigned int i = 0; i < polygons.size(); i++) {
		int xx = 0;
		int yy = 0;
		if (polygons[i].cutit == 1) {
			Sutherland_Hodgman(cut1, cut2, polygons[i]);
			continue;//把需要裁剪的曲线不输出
		}
			
		for (int j = 0; j < polygons[i].points.size(); j++) {

			if (j == 0) {
				xx = polygons[i].points[j].x;
				yy = polygons[i].points[j].y;
				setPixel_point(xx, yy);
			}
			else {
				lineDDA(xx, yy, polygons[i].points[j].x, polygons[i].points[j].y);
				xx = polygons[i].points[j].x;
				yy = polygons[i].points[j].y;
				
			}
			if (polygons[i].points[j].color == 1) {
				setPixel_point_red(polygons[i].points[j].x, polygons[j].points[0].y);
			}
			else {
				setPixel_point(polygons[i].points[j].x, polygons[i].points[j].y);
			}
		}
		// 注意y值的改变
		//填充算法判断
		if (polygons[i].full == 1) {
			PolyScan(polygons[i].points);
		}
		if (polygons[i].bezier == 1) {
			bezier(polygons[i].points);
		}
	}
	for (unsigned int i = 0; i < circles.size(); i++) {//画椭圆
		if (circles[i].points.size() != 2) continue;
		int xc = circles[i].points[0].x, yc = circles[i].points[0].y;
		int rx = circles[i].points[1].x, ry = circles[i].points[1].y;
		eclipseBres(xc, yc, rx, ry);
		if (circles[i].points[0].color == 1) {
			setPixel_point_red(circles[i].points[0].x, circles[i].points[0].y);
		}
		else {
			setPixel_point(circles[i].points[0].x, circles[i].points[0].y);
		}
	}
	//处理裁剪

	glEnd();
	glutSwapBuffers();//清空命令缓冲区并交换帧缓存：
					//交换前后台两个缓冲区指针
}

point findNearPointNum(float x, float y, float d) {
	point o{ -1, -1 };
	if (polygons.size() == 0) {
		return o;
	}
	double min = -1;
	for (unsigned int i = 0; i < polygons.size(); i++) {
		for (unsigned int j = 0; j < polygons[i].points.size(); j++) {
			double distence = (polygons[i].points[j].x - x)*(polygons[i].points[j].x - x) + (polygons[i].points[j].y - y)*(polygons[i].points[j].y - y);
			if (min == -1) {
				min = distence;
				o.x = i;
				o.y = j;
			}
			else if (min > distence) {
				min = distence;
				o.x = i;
				o.y = j;
				cout << "!!";
			}
			cout << polygons[i].points[j].x << " " << x << endl;
			cout << min <<" " << distence << endl;
		}
		
		// 注意y值的改变
	}
	if (min > d * d * d) {
		o.x = -1;
		return o;
	}
	return o;
}

void work_point(int button, int state, int x, int y) {
	y = 600 - y;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {//鼠标左键按下
		if (menu_num == menu_mode.menu1_1) {
			point o{ -2,-2 };
			o = findNearPointNum(x, y, 10);
			if (o.x != -1) {
				x = polygons[o.x].points[o.y].x;
				y = polygons[o.x].points[o.y].y;
			}
			point t{ x, y };
			if (first) {
				p.points.push_back(t);
				polygons.push_back(p);
				first = false;
			}
			else {
				p.points.push_back(t);
				polygons.pop_back();
				polygons.push_back(p);
			}
		}
		if (menu_num == menu_mode.menu1_2) {
			point t{ x, y };
			if (c_first) {
				c.points.push_back(t);
				cout << "请输入a和b" << endl;
				int xx, yy;
				cin >> xx >> yy;
				point t{ xx, yy };
				c.points.push_back(t);
				circles.push_back(c);
				first = false;
			}
			c_first = true;
			for (unsigned int i = 0; i < circles.size(); i++) {
				for (unsigned int j = 0; j < circles[i].points.size(); j++) {
					cout << circles[i].points[j].x << " " << circles[i].points[j].y << endl;
				}
				// 注意y值的改变
			}
			c.points.clear();
			menu_num = 0;
		}
		//找到那个点
		if (menu_num == menu_mode.menu2_1 || menu_num == menu_mode.menu2_2 || menu_num == menu_mode.menu2_3) {
			point o{ -2,-2 };
			matrix3x3SetIdentity(matComposite);
			o = findNearPointNum(x, y, 20);
			cout << "?" << o.y << endl;
			if (o.x != -1) {
				tran_x = polygons[o.x].points[o.y].x;
				tran_y = polygons[o.x].points[o.y].y;
				polygons[o.x].points[o.y].color = 1;
				tem = o;
			}
			
		}
		if (menu_num == menu_mode.menu3_1 || menu_num == menu_mode.menu3_2) {
			point o{ -2,-2 };
			o = findNearPointNum(x, y, 20);
			if (o.x != -1) {
				polygons[o.x].full = 1;
				tem = o;
			}
		}
		if (menu_num == menu_mode.menu3_1 || menu_num == menu_mode.menu3_2) {
			point o{ -2,-2 };
			o = findNearPointNum(x, y, 20);
			if (o.x != -1) {
				polygons[o.x].full = 1;
				tem = o;
			}
		}
		if (menu_num == menu_mode.menu4) {//删除
			point o{ -2,-2 };
			o = findNearPointNum(x, y, 20);
			if (o.x != -1) {
				polygons.erase(polygons.begin() + o.x, polygons.begin() + o.x + 1);
				return;
			}
		}
		if (menu_num == menu_mode.menu5_1) {//裁剪选定边框
			if (cut_point >= 2) {
				return;
			}
			else {
				if (cut_point == 0) {
					cut1.x = x, cut1.y = y;
					cut_point++;
				}
				else {
					cut2.x = x, cut2.y = y;
					cut_point++;
				}
			}
		}
		if (menu_num == menu_mode.menu5_2) {//裁剪选定
			if (cut_point < 2) {
				return;
			}
			point o{ -2,-2 };
			o = findNearPointNum(x, y, 20);
			if (o.x != -1) {
				tem = o;
				polygons[o.x].cutit = 1;
				polygons[o.x].points[o.y].color = 1;
				return;
			}
		}
		if (menu_num == menu_mode.menu5_3) {//裁剪取消
			cut_point = 0;
			point o{0, 0};
			cut1 = cut2 = o;
			polygons[tem.x].cutit = 0;
		}
		if (menu_num == menu_mode.menu6) {//裁剪选定
			point o{ -2,-2 };
			o = findNearPointNum(x, y, 20);
			if (o.x != -1) {
				polygons[o.x].bezier = 1;
				return;
			}
		}

	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {//鼠标右键按下
		if (menu_num == menu_mode.menu1_1) {
			first = true;
			for (unsigned int i = 0; i < polygons.size(); i++) {
				for (unsigned int j = 0; j < polygons[i].points.size(); j++) {
					cout << polygons[i].points[j].x << " " << polygons[i].points[j].y <<" "<< polygons[i].points[j].color << endl;
				}
				// 注意y值的改变
			}
			p.points.clear();
			menu_num = 0;
		}
		if (menu_num == menu_mode.menu2_1 || menu_num == menu_mode.menu2_2 || menu_num == menu_mode.menu2_3) {
			if (tem.x != -1) {
				int delx = x - polygons[tem.x].points[tem.y].x;
				int dely = y - polygons[tem.x].points[tem.y].y;
				for (int i = 0; i < polygons[tem.x].points.size(); i++) {
					polygons[tem.x].points[i].x += delx;
					polygons[tem.x].points[i].y += dely;
				}
				polygons[tem.x].points[tem.y].color = 0;
				menu_num = 0;
				tem.x = -1, tem.y = -1;
				tran_x = 0, tran_y = 0, _xtmp = 0, _ytmp = 0;
			}
			
		}
		if (menu_num == menu_mode.menu3_1) {//保存
			tem.x = -1, tem.y = -1;
			menu_num = 0;
		}
		if (menu_num == menu_mode.menu3_2) {//不保存
			polygons[tem.x].full = 0;
			tem.x = -1, tem.y = -1;
			menu_num = 0;
		}
		if (menu_num == menu_mode.menu4) {//删除
			menu_num = 0;
		}
		if (menu_num == menu_mode.menu5_2) {
			cut1.x = -1, cut1.y = -1;
			cut2.x = -1, cut2.y = -1;
			menu_num = 0;
		}
		if (menu_num == menu_mode.menu6) {
			menu_num = 0;
		}
	}
}



void dragmouse(int x, int y) {
	y = 600 - y;
	float ssx = 1, ssy = 1;
	if (tem.x == -1) return;
	cout << "wuwu" << endl;
	if (menu_num == menu_mode.menu2_1) {//平移
		translate2D(x - tran_x, y - tran_y);
		transformVerts2D(polygons[tem.x].points);
		tran_x = x;
		tran_y = y;
	}
	if (menu_num == menu_mode.menu2_2) {//旋转
		if (x <= _xtmp && y >= _ytmp)
			rotate2D(tran_x, tran_y, -20);
		else
			rotate2D(tran_x, tran_y, 20);
		transformVerts2D(polygons[tem.x].points);
		_xtmp = x;
		_ytmp = y;
	}
	if (menu_num == menu_mode.menu2_3) {//缩放
		/*不等比例缩放*/
		if (x > _xtmp) {
			ssx += 0.01f;
		}
		else if (x < _xtmp&&ssx>0) {
			ssx -= 0.01f;
		}
		if (y < _ytmp) {
			ssy += 0.01f;
		}
		else if (y > _ytmp&&ssy > 0) {
			ssy -= 0.01f;
		}
		scale2D(ssx, ssy, tran_x, tran_y);
		transformVerts2D(polygons[tem.x].points);
		_xtmp = x;
		_ytmp = y;
	}

}



