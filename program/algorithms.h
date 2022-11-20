#pragma once
#include <GL/glut.h>
#include <cmath>
#include"graphics.h"

void setPixel(int x, int y) {
	//用OpenGL自己的函数实现书上的setPixel
	glPointSize(3.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
	glFlush();
}

void setPixel(int x, int y, int color) {
	//用OpenGL自己的函数实现书上的setPixel
	glPointSize(3.0f);
	switch (color) {
	case 0:
		glColor3f(1.0, 0.0, 0.0);
		break;
	case 1:
		glColor3f(0.0, 1.0, 0.0);
		break;
	case 2:
		glColor3f(0.0, 0.0, 1.0);
		break;
	default:
		glColor3f(1.0, 1.0, 1.0);
		break;
	}
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
	glFlush();
}

void setPixel_point(int x, int y) {
	//用OpenGL自己的函数实现书上的setPixel
	glColor3f(1.0f, 1.0f, 1.0f);
	glPointSize(10.0f);
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
	glFlush();
}

void setPixel_point_red(int x, int y) {
	//用OpenGL自己的函数实现书上的setPixel
	glPointSize(10.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnd();
	glFlush();
}

void lineDDA(int x0, int y0, int xEnd, int yEnd) {
	glPointSize(1.0f);//设置像素点大小
	int dx = xEnd - x0, dy = yEnd - y0, steps, k;
	float xIncrement, yIncrement, x = x0, y = y0;
	if (abs(dx) > abs(dy))//确定步长，谁大就取谁
		steps = abs(dx);
	else
		steps = abs(dy);
	xIncrement = float(dx) / float(steps);//增量当中有一个会为1，另一个会为斜率k
	yIncrement = float(dy) / float(steps);
	setPixel(round(x), round(y));//由于每次都加了小于1的增量，所以需要取整
	for (k = 0; k < steps; k++) {
		/*
		glBegin(GL_POINTS);
		glVertex2i((int)x,(int)y);
		glEnd();
		glFlush();
		*/
		x += xIncrement;
		y += yIncrement;
		setPixel(round(x), round(y));
	}
}

void lineDDA(int x0, int y0, int xEnd, int yEnd, int color) {
	glPointSize(1.0f);//设置像素点大小
	int dx = xEnd - x0, dy = yEnd - y0, steps, k;
	float xIncrement, yIncrement, x = x0, y = y0;
	if (abs(dx) > abs(dy))//确定步长，谁大就取谁
		steps = abs(dx);
	else
		steps = abs(dy);
	xIncrement = float(dx) / float(steps);//增量当中有一个会为1，另一个会为斜率k
	yIncrement = float(dy) / float(steps);
	setPixel(round(x), round(y), color);//由于每次都加了小于1的增量，所以需要取整
	for (k = 0; k < steps; k++) {
		/*
		glBegin(GL_POINTS);
		glVertex2i((int)x,(int)y);
		glEnd();
		glFlush();
		*/
		x += xIncrement;
		y += yIncrement;
		setPixel(round(x), round(y), color);
	}
}

void symmetricPoints(int xc, int yc, int rx, int ry) {
	setPixel(xc + rx, yc + ry);
	setPixel(xc - rx, yc + ry);
	setPixel(xc + rx, yc - ry);
	setPixel(xc - rx, yc - ry);
}

void eclipseBres(int xc, int yc, int rx, int ry) {
	int x = 0, y = ry;
	int rx2 = rx * rx, ry2 = ry * ry;
	int tworx2 = 2 * rx2, twory2 = 2 * ry2;
	int px = 0, py = tworx2 * y;
	int p = round(ry2 - rx2 * ry + 0.25*rx2);
	symmetricPoints(xc, yc, x, y);
	while (py > px) {
		x++;
		px += twory2;
		if (p < 0)
			p += ry2 + px;
		else {
			y--;
			py -= tworx2;
			p += ry2 + px - py;
		}
		symmetricPoints(xc, yc, x, y);
	}
	//region2
	p = round(ry2*(x + 0.5)*(x + 0.5) + rx2 * (y - 1)*(y - 1) - rx2 * ry2);
	while (y > 0) {
		y--;
		py -= tworx2;
		if (p > 0)
			p += rx2 - py;
		else {
			x++;
			px += twory2;
			p += rx2 - py + px;
		}
		symmetricPoints(xc, yc, x, y);
	}
}

// 二维变换
#define PI 3.14159265358
typedef GLfloat Matrix3x3[3][3];

//设置为单位矩阵
void matrix3x3SetIdentity(Matrix3x3 mat) {
	GLint row, col;
	for (row = 0; row < 3; row++) {
		for (col = 0; col < 3; col++)
			mat[row][col] = (row == col);
	}
}
Matrix3x3 matComposite;//复合矩阵

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
	cout << matComposite[0][0] << "!!??" << endl;
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
void transformVerts2D(vector<point> &points) {
	GLfloat tmp;
	for (int i = 0; i < points.size(); i++) {
		tmp = matComposite[0][0] * points[i].x + matComposite[0][1] * points[i].y + matComposite[0][2];
		points[i].y = matComposite[1][0] * points[i].x + matComposite[1][1] * points[i].y + matComposite[1][2];
		points[i].x = tmp;
	}
	matrix3x3SetIdentity(matComposite);
}

//填充算法
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

void PolyScan(vector<point> &vertice)
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
//画裁剪窗口
void draw_window() {
	glBegin(GL_LINE_LOOP);
	glVertex2i(myWindow.l, myWindow.b);
	glVertex2i(myWindow.l, myWindow.t);
	glVertex2i(myWindow.r, myWindow.t);
	glVertex2i(myWindow.r, myWindow.y);
	glEnd();
}
EDGE left(200, 450, 200, 200);
EDGE bottom(200, 200, 600, 200);
EDGE right(600, 200, 600, 450);
EDGE top(600, 450, 200, 450);

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

vector<point> SutherlandHodgmanClip(EDGE ClipBoundary, vector<point> input_vertice)
{
	point s, p, ip;
	vector<point> output_vertice;
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
	return output_vertice;//这次的输出作为下一次的输入，input_vertice和output_vertice是全局变量
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

// bezier曲线
double setBezier(int n, int i, float t) {	// n阶贝塞尔曲线绘制的代码
	int sum = 1;
	if (i == n || i == 0) return pow(t, i) * pow(1 - t, n - i);
	int j;
	for (j = n - i + 1; j <= n; j++)
		sum *= j;
	for (j = 1; j <= i; j++)
		sum /= j;
	return sum * pow(t, i) * pow(1 - t, n - i);
}

void bezier(vector<point>  ctrlPts) {
	//输入：nCtrlPts个控制点，坐标保存于ctrlPts数组中 
	//输出：nBezCurvePts个曲线上的点，坐标保存于数组bezCurvePts中

	//1.在此处添加代码
	int nCtrlPts = ctrlPts.size();
	int nBezCurvePts = (nCtrlPts - 1) * 5 + 1;
	int cnt = 0;
	point p_current = ctrlPts[cnt];	// 把当前的控制点设为该段贝塞尔曲线的起点	
	for (float c = 0; c <= 1; c += 1.0 / nBezCurvePts) {
		point p;
		p.x = 0;
		p.y = 0;
		for (int k = 0; k < nCtrlPts; ++k) {	// 计算该段贝塞尔曲线的贝塞尔点
			p.x += ctrlPts[k].x * setBezier(nCtrlPts - 1, k, c);
			p.y += ctrlPts[k].y * setBezier(nCtrlPts - 1, k, c);
		}
		glLineWidth(1.5f);	// 设置线宽
		glBegin(GL_LINE_STRIP);
		glVertex2f(p_current.x, p_current.y);	// 绘制该段贝塞尔曲线的贝塞尔点
		glVertex2f(p.x, p.y);		// 绘制该段贝塞尔曲线的贝塞尔点
		glEnd();
		p_current = p;
	}
	// 绘制最后一段的点
	glLineWidth(1.5f);
	glBegin(GL_LINE_STRIP);
	glVertex2f(p_current.x, p_current.y);
	glVertex2f(ctrlPts[nCtrlPts - 1].x, ctrlPts[nCtrlPts - 1].y);
	glEnd();
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


//枚举类的替代, C++中enum的操作与C有很大不同
const int Left = 0, Right = 1, Bottom = 2, Top = 3;

//typedef enum{
//	Left, Right, Bottom, Top
//} Boundary;

using namespace std;

const GLint nClip = 4;


//判断点p是否在显示框内
GLint inside(wcPt2D p, int b, wcPt2D wMin, wcPt2D wMax) {
	int flag = true;
	switch (b) {
	case Left: {
		if (p.getx() < wMin.getx()) {
			flag = false;
		}
		break;
	}
	case Right: {
		if (p.getx() > wMax.getx()) {
			flag = false;
		}
		break;
	}
	case Bottom: {
		if (p.gety() < wMin.gety()) {
			flag = false;
		}
		break;
	}
	case Top: {
		if (p.gety() > wMax.gety()) {
			flag = false;
		}
		break;
	}
	}
	return flag;
}

//判断向量(p1, p2)是否与边界相交
GLint cross(wcPt2D p1, wcPt2D p2, int winEdge, wcPt2D wMin, wcPt2D wMax) {
	if (inside(p1, winEdge, wMin, wMax) == inside(p2, winEdge, wMin, wMax)) {
		return false;
	}
	else {
		return true;
	}
}

//返回向量(p1, p2)与相应边界的交点
wcPt2D intersect(wcPt2D p1, wcPt2D p2, int winEdge, wcPt2D wMin, wcPt2D wMax) {
	wcPt2D iPt;
	GLfloat m;

	if (p1.x != p2.x) {
		m = (p1.gety() - p2.gety()) / (p1.getx() - p2.getx());
	}
	switch (winEdge) {
	case Left: {
		iPt.x = wMin.x;
		iPt.y = p2.y + (wMin.x - p2.x)*m;
		break;
	}
	case Right: {
		iPt.x = wMax.x;
		iPt.y = p2.y + (wMax.x - p2.x)*m;
		break;
	}
	case Bottom: {
		iPt.y = wMin.y;
		if (p1.x != p2.x) {
			iPt.x = p2.x + (wMin.y - p2.y) / m;
		}
		else {
			iPt.x = p2.x;
		}
		break;
	}
	case Top: {
		iPt.y = wMax.y;
		if (p1.x != p2.x) {
			iPt.x = p2.x + (wMax.y - p2.y) / m;
		}
		else {
			iPt.x = p2.x;
		}
		break;
	}
	}
	return iPt;
}

void clipPoint(wcPt2D &p, int winEdge, wcPt2D wMin, wcPt2D wMax, wcPt2D * pOut,
	int *cnt, wcPt2D first[], wcPt2D *s) {
	wcPt2D iPt;

	//判断first[winEdge]为nullPtr
	if (first[winEdge].x == 0 && first[winEdge].y == 0) {
		first[winEdge] = p;
	}
	else {
		if (cross(p, s[winEdge], winEdge, wMin, wMax)) {
			iPt = intersect(p, s[winEdge], winEdge, wMin, wMax);
			if (winEdge < Top) {
				clipPoint(iPt, winEdge + 1, wMin, wMax, pOut, cnt, first, s);
			}
			else {
				//存交点
				pOut[*cnt] = iPt;
				(*cnt)++;
			}
		}
	}
	s[winEdge] = p;
	if (inside(p, winEdge, wMin, wMax)) {
		if (winEdge < Top) {
			clipPoint(p, winEdge + 1, wMin, wMax, pOut, cnt, first, s);
		}
		else {
			pOut[*cnt] = p;
			(*cnt)++;
		}
	}
}

void closeClip(wcPt2D wMin, wcPt2D wMax, wcPt2D *pOut, GLint *cnt,
	wcPt2D first[], wcPt2D *s) {
	wcPt2D pt;
	int winEdge;
	for (winEdge = Left; winEdge <= Top; winEdge++) {
		if (cross(s[winEdge], first[winEdge], winEdge, wMin, wMax)) {
			pt = intersect(s[winEdge], first[winEdge], winEdge, wMin, wMax);
			if (winEdge < Top) {
				clipPoint(pt, winEdge + 1, wMin, wMax, pOut, cnt, first, s);
			}
			else {
				pOut[*cnt] = pt;
				(*cnt)++;
			}
		}
	}
}

GLint polygonClipSuthHodg(wcPt2D wMin, wcPt2D wMax, GLint n, wcPt2D *pIn, wcPt2D *pOut) {
	wcPt2D first[nClip], s[nClip];
	GLint k, cnt = 0;
	for (k = 0; k < n; k++) {
		clipPoint(pIn[k], Left, wMin, wMax, pOut, &cnt, first, s);
	}
	closeClip(wMin, wMax, pOut, &cnt, first, s);
	return cnt;
}


//---------------------------------------------


//绘制程序
void Sutherland_Hodgman(point minP, point maxP, Polygon1 in) {

	//Sutherland-Hodgman多边形裁剪算法
	glClear(GL_COLOR_BUFFER_BIT);
	//可视化边界:

	int minX = minP.x, minY = minP.y, maxX = maxP.x, maxY = maxP.y;
	if (minX > maxX) {
		swap(minX, maxX);
	}
	if (minY > maxY) {
		swap(minY, maxY);
	}
	//	glColor3f(1.0, 0.0, 0.0);
	lineDDA(minX, minY, minX, maxY, 0);
	lineDDA(minX, minY, maxX, minY, 0);
	lineDDA(maxX, maxY, minX, maxY, 0);
	lineDDA(maxX, maxY, maxX, minY, 0);

	//定义多边形的颜色和顶点
//	glColor3f(0.0, 1.0, 0.0);
	GLint n = in.points.size();
	wcPt2D pIn[20];
	for (int i = 0; i < n; i++) {
		pIn[i].setCoords(in.points[i].x, in.points[i].y);
	}
	//绘制原始多边形

	for (int i = 0; i < n; ++i) {
		lineDDA(pIn[i].x, pIn[i].y, pIn[(i + 1) % n].x, pIn[(i + 1) % n].y, 1);
	}

	//获取裁剪后的点集
	wcPt2D pOut[20];
	int count = polygonClipSuthHodg(wcPt2D(minX, minY), wcPt2D(maxX, maxY), n, pIn, pOut);

	//定义裁剪后的多边形颜色
//	glColor3f(0.0, 0.0, 1.0);
	//绘制裁剪后的多边形
	for (int i = 1; i <= count; ++i) {
		lineDDA(pOut[i - 1].x, pOut[i - 1].y, pOut[i%count].x, pOut[i%count].y, 2);
	}

	glFlush();


	return;
}
