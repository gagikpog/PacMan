#pragma once
#include <iostream>
#include <vector>
#include <GL\freeglut.h>
#include <fstream>
#include <ctime>
#include <string>

enum Cubes
{
	C_void,C_wall,C_track,C_drug,C_food
};

enum M_direct
{
	d_up,d_left,d_down,d_right
};

enum Game_stat
{
	G_game, G_pause, G_win,G_over
};

M_direct& operator!(M_direct& f)
{
	switch (f)
	{
	case M_direct::d_up: return f = M_direct::d_down;
	case M_direct::d_down: return f = M_direct::d_up;
	case M_direct::d_left: return f = M_direct::d_right;
	case M_direct::d_right: return f = M_direct::d_left;
	}
}

class Man
{
public:
	Man(std::vector<std::vector<Cubes>>* matrix) 
	{
		Matrix = matrix;
	}
	~Man() {}
	int x = 3, y = 5;
	
	int Step()
	{
		switch (nextdir)
		{
		case d_right:
			if ((x == (*Matrix)[0].size() - 1) && ((*Matrix)[y][0] == C_track || (*Matrix)[y][0] == C_food))
				x = 0;
			if (x < (*Matrix)[0].size()-1)
				if (((*Matrix)[y][x + 1]) == C_track || (*Matrix)[y][x + 1] == C_food || (*Matrix)[y][x + 1] == C_drug)
					direct = nextdir;
			break;
		case d_up:
			if ((y == 0) && ((*Matrix)[(*Matrix).size() - 1][x] == C_track || (*Matrix)[(*Matrix).size() - 1][x] == C_food))
				y = (*Matrix).size() - 1;
			if (y>0)
				if (((*Matrix)[y - 1][x]) == C_track || (*Matrix)[y - 1][x] == C_food || (*Matrix)[y - 1][x] == C_drug)
					direct = nextdir;
			break;
		case d_left:
			if (x == 0 && ((*Matrix)[y][(*Matrix)[0].size() - 1] == C_track || (*Matrix)[y][(*Matrix)[0].size() - 1] == C_food))
				x = (*Matrix)[0].size() - 1;
			if (x>0)
				if (((*Matrix)[y][x - 1]) == C_track || (*Matrix)[y][x - 1] == C_food || (*Matrix)[y][x - 1] == C_drug)
					direct = nextdir;
			break;
		case d_down:
			if ((y == (*Matrix).size() - 1) && ((*Matrix)[0][x] == C_track || (*Matrix)[0][x] == C_food))
				y = 0;
			if (y<(*Matrix).size()-1)
				if (((*Matrix)[y + 1][x]) == C_track || (*Matrix)[y + 1][x] == C_food || (*Matrix)[y + 1][x] == C_drug)
					direct = nextdir;
			break;
		}

		switch (direct)
		{
		case d_right: 
			if(x<(*Matrix)[0].size()-1)
			if (((*Matrix)[y][x + 1]) == C_track || ((*Matrix)[y][x + 1]) == C_food || ((*Matrix)[y][x + 1]) == C_drug)
				x++;
			break;
		case d_up:
			if (y>0)
			if (((*Matrix)[y - 1][x]) == C_track || ((*Matrix)[y - 1][x]) == C_food || ((*Matrix)[y - 1][x]) == C_drug)
				y--;
			break;
		case d_left:
			if (x>0)
			if (((*Matrix)[y][x - 1]) == C_track || ((*Matrix)[y][x - 1]) == C_food || ((*Matrix)[y][x - 1]) == C_drug)
				x--;
			break;
		case d_down:
			if (y<(*Matrix).size()-1)
			if (((*Matrix)[y + 1][x]) == C_track || ((*Matrix)[y + 1][x]) == C_food || ((*Matrix)[y + 1][x]) == C_drug)
				y++;
			break;
		default:
			break;
		}
		return 0;
	}
	void Draw(float w,float h,float wndh)
	{
		glBegin(GL_TRIANGLE_STRIP);
		glColor3ub(0, 100, 0);
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
	bool Collision(const Man & m)
	{
		return abs(x - m.x) < 4 && abs(y - m.y)<4;
	}
	static int undyingTimer;
protected:
	std::vector<std::vector<Cubes>>* Matrix;
	M_direct nextdir = d_left;
	M_direct direct = d_right;
};
int Man::undyingTimer = 0;

class Spook:public Man
{
public:
	Spook(std::vector<std::vector<Cubes>>* matrix):Man(matrix)
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
	int Step()
	{
		std::vector<M_direct>arr;

		if (((*Matrix)[y + 1][x] == C_track || ((*Matrix)[y + 1][x]) == C_food || ((*Matrix)[y + 1][x]) == C_drug) && direct != d_up)
			arr.push_back(d_down);

		if (((*Matrix)[y - 1][x] == C_track || ((*Matrix)[y - 1][x]) == C_food || ((*Matrix)[y - 1][x]) == C_drug) && direct != d_down)
			arr.push_back(d_up);

		if (x < (*Matrix)[0].size() - 1)
			if (((*Matrix)[y][x + 1] == C_track || ((*Matrix)[y][x + 1]) == C_food || ((*Matrix)[y][x + 1]) == C_drug)&& direct != d_left)
				arr.push_back(d_right);

		if (x > 0)
			if (((*Matrix)[y][x - 1] == C_track || ((*Matrix)[y][x - 1]) == C_food || ((*Matrix)[y][x - 1]) == C_drug) && direct != d_right)
				arr.push_back(d_left);

		if (arr.empty())
			nextdir = !direct;
		else 
			nextdir = arr[rand() % (arr.size())];

		Man::Step();
		return 0;
	}
	
private:
	void SetDirect(M_direct d) {}
};

class Pac :public Man
{
public:
	Pac (std::vector<std::vector<Cubes>>* matrix):Man(matrix){}
	~Pac() {}
	
	void Draw(float w, float h, float wndh)
	{
		glBegin(GL_TRIANGLE_STRIP);
		if(undyingTimer == 0)
			glColor3ub(255, 255, 0);
		else glColor3ub(0, 0, 255);
		glVertex2f(w*x, wndh - h*y - h);
		glVertex2f(w*x + w * 4, wndh - h - h*y);
		glVertex2f(w*x, wndh - h - h*y + h * 4);
		glVertex2f(w*x + w * 4, wndh - h - h*y + h * 4);
		glEnd();
	}

	int Step()
	{
		Man::Step();
		if ((*Matrix)[y][x] == C_drug) 
			undyingTimer = 70;
		
		if (undyingTimer > 0)
			undyingTimer--;
		int res = 0;
		if ((*Matrix)[y][x] == C_food)
			res = 10;

		if ((*Matrix)[y][x] == C_drug)
			res = 50;

		(*Matrix)[y][x] = C_track;

		return res;

	}
private:
};