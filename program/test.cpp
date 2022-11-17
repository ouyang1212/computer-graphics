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

int main(int argc, char **argv) {
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
}#include <GL/glut.h>
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

int main(int argc, char **argv) {
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