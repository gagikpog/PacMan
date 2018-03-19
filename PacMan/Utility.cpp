#include "Utility.h"

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

bool Compare(const Cubes &a, const bool& is_pac)
{
	bool res = true;

	if (a == C_void || a == C_wall)
		res = false;
	if (is_pac && a == C_spawnS)
		res = false;
	return res;
}

void drawPac(const int& x, const  int& y, const  float& w, const float& h, const  float& wndh, const  float& angle, const  M_direct& direct)
{

	glBegin(GL_TRIANGLE_FAN);
	glColor3ub(255, 255, 0);

	float begin = angle, end = 2 * PI - angle;

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
		begin += 3 * PI / 2;
		end += 3 * PI / 2;
		break;
	}
	float r = 1.4f * w;
	for (float i = begin; i < end; i += 0.1f)
		glVertex2f(w*(x + 2) + cosf(i)*r, wndh - h*(y - 1) + sinf(i)*r);

	glEnd();
}

void TextPrint(float x, float y,const char  * str)
{
	int l, i;
	l = strlen(str);
	glRasterPos2f(x, y);
	for (i = 0; i < l; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
}

GLuint LoadBMP(const char* fileName)
{
	FILE *file;
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int size;
	unsigned int width, height;
	unsigned char *data;

	file = fopen(fileName, "rb");

	if (file == NULL)
	{
		MessageBox(NULL, L"Error: Invaild file path!", L"Error", MB_OK);
		return false;
	}

	if (fread(header, 1, 54, file) != 54)
	{
		MessageBox(NULL, L"Error: Invaild file!", L"Error", MB_OK);
		fclose(file);
		return false;
	}

	if (header[0] != 'B' || header[1] != 'M')
	{
		MessageBox(NULL, L"Error: Invaild file!", L"Error", MB_OK);
		fclose(file);
		return false;
	}

	dataPos = *(int*)&(header[0x0A]);
	size = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	if (size == NULL)
		size = width * height * 3;
	if (dataPos == NULL)
		dataPos = 54;

	data = new unsigned char[size];

	fread(data, 1, size, file);

	fclose(file);

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glEnable(GL_TEXTURE_2D);

	return texture;
}
