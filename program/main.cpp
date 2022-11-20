#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cmath>
#include "Diasplay.h"
#include "menu.h"
using namespace std;

int main(int argc, char **argv) {
	//初始化窗口
	glutInit(&argc, argv);//初始化GLUT库,处理命令行参数
	
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);//初始化窗口的显示模式：使用RGB颜色 | 双缓冲
	glutInitWindowPosition(100, 100);//设置窗口的位置
	glutInitWindowSize(1000, 600);
	glutCreateWindow("Polygon");//窗口名称
	glClear(GL_COLOR_BUFFER_BIT);//清除颜色缓冲区

	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, 1000, 0, 600);//正交投影:
	glutDisplayFunc(myDisplay);//设置当前窗口的显示回调函数
	glutIdleFunc(myDisplay);
	addmenu();
	glutMouseFunc(work_point);//处理鼠标点击事件的函数
	glutMotionFunc(dragmouse);
	glutMainLoop();//启动主GLUT事件处理循环

	return 0;
}