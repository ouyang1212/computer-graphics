#pragma once
#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include<iostream>
using namespace std;
struct Menu_mode {
	int menu1_1 = 1, menu1_2 = 2;
	int menu2_1 = 3, menu2_2 = 4, menu2_3 = 5;
	int menu3_1 = 6, menu3_2 = 7;
	int menu4 = 8;
	int menu5_1 = 9;
	int menu5_2 = 10;
	int menu5_3 = 11;
	int menu6 = 12;
}menu_mode;//不同菜单的返回

int menu_num = 0;
int menu = 0, submenu1 = 0, submenu2, submenu3, submenu4;

//第一个子菜单
void submenufunc1(int data) {
	switch (data) {
	case 1:
		MessageBox(0, "请按左键在屏幕上输入多边形的多个顶点，按右键结束", "多边形绘制", MB_OK);
		menu_num = menu_mode.menu1_1;
		break;
	case 2:
		MessageBox(0, "请按左键在屏幕上输入圆心，在终端输入a和b，按右键结束", "椭圆绘制", MB_OK);
		menu_num = menu_mode.menu1_2;
		break;
	}
}
//第二个子菜单
void submenufunc2(int data) {
	switch (data) {
	case 1:
		MessageBox(0, "请按左键在屏幕上选定一个多边形顶点，并按住左键拖动该点到平移的点，按右键结束", "平移操作", MB_OK);
		menu_num = menu_mode.menu2_1;
		break;

	case 2:
		MessageBox(0, "请按左键在屏幕上选定一个多边形顶点，并按住左键拖动该点进行旋转，按右键结束", "旋转操作", MB_OK);
		menu_num = menu_mode.menu2_2;
		break;
	case 3:
		MessageBox(0, "请按左键在屏幕上选定一个多边形顶点，并按住左键拖动该点进行缩放 ，按右键结束", "缩放操作", MB_OK);
		menu_num = menu_mode.menu2_3;
		break;
	}
}
//第三个子菜单
void submenufunc3(int data) {
	switch (data) {
	case 1:
		MessageBox(0, "请按左键在屏幕上选定一个多边形顶点，该多边形会填充并一直保存，按右键结束", "填充保存操作", MB_OK);
		menu_num = menu_mode.menu3_1;
		break;
	case 2:
		printf("填充不保存\n");
		MessageBox(0, "请按左键在屏幕上选定一个多边形顶点，该多边形会填充，按右键结束并取消填充", "填充不保存操作", MB_OK);
		menu_num = menu_mode.menu3_2;
		break;
	}
}
//第五个子菜单
void submenufunc4(int data) {
	switch (data) {
	case 1:
		MessageBox(0, "请按左键在屏幕上分别选定两个顶点作为矩形裁剪边框", "裁剪框选定", MB_OK);
		menu_num = menu_mode.menu5_1;
		break;
	case 2:
		MessageBox(0, "请按左键在屏幕上选定一个多边形顶点，并进行裁剪（注意：需要有两个裁剪框顶点）", "裁剪图形选定", MB_OK);
		menu_num = menu_mode.menu5_2;
		break;
	case 3:
		MessageBox(0, "取消裁剪顶点、裁剪多边形的选定", "取消裁剪图形选定", MB_OK);
		menu_num = menu_mode.menu5_3;
		break;
	}
}
//主菜单
void menufunc(int data) {
	switch (data) {
	case 4:
		MessageBox(0, "请按左键在屏幕上选定一个多边形顶点，删除该多边形", "删除", MB_OK);
		menu_num = menu_mode.menu4;
		break;
	case 5:
		MessageBox(0, "请按左键在屏幕上选定一个多边形顶点，使用曲线拟合该多边形", "曲线绘制", MB_OK);
		menu_num = menu_mode.menu6;
		break;
	}
}


void addmenu() {
	cout << "??";
	if (menu_num != 0) {
		cout << "正在运行某个功能" << endl;
		return;
	}
	cout << "!!";
	menu_num = 0;
	//构建子菜单1插入图形的内容
	submenu1 = glutCreateMenu(submenufunc1);
	glutAddMenuEntry("插入一个多边形", 1);
	glutAddMenuEntry("插入一个圆形", 2);
	glutAttachMenu(GLUT_MIDDLE_BUTTON);
	//构建子菜单2的内容
	submenu2 = glutCreateMenu(submenufunc2);
	glutAddMenuEntry("平移", 1);
	glutAddMenuEntry("旋转", 2);
	glutAddMenuEntry("缩放", 3);
	glutAttachMenu(GLUT_MIDDLE_BUTTON);
	//构建子菜单3的内容
	submenu3 = glutCreateMenu(submenufunc3);
	glutAddMenuEntry("填充保存", 1);
	glutAddMenuEntry("填充不保存", 2);
	glutAttachMenu(GLUT_MIDDLE_BUTTON);
	//构建子菜单4的内容
	submenu4 = glutCreateMenu(submenufunc4);
	glutAddMenuEntry("选定裁剪边框", 1);
	glutAddMenuEntry("选定裁剪图形", 2);
	glutAddMenuEntry("取消所有选定", 3);
	glutAttachMenu(GLUT_MIDDLE_BUTTON);
	//构建主菜单的内容
	menu = glutCreateMenu(menufunc);
	glutAddSubMenu("插入图形", submenu1);
	glutAddSubMenu("几何变换", submenu2);
	glutAddSubMenu("填充图形", submenu3);
	glutAddMenuEntry("删除", 4);
	glutAddSubMenu("裁剪", submenu4);
	glutAddMenuEntry("曲线", 5);
	//将两个菜单变为另一个菜单的子菜单
	
	//点击鼠标中键时显示菜单
	glutAttachMenu(GLUT_MIDDLE_BUTTON);
}
