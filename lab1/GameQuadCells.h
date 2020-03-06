#pragma once
class GameQuadCells
{
private:
	int c1, c2, c3, c4;
public:
	int& getC1();
	int& getC2();
	int& getC3();
	int& getC4();
	GameQuadCells(int c1 = 0, int c2 = 0, int c3 = 0, int c4 = 0);
	GameQuadCells(GameQuadCells& gqc);
	GameQuadCells(GameQuadCells&& gqc);
	GameQuadCells& operator=(const GameQuadCells& gqc);
	void setC1(int num);
	void setC2(int num);
	void setC3(int num);
	void setC4(int num);
};

