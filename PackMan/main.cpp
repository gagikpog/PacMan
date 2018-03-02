#include "Header.h"
#include "ConfigINI.h"

using namespace std;

float WndW = 600.f, WndH = 600.f;
vector<vector<Cubes>> Matrix;
vector<Spook> Spooks(4, &Matrix);
Pac packman(&Matrix);
Game_stat Game = G_game;
int score = 0;

GLuint mark_textures;

void Init();
void Open(string fname);
void NewGame();
void NextStepPac(int t = 0);
void NextStepSpook(int t = 0);
GLuint LoadBMP(const char* fileName);
bool NextLevel(bool drop = false);

void Sprint(int x, int y,const char *st)
{
	int l, i;
	l = strlen(st);
	glRasterPos2i(x, y); 
	for (i = 0; i < l; i++) 
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, st[i]);
}

void DrawImage(void) {
	glDisable(GL_LIGHTING);
	
	glColor3f(0, 0, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mark_textures);

	// Draw a textured quad
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
	glTexCoord2f(0, 1); glVertex3f(0, WndH, 0);
	glTexCoord2f(1, 1); glVertex3f(WndW, WndH, 0);
	glTexCoord2f(1, 0); glVertex3f(WndW, 0, 0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void CollisionDetection()
{
	for (size_t i = 0; i < Spooks.size(); i++) 
	{
		if (packman.Collision(Spooks[i])) 
		{
			if (packman.undyingTimer == 0) 
			{
				Game = G_over;
			}else {
				Spooks[i].x = 29;
				Spooks[i].y = 25;
				score += 200;
			}
		}
	}
}

bool WinGame()
{
	static int i = 0;
	if (Matrix.empty())
		return 0;
	int size = Matrix.size()*Matrix[0].size();

	while (i < size &&( Matrix[i/Matrix[0].size()][i%Matrix[0].size()] != C_drug &&  Matrix[i / Matrix[0].size()][i%Matrix[0].size()] != C_food))
	{
		i++;
	}

	if (i == size) 
	{
		i = 0;
		return 1;
	}
	return 0;
}

void Draw(float x,float  y,float w,float h, Cubes b)
{
	glBegin(GL_QUADS);
	switch (b)
	{
	case C_wall:break; glColor3ub(0, 10, 20);
		break;
	case C_void:break; glColor3ub(0, 180, 200);
		break;
	case C_food:
		glColor3ub(255, 255, 255);
		glVertex2f(x + w*1.5f  , y + h*1.5f);
		glVertex2f(x + w*2.f, y + h*1.5f);
		glVertex2f(x + w*2.f, y + h*2.f);
		glVertex2f(x + w*1.5f  , y + h*2.f);
	//	break;
	case C_track:break; glColor3ub(0, 150,200);
		break;
	case C_drug:glColor3ub(255, 255, 0);
		glVertex2f(x + w, y + h);
		glVertex2f(x + w * 2, y + h);
		glVertex2f(x + w * 2, y + h * 2);
		glVertex2f(x + w, y + h * 2);
		glColor3ub(0, 150, 200);
		break;
	}
	glEnd();
	return;
	glVertex2f(x, y);
	glVertex2f(x+w, y);
	glVertex2f(x+w, y+h);
	glVertex2f(x, y+h);

}

void Display()
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	DrawImage();

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



	glColor3f(1, 1, 1);
	switch (Game)
	{
	case G_game:
		break;
	case G_pause:
		Sprint(WndW / 2 - 30, WndH / 2 + 5, "Pause");
		break;
	case G_win:
		Sprint(WndW / 2 - 20, WndH / 2 + 5, "Win");
		break;
	case G_over:
		Sprint(WndW / 2 - 30, WndH / 2 + 15, "Game");
		Sprint(WndW / 2 - 25, WndH / 2 - 5, "over");
		break;
	}
	string str = "Score  " + to_string(score);
	Sprint(10, -15, str.c_str());

	glutSwapBuffers();
}

void Keys(unsigned char key, int ax, int ay)
{
	if (key == 'r' && (Game == G_over || Game == G_win))
	{
		NewGame();
	}
	if (key == 'p')
	{
		if (Game == G_game) 
		{
			Game = G_pause;
		}else {
			if (Game == G_pause)
			{
				Game = G_game;
				NextStepPac();
				NextStepSpook();
			}
		}
	}
}

void Keys(int key, int ax, int ay)
{
	switch (key)
	{// d_up, d_left, d_down, d_right
	case GLUT_KEY_UP: packman.SetDirect(d_up); break;
	case GLUT_KEY_DOWN: packman.SetDirect(d_down); break;
	case GLUT_KEY_LEFT: packman.SetDirect(d_left); break;
	case GLUT_KEY_RIGHT: packman.SetDirect(d_right); break;
	}	
}

void NextStepPac(int t)
{
	glutPostRedisplay();
	if (Game != G_game)
		return;
	int speed = 70;
	if (Man::undyingTimer > 0)
		speed = 60;
	glutTimerFunc(speed, NextStepPac, 0);
	score += packman.Step();
	CollisionDetection();

	if (WinGame())
		Game = G_win;
}

void NextStepSpook(int t)
{
	glutPostRedisplay();
	if (Game != G_game)
		return;
	int speed = 70;
	if (Man::undyingTimer > 0)
		speed = 100;
	glutTimerFunc(speed, NextStepSpook, 0);
	for (size_t i = 0; i < Spooks.size(); i++)
		Spooks[i].Step();
	CollisionDetection();
	if (WinGame())
		Game = G_win;
}

int main(int argc ,char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WndW, WndH + 20);

	glutCreateWindow("PacMan");

	gluOrtho2D(0, WndW, -20, WndH);
	glMatrixMode(GL_MODELVIEW);

	Init();

	glutSpecialFunc(Keys);
	glutKeyboardFunc(Keys);
	glutDisplayFunc(Display);
	glutMainLoop();
	return 0;
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
		return false;
	}

	if (header[0] != 'B' || header[1] != 'M')
	{
		MessageBox(NULL, L"Error: Invaild file!", L"Error", MB_OK);
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

void Init()
{
	srand(time(NULL));
	NewGame();
}

void Open(string fname)
{
	ifstream fin(fname);
	Matrix.clear();
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

void NewGame()
{
	score = 0;
	NextLevel(true);

	for (size_t i = 0; i < Spooks.size(); i++)
	{
		Spooks[i].x = 29;
		Spooks[i].y = 25;
	}
	
	packman.x = 3;
	packman.y = 5;
	packman.undyingTimer = 0;
	Game = G_game;
	NextStepPac();
	NextStepSpook();
}

bool NextLevel(bool drop)
{
	static int nlevel = 1;
	if (drop)
		nlevel = 1;
	
	ConfigINI ini("Levels.ini");
	string resB = ini.getOptionToString("Level" + to_string(nlevel), "backgr");
	string resM = ini.getOptionToString("Level" + to_string(nlevel), "map");
	if (resM == "NAN" || resB == "NAN")
		return false;
	mark_textures = LoadBMP(resB.c_str());
	Open(resM);
	nlevel++;
	return true;
}