#pragma once
#ifndef GAME_QUAD_H
#define GAME_QUAD_H
#include <SFML/Graphics.hpp>
#include "GameQuadCells.h"
enum class Direction
{
	Right = 0,
	Left,
	Up,
	Down
};
class GameQuad
{
private:
	const int GAMEFIELDPOSX = 120, GAMEFIELDPOSY = 80;
	const int CELL_WIDTH = 80, CELL_HEIGHT = 80;
	const int WIDTH = 2, HEIGHT = 2;
	GameQuadCells gameQuadCells;
	int fieldWidth, fieldHeight;
	int horizontalPos, verticalPos;
public:
	GameQuad(int fieldWidth, int fieldHeight);
	void Move(Direction direction);
	void Draw(sf::RenderWindow* window);
	GameQuadCells getGameQuadCells();
	void setGameQuadCells(GameQuadCells gqc);
};
#endif // !GAME_QUAD_H


