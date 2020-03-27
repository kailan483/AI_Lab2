#include "GameField.h"
#include <iostream>
using namespace std;

void GameField::Draw(RenderWindow* window)
{
	sf::Color color = sf::Color::Blue;
	sf::RectangleShape shape(sf::Vector2f(CELL_WIDTH - 2, CELL_HEIGHT - 2));
	shape.setOutlineThickness(2.f);
	shape.setOutlineColor(color);
	shape.setFillColor(sf::Color::Transparent);
	sf::CircleShape cShape(16.0f, 100);
	cShape.setOutlineThickness(2.f);
	cShape.setOutlineColor(sf::Color::Green);
	cShape.setFillColor(sf::Color::Green);
	cShape.setOrigin(cShape.getRadius(), cShape.getRadius());
	for (size_t i = 0; i < WIDTH * HEIGHT; i++)
	{
		sf::Vector2f position(POSX + i % WIDTH * CELL_WIDTH, POSY + i / HEIGHT * CELL_HEIGHT);
		shape.setPosition(position);
		window->draw(shape);
	}

	auto tmp = state->getBallsPositions();
	if (tmp != nullptr)
	{
		for (size_t i = 0; i < 5; i++)
		{
			for (size_t j = 0; j < 5; j++)
			{
				if (tmp[i][j] != '0')
				{
					sf::Vector2f circlePosition(POSX + j * CELL_WIDTH + CELL_WIDTH / 2, POSY + i * CELL_HEIGHT + CELL_HEIGHT / 2);
					cShape.setPosition(circlePosition);
					window->draw(cShape);
				}

			}
		}
	}	
	if (gq != nullptr)
		gq->Draw(window);
}

GameField::GameField(int posx, int posy, GameQuad* gq, State* state)
{
	POSX = posx;
	POSY = posy;	
	this->gq = gq;
	this->state = state;	
}

int GameField::getWidth()
{
	return WIDTH;
}

int GameField::getHeight()
{
	return HEIGHT;
}

//void GameField::setBallsPositions(std::vector<int> ballsPositions)
//{
//	state->getBallsPositions().clear();
//	for (auto i : ballsPositions)
//	{
//		state->getBallsPositions().push_back(i);
//	}
//	std::sort(state->getBallsPositions().begin(), state->getBallsPositions().end());
//}

void GameField::turnBalls()
{
	GameQuadCells gqc = gq->getGameQuadCells();
	auto tmp = state->getBallsPositions();

	char c1 = tmp[gqc.getC1() / 5][gqc.getC1() % 5]
		, c2 = tmp[gqc.getC2() / 5][gqc.getC2() % 5]
		, c3 = tmp[gqc.getC3() / 5][gqc.getC3() % 5]
		, c4 = tmp[gqc.getC4() / 5][gqc.getC4() % 5];
	tmp[gqc.getC2() / 5][gqc.getC2() % 5] = c1;
	tmp[gqc.getC4() / 5][gqc.getC4() % 5] = c2;
	tmp[gqc.getC1() / 5][gqc.getC1() % 5] = c3;
	tmp[gqc.getC3() / 5][gqc.getC3() % 5] = c4;
	/*cout << *state << endl;*/
	state->setGameQuadCellsNumber(gqc);
}

void GameField::moveGameQuad(Direction dir)
{
	gq->Move(dir);
}

char** GameField::getBallsPositions()
{
	return state->getBallsPositions();
}

void GameField::setBallsPositions(char** ballsPositions)
{
	auto tmp = state->getBallsPositions();
	if (tmp == nullptr)
	{
		tmp = new char* [5];
		for (size_t i = 0; i < 5; i++)
		{
			tmp[i] = new char[5];
		}
	}
	if (ballsPositions != nullptr)
		for (size_t i = 0; i < 5; i++)
		{
			for (size_t j = 0; j < 5; j++)
			{
				tmp[i][j] = ballsPositions[i][j];
			}
		}
}

void GameField::setGameQuadCells(GameQuadCells gqc)
{	
	this->gq->setGameQuadCells(gqc);
	this->state->setGameQuadCellsNumber(gqc);
}

State* GameField::getState() const
{
	return state;
}

GameField::~GameField()
{
	delete gq;
	delete state;
}
