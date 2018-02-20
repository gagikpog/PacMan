#pragma once
#include <iostream>
#include <vector>
#include <GL\freeglut.h>
#include <fstream>

enum Cubes
{
	C_void,C_wall,C_track
};
enum M_direct
{
	d_up,d_left,d_down,d_right
};

class Man
{
public:
	Man() {}
	~Man() {}
	int x = 3, y = 5;
	
	std::vector<std::vector<Cubes>>* Matrix;
	void Step()
	{
		switch (nextdir)
		{
		case d_right:
			if ((x == (*Matrix)[0].size() - 1) && ((*Matrix)[y][0] == C_track))
				x = 0;
			if (x < (*Matrix)[0].size()-1)
				if (((*Matrix)[y][x + 1]) == C_track)
					direct = nextdir;
			break;
		case d_up:
			if ((y == 0) && ((*Matrix)[(*Matrix).size() - 1][x] == C_track))
				y = (*Matrix).size() - 1;
			if (y>0)
				if (((*Matrix)[y - 1][x]) == C_track)
					direct = nextdir;
			break;
		case d_left:
			if (x == 0 && ((*Matrix)[y][(*Matrix)[0].size() - 1] == C_track))
				x = (*Matrix)[0].size() - 1;
			if (x>0)
				if (((*Matrix)[y][x - 1]) == C_track)
					direct = nextdir;
			break;
		case d_down:
			if ((y == (*Matrix).size() - 1) && ((*Matrix)[0][x] == C_track))
				y = 0;
			if (y<(*Matrix).size()-1)
				if (((*Matrix)[y + 1][x]) == C_track)
					direct = nextdir;
			break;
		}

		switch (direct)
		{
		case d_right: 
			if(x<(*Matrix)[0].size()-1)
			if (((*Matrix)[y][x + 1]) == C_track)
				x++;
			break;
		case d_up:
			if (y>0)
			if (((*Matrix)[y - 1][x]) == C_track)
				y--;
			break;
		case d_left:
			if (x>0)
			if (((*Matrix)[y][x - 1]) == C_track)
				x--;
			break;
		case d_down:
			if (y<(*Matrix).size()-1)
			if (((*Matrix)[y + 1][x]) == C_track)
				y++;
			break;
		default:
			break;
		}
	}
	void Draw(float w,float h,float wndh)
	{
		glBegin(GL_TRIANGLE_STRIP);
		glColor3ub(255, 255, 0);
		glVertex2f(w*x,wndh- h*y-h);
		glVertex2f(w*x+w*4, wndh -h- h*y);
		glVertex2f(w*x, wndh -h- h*y+h*4);
		glVertex2f(w*x+w*4, wndh -h- h*y+h*4);
		glEnd();
	}

	void SetDirect(M_direct d)
	{
		nextdir = d;
	}

protected:
	M_direct nextdir = d_left;
	M_direct direct = d_right;
};


class Spook:public Man
{
public:
	Spook() 
	{
		x = 24;
		y = 5;
	}
	~Spook() {}
	void Draw(float w, float h, float wndh)
	{
		glBegin(GL_TRIANGLE_STRIP);
		glColor3ub(255, 5, 0);
		glVertex2f(w*x, wndh - h*y - h);
		glVertex2f(w*x + w * 4, wndh - h - h*y);
		glVertex2f(w*x, wndh - h - h*y + h * 4);
		glVertex2f(w*x + w * 4, wndh - h - h*y + h * 4);
		glEnd();
	}
	void Step()
	{
		if (nextdir == direct)
			nextdir = (M_direct)(rand() % 4);
		Man::Step();
	}
private:
	void SetDirect(M_direct d) {}
};
