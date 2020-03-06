//#pragma once
#ifndef STATE_H
#define STATE_H
#include <vector>
#include <iostream>
#include <algorithm>
#include <ctime>
#include "GameQuadCells.h"
#include <functional>

class State;

std::vector<State*>::const_iterator getIterator(std::vector<State*>& V, State* elem);
float heuristic(State* currentState, State* targetState);
float heuristic2(State* currentState, State* targetState);
using HeuristicFunctionPtr = float(*)(State*, State*);
float CalculateFx(int gx, float hx);
class State
{
private:
	
	std::vector<int> ballsPositions;
	State* parent = nullptr;
	bool Search(std::vector<State*>& v, State* what);
	bool Repeats(std::vector<State*>& v);
	GameQuadCells gameQuadCells;
	int gx;
	float hx;
	float fx;
	
public:
	void setGx(int gx);
	void setHx(float hx);
	float getFx();
	void setFx(float fx);
	float getHx() const;
	int getGx() const;
	float getFx() const;
	GameQuadCells& getGameQuadCellsNumber();
	void setGameQuadCellsNumber(GameQuadCells gqc);
	State* getParent() const;
	State(std::vector<int> ballsPositions, int gx = 0, GameQuadCells gameQuadCells = GameQuadCells());
	State(const State& state);
	State& operator=(const State& state);
	std::vector<int>& getBallsPositions();
	friend std::ostream& operator<<(std::ostream& out, const State& state);
	void Expand(std::vector<State*>& O, std::vector<State*>& C, State* targetState, HeuristicFunctionPtr hfPtr);
	~State();
	bool operator==(const State& rhs) const;
	bool operator!=(const State& rhs) const;
	int Compare(const State* const rhs) const;
};
#endif // !STATE_H
