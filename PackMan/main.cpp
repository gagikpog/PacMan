
#include "Header.h"

using namespace std;

float WndW = 400.f, WndH = 400.f;
vector<vector<Cubes>> Matrix;
vector<Spook> Spooks(1);
Man packman;


void Init();
void open();

void Draw(float x,float  y,float w,float h, Cubes b)
{
	glBegin(GL_QUADS);
	switch (b)
	{
	case C_wall:glColor3ub(0, 10,20);
		break;
	case C_track:glColor3ub(0, 150,200);
		break;
	case C_void:glColor3ub(0, 180, 200);
		break;
	}
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
		float h = WndH / (Matrix.size());
		for (size_t i = 0; i < Matrix.size(); i++)
		{
			for (size_t j = 0; j < Matrix[i].size(); j++)
			{
				Draw(j*w,WndH-i*h-h,w,h,Matrix[i][j]);
			}
		}
		for (size_t i = 0; i < Spooks.size(); i++)
			Spooks[i].Draw(w,h,WndH);
		packman.Draw(w, h,WndH);
	}

	glutSwapBuffers();
}

void Keys(int key,int ax,int ay)
{
	switch (key)
	{// d_up, d_left, d_down, d_right
	case GLUT_KEY_UP: packman.SetDirect(d_up); break;
	case GLUT_KEY_DOWN: packman.SetDirect(d_down); break;
	case GLUT_KEY_LEFT: packman.SetDirect(d_left); break;
	case GLUT_KEY_RIGHT: packman.SetDirect(d_right); break;
	}	
}

void NextStep(int t = 0)
{
	glutPostRedisplay();
	glutTimerFunc(100, NextStep, 0);
	for (size_t i = 0; i < Spooks.size(); i++)
		Spooks[i].Step();
	packman.Step();
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

	glutSpecialFunc(Keys);
	glutDisplayFunc(Display);
	glutMainLoop();
	return 0;
}

void Init()
{
	open();
	packman.Matrix = &Matrix;
	for (size_t i = 0; i < Spooks.size(); i++)
		Spooks[i].Matrix = &Matrix;
	NextStep();
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
			Matrix.push_back(vector<Cubes>());
			for (size_t j = 0; j < n; j++)
			{
				fin >> t;
				Matrix.back().push_back(Cubes(t - '0'));
			}
		}
	}
}
