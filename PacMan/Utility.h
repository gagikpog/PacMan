#ifndef GLULILITY
#define GLULILITY

#include <GL\freeglut.h>
#include <ctime>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <utility>

const float PI = 3.14159265358979323846f;

enum Cubes
{
	C_void, C_wall, C_track, C_drug, C_food, C_spawnS, C_spawnP
};

enum M_direct
{
	d_up, d_left, d_down, d_right
};

enum Game_stat
{
	G_game, G_pause, G_win, G_over
};

struct Point
{
	int x = 0;
	int y = 0;
};
//Оператор ! возвращает противоположную сторону
M_direct& operator!(M_direct& f);
//Определяет может ли пакман или привидение пройти по этой клетке
bool Compare(const Cubes &a, const bool& is_pac);
//рисует пакмена
void drawPac(const int& x, const  int& y, const  float& w, const float& h, const  float& wndh, const  float& angle, const  M_direct& direct);

void TextPrint(float x, float y, const char  *  str);

GLuint LoadBMP(const char* fileName);

#endif // !GLULILITY

