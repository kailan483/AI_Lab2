#pragma once
#include <SFML/Graphics.hpp>
#include "State.h"
#include "GameQuad.h"
using namespace sf;
class GameField
{
public:
	
	GameField(int posx, int posy, GameQuad* gq = nullptr, State* state = nullptr);
	~GameField();

	int getWidth();
	int getHeight();
	char** getBallsPositions();
	State* getState() const;

	void setBallsPositions(char** ballsPositions);
	void setGameQuadCells(GameQuadCells gqc);
	void turnBalls();
	void moveGameQuad(Direction dir);

	void Draw(RenderWindow* window);
private:		
	const int WIDTH = 5, HEIGHT = 5;
	int POSX, POSY;
	const int CELL_WIDTH = 80, CELL_HEIGHT = 80;		
	GameQuad* gq;
	State* state;
};

