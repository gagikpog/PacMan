#include "Game.h"
#include "ConfigINI.h"
#include "resource.h"

using namespace std;

HDC         hDC = NULL;
HWND        hWnd = NULL;

float WndW = 600.f, WndH = 600.f;
vector<vector<Cubes>> Matrix;
vector<Spook> Spooks(4, &Matrix);
pair<int, int> spawnP, spawnS;
Pac packman(&Matrix);
Game_stat Game = G_game;
int score = 0, lifes = 3;

string ProgramName;
GLuint mark_textures;

void Init();
void Open(string fname);
void NewGame();
void NextStepPac(int t = 0);
void NextStepSpook(int t = 0);
GLuint LoadBMP(const char* fileName);
bool NextLevel(bool drop = false);

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
				lifes--;
				if (lifes <= 0) {
					Game = G_over;
				}else {
					packman.y = spawnP.first;
					packman.x = spawnP.second;
					packman.SetDirect(d_right);
					for (size_t j = 0; j < Spooks.size(); j++)
					{
						Spooks[j].y = spawnS.first;
						Spooks[j].x = spawnS.second;
					}
				}
				break;
			}else {
				Spooks[i].y = spawnS.first;
				Spooks[i].x = spawnS.second;
				score += 200;
			}
		}
	}
}

bool WinGame(bool drop = false)
{
	static int i = 0;
	
	if (Matrix.empty())
		return false;
	int size = Matrix.size()*Matrix[0].size();

	while (i < size &&( Matrix[i/Matrix[0].size()][i%Matrix[0].size()] != C_drug &&  Matrix[i / Matrix[0].size()][i%Matrix[0].size()] != C_food))
	{
		i++;
	}

	if (i >= size || drop) 
	{
		i = 0;
		return !drop;
	}
	return false;
}

void Draw(float x,float  y,float w,float h, Cubes b)
{
	glBegin(GL_QUADS);
	switch (b)
	{
	case C_wall:
		if (mark_textures > 0)break;
			glColor3ub(0, 200, 200);
			glVertex2f(x, y);
			glVertex2f(x + w, y);
			glVertex2f(x + w, y + h);
			glVertex2f(x, y + h);
		break;
	case C_void:break; 
	case C_food:
		glColor3ub(255, 255, 255);
		glVertex2f(x + w*1.5f  , y + h*1.5f);
		glVertex2f(x + w*2.f, y + h*1.5f);
		glVertex2f(x + w*2.f, y + h*2.f);
		glVertex2f(x + w*1.5f  , y + h*2.f);
		break;
	case C_track:break; 
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
}

void Display()
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	if(mark_textures>0)
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
		for (int i = 1; i <= lifes; i++)
		{
			drawPac(Matrix[0].size()-4*i, Matrix.size()+1, w, h, WndH,0.2f, d_left);
		}
	}
	
	glColor3f(1, 1, 1);
	switch (Game)
	{
	case G_game:
		break;
	case G_pause:
		TextPrint(WndW / 2 - 30, WndH / 2 + 5, "Pause");
		break;
	case G_win:
		TextPrint(WndW / 2 - 20, WndH / 2 + 5, "Win");
		break;
	case G_over:
		TextPrint(WndW / 2 - 30, WndH / 2 + 15, "Game");
		TextPrint(WndW / 2 - 25, WndH / 2 - 5, "over");
		break;
	}
	string str = "Score  " + to_string(score);
	TextPrint(10, -15, str.c_str());

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
	{
		if(!NextLevel())
			Game = G_win;
	}
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
	{
		if (!NextLevel())
			Game = G_win;
	}
}

int main(int argc ,char** argv)
{
	string str = argv[0];
	ProgramName = str.substr(str.find_last_of('\\')+1);
	ProgramName = ProgramName.substr(0, ProgramName.find_last_of('.'));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize((int)WndW, (int)WndH + 20);

	glutCreateWindow(ProgramName.c_str());

	gluOrtho2D(0, WndW, -20, WndH);
	glMatrixMode(GL_MODELVIEW);

	Init();

	glutSpecialFunc(Keys);
	glutKeyboardFunc(Keys);
	glutDisplayFunc(Display);
	glutMainLoop();
	return 0;
}

void Init()
{
	srand((size_t)time(NULL));

	hWnd = FindWindowA(0, ProgramName.c_str());
	hDC = GetDC(hWnd);
	
	HANDLE icon = LoadImage(GetModuleHandle(L"PacMan.exe"), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, LR_COLOR);
	SendMessage(hWnd, (UINT)WM_SETICON, ICON_BIG, (LPARAM)icon);
	NewGame();
}

void Open(string fname)
{
	ifstream fin(fname);
	Matrix.clear();
	if (fin.is_open())
	{
		char t;
		size_t m, n;
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
	lifes = 3;
	NextLevel(true);
	WinGame(true);

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
	bool b1 = true,b2 = true;
	for (size_t i = 0; i < Matrix.size() && (b1 || b2) ; i++)
	{
		for (size_t j = 0; j < Matrix[i].size() && (b1 || b2); j++)
		{
			if (Matrix[i][j] == C_spawnS)
			{
				spawnS = make_pair(i, j);
				b1 = false;
			}
			if (Matrix[i][j] == C_spawnP)
			{
				spawnP = make_pair(i, j);
				b2 = false;
			}
		}
	}

	for (size_t i = 0; i < Spooks.size(); i++)
	{
		Spooks[i].y = spawnS.first;
		Spooks[i].x = spawnS.second;
	}

	packman.y = spawnP.first;
	packman.x = spawnP.second;

	nlevel++;
	return true;
}