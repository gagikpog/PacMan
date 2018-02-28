#pragma once
#include <iostream>
#include <vector>
#include <GL\freeglut.h>
#include <fstream>
#include <ctime>
#include <string>
const float PI = 3.14159265358979323846f;

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
		return abs(x - m.x) < 3 && abs(y - m.y)<3;
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

	Spook(const Spook & object):Man(object.Matrix)
	{
		color[0] = 50 + rand() % 200;
		color[1] = 50 + rand() % 200;
		color[2] = 50 + rand() % 200;
		Matrix = object.Matrix;
		x = object.x;
		y = object.y;
	}

	~Spook() {}
	void Draw(float w, float h, float wndh)
	{
		
		glColor3ub(color[0], color[1],color[2]);

		switch (undyingTimer)
		{
		case 0:	case 2:	case 4:	case 6:	case 8:	case 10: case 12:
		case 14: case 15: case 16: case 17:
		case 22: case 23: case 24: case 25:
				glColor3ub(color[0], color[1], color[2]);
				break;
		default:
			glColor3ub(50, 50, 250);
			break;
		}

		float r = 1.5 * w;

		glBegin(GL_TRIANGLE_FAN);
		for (float i = PI/12; i < 11*PI/12; i += 0.1f)
			glVertex2f(w*(x + 2) + cosf(i)*r, wndh - h*(y - 1) + sinf(i)*r);

		for (float i = -1.5f; i < 1.5f; i +=0.1f)
				glVertex2f(w*(x + 2) + i*w, wndh - h*(y - 1) - (1.0+sinf(i*7)/6)*w);

		glEnd();
		float sx, sy;
		switch (direct)
		{
		case d_up: 
			sx = 0;
			sy = 0.2*w;
			break;
		case d_left:
			sx = -0.2*w;
			sy = 0;
			break;
		case d_down:
			sx = 0;
			sy = -0.2*w;
			break;
		case d_right:
			sx = 0.2*w;
			sy = 0;
			break;
		}
		r = w*0.5f;

		glBegin(GL_TRIANGLE_FAN);
		glColor3ub(220, 250, 250);
		for (float i = 0; i < 2 * PI; i += 0.1f)
			glVertex2f(w*(x + 1.5f) + cosf(i)*r + sx, wndh - h*(y - 1.5) + sinf(i)*r + sy);
		glEnd();

		glBegin(GL_TRIANGLE_FAN);
		glColor3ub(220, 250, 250);
		for (float i = 0; i < 2 * PI; i += 0.1f)
			glVertex2f(w*(x + 2.5f) + cosf(i)*r + sx, wndh - h*(y - 1.5) + sinf(i)*r + sy);
		glEnd();

		glColor3ub(20, 20, 20);
		glPointSize(3);
		glBegin(GL_POINTS);
		glVertex2f(w*(x + 1.5f) + sx, wndh - h*(y - 1.5) + sy);
		glVertex2f(w*(x + 2.5f) + sx, wndh - h*(y - 1.5) + sy);
		glEnd();
		glPointSize(1);

		if (undyingTimer > 0) 
		{
			glColor3ub(250, 200, 250);
			glBegin(GL_LINE_STRIP);
			for (float i = -1.2f; i < 1.2f; i += 0.1f)
				glVertex2f(w*(x + 2) + i*w, wndh - h*(y - 1) - (0.5 + sinf(i * 10) / 8)*w);
			glEnd();
		}

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
	unsigned char color[3];
};

class Pac :public Man
{
public:
	Pac (std::vector<std::vector<Cubes>>* matrix):Man(matrix){}
	~Pac() {}
	
	void Draw(float w, float h, float wndh)
	{
		glBegin(GL_TRIANGLE_FAN);

		if(undyingTimer == 0)
			glColor3ub(255, 255, 0);
		else glColor3ub(0, 0, 255);

		static bool b = true;
		if(b)
			angle += 0.3;
		else angle -= 0.3;
		if (angle >= 1.1&&b)
			b = !b;
		else if (angle <= 0 && !b)
			b = !b;

		float begin = angle, end = 2*PI-angle;


		glVertex2f(w*(x + 2), wndh - h*(y - 1));

		switch (direct)
		{
		case d_up:
			begin += PI / 2;
			end += PI / 2;
			break;
		case d_left:
			begin += PI;
			end += PI;
			break;
		case d_down:
			begin += 3*PI / 2;
			end += 3*PI / 2;
			break;
		}
		float r = 1.5 * w;
		for (float i = begin; i < end; i+=0.1f)
			glVertex2f(w*(x + 2)+cosf(i)*r, wndh - h*(y - 1) + sinf(i)*r);

		glEnd();
	}

	int Step()
	{
		int timeout = 70;
		Man::Step();
		if ((*Matrix)[y][x] == C_drug) 
			undyingTimer = timeout;
		
		if (undyingTimer > 0)
			undyingTimer--;
		int res = 0;
		if ((*Matrix)[y][x] == C_food)
			res = 10;

		if (y + 1<(*Matrix).size())
			if ((*Matrix)[y + 1][x] == C_food)
			{
				res += 10;
				(*Matrix)[y + 1][x] = C_track;
			}

		if (x + 1<(*Matrix)[0].size())
			if ((*Matrix)[y][x + 1] == C_food)
			{
				res += 10;
				(*Matrix)[y][x + 1] = C_track;
			}

		if ((*Matrix)[y][x] == C_drug)
			res += 50;

		if (y + 1<(*Matrix).size())
			if ((*Matrix)[y + 1][x] == C_drug)
			{
				res += 50;
				(*Matrix)[y + 1][x] = C_track;
				undyingTimer = timeout;
			}

		if (x + 1<(*Matrix)[0].size())
			if ((*Matrix)[y][x + 1] == C_drug)
			{
				res += 50;
				(*Matrix)[y][x + 1] = C_track;
				undyingTimer = timeout;
			}

		(*Matrix)[y][x] = C_track;

		return res;
	}
private:
	float angle = 0;
};