#include <iostream>
#include <vector>
#include <GL\freeglut.h>
#include <fstream>

using namespace std;

float WndW = 400.f, WndH = 400.f;
vector<vector<bool>> Matrix;

void Init();
void open();

void Draw(float x,float  y,float w,float h,bool b)
{
	glBegin(GL_QUADS);
	if(b)
		glColor3ub(0, 0, 0);
	else  glColor3ub(100, 100, 0);
	glVertex2f(x, y);
	glVertex2f(x+w, y);
	glVertex2f(x+w, y+h);
	glVertex2f(x, y+h);
	glEnd();
}

void Display()
{
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	
	if (Matrix.size()) {
		float w = WndW / Matrix[0].size();
		float h = WndH / Matrix.size();
		for (size_t i = 0; i < Matrix.size(); i++)
		{
			for (size_t j = 0; j < Matrix[i].size(); j++)
			{
				Draw(j*w,WndH-i*h,w,h,Matrix[i][j]);
			}
		}
	}
	glutSwapBuffers();
}

int main(int argc ,char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WndH, WndW);

	glutCreateWindow("PackMan");

	gluOrtho2D(0, WndW, 0, WndH);
	glMatrixMode(GL_MODELVIEW);

	Init();

	glutDisplayFunc(Display);
	glutMainLoop();
	return 0;
}

void Init()
{
	open();
}

void open()
{
	ifstream fin("square.txt");
	if (fin.is_open())
	{
		char t;
		int m, n;
		fin >> m >> n;
		for (size_t i = 0; i < m; i++)
		{
			Matrix.push_back(vector<bool>());
			for (size_t j = 0; j < n; j++)
			{
				fin >> t;
				Matrix.back().push_back(t - '0');
			}
		}
	}
}
