#include "GameQuadCells.h"

int& GameQuadCells::getC1()
{
	return c1;	
}

int& GameQuadCells::getC2()
{
	return c2;
}

int& GameQuadCells::getC3()
{
	return c3;
}

int& GameQuadCells::getC4()
{
	return c4;
}

GameQuadCells::GameQuadCells(int c1, int c2, int c3, int c4):c1(c1),c2(c2),c3(c3),c4(c4)
{
}

GameQuadCells::GameQuadCells(GameQuadCells& gqc)
{
	this->c1 = gqc.c1;
	this->c2 = gqc.c2;
	this->c3 = gqc.c3;
	this->c4 = gqc.c4;
}

GameQuadCells::GameQuadCells(GameQuadCells&& gqc)
{
	this->c1 = gqc.c1;
	this->c2 = gqc.c2;
	this->c3 = gqc.c3;
	this->c4 = gqc.c4;
}

GameQuadCells& GameQuadCells::operator=(const GameQuadCells& gqc)
{
	this->c1 = gqc.c1;
	this->c2 = gqc.c2;
	this->c3 = gqc.c3;
	this->c4 = gqc.c4;
	return *this;
}

void GameQuadCells::setC1(int num)
{
	c1 = num;
}

void GameQuadCells::setC2(int num)
{
	c2 = num;
}

void GameQuadCells::setC3(int num)
{
	c3 = num;
}

void GameQuadCells::setC4(int num)
{
	c4 = num;
}
