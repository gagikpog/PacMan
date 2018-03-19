#ifndef GAME_H
#define GMAE_H

#include "Utility.h"

class Man
{
public:
	Man(std::vector<std::vector<Cubes>>* matrix);
	virtual void Draw(float w, float h, float wndh)const = 0;
	void SetDirect(M_direct d);
	bool Collision(const Man & m);

	int x = 3, y = 5;
	static int undyingTimer;

protected:
	int Step(bool is_pac);

	std::vector<std::vector<Cubes>>* Matrix;
	M_direct nextdir = d_left;
	M_direct direct = d_right;	
};

class Spook:public Man
{
public:
	Spook(std::vector<std::vector<Cubes>>* matrix);
	Spook(const Spook & object);
	void Draw(float w, float h, float wndh)const;
	int Step();
	
private:
	void SetDirect(M_direct d) {}
	unsigned char color[3];
};

class Pac :public Man
{
public:
	Pac (std::vector<std::vector<Cubes>>* matrix):Man(matrix){}
	void Draw(float w, float h, float wndh)const;
	int Step();

private:
	float angle = 0;
};

#endif // !GAME_H