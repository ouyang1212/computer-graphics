#pragma once
#pragma once
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cmath>
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

void setPixel(int x, int y) {
	//用OpenGL自己的函数实现书上的setPixel
	glPointSize(3.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POINTS);
	glVertex2i(x, y);
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
	wcPt2D pIn[20];
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


void test_main() {
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
	glutDisplayFunc(display);//设置当前窗口的显示回调函数
	glutIdleFunc(display);
}