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
	sf::CircleShape cShape(16.0f,100);
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
	for (auto& i : state->getBallsPositions())
	{
		sf::Vector2f circlePosition(POSX + i % WIDTH * CELL_WIDTH + CELL_WIDTH / 2, POSY + i / HEIGHT * CELL_HEIGHT + CELL_HEIGHT / 2);
		cShape.setPosition(circlePosition);
		window->draw(cShape);
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
	std::sort(this->state->getBallsPositions().begin(), this->state->getBallsPositions().end());
}

int GameField::getWidth()
{
	return WIDTH;
}

int GameField::getHeight()
{
	return HEIGHT;
}

void GameField::setBallsPositions(std::vector<int> ballsPositions)
{
	state->getBallsPositions().clear();
	for (auto i : ballsPositions)
	{
		state->getBallsPositions().push_back(i);
	}
	std::sort(state->getBallsPositions().begin(), state->getBallsPositions().end());
}

void GameField::turnBalls()
{
	GameQuadCells gqc = gq->getGameQuadCells();
	for (size_t i = 0; i < state->getBallsPositions().size(); i++)
	{
		if (state->getBallsPositions()[i] == gqc.getC1()) {
			state->getBallsPositions()[i]++;
			continue;
		}
		if (state->getBallsPositions()[i] == gqc.getC2()) {
			state->getBallsPositions()[i] += WIDTH;
			continue;
		}
		if (state->getBallsPositions()[i] == gqc.getC3()) {
			state->getBallsPositions()[i] -= WIDTH;
			continue;
		}
		if (state->getBallsPositions()[i] == gqc.getC4()) {
			state->getBallsPositions()[i]--;
			continue;
		}
	}	
	state->setGameQuadCellsNumber(gqc);
	std::sort(state->getBallsPositions().begin(), state->getBallsPositions().end());
}

void GameField::moveGameQuad(Direction dir)
{
	gq->Move(dir);
}

std::vector<int>& GameField::getBallsPositions()
{
	return state->getBallsPositions();
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
