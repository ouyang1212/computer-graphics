#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cmath>
#include "Diasplay.h"
#include "menu.h"
//#include "test.h"
using namespace std;


int main(int argc, char **argv) {
	//��ʼ������
	glutInit(&argc, argv);//��ʼ��GLUT��,���������в���

//	test_main();


	
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
	glutDisplayFunc(myDisplay);//���õ�ǰ���ڵ���ʾ�ص�����
	glutIdleFunc(myDisplay);
	addmenu();
	glutMouseFunc(work_point);//����������¼��ĺ���
	glutMotionFunc(dragmouse);
	/*ָ������ͼ��Χ�����(0,200,0,200)�Ļ���ֻ���ڵ�һ������ʾ��
	��1/4Բ����1��3�Ĳ��������½����꣬��2��4�Ĳ��������Ͻ�����
	*/
	//glViewport(0, 0, 500, 500);//�����ӿڴ�С
	/*
	glutDisplayFunc(myDisplay);//���õ�ǰ���ڵ���ʾ�ص�����
	glutIdleFunc(myDisplay);//�����ڳ�����е�ʱ��ͻᱻ���õĺ���
	
	glutKeyboardFunc(key_action);//������ͨ������Ϣ�ĺ���
	*/
	
	/*��һ����ʾ���µļ���ASCII�룬���������ṩ�ˣ���������ʱ��ǰ�����λ�á�
	���λ������ڵ�ǰ�ͻ����ڵ����ϽǶ���*/

	glutMainLoop();//������GLUT�¼�����ѭ��

	return 0;
}