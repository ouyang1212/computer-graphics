#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cmath>
#include "Diasplay.h"
#include "menu.h"
using namespace std;

int main(int argc, char **argv) {
	//��ʼ������
	glutInit(&argc, argv);//��ʼ��GLUT��,���������в���
	
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);//��ʼ�����ڵ���ʾģʽ��ʹ��RGB��ɫ | ˫����
	glutInitWindowPosition(100, 100);//���ô��ڵ�λ��
	glutInitWindowSize(1000, 600);
	glutCreateWindow("Polygon");//��������
	glClear(GL_COLOR_BUFFER_BIT);//�����ɫ������

	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, 1000, 0, 600);//����ͶӰ:
	glutDisplayFunc(myDisplay);//���õ�ǰ���ڵ���ʾ�ص�����
	glutIdleFunc(myDisplay);
	addmenu();
	glutMouseFunc(work_point);//����������¼��ĺ���
	glutMotionFunc(dragmouse);
	glutMainLoop();//������GLUT�¼�����ѭ��

	return 0;
}