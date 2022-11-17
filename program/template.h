#pragma once
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

struct point {
	float x;
	float y;
	bool polygonStart;// һ������ε���ʼ��
};

vector<point> points;
int mode = 0;
/*	mode
	1:�½�
	2:ɾ����
	3:�ƶ���
	4:�ƶ�ͼ��
	5:����-�Ŵ�
	6:��ת
	7:��С
*/
int flag = 0;//������
bool dragging = false;// ���ڱ�������϶�
int draggedNum = -1;// ���϶��ĵ�ı��
int old_x, old_y;//��¼ԭ��λ��
const double PI = atan(1.)*4.;//Բ����

void myDisplay() {
	glClearColor(0.0, 0.0, 0.0, 0.0);//���ñ���ɫΪ��ɫ
	glClear(GL_COLOR_BUFFER_BIT);//�����ɫ������
	glColor3f(1.0, 1.0, 1.0);// ������ʾ��ɫ
	for (int i = 0; i < points.size(); i++) {
		if (points[i].polygonStart) {
			if (i != 0) {
				glEnd();
			}
			glBegin(GL_LINE_LOOP);
			//glBegin(GL_LINE_STRIP);
		}
		// ע��yֵ�ĸı�
		glVertex2f(points[i].x, points[i].y);
	}
	//polyfill();
	glEnd();

	glutSwapBuffers();//����������������֡���棺
					//����ǰ��̨����������ָ��
}

// -1 when no points
//�ҵ���������ĵ�
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

//�����������
void mouse_click(int button, int state, int x, int y) {
	y = 600 - y;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {//����������
		if (mode == 2) {//ɾ����
			//�ҵ����������λ������ĵ�
			int near = findNearPointNum(x, y);
			if (near == -1) {//������
				return;
			}

			//�������ʼ�㣬������һ�����Ϊ��ʼ��
			if (points[near].polygonStart) {
				if (points.size() > near + 1) {
					points[near + 1].polygonStart = true;
				}
			}

			//ɾ����
			points.erase(points.begin() + near, points.begin() + near + 1);
			return;
		}
		if (mode == 3) {//�ƶ���
			dragging = true;
			//�ҵ���������ĵ�
			draggedNum = findNearPointNum(x, y);
			return;
		}

		point t{ x,y,false };
		if (mode == 1) {//�½������
			//mode=0;
			if (flag == 0) {
				t.polygonStart = true;//���
				flag = 1;
			}
			cout << x << " " << y << endl;
			points.push_back(t);
		}

		if (mode == 4) {//�ƶ������
			old_x = x;
			old_y = y;

		}

		if (mode == 5) {//���Ŷ����
			old_x = x;
			old_y = y;
			//cout << old_x << endl << old_y << endl;
		}

		if (mode == 7) {//���Ŷ����
			old_x = x;
			old_y = y;
			//cout << old_x << endl << old_y << endl;
		}

		if (mode == 6) {//��ת�����
			old_x = x;
			old_y = y;
			//cout << old_x << endl << old_y << endl;
		}
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {//�������ɿ�
		if (dragging) {
			dragging = false;
			if (draggedNum != -1) {
				points[draggedNum].x = x;
				points[draggedNum].y = y;
				return;
			}
		}

		if (mode == 4) {//�ƶ������
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

		if (mode == 7) {//��С�����
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
			float xr = (max_x + min_x) / 2.0, yr = (max_y + min_y) / 2.0;//��׼��
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

		if (mode == 5) {//�Ŵ�����
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
			float xr = (max_x + min_x) / 2.0, yr = (max_y + min_y) / 2.0;//��׼��
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

		if (mode == 6) {//��ת�����
			int sumx = 0, sumy = 0;
			for (size_t i = 0; i < points.size(); i++) {
				sumx += points[i].x;
				sumy += points[i].y;
			}
			int xr = sumx / points.size(), yr = sumy / points.size();//��ת�Ļ�׼��*/
			//int xr = 0, yr = 0;

			double a1 = atan((double)(old_y - yr) / (double)(old_x - xr));
			double a2 = atan((double)(y - yr) / (double)(x - xr));
			double theta = a2 - a1;//��ת�Ƕ�
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

//���̰���������
void key_action(unsigned char key, int x, int y) {
	if (key == '1') {//�½������
		mode = 1;
		flag = 0;
	}
	if (key == '2') {//ɾ����
		mode = 2;
	}
	if (key == '3') {//�ƶ���
		mode = 3;
	}
	if (key == '4') {//�ƶ�ͼ��
		mode = 4;
	}
	if (key == '5') {//�Ŵ�
		mode = 5;
	}
	if (key == '7') {//��С
		mode = 7;
	}
	if (key == '6') {//��ת
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
	//��ʼ������
	glutInit(&argc, argv);//��ʼ��GLUT��,���������в���
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);//��ʼ�����ڵ���ʾģʽ��ʹ��RGB��ɫ | ˫����
	glutInitWindowPosition(100, 100);//���ô��ڵ�λ��
	glutInitWindowSize(1000, 600);
	glutCreateWindow("Polygon");//��������

	//glClearColor(0.0, 0.0, 0.0, 0.0);//���ñ���ɫΪ��ɫ
	glClear(GL_COLOR_BUFFER_BIT);//�����ɫ������
	//glColor3f(1.0, 1.0, 1.0);//����������ɫΪ��ɫ

	glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();//���ص�λ����

	gluOrtho2D(0, 1000, 0, 600);//����ͶӰ:
	/*ָ������ͼ��Χ�����(0,200,0,200)�Ļ���ֻ���ڵ�һ������ʾ��
	��1/4Բ����1��3�Ĳ��������½����꣬��2��4�Ĳ��������Ͻ�����
	*/
	//glViewport(0, 0, 500, 500);//�����ӿڴ�С

	glutDisplayFunc(myDisplay);//���õ�ǰ���ڵ���ʾ�ص�����
	glutIdleFunc(myDisplay);//�����ڳ�����е�ʱ��ͻᱻ���õĺ���
	glutMouseFunc(mouse_click);//����������¼��ĺ���
	glutKeyboardFunc(key_action);//������ͨ������Ϣ�ĺ���
	/*��һ����ʾ���µļ���ASCII�룬���������ṩ�ˣ���������ʱ��ǰ�����λ�á�
	���λ������ڵ�ǰ�ͻ����ڵ����ϽǶ���*/

	glutMainLoop();//������GLUT�¼�����ѭ��

	return 0;
}


#include<windows.h>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

int menu = 0, submenu1 = 0, submenu2;
//��һ����ɫ�����
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
//���ĳ����Ŀʱ���������Ŀ�����ĸ��˵�
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

//��һ���Ӳ˵�
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
//�ڶ����Ӳ˵�
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
//���˵�
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

	//�����Ӳ˵�1������
	submenu1 = glutCreateMenu(submenufunc1);
	glutAddMenuEntry("1's item 1", 1);
	glutAddMenuEntry("1's item 2", 2);
	glutAddMenuEntry("1's item 3", 3);
	glutAttachMenu(GLUT_LEFT_BUTTON);

	//�����Ӳ˵�2������
	submenu2 = glutCreateMenu(submenufunc2);
	glutAddMenuEntry("2's item 1", 1);
	glutAddMenuEntry("2's item 2", 2);
	glutAddMenuEntry("2's item 3", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	//�������˵�������
	menu = glutCreateMenu(menufunc);
	glutAddMenuEntry("item 1", 1);
	glutAddMenuEntry("item 2", 2);
	glutAddMenuEntry("item 3", 3);

	//�������˵���Ϊ��һ���˵����Ӳ˵�
	glutAddSubMenu("sub1", submenu1);
	glutAddSubMenu("sub2", submenu2);
	//�������Ҽ�ʱ��ʾ�˵�
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();
	return 0;
}

#define PI 3.14159265358
//q,w,e,r�ֱ�Ϊƽ�ƣ����ƶ���Σ���ת������
#define TRANSLATE 0
#define DRAWPOLYGON 1
#define ROTATE 2
#define SCALE 3
int tran_x, tran_y;
int _xtmp, _ytmp;//��Ϊ���ű�����
int mode = DRAWPOLYGON;//Ĭ��Ϊ����ģʽ
using namespace std;
const int winwidth = 800;
const int winheight = 640;
struct position {
	double x;
	double y;
};
typedef GLfloat Matrix3x3[3][3];
Matrix3x3 matComposite;//���Ͼ���
vector<position> xy;
position tmp;
void DrawPolygon();
void dragmouse(int x, int y);
void mymouse(int button, int state, int x, int y);
void myKeyboard(unsigned char key, int x, int y);
void myKeyboardUp(unsigned char key, int x, int y);
//����Ϊ��λ����
void matrix3x3SetIdentity(Matrix3x3 mat) {
	GLint row, col;
	for (row = 0; row < 3; row++) {
		for (col = 0; col < 3; col++)
			mat[row][col] = (row == col);
	}
}
void init() {
	glClearColor(1.0, 1.0, 1.0, 1.0);//���û��ƴ�����ɫΪ��ɫ
	glClear(GL_COLOR_BUFFER_BIT);//���������ʾ����
	/*����ΪͶӰ����ģʽ�������۲����*/
	glPointSize(3.0f);
	glColor3f(1.0, 0.0, 0.0);//������ɫΪ��
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, winwidth, winheight, 0);
	matrix3x3SetIdentity(matComposite);
}

//�������
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
//ƽ��
void translate2D(GLfloat tx, GLfloat ty) {
	Matrix3x3 matTransl;
	matrix3x3SetIdentity(matTransl);//����Ϊ��λ����
	matTransl[0][2] = tx;
	matTransl[1][2] = ty;
	matrixpreMultiply(matTransl, matComposite);

}
//��ת
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

//����
void scale2D(GLfloat sx, GLfloat sy, int x, int y) {
	Matrix3x3 matScale;
	matrix3x3SetIdentity(matScale);
	matScale[0][0] = sx;
	matScale[0][2] = (1 - sx)*x;
	matScale[1][1] = sy;
	matScale[1][2] = (1 - sy)*y;
	matrixpreMultiply(matScale, matComposite);
}
//���Ͼ���
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


//���ƶ����
void DrawPolygon() {
	//  glEnable(GL_POLYGON_STIPPLE);
	glPolygonMode(GL_BACK, GL_LINE);//���÷���Ϊ����ģʽ
	glPolygonMode(GL_FRONT, GL_LINE);//��������Ϊ����ģʽ
	glClear(GL_COLOR_BUFFER_BIT);//���������ʾ����
	glBegin(GL_POLYGON);
	for (unsigned int i = 0; i < xy.size(); i++) {
		glVertex2f(xy[i].x, xy[i].y);
	}
	glEnd();
	glFlush();
}

//����϶�
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


		/*���ȱ�������*/
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

		/*�ȱ�������
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

//������
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
//���̼���
void myKeyboard(unsigned char key, int x, int y) {
	//���ɾ��
	if (key == 'a') {
		glClear(GL_COLOR_BUFFER_BIT);//���������ʾ����
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
	glutInit(&argc, argv);//��ʼ��
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);//���û���ģʽ
	glutInitWindowPosition(500, 300);
	glutInitWindowSize(winwidth, winheight);
	glutCreateWindow("��άͼ�εı任");//��������

	int id = glutCreateMenu(mymenu);

	glutAddMenuEntry("ƽ��", 0);
	glutAddMenuEntry("���ƶ����", 1);
	glutAddMenuEntry("��ת", 2);
	glutAddMenuEntry("����", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	init();
	glutDisplayFunc(myDisplay);
	glutMouseFunc(mymouse);//�������ص�����
	glutMotionFunc(dragmouse);//����϶�

	glutKeyboardFunc(myKeyboard);//���̼���
	glutKeyboardUpFunc(myKeyboardUp);//���̵���״̬
	glutMainLoop();
}

//����㷨

vector<point> vertice; //����
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
	float dx;  // �ӵ�ǰɨ���ߵ���һ��ɨ���߼�x����������б�ʵĵ���
	float ymax; //�ñ����������ɨ���ߵ�����ֵymax
	XET* next;
}AET, NET; //AET ���Ա߱� NET�±߱�


//���㺯��
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
	/*�õ���ߵ��y����*/
	int Max_Y = 0;
	for (int i = 0; i < vertice.size(); i++)
		/*Max_Y = max(Max_Y, vertice[i].y);*/
		if (vertice[i].y > Max_Y)
			Max_Y = vertice[i].y;


	//��ʼ��AET��
	AET* pAET = new AET;
	pAET->next = NULL;

	//��ʼ��NET��
	NET* pNET[800]; //��Ͱ
	for (int i = 0; i <= Max_Y; i++)
	{
		pNET[i] = new NET;
		pNET[i]->next = NULL;;
	}

	//ɨ�貢�ҽ���NET��
	int len = vertice.size(); //�������
	for (int i = 0; i <= Max_Y; i++)
	{
		for (int j = 0; j < len; j++) //ɨ��ÿ����
		{
			if (i == vertice[j].y)
			{
				//���һ�����ǰһ������һ������������õ�ͺ���һ����Ҳ����
				//�����ʽ�� �������һ������͵�һ�������� �� ��ֹ���ָ���

				//�жϵ�ǰ��ĸߵͣ�ʹymax��DX��DY�ļ����б仯
				if (vertice[(j - 1 + len) % len].y > vertice[j].y)
				{
					//ǰһ�����ڵ�ǰ����Ϸ�
					NET* p = new NET;
					p->x = vertice[j].x;
					p->ymax = vertice[(j - 1 + len) % len].y;//�뵱ǰɨ�����ཻ�Ļ��Ա� �� ��ߵ㼴Ϊ���ڶ����y
					float DX = vertice[(j - 1 + len) % len].x - vertice[j].x;
					float DY = vertice[(j - 1 + len) % len].y - vertice[j].y;
					p->dx = DX / DY;//dxΪֱ��б�ʵĵ���
					p->next = pNET[i]->next;
					pNET[i]->next = p;
				}
				if (vertice[(j + 1) % len].y > vertice[j].y)
				{
					//��һ�����ڵ�ǰ����Ϸ�
					NET* p = new NET;
					p->x = vertice[j].x;
					p->ymax = vertice[(j + 1) % len].y;
					float DX = vertice[(j + 1) % len].x - vertice[j].x;
					float DY = vertice[(j + 1) % len].y - vertice[j].y;
					p->dx = DX / DY;//dxΪֱ��б�ʵĵ���
					p->next = pNET[i]->next;
					pNET[i]->next = p;
				}
			}
		}
	}

	//�������Ҹ��»��Ա߱�AET
	//����ɨ����i
	for (int i = 0; i <= Max_Y; i++)
	{
		/*���±߱�NET[i] �еı߽���ò������򷨲���AET��ʹ֮��x�������˳������*/

		//����ÿ��ɨ�����ϲ�ͬ�߲������µĽ���x������AET
		NET* p = pAET->next;
		while (p)
		{
			p->x = p->x + p->dx; //����x����
			p = p->next;
		}

		//�ϱ�����,���ٿ��ٿռ� 
		AET* tq = pAET;
		p = pAET->next;
		tq->next = NULL;
		while (p)//˳������������
		{
			//�ҵ���һ�������������tq->next->next->x�����p->next��tq->next���Ǳ�p->xС��
			while (tq->next != NULL && tq->next->x <= p->x)
				tq = tq->next;
			//����p��tq��tq->next֮��
			NET* t = p->next;
			p->next = tq->next;
			tq->next = p;
			p = t;

			tq = pAET;//�ص�ͷ
		}

		/*(�Ľ��㷨) ȡ���󽻣����ټ�����*/
		//�ȴ�AET����ɾ��ymax==i�Ľ��****************************************/
		//���ص�ȡ�����⣬��֤����εġ��±��Ͽ���������������󻯣�����ĸ���Ӧ��֤Ϊż������
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

		//��NET�����µ㣬�����ò��뷨����AET����x������˳������
		p = pNET[i]->next;
		q = pAET;
		while (p)
		{
			while (q->next && p->x >= q->next->x)
				q = q->next;
			//����p
			NET* t = p->next;
			p->next = q->next;
			q->next = p;
			p = t;

			q = pAET;//�ص�ͷ
		}

		//��Ժ������ɫ
		p = pAET->next;
		while (p && p->next != NULL)
		{
			for (float j = p->x; j <= p->next->x; j++)
			{
				//ɨ���߻���
				draw_a_point(j, i);
				//cout << "(" << j << ", " << i << ")" << endl;
			}
			p = p->next->next;//���Ƕ˵����
		}
	}
	glFlush();
}


//�ü��㷨
struct EDGE//Edge��
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
	int l, r, t, b; //�߽磺��������
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
vector<point> input_vertice; //����
vector<point> output_vertice; //���
float intersect_point_color[3] = { 1,0,0 };
float polygon_point_color[3] = { 0,0,1 };
EDGE left(200, 450, 200, 200);
EDGE bottom(200, 200, 600, 200);
EDGE right(600, 200, 600, 450);
EDGE top(600, 450, 200, 450);

void draw_a_point(float x, float y, float color[]);
void draw_window();
bool inside(point& pt, EDGE ClipBoundary);//�жϵ��Ƿ�ɼ�
void intersect(point& s, point& p, EDGE ClipBoundary, point& intersect_pt);//ֱ�߶�SP�ͱ߽��󽻣����ؽ���
void SutherlandHodgmanClip(EDGE ClipBoundary, int in_len, int& out_len);
void mymouse(int button, int state, int x, int y);
void keyboard(unsigned char key, int x, int y);
void display();

bool inside(point& pt, EDGE ClipBoundary)//�жϵ��Ƿ�ɼ�
{
	if (ClipBoundary.ex > ClipBoundary.bx)
	{
		if (pt.y >= ClipBoundary.by)//�ü���Ϊ�����±���
			return true;
	}
	else if (ClipBoundary.ex < ClipBoundary.bx)
	{
		if (pt.y <= ClipBoundary.by)//�ü���Ϊ�����ϱ���
			return true;
	}
	else if (ClipBoundary.ey > ClipBoundary.by)//�ü���Ϊ�����ұ���
	{
		if (pt.x <= ClipBoundary.bx)
			return true;
	}
	else if (ClipBoundary.ey < ClipBoundary.by)//�ü���Ϊ���������
	{
		if (pt.x >= ClipBoundary.bx)
			return true;
	}
	return false;
}

//ֱ�߶�SP�ͱ߽��󽻣����ؽ���
void intersect(point& s, point& p, EDGE ClipBoundary, point& intersect_pt)
{
	if (ClipBoundary.by == ClipBoundary.ey)//ˮƽ�ü��߽�
	{
		intersect_pt.y = ClipBoundary.by;
		//x=���ĺ�����+dy/spб��
		intersect_pt.x = s.x + (ClipBoundary.by - s.y) * (p.x - s.x) / (p.y - s.y);
	}
	else//��ֱ�ü��߽�
	{
		intersect_pt.x = ClipBoundary.bx;
		intersect_pt.y = s.y + (ClipBoundary.bx - s.x) * (p.y - s.y) / (p.x - s.x);
	}
}

void SutherlandHodgmanClip(EDGE ClipBoundary)
{
	point s, p, ip;
	output_vertice.clear();
	s = input_vertice[input_vertice.size() - 1];//�ȴ����һ����ָ���һ������߶ο�ʼ����

	for (int j = 0; j < input_vertice.size(); j++)
	{
		p = input_vertice[j];
		if (inside(p, ClipBoundary))//p����
		{
			if (inside(s, ClipBoundary))//sp���ڴ�����
			{
				output_vertice.push_back(p);
			}
			else//p������ s����
			{
				intersect(s, p, ClipBoundary, ip);
				output_vertice.push_back(ip);
				output_vertice.push_back(p);
			}
		}
		else//p������
		{
			if (inside(s, ClipBoundary))//s�ڴ�����p�ڴ�����
			{
				intersect(s, p, ClipBoundary, ip);
				output_vertice.push_back(ip);
			}
			//sp���������������
		}
		s = p;
	}
	input_vertice = output_vertice;//��ε������Ϊ��һ�ε����룬input_vertice��output_vertice��ȫ�ֱ���
}

//���㺯��
void draw_a_point(float x, float y, float color[])
{
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	glColor3fv(color);
	glVertex2f(x, y);
	glEnd();
	glFlush();
}

//���ü�����
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
		cout << "����ζ���" << input_vertice.size() << ":(" << x << ", " << window_height - y << ")" << endl;
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		glLineWidth(2.0f);
		glBegin(GL_LINES);
		glColor3fv(polygon_point_color);
		//���ƶ����
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
		//˳ʱ��
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

//����βü��㷨+�����βü���
//-----------------------------------------
//Liang-Barsky �߶βü��㷨

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
	//���ӻ��߽�:
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
