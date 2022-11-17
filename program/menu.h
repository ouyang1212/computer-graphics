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
}menu_mode;//��ͬ�˵��ķ���

int menu_num = 0;
int menu = 0, submenu1 = 0, submenu2, submenu3, submenu4;

//��һ���Ӳ˵�
void submenufunc1(int data) {
	switch (data) {
	case 1:
		MessageBox(0, "�밴�������Ļ���������εĶ�����㣬���Ҽ�����", "����λ���", MB_OK);
		menu_num = menu_mode.menu1_1;
		break;
	case 2:
		MessageBox(0, "�밴�������Ļ������Բ�ģ����ն�����a��b�����Ҽ�����", "��Բ����", MB_OK);
		menu_num = menu_mode.menu1_2;
		break;
	}
}
//�ڶ����Ӳ˵�
void submenufunc2(int data) {
	switch (data) {
	case 1:
		MessageBox(0, "�밴�������Ļ��ѡ��һ������ζ��㣬����ס����϶��õ㵽ƽ�Ƶĵ㣬���Ҽ�����", "ƽ�Ʋ���", MB_OK);
		menu_num = menu_mode.menu2_1;
		break;

	case 2:
		MessageBox(0, "�밴�������Ļ��ѡ��һ������ζ��㣬����ס����϶��õ������ת�����Ҽ�����", "��ת����", MB_OK);
		menu_num = menu_mode.menu2_2;
		break;
	case 3:
		MessageBox(0, "�밴�������Ļ��ѡ��һ������ζ��㣬����ס����϶��õ�������� �����Ҽ�����", "���Ų���", MB_OK);
		menu_num = menu_mode.menu2_3;
		break;
	}
}
//�������Ӳ˵�
void submenufunc3(int data) {
	switch (data) {
	case 1:
		MessageBox(0, "�밴�������Ļ��ѡ��һ������ζ��㣬�ö���λ���䲢һֱ���棬���Ҽ�����", "��䱣�����", MB_OK);
		menu_num = menu_mode.menu3_1;
		break;
	case 2:
		printf("��䲻����\n");
		MessageBox(0, "�밴�������Ļ��ѡ��һ������ζ��㣬�ö���λ���䣬���Ҽ�������ȡ�����", "��䲻�������", MB_OK);
		menu_num = menu_mode.menu3_2;
		break;
	}
}
//������Ӳ˵�
void submenufunc4(int data) {
	switch (data) {
	case 1:
		MessageBox(0, "�밴�������Ļ�Ϸֱ�ѡ������������Ϊ���βü��߿�", "�ü���ѡ��", MB_OK);
		menu_num = menu_mode.menu5_1;
		break;
	case 2:
		MessageBox(0, "�밴�������Ļ��ѡ��һ������ζ��㣬�����вü���ע�⣺��Ҫ�������ü��򶥵㣩", "�ü�ͼ��ѡ��", MB_OK);
		menu_num = menu_mode.menu5_2;
		break;
	case 3:
		MessageBox(0, "ȡ���ü����㡢�ü�����ε�ѡ��", "ȡ���ü�ͼ��ѡ��", MB_OK);
		menu_num = menu_mode.menu5_3;
		break;
	}
}
//���˵�
void menufunc(int data) {
	switch (data) {
	case 4:
		MessageBox(0, "�밴�������Ļ��ѡ��һ������ζ��㣬ɾ���ö����", "ɾ��", MB_OK);
		menu_num = menu_mode.menu4;
		break;
	case 5:
		MessageBox(0, "�밴�������Ļ��ѡ��һ������ζ��㣬ʹ��������ϸö����", "���߻���", MB_OK);
		menu_num = menu_mode.menu6;
		break;
	}
}


void addmenu() {
	cout << "??";
	if (menu_num != 0) {
		cout << "��������ĳ������" << endl;
		return;
	}
	cout << "!!";
	menu_num = 0;
	//�����Ӳ˵�1����ͼ�ε�����
	submenu1 = glutCreateMenu(submenufunc1);
	glutAddMenuEntry("����һ�������", 1);
	glutAddMenuEntry("����һ��Բ��", 2);
	glutAttachMenu(GLUT_MIDDLE_BUTTON);
	//�����Ӳ˵�2������
	submenu2 = glutCreateMenu(submenufunc2);
	glutAddMenuEntry("ƽ��", 1);
	glutAddMenuEntry("��ת", 2);
	glutAddMenuEntry("����", 3);
	glutAttachMenu(GLUT_MIDDLE_BUTTON);
	//�����Ӳ˵�3������
	submenu3 = glutCreateMenu(submenufunc3);
	glutAddMenuEntry("��䱣��", 1);
	glutAddMenuEntry("��䲻����", 2);
	glutAttachMenu(GLUT_MIDDLE_BUTTON);
	//�����Ӳ˵�4������
	submenu4 = glutCreateMenu(submenufunc4);
	glutAddMenuEntry("ѡ���ü��߿�", 1);
	glutAddMenuEntry("ѡ���ü�ͼ��", 2);
	glutAddMenuEntry("ȡ������ѡ��", 3);
	glutAttachMenu(GLUT_MIDDLE_BUTTON);
	//�������˵�������
	menu = glutCreateMenu(menufunc);
	glutAddSubMenu("����ͼ��", submenu1);
	glutAddSubMenu("���α任", submenu2);
	glutAddSubMenu("���ͼ��", submenu3);
	glutAddMenuEntry("ɾ��", 4);
	glutAddSubMenu("�ü�", submenu4);
	glutAddMenuEntry("����", 5);
	//�������˵���Ϊ��һ���˵����Ӳ˵�
	
	//�������м�ʱ��ʾ�˵�
	glutAttachMenu(GLUT_MIDDLE_BUTTON);
}
