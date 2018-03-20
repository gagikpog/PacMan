#include "Game.h"

int Man::undyingTimer = 0;

#pragma region Class Man

Man::Man(std::vector<std::vector<Cubes>>* matrix) 
{
	Matrix = matrix;
}

void Man::SetDirect(M_direct d)
{
	nextdir = d;
}

bool Man::Collision(const Man & m)
{
	return abs(x - m.x) < 3 && abs(y - m.y)<3;
}

int Man::Step(bool is_pac)
{
	switch (nextdir)
	{
	case d_right:
		if ((x == (*Matrix)[0].size() - 1) && Compare((*Matrix)[y][0], is_pac))
			x = 0;
		if (x < (*Matrix)[0].size() - 1)
			if (Compare((*Matrix)[y][x + 1], is_pac))
				direct = nextdir;
		break;
	case d_up:
		if ((y == 0) && Compare((*Matrix)[(*Matrix).size() - 1][x], is_pac))
			y = (*Matrix).size() - 1;
		if (y>0)
			if (Compare((*Matrix)[y - 1][x], is_pac))
				direct = nextdir;
		break;
	case d_left:
		if (x == 0 && Compare((*Matrix)[y][(*Matrix)[0].size() - 1], is_pac))
			x = (*Matrix)[0].size() - 1;
		if (x>0)
			if (Compare((*Matrix)[y][x - 1], is_pac))
				direct = nextdir;
		break;
	case d_down:
		if ((y == (*Matrix).size() - 1) && Compare((*Matrix)[0][x], is_pac))
			y = 0;
		if (y<(*Matrix).size() - 1)
			if (Compare((*Matrix)[y + 1][x], is_pac))
				direct = nextdir;
		break;
	}

	switch (direct)
	{
	case d_right:
		if (x<(*Matrix)[0].size() - 1)
			if (Compare((*Matrix)[y][x + 1], is_pac))
				x++;
		break;
	case d_up:
		if (y>0)
			if (Compare((*Matrix)[y - 1][x], is_pac))
				y--;
		break;
	case d_left:
		if (x>0)
			if (Compare((*Matrix)[y][x - 1], is_pac))
				x--;
		break;
	case d_down:
		if (y<(*Matrix).size() - 1)
			if (Compare((*Matrix)[y + 1][x], is_pac))
				y++;
		break;
	}
	return 0;
}

#pragma endregion

#pragma region Class Spook

Spook::Spook(std::vector<std::vector<Cubes>>* matrix):Man(matrix)
{
	x = 24;
	y = 5;
	color[0] = 50 + rand() % 200;
	color[1] = 50 + rand() % 200;
	color[2] = 50 + rand() % 200;
}

Spook::Spook(const Spook & object):Man(object.Matrix)
{
	operator=(object);
}

const Spook& Spook::operator=(const Spook& object)
{
	color[0] = 50 + rand() % 200;
	color[1] = 50 + rand() % 200;
	color[2] = 50 + rand() % 200;
	Matrix = object.Matrix;
	x = object.x;
	y = object.y;
	return object;
}

void Spook::Draw(float w, float h, float wndh)const
{

	glColor3ub(color[0], color[1], color[2]);

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

	float r = 1.5f * w;

	glBegin(GL_TRIANGLE_FAN);
	for (float i = PI / 12; i < 11 * PI / 12; i += 0.1f)
		glVertex2f(w*(x + 2) + cosf(i)*r, wndh - h*(y - 1) + sinf(i)*r);

	for (float i = -1.5f; i < 1.5f; i += 0.1f)
		glVertex2f(w*(x + 2.f) + i*w, wndh - h*(y - 1.f) - (1.0f + sinf(i*7.f) / 6.f)*w);

	glEnd();
	float sx, sy;
	switch (direct)
	{
	case d_up:
		sx = 0.f;
		sy = 0.2f*w;
		break;
	case d_left:
		sx = -0.2f*w;
		sy = 0.f;
		break;
	case d_down:
		sx = 0.f;
		sy = -0.2f*w;
		break;
	case d_right:
		sx = 0.2f*w;
		sy = 0.f;
		break;
	}
	r = w*0.5f;

	glBegin(GL_TRIANGLE_FAN);
	glColor3ub(220, 250, 250);
	for (float i = 0; i < 2.f * PI; i += 0.1f)
		glVertex2f(w*(x + 1.5f) + cosf(i)*r + sx, wndh - h*(y - 1.5f) + sinf(i)*r + sy);
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glColor3ub(220, 250, 250);
	for (float i = 0; i < 2 * PI; i += 0.1f)
		glVertex2f(w*(x + 2.5f) + cosf(i)*r + sx, wndh - h*(y - 1.5f) + sinf(i)*r + sy);
	glEnd();

	glColor3ub(20, 20, 20);
	glPointSize(3);
	glBegin(GL_POINTS);
	glVertex2f(w*(x + 1.5f) + sx, wndh - h*(y - 1.5f) + sy);
	glVertex2f(w*(x + 2.5f) + sx, wndh - h*(y - 1.5f) + sy);
	glEnd();
	glPointSize(1);

	if (undyingTimer > 0)
	{
		glColor3ub(250, 200, 250);
		glBegin(GL_LINE_STRIP);
		for (float i = -1.2f; i < 1.2f; i += 0.1f)
			glVertex2f(w*(x + 2.f) + i*w, wndh - h*(y - 1.f) - (0.5f + sinf(i * 10.f) / 8.f)*w);
		glEnd();
	}
}

int Spook::Step()
{
	std::vector<M_direct>arr;

	if (Compare((*Matrix)[y + 1][x], false) && direct != d_up)
		arr.push_back(d_down);

	if (Compare((*Matrix)[y - 1][x], false) && direct != d_down)
		arr.push_back(d_up);

	if (x < (*Matrix)[0].size() - 1)
		if (Compare((*Matrix)[y][x + 1], false) && direct != d_left)
			arr.push_back(d_right);

	if (x > 0)
		if (Compare((*Matrix)[y][x - 1], false) && direct != d_right)
			arr.push_back(d_left);

	if (arr.empty())
		nextdir = !direct;
	else
		nextdir = arr[rand() % (arr.size())];

	Man::Step(false);
	return 0;
}

#pragma endregion

#pragma region Class Pac

void Pac::Draw(float w, float h, float wndh)const
{
	drawPac(x, y, w, h, wndh, angle, direct);
}

int Pac::Step()
{
	static bool b = true;
	if (b) angle += 0.3f;
	else  angle -= 0.3f;
	if (angle >= 1.1f && b)
		b = !b;
	else if (angle <= 0 && !b)
		b = !b;

	int timeout = 100;
	Man::Step(true);
	if ((*Matrix)[y][x] == C_drug)
		undyingTimer = timeout;

	if (undyingTimer > 0)
		undyingTimer--;
	int res = 0;
	if ((*Matrix)[y][x] == C_food)
		res = 10;

	if (y + 1.f<(*Matrix).size())
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

#pragma endregion