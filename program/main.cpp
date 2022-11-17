#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cmath>
#include "Diasplay.h"
#include "menu.h"
//#include "test.h"
using namespace std;


int main(int argc, char **argv) {
	//初始化窗口
	glutInit(&argc, argv);//初始化GLUT库,处理命令行参数

//	test_main();


	
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
	glutDisplayFunc(myDisplay);//设置当前窗口的显示回调函数
	glutIdleFunc(myDisplay);
	addmenu();
	glutMouseFunc(work_point);//处理鼠标点击事件的函数
	glutMotionFunc(dragmouse);
	/*指的是视图范围，如果(0,200,0,200)的话就只能在第一象限显示，
	就1/4圆，第1，3的参数是左下角坐标，第2，4的参数是右上角坐标
	*/
	//glViewport(0, 0, 500, 500);//设置视口大小
	/*
	glutDisplayFunc(myDisplay);//设置当前窗口的显示回调函数
	glutIdleFunc(myDisplay);//设置在程序空闲的时候就会被调用的函数
	
	glutKeyboardFunc(key_action);//处理普通按键消息的函数
	*/
	
	/*第一个表示按下的键的ASCII码，其余两个提供了，当键按下时当前的鼠标位置。
	鼠标位置相对于当前客户窗口的左上角而言*/

	glutMainLoop();//启动主GLUT事件处理循环

	return 0;
}