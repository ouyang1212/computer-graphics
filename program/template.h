#pragma once
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

struct point {
	float x;
	float y;
	bool polygonStart;// 一个多边形的起始点
};

vector<point> points;
int mode = 0;
/*	mode
	1:新建
	2:删除点
	3:移动点
	4:移动图形
	5:缩放-放大
	6:旋转
	7:缩小
*/
int flag = 0;//标记起点
bool dragging = false;// 用于标记正在拖动
int draggedNum = -1;// 被拖动的点的编号
int old_x, old_y;//记录原来位置
const double PI = atan(1.)*4.;//圆周率

void myDisplay() {
	glClearColor(0.0, 0.0, 0.0, 0.0);//设置背景色为黑色
	glClear(GL_COLOR_BUFFER_BIT);//清除颜色缓冲区
	glColor3f(1.0, 1.0, 1.0);// 内容显示颜色
	for (int i = 0; i < points.size(); i++) {
		if (points[i].polygonStart) {
			if (i != 0) {
				glEnd();
			}
			glBegin(GL_LINE_LOOP);
			//glBegin(GL_LINE_STRIP);
		}
		// 注意y值的改变
		glVertex2f(points[i].x, points[i].y);
	}
	//polyfill();
	glEnd();

	glutSwapBuffers();//清空命令缓冲区并交换帧缓存：
					//交换前后台两个缓冲区指针
}

// -1 when no points
//找到距离最近的点
int findNearPointNum(float x, float y) {
	if (points.size() == 0) {
		return -1;
	}
	int near = 0;
	double min = -1;
	for (int i = 0; i < points.size(); i++) {
		double distence = (points[i].x - x)*(points[i].x - x) + (points[i].y - y)*(points[i].y - y);
		if (min == -1) {
			min = distence;
		}
		else if (min > distence) {
			min = distence;
			near = i;
		}
	}
	return near;
}

//鼠标点击处理函数
void mouse_click(int button, int state, int x, int y) {
	y = 600 - y;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {//鼠标左键按下
		if (mode == 2) {//删除点
			//找到距离鼠标点击位置最近的点
			int near = findNearPointNum(x, y);
			if (near == -1) {//不存在
				return;
			}

			//如果是起始点，则让下一个点成为起始点
			if (points[near].polygonStart) {
				if (points.size() > near + 1) {
					points[near + 1].polygonStart = true;
				}
			}

			//删除点
			points.erase(points.begin() + near, points.begin() + near + 1);
			return;
		}
		if (mode == 3) {//移动点
			dragging = true;
			//找到距离最近的点
			draggedNum = findNearPointNum(x, y);
			return;
		}

		point t{ x,y,false };
		if (mode == 1) {//新建多边形
			//mode=0;
			if (flag == 0) {
				t.polygonStart = true;//起点
				flag = 1;
			}
			cout << x << " " << y << endl;
			points.push_back(t);
		}

		if (mode == 4) {//移动多边形
			old_x = x;
			old_y = y;

		}

		if (mode == 5) {//缩放多边形
			old_x = x;
			old_y = y;
			//cout << old_x << endl << old_y << endl;
		}

		if (mode == 7) {//缩放多边形
			old_x = x;
			old_y = y;
			//cout << old_x << endl << old_y << endl;
		}

		if (mode == 6) {//旋转多边形
			old_x = x;
			old_y = y;
			//cout << old_x << endl << old_y << endl;
		}
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {//鼠标左键松开
		if (dragging) {
			dragging = false;
			if (draggedNum != -1) {
				points[draggedNum].x = x;
				points[draggedNum].y = y;
				return;
			}
		}

		if (mode == 4) {//移动多边形
			int dx = x - old_x, dy = y - old_y;
			cout << dx << endl << dy << endl;
			point t{ 0,0,false };
			for (size_t i = 0; i < points.size(); i++) {
				t.x = points[i].x + dx;
				t.y = points[i].y + dy;
				t.polygonStart = points[i].polygonStart;
				points[i] = t;
			}
			return;
		}

		if (mode == 7) {//缩小多边形
			int max_x = 0, min_x = 1000, max_y = 0, min_y = 600;
			for (size_t i = 0; i < points.size(); i++) {
				if (points[i].x > max_x) {
					max_x = points[i].x;
				}
				if (points[i].x < min_x) {
					min_x = points[i].x;
				}
				if (points[i].y > max_y) {
					max_y = points[i].y;
				}
				if (points[i].y < min_y) {
					min_y = points[i].y;
				}
			}
			float xr = (max_x + min_x) / 2.0, yr = (max_y + min_y) / 2.0;//基准点
			float s = 0.5;

			point t{ 0,0,false };
			for (size_t i = 0; i < points.size(); i++) {
				t.x = xr + (points[i].x - xr)*s;
				t.y = xr + (points[i].y - yr)*s;
				//t.x = xr + points[i].x + d1/2;  
				//t.y = yr + points[i].y + d2/2;    
				t.polygonStart = points[i].polygonStart;
				points[i] = t;
				//cout << "x=" << t.x << " y=" << t.y << endl;
			}

			return;
		}

		if (mode == 5) {//放大多边形
			int max_x = 0, min_x = 1000, max_y = 0, min_y = 600;
			for (size_t i = 0; i < points.size(); i++) {
				if (points[i].x > max_x) {
					max_x = points[i].x;
				}
				if (points[i].x < min_x) {
					min_x = points[i].x;
				}
				if (points[i].y > max_y) {
					max_y = points[i].y;
				}
				if (points[i].y < min_y) {
					min_y = points[i].y;
				}
			}
			float xr = (max_x + min_x) / 2.0, yr = (max_y + min_y) / 2.0;//基准点
			float s = 2.0;

			point t{ 0,0,false };
			for (size_t i = 0; i < points.size(); i++) {
				t.x = xr + (points[i].x - xr)*s;
				t.y = xr + (points[i].y - yr)*s;
				//t.x = xr + points[i].x + d1/2;  
				//t.y = yr + points[i].y + d2/2;    
				t.polygonStart = points[i].polygonStart;
				points[i] = t;
				//cout << "x=" << t.x << " y=" << t.y << endl;
			}

			return;
		}

		if (mode == 6) {//旋转多边形
			int sumx = 0, sumy = 0;
			for (size_t i = 0; i < points.size(); i++) {
				sumx += points[i].x;
				sumy += points[i].y;
			}
			int xr = sumx / points.size(), yr = sumy / points.size();//旋转的基准点*/
			//int xr = 0, yr = 0;

			double a1 = atan((double)(old_y - yr) / (double)(old_x - xr));
			double a2 = atan((double)(y - yr) / (double)(x - xr));
			double theta = a2 - a1;//旋转角度
			/*cout << "a1=" << a1 << endl;
			cout << "a2=" << a2 << endl;
			cout << "theta=" << theta << endl;*/

			point t{ 0,0,false };
			for (size_t i = 0; i < points.size(); i++) {
				/*double tmp = theta;
				 while(tmp - PI/2 > 1e-6) {
					 t.x = (int)(xr - (points[i].y - yr));
					 t.y = (int)(yr + (points[i].x - xr));
					 points[i] = t;
					 tmp -= PI / 2;
				}*/
				t.x = (int)(xr + (points[i].x - xr)*cos(theta) - (points[i].y - yr)*sin(theta));
				t.y = (int)(yr + (points[i].x - xr)*sin(theta) + (points[i].y - yr)*cos(theta));
				t.polygonStart = points[i].polygonStart;
				points[i] = t;
			}

			return;
		}


	}
}

//键盘按键处理函数
void key_action(unsigned char key, int x, int y) {
	if (key == '1') {//新建多边形
		mode = 1;
		flag = 0;
	}
	if (key == '2') {//删除点
		mode = 2;
	}
	if (key == '3') {//移动点
		mode = 3;
	}
	if (key == '4') {//移动图形
		mode = 4;
	}
	if (key == '5') {//放大
		mode = 5;
	}
	if (key == '7') {//缩小
		mode = 7;
	}
	if (key == '6') {//旋转
		mode = 6;
	}
	if (key == 'r') {
		mode = 0;
		points.clear();
	}
	if (key == 'q') {
		exit(0);
	}

}

int main1(int argc, char **argv) {
	//初始化窗口
	glutInit(&argc, argv);//初始化GLUT库,处理命令行参数
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);//初始化窗口的显示模式：使用RGB颜色 | 双缓冲
	glutInitWindowPosition(100, 100);//设置窗口的位置
	glutInitWindowSize(1000, 600);
	glutCreateWindow("Polygon");//窗口名称

	//glClearColor(0.0, 0.0, 0.0, 0.0);//设置背景色为黑色
	glClear(GL_COLOR_BUFFER_BIT);//清除颜色缓冲区
	//glColor3f(1.0, 1.0, 1.0);//设置像素颜色为白色

	glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();//加载单位矩阵

	gluOrtho2D(0, 1000, 0, 600);//正交投影:
	/*指的是视图范围，如果(0,200,0,200)的话就只能在第一象限显示，
	就1/4圆，第1，3的参数是左下角坐标，第2，4的参数是右上角坐标
	*/
	//glViewport(0, 0, 500, 500);//设置视口大小

	glutDisplayFunc(myDisplay);//设置当前窗口的显示回调函数
	glutIdleFunc(myDisplay);//设置在程序空闲的时候就会被调用的函数
	glutMouseFunc(mouse_click);//处理鼠标点击事件的函数
	glutKeyboardFunc(key_action);//处理普通按键消息的函数
	/*第一个表示按下的键的ASCII码，其余两个提供了，当键按下时当前的鼠标位置。
	鼠标位置相对于当前客户窗口的左上角而言*/

	glutMainLoop();//启动主GLUT事件处理循环

	return 0;
}


#include<windows.h>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

int menu = 0, submenu1 = 0, submenu2;
//画一个绿色多边形
void draw() {
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0, 1, 0);
	glOrtho(-1, 1, -1, 1, -1, 1);
	glBegin(GL_POLYGON);
	glVertex2f(-0.5, -0.5);
	glVertex2f(-0.5, 0.5);
	glVertex2f(0.5, 0.5);
	glVertex2f(0.5, -0.5);
	glEnd();
	glutSwapBuffers();
}
//点击某个条目时，输出该条目属于哪个菜单
void getcurrentmenu() {
	int nmenu;
	nmenu = glutGetMenu();
	if (nmenu == menu)
		printf("current is main menu.\n");
	else if (nmenu == submenu1)
		printf("current is sub menu 1.\n");
	else if (nmenu == submenu2)
		printf("current is sub menu 2.\n");
}

//第一个子菜单
void submenufunc1(int data) {
	getcurrentmenu();
	switch (data) {
	case 1:
		printf("sub menu 1's item 1 is triggered.\n");
		break;
	case 2:
		printf("sub menu 1's item 2 is triggered.\n");
		break;
	case 3:
		printf("sub menu 1's item 3 is triggered.\n");
		break;
	}
}
//第二个子菜单
void submenufunc2(int data) {
	getcurrentmenu();
	switch (data) {
	case 1:
		printf("sub menu 2's item 1 is triggered.\n");
		break;
	case 2:
		printf("sub menu 2's item 2 is triggered.\n");
		break;
	case 3:
		printf("sub menu 2's item 3 is triggered.\n");
		break;
	}
}
//主菜单
void menufunc(int data) {
	getcurrentmenu();
	switch (data) {
	case 1:
		printf("menu's item 1 is triggered.\n");
		break;
	case 2:
		printf("menu's item 2 is triggered.\n");
		break;
	case 3:
		printf("menu's item 3 is triggered.\n");
		break;
	}
}

int main3(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(512, 512);
	glutCreateWindow("test");
	glutDisplayFunc(draw);

	//构建子菜单1的内容
	submenu1 = glutCreateMenu(submenufunc1);
	glutAddMenuEntry("1's item 1", 1);
	glutAddMenuEntry("1's item 2", 2);
	glutAddMenuEntry("1's item 3", 3);
	glutAttachMenu(GLUT_LEFT_BUTTON);

	//构建子菜单2的内容
	submenu2 = glutCreateMenu(submenufunc2);
	glutAddMenuEntry("2's item 1", 1);
	glutAddMenuEntry("2's item 2", 2);
	glutAddMenuEntry("2's item 3", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	//构建主菜单的内容
	menu = glutCreateMenu(menufunc);
	glutAddMenuEntry("item 1", 1);
	glutAddMenuEntry("item 2", 2);
	glutAddMenuEntry("item 3", 3);

	//将两个菜单变为另一个菜单的子菜单
	glutAddSubMenu("sub1", submenu1);
	glutAddSubMenu("sub2", submenu2);
	//点击鼠标右键时显示菜单
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();
	return 0;
}

#define PI 3.14159265358
//q,w,e,r分别为平移，绘制多边形，旋转，缩放
#define TRANSLATE 0
#define DRAWPOLYGON 1
#define ROTATE 2
#define SCALE 3
int tran_x, tran_y;
int _xtmp, _ytmp;//作为缩放变量用
int mode = DRAWPOLYGON;//默认为绘制模式
using namespace std;
const int winwidth = 800;
const int winheight = 640;
struct position {
	double x;
	double y;
};
typedef GLfloat Matrix3x3[3][3];
Matrix3x3 matComposite;//复合矩阵
vector<position> xy;
position tmp;
void DrawPolygon();
void dragmouse(int x, int y);
void mymouse(int button, int state, int x, int y);
void myKeyboard(unsigned char key, int x, int y);
void myKeyboardUp(unsigned char key, int x, int y);
//设置为单位矩阵
void matrix3x3SetIdentity(Matrix3x3 mat) {
	GLint row, col;
	for (row = 0; row < 3; row++) {
		for (col = 0; col < 3; col++)
			mat[row][col] = (row == col);
	}
}
void init() {
	glClearColor(1.0, 1.0, 1.0, 1.0);//设置绘制窗口颜色为白色
	glClear(GL_COLOR_BUFFER_BIT);//清除窗口显示内容
	/*设置为投影类型模式和其他观察参数*/
	glPointSize(3.0f);
	glColor3f(1.0, 0.0, 0.0);//设置颜色为红
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, winwidth, winheight, 0);
	matrix3x3SetIdentity(matComposite);
}

//矩阵相乘
void matrixpreMultiply(Matrix3x3 m1, Matrix3x3 m2) {
	GLint row, col;
	Matrix3x3 tmp;
	for (row = 0; row < 3; row++) {
		for (col = 0; col < 3; col++) {
			tmp[row][col] = m1[row][0] * m2[0][col] + m1[row][1] *
				m2[1][col] + m1[row][2] * m2[2][col];
		}
	}
	for (row = 0; row < 3; row++) {
		for (col = 0; col < 3; col++) {
			m2[row][col] = tmp[row][col];
		}
	}
}
//平移
void translate2D(GLfloat tx, GLfloat ty) {
	Matrix3x3 matTransl;
	matrix3x3SetIdentity(matTransl);//设置为单位矩阵
	matTransl[0][2] = tx;
	matTransl[1][2] = ty;
	matrixpreMultiply(matTransl, matComposite);

}
//旋转
void rotate2D(int x, int y, float theta) {
	theta = theta / 180 * PI;
	Matrix3x3 matRot;
	matrix3x3SetIdentity(matRot);
	matRot[0][0] = cos(theta);
	matRot[0][1] = -sin(theta);
	matRot[0][2] = x * (1 - cos(theta)) + y * sin(theta);
	matRot[1][0] = sin(theta);
	matRot[1][1] = cos(theta);
	matRot[1][2] = y * (1 - cos(theta)) - x * sin(theta);
	matrixpreMultiply(matRot, matComposite);
}

//缩放
void scale2D(GLfloat sx, GLfloat sy, int x, int y) {
	Matrix3x3 matScale;
	matrix3x3SetIdentity(matScale);
	matScale[0][0] = sx;
	matScale[0][2] = (1 - sx)*x;
	matScale[1][1] = sy;
	matScale[1][2] = (1 - sy)*y;
	matrixpreMultiply(matScale, matComposite);
}
//复合矩阵
void transformVerts2D() {
	GLfloat tmp;
	for (int i = 0; i < xy.size(); i++) {
		tmp = matComposite[0][0] * xy[i].x + matComposite[0][1] * xy[i].y + matComposite[0][2];
		xy[i].y = matComposite[1][0] * xy[i].x + matComposite[1][1] * xy[i].y + matComposite[1][2];
		xy[i].x = tmp;
	}
	DrawPolygon();
	matrix3x3SetIdentity(matComposite);
}


//绘制多边形
void DrawPolygon() {
	//  glEnable(GL_POLYGON_STIPPLE);
	glPolygonMode(GL_BACK, GL_LINE);//设置反面为线性模式
	glPolygonMode(GL_FRONT, GL_LINE);//设置正面为线性模式
	glClear(GL_COLOR_BUFFER_BIT);//清除窗口显示内容
	glBegin(GL_POLYGON);
	for (unsigned int i = 0; i < xy.size(); i++) {
		glVertex2f(xy[i].x, xy[i].y);
	}
	glEnd();
	glFlush();
}

//鼠标拖动
void dragmouse(int x, int y) {
	float ssx = 1, ssy = 1;
	switch (mode)
	{
		//q,w,e,r
	case TRANSLATE:
		translate2D(x - tran_x, y - tran_y);
		transformVerts2D();
		tran_x = x;
		tran_y = y;
		break;
	case DRAWPOLYGON:

		break;
	case ROTATE:
		if (x <= _xtmp && y >= _ytmp)
			rotate2D(tran_x, tran_y, -8);
		else
			rotate2D(tran_x, tran_y, 8);
		transformVerts2D();
		_xtmp = x;
		_ytmp = y;
		break;
	case SCALE:


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

		/*等比例缩放
		if(x<=_xtmp&&y>=_ytmp){
			ssx-=0.01f;
			ssy-=0.01f;
		}else{
			ssx+=0.01f;
			ssy+=0.01f;
		}*/





		scale2D(ssx, ssy, tran_x, tran_y);
		transformVerts2D();
		_xtmp = x;
		_ytmp = y;
		break;
	default:
		break;
	}

}

//鼠标监听
void mymouse(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		switch (mode)
		{
			//q,w,e,r
		case TRANSLATE:
			tran_x = x;
			tran_y = y;

			break;
		case DRAWPOLYGON:
			tmp.x = x;
			tmp.y = y;
			xy.push_back(tmp);
			DrawPolygon();
			break;
		case ROTATE:
			tran_x = x;
			tran_y = y;
			_xtmp = x;
			_ytmp = y;
			break;
		case SCALE:
			tran_x = x;
			tran_y = y;
			break;
		default:
			break;
		}

	}



}
//键盘监听
void myKeyboard(unsigned char key, int x, int y) {
	//清空删除
	if (key == 'a') {
		glClear(GL_COLOR_BUFFER_BIT);//清除窗口显示内容
		glFlush();
		xy.clear();
	}


}

void myKeyboardUp(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'q':mode = TRANSLATE;
		break;
	case 'w':mode = DRAWPOLYGON;
		break;
	case 'e':mode = ROTATE;
		break;
	case 'r':mode = SCALE;
		break;
	default:
		break;
	}
}
void myDisplay() {
	glFlush();
}
void mymenu(int id) {
	if (id == 0)
		mode = 0;
	else if (id == 1)
		mode = 1;
	else if (id == 2)
		mode = 2;
	else if (id == 3)
		mode = 3;
}

int main4(int argc, char**argv) {
	glutInit(&argc, argv);//初始化
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);//设置绘制模式
	glutInitWindowPosition(500, 300);
	glutInitWindowSize(winwidth, winheight);
	glutCreateWindow("二维图形的变换");//创建窗口

	int id = glutCreateMenu(mymenu);

	glutAddMenuEntry("平移", 0);
	glutAddMenuEntry("绘制多边形", 1);
	glutAddMenuEntry("旋转", 2);
	glutAddMenuEntry("缩放", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	init();
	glutDisplayFunc(myDisplay);
	glutMouseFunc(mymouse);//鼠标监听回调函数
	glutMotionFunc(dragmouse);//鼠标拖动

	glutKeyboardFunc(myKeyboard);//键盘监听
	glutKeyboardUpFunc(myKeyboardUp);//键盘弹起状态
	glutMainLoop();
}

//填充算法

vector<point> vertice; //顶点
struct point
{
	float x, y;
	point() {}
	point(int xx, int yy) :
		x(xx), y(yy) {}
};
typedef struct XET
{
	float x;
	float dx;  // 从当前扫描线到下一条扫描线间x的增量，即斜率的倒数
	float ymax; //该边所交的最高扫描线的坐标值ymax
	XET* next;
}AET, NET; //AET 活性边表； NET新边表


//画点函数
void draw_a_point(int x, int y)
{
	glBegin(GL_POINTS);
	glColor3f(0, 1, 1);
	glVertex2f(x, y);
	glEnd();
	glFlush();
}

void PolyScan()
{
	/*得到最高点的y坐标*/
	int Max_Y = 0;
	for (int i = 0; i < vertice.size(); i++)
		/*Max_Y = max(Max_Y, vertice[i].y);*/
		if (vertice[i].y > Max_Y)
			Max_Y = vertice[i].y;


	//初始化AET表
	AET* pAET = new AET;
	pAET->next = NULL;

	//初始化NET表
	NET* pNET[800]; //吊桶
	for (int i = 0; i <= Max_Y; i++)
	{
		pNET[i] = new NET;
		pNET[i]->next = NULL;;
	}

	//扫描并且建立NET表
	int len = vertice.size(); //顶点个数
	for (int i = 0; i <= Max_Y; i++)
	{
		for (int j = 0; j < len; j++) //扫描每个点
		{
			if (i == vertice[j].y)
			{
				//如果一个点和前一个点有一条边相连，则该点和后面一个点也相连
				//！这个式子 便于最后一个顶点和第一个点相连 和 防止出现负数

				//判断当前点的高低，使ymax、DX、DY的计算有变化
				if (vertice[(j - 1 + len) % len].y > vertice[j].y)
				{
					//前一个点在当前点的上方
					NET* p = new NET;
					p->x = vertice[j].x;
					p->ymax = vertice[(j - 1 + len) % len].y;//与当前扫描线相交的活性边 的 最高点即为相邻顶点的y
					float DX = vertice[(j - 1 + len) % len].x - vertice[j].x;
					float DY = vertice[(j - 1 + len) % len].y - vertice[j].y;
					p->dx = DX / DY;//dx为直线斜率的倒数
					p->next = pNET[i]->next;
					pNET[i]->next = p;
				}
				if (vertice[(j + 1) % len].y > vertice[j].y)
				{
					//后一个点在当前点的上方
					NET* p = new NET;
					p->x = vertice[j].x;
					p->ymax = vertice[(j + 1) % len].y;
					float DX = vertice[(j + 1) % len].x - vertice[j].x;
					float DY = vertice[(j + 1) % len].y - vertice[j].y;
					p->dx = DX / DY;//dx为直线斜率的倒数
					p->next = pNET[i]->next;
					pNET[i]->next = p;
				}
			}
		}
	}

	//建立并且更新活性边表AET
	//各条扫描线i
	for (int i = 0; i <= Max_Y; i++)
	{
		/*把新边表NET[i] 中的边结点用插入排序法插入AET表，使之按x坐标递增顺序排列*/

		//计算每条扫描线上不同线产生的新的交点x，更新AET
		NET* p = pAET->next;
		while (p)
		{
			p->x = p->x + p->dx; //更新x坐标
			p = p->next;
		}

		//断表排序,不再开辟空间 
		AET* tq = pAET;
		p = pAET->next;
		tq->next = NULL;
		while (p)//顺着链表往下走
		{
			//找到第一个比它大的数字tq->next->next->x，则从p->next到tq->next都是比p->x小的
			while (tq->next != NULL && tq->next->x <= p->x)
				tq = tq->next;
			//插入p到tq和tq->next之间
			NET* t = p->next;
			p->next = tq->next;
			tq->next = p;
			p = t;

			tq = pAET;//回到头
		}

		/*(改进算法) 取消求交，减少计算量*/
		//先从AET表中删除ymax==i的结点****************************************/
		//像素的取舍问题，保证多边形的“下闭上开”，避免填充扩大化（交点的个数应保证为偶数个）
		AET* q = pAET;
		p = q->next;
		while (p)
		{
			if (p->ymax == i)
			{
				q->next = p->next;
				delete p;
				p = q->next;
			}
			else
			{
				q = q->next;
				p = q->next;
			}
		}

		//若NET中有新点，将其用插入法插入AET，按x递增的顺序排列
		p = pNET[i]->next;
		q = pAET;
		while (p)
		{
			while (q->next && p->x >= q->next->x)
				q = q->next;
			//插入p
			NET* t = p->next;
			p->next = q->next;
			q->next = p;
			p = t;

			q = pAET;//回到头
		}

		//配对后填充颜色
		p = pAET->next;
		while (p && p->next != NULL)
		{
			for (float j = p->x; j <= p->next->x; j++)
			{
				//扫描线画点
				draw_a_point(j, i);
				//cout << "(" << j << ", " << i << ")" << endl;
			}
			p = p->next->next;//考虑端点情况
		}
	}
	glFlush();
}


//裁剪算法
struct EDGE//Edge边
{
	float bx, by, ex, ey;
	EDGE() {}
	EDGE(float bxx, float byy, float exx, float eyy)
		:bx(bxx), by(byy), ex(exx), ey(eyy) {}
};
struct Window {
public:
	int x, y;
	int width;
	int height;
	int l, r, t, b; //边界：左右上下
	Window(int x, int y, int width, int height) {
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
		l = x;
		r = x + width;
		t = y + height;
		b = y;
	}
};
Window myWindow(200, 200, 400, 250);

//map<point, int> vis;
vector<point> input_vertice; //输入
vector<point> output_vertice; //输出
float intersect_point_color[3] = { 1,0,0 };
float polygon_point_color[3] = { 0,0,1 };
EDGE left(200, 450, 200, 200);
EDGE bottom(200, 200, 600, 200);
EDGE right(600, 200, 600, 450);
EDGE top(600, 450, 200, 450);

void draw_a_point(float x, float y, float color[]);
void draw_window();
bool inside(point& pt, EDGE ClipBoundary);//判断点是否可见
void intersect(point& s, point& p, EDGE ClipBoundary, point& intersect_pt);//直线段SP和边界求交，返回交点
void SutherlandHodgmanClip(EDGE ClipBoundary, int in_len, int& out_len);
void mymouse(int button, int state, int x, int y);
void keyboard(unsigned char key, int x, int y);
void display();

bool inside(point& pt, EDGE ClipBoundary)//判断点是否可见
{
	if (ClipBoundary.ex > ClipBoundary.bx)
	{
		if (pt.y >= ClipBoundary.by)//裁剪边为窗口下边沿
			return true;
	}
	else if (ClipBoundary.ex < ClipBoundary.bx)
	{
		if (pt.y <= ClipBoundary.by)//裁剪边为窗口上边沿
			return true;
	}
	else if (ClipBoundary.ey > ClipBoundary.by)//裁剪边为窗口右边沿
	{
		if (pt.x <= ClipBoundary.bx)
			return true;
	}
	else if (ClipBoundary.ey < ClipBoundary.by)//裁剪边为窗口左边沿
	{
		if (pt.x >= ClipBoundary.bx)
			return true;
	}
	return false;
}

//直线段SP和边界求交，返回交点
void intersect(point& s, point& p, EDGE ClipBoundary, point& intersect_pt)
{
	if (ClipBoundary.by == ClipBoundary.ey)//水平裁剪边界
	{
		intersect_pt.y = ClipBoundary.by;
		//x=起点的横坐标+dy/sp斜率
		intersect_pt.x = s.x + (ClipBoundary.by - s.y) * (p.x - s.x) / (p.y - s.y);
	}
	else//垂直裁剪边界
	{
		intersect_pt.x = ClipBoundary.bx;
		intersect_pt.y = s.y + (ClipBoundary.bx - s.x) * (p.y - s.y) / (p.x - s.x);
	}
}

void SutherlandHodgmanClip(EDGE ClipBoundary)
{
	point s, p, ip;
	output_vertice.clear();
	s = input_vertice[input_vertice.size() - 1];//先从最后一个点指向第一个点的线段开始检验

	for (int j = 0; j < input_vertice.size(); j++)
	{
		p = input_vertice[j];
		if (inside(p, ClipBoundary))//p在内
		{
			if (inside(s, ClipBoundary))//sp都在窗口内
			{
				output_vertice.push_back(p);
			}
			else//p在里面 s不在
			{
				intersect(s, p, ClipBoundary, ip);
				output_vertice.push_back(ip);
				output_vertice.push_back(p);
			}
		}
		else//p在外面
		{
			if (inside(s, ClipBoundary))//s在窗口内p在窗口外
			{
				intersect(s, p, ClipBoundary, ip);
				output_vertice.push_back(ip);
			}
			//sp都在外面则无输出
		}
		s = p;
	}
	input_vertice = output_vertice;//这次的输出作为下一次的输入，input_vertice和output_vertice是全局变量
}

//画点函数
void draw_a_point(float x, float y, float color[])
{
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	glColor3fv(color);
	glVertex2f(x, y);
	glEnd();
	glFlush();
}

//画裁剪窗口
void draw_window() {
	glBegin(GL_LINE_LOOP);
	glVertex2i(myWindow.l, myWindow.b);
	glVertex2i(myWindow.l, myWindow.t);
	glVertex2i(myWindow.r, myWindow.t);
	glVertex2i(myWindow.r, myWindow.y);
	glEnd();
}

void mymouse(int button, int state, int x, int y)
{
	glClearColor(1, 1, 1, 1);
	//bool flag = 0;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		draw_a_point(x, window_height - y, polygon_point_color);
		point p(x, window_height - y);
		input_vertice.push_back(p);
		cout << "多边形顶点" << input_vertice.size() << ":(" << x << ", " << window_height - y << ")" << endl;
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		glLineWidth(2.0f);
		glBegin(GL_LINES);
		glColor3fv(polygon_point_color);
		//绘制多边形
		for (int i = 0; i < input_vertice.size(); i++)
		{
			if (i == input_vertice.size() - 1)
			{
				glVertex2f(input_vertice[0].x, input_vertice[0].y);
				glVertex2f(input_vertice[i].x, input_vertice[i].y);
			}
			else
			{
				glVertex2f(input_vertice[i].x, input_vertice[i].y);
				glVertex2f(input_vertice[i + 1].x, input_vertice[i + 1].y);
			}
		}
		glEnd();
		glFlush();
	}
}

void keyboard(unsigned char key, int x, int y)
{
	if (key == 32)
	{
		//顺时针
		SutherlandHodgmanClip(::left);
		SutherlandHodgmanClip(bottom);
		SutherlandHodgmanClip(::right);
		SutherlandHodgmanClip(top);

		glLineWidth(4.0f);
		glBegin(GL_LINE_LOOP);
		glColor3fv(intersect_point_color);
		for (int i = 0; i < output_vertice.size(); i++)
		{//draw_a_point(output_vertice[i].x, output_vertice[i].y, intersect_point_color);
			glVertex2f(output_vertice[i].x, output_vertice[i].y);
		}
		glEnd();
		glFlush();

	}
}

//多边形裁剪算法+长方形裁剪框
//-----------------------------------------
//Liang-Barsky 线段裁剪算法

class wcPt2D {

public:
	GLfloat x, y;
public:
	/*
Default Constructor:initalize position 48(0.0,0.0).*/
	wcPt2D() {
		x = y = 0.0;
	}

	wcPt2D(GLfloat nx, GLfloat ny) : x(nx), y(ny) {}

	wcPt2D(const wcPt2D& pCopy) {
		this->x = pCopy.x;
		this->y = pCopy.y;
	}

	void setCoords(GLfloat xCoord, GLfloat yCoord) {
		x = xCoord;
		y = yCoord;
		return;
	}

	wcPt2D& operator= (wcPt2D p2) {
		this->x = p2.getx();
		this->y = p2.gety();
		return *this;
	}


	GLfloat getx()const {
		return x;
	}
	GLfloat gety() const {
		return y;
	}
};

inline GLint round(const GLfloat a) {
	return GLint(a + 0.5);
}

GLint clipTest(GLfloat p, GLfloat q, GLfloat * u1, GLfloat *u2) {
	GLfloat r;
	GLint returnValue = true;
	if (p < 0.0) {
		r = q / p;
		if (r > *u2) {
			returnValue = false;
		}
		else if (r > *u1) {
			*u1 = r;
		}
	}
	else {
		if (p > 0.0) {
			r = q / p;
			if (r < *u1) {
				returnValue = false;
			}
			else if (r < *u2) {
				*u2 = r;
			}
		}
		else {

			if (q < 0.0) {
				returnValue = false;
			}
		}
	}
	return (returnValue);
}


void lineClipLiangBrask(wcPt2D winMin, wcPt2D winMax, wcPt2D p1, wcPt2D p2) {
	GLfloat u1 = 0.0, u2 = 1.0, dx = p2.getx() - p1.getx(), dy;
	if (clipTest(-dx, p1.getx() - winMin.getx(), &u1, &u2)) {
		if (clipTest(dx, winMax.getx() - p1.getx(), &u1, &u2)) {
			dy = p2.gety() - p1.gety();
			if (clipTest(-dy, p1.gety() - winMin.gety(), &u1, &u2)) {
				if (clipTest(dy, winMax.gety() - p1.gety(), &u1, &u2)) {
					if (u2 < 1.0) {
						p2.setCoords(p1.getx() + u2 * dx, p1.gety() + u2 * dy);
					}
					if (u1 > 0.0) {
						p1.setCoords(p1.getx() + u1 * dx, p1.gety() + u1 * dy);
					}
					lineDDA(round(p1.getx()), round(p1.gety()), round(p2.getx()), round(p2.gety()));
					//					return 2;
				}
			}
		}
	}

	return;
}

int lineClipLiangBrask(wcPt2D winMin, wcPt2D winMax, wcPt2D p1, wcPt2D p2, wcPt2D pOut[]) {
	GLfloat u1 = 0.0, u2 = 1.0, dx = p2.getx() - p1.getx(), dy;
	if (clipTest(-dx, p1.getx() - winMin.getx(), &u1, &u2)) {
		if (clipTest(dx, winMax.getx() - p1.getx(), &u1, &u2)) {
			dy = p2.gety() - p1.gety();
			if (clipTest(-dy, p1.gety() - winMin.gety(), &u1, &u2)) {
				if (clipTest(dy, winMax.gety() - p1.gety(), &u1, &u2)) {
					if (u2 < 1.0) {
						p2.setCoords(p1.getx() + u2 * dx, p1.gety() + u2 * dy);
					}
					if (u1 > 0.0) {
						p1.setCoords(p1.getx() + u1 * dx, p1.gety() + u1 * dy);
					}
					pOut[0] = p1;
					pOut[1] = p2;
					//					lineDDA (round(p1.getx ()), round (p1.gety ()),round(p2.getx ()),round(p2.gety ()));
					return 2;
				}
			}
		}
	}

	return 0;
}


void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	//可视化边界:
	const int minX = 100, minY = 100, maxX = 300, maxY = 300;
	glColor3f(1.0, 0.0, 0.0);
	lineDDA(minX, minY, minX, maxY);
	lineDDA(minX, minY, maxX, minY);
	lineDDA(maxX, maxY, minX, maxY);
	lineDDA(maxX, maxY, maxX, minY);

	glColor3f(0.0, 1.0, 0.0);
	GLint n = 5;
	wcPt2D pIn[n];
	pIn[0].setCoords(0, 200);
	pIn[1].setCoords(150, 250);
	pIn[2].setCoords(250, 250);
	pIn[3].setCoords(400, 200);
	pIn[4].setCoords(200, 50);

	for (int i = 0; i < n; ++i) {
		lineDDA(pIn[i].x, pIn[i].y, pIn[(i + 1) % n].x, pIn[(i + 1) % n].y);
	}


	wcPt2D pOut[20];
	wcPt2D tempPOut[2];
	int outCount = 0;
	int flag = 0;
	for (int i = 0; i < n; ++i) {
		flag = lineClipLiangBrask(wcPt2D(minX, minY), wcPt2D(maxX, maxY), pIn[i], pIn[(i + 1) % n], tempPOut);
		if (flag == 2) {
			for (int j = 0; j < 2; ++j) {
				pOut[outCount++] = tempPOut[j];
			}
		}
	}

	glColor3f(0.0, 0.0, 1.0);
	int i = 1;
	for (; i <= outCount; ++i) {
		lineDDA(pOut[i - 1].x, pOut[i - 1].y, pOut[i%outCount].x, pOut[i%outCount].y);
	}

	glFlush();


	return;
}
