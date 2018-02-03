#include <iostream>
#include <vector>
#include <GL\freeglut.h>

float WndW = 400.f, WndH = 400.f;
using namespace std;

void Display()
{
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);


	glutSwapBuffers();
}

int main(int argc ,char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WndH, WndW);

	glutCreateWindow("PackMan");
	glMatrixMode(GL_MODELVIEW);

	glutDisplayFunc(Display);
	glutMainLoop();
	return 0;
}