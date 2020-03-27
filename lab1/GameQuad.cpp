#include "GameQuad.h"

GameQuad::GameQuad(int fieldWidth, int fieldHeight):fieldHeight(fieldHeight),fieldWidth(fieldWidth)
{
	gameQuadCells.setC1(0);
	gameQuadCells.setC2(1);
	gameQuadCells.setC3(fieldWidth);
	gameQuadCells.setC4(fieldWidth + 1);
	horizontalPos = 0;
	verticalPos = 0;
}

void GameQuad::Move(Direction direction)
{
	switch (direction)
	{
	case Direction::Right:
		horizontalPos = horizontalPos >= (fieldWidth - WIDTH) ? 0 : (horizontalPos + 1) % fieldWidth;		
		break;
	case Direction::Left:
		horizontalPos = horizontalPos == 0 ? fieldWidth - HEIGHT : horizontalPos - 1;		
		break;
	case Direction::Up:
		verticalPos = verticalPos == 0 ? fieldHeight - WIDTH : verticalPos - 1;		
		break;
	case Direction::Down:
		verticalPos = verticalPos >= (fieldHeight - HEIGHT) ? 0 : (verticalPos + 1) % fieldHeight;				
		break;
	default:
		break;
	}

	gameQuadCells.setC1(verticalPos * fieldWidth + horizontalPos);
	gameQuadCells.setC2(verticalPos * fieldWidth + horizontalPos + 1);
	gameQuadCells.setC3((verticalPos + 1) * fieldWidth + horizontalPos);
	gameQuadCells.setC4((verticalPos + 1) * fieldWidth + horizontalPos + 1);
}

void GameQuad::Draw(sf::RenderWindow* window)
{
	sf::RectangleShape quad;
	quad.setOutlineThickness(2.0f);
	quad.setFillColor(sf::Color::Transparent);
	quad.setOutlineColor(sf::Color::Magenta);
	quad.setPosition(GAMEFIELDPOSX + horizontalPos % fieldWidth * CELL_WIDTH , GAMEFIELDPOSY + verticalPos % fieldHeight * CELL_HEIGHT);
	quad.setSize(sf::Vector2f(CELL_WIDTH * 2 - 2.f, CELL_HEIGHT * 2 - 2.f));
	window->draw(quad);
}

GameQuadCells GameQuad::getGameQuadCells()
{
	return gameQuadCells;
}

void GameQuad::setGameQuadCells(GameQuadCells gqc)
{	
	gameQuadCells.setC1(gqc.getC1());
	gameQuadCells.setC2(gqc.getC2());
	gameQuadCells.setC3(gqc.getC3());
	gameQuadCells.setC4(gqc.getC4());
	verticalPos = gameQuadCells.getC1() / fieldHeight;
	horizontalPos = gameQuadCells.getC1() % fieldWidth;
}
