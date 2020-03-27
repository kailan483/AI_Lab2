#include "State.h"
int State::iterationsCount = 0;
bool State::Search(std::vector<State*>& v, State* what)
{
	for (auto i : v)
	{
		if (*i == *what)
			return true;
	}
	return false;
}

bool State::Repeats(std::vector<State*>& v)
{
	for (size_t i = 0; i < v.size(); i++)
	{
		for (size_t j = i + 1; j < v.size(); j++)
		{
			if (*v[i] == *v[j])
				return true;
		}
	}
	return false;
}

void State::setParent(State* state)
{
	if (state != nullptr)
		parent = new State(*state);
	else
		parent = nullptr;
}

void State::setGx(int gx)
{
	this->gx = gx;
}

void State::setHx(float hx)
{
	this->hx = hx;
}

float State::getFx()
{
	return fx;
}

void State::setFx(float fx)
{
	this->fx = fx;
}

float State::getHx() const
{
	return hx;
}

int State::getGx() const
{
	return gx;
}

float State::getFx() const
{
	return fx;
}

GameQuadCells& State::getGameQuadCellsNumber()
{
	return gameQuadCells;
}

void State::setGameQuadCellsNumber(GameQuadCells gqc)
{
	gameQuadCells.setC1(gqc.getC1());
	gameQuadCells.setC2(gqc.getC2());
	gameQuadCells.setC3(gqc.getC3());
	gameQuadCells.setC4(gqc.getC4());
}

State* State::getParent() const
{
	return parent;
}

State::State(char** ballsPositions, int gx, GameQuadCells gameQuadCells)
{
	this->ballsPositions = new char* [5];
	for (size_t i = 0; i < 5; i++)
	{
		this->ballsPositions[i] = new char[5];
	}
	for (size_t i = 0; i < 5; i++)
	{
		for (size_t j = 0; j < 5; j++)
		{
			this->ballsPositions[i][j] = ballsPositions[i][j];
		}
	}
	this->gx = gx;
	this->gameQuadCells = gameQuadCells;
}

//State::State(std::vector<int> ballsPositions, int gx, GameQuadCells gameQuadCells)z
//{
//	for (auto i : ballsPositions)
//	{
//		this->ballsPositions.push_back(i);
//	}	
//	this->gx = gx;
//	this->gameQuadCells = gameQuadCells;
//	std::sort(this->ballsPositions.begin(), this->ballsPositions.end());
//}

State::State(const State& state)
{
	if (ballsPositions == nullptr)
	{
		ballsPositions = new char* [5];
		for (size_t i = 0; i < 5; i++)
		{
			ballsPositions[i] = new char[5];
		}
	}
	for (size_t i = 0; i < 5; i++)
	{
		for (size_t j = 0; j < 5; j++)
		{
			this->ballsPositions[i][j] = state.ballsPositions[i][j];
		}
	}
	this->gx = state.gx;
	this->fx = state.fx;
	this->hx = state.hx;
	this->gameQuadCells = state.gameQuadCells;	
}
State& State::operator=(const State& state)
{
	if (state == *this)
		return *this;

	if (ballsPositions == nullptr)
	{
		ballsPositions = new char* [5];
		for (size_t i = 0; i < 5; i++)
		{
			ballsPositions[i] = new char[5];
		}
	}
	for (size_t i = 0; i < 5; i++)
	{
		for (size_t j = 0; j < 5; j++)
		{
			this->ballsPositions[i][j] = state.ballsPositions[i][j];
		}
	}
	this->gx = state.gx;
	this->hx = state.hx;
	this->fx = state.fx;
	this->gameQuadCells = state.gameQuadCells;	
	return *this;
}

char** State::getBallsPositions()
{
	return ballsPositions;
}

void State::Expand(std::vector<State*>& O, std::vector<State*>& C, State* targetState, HeuristicFunctionPtr hfPtr)
{
	iterationsCount++;
	/*std::cout << this << std::endl;*/
	int verticalPos = 0, horizontalPos = 0;
	char c1, c2, c3, c4;
	for (verticalPos = 0; verticalPos < 4; verticalPos++)
	{
		
		for (horizontalPos = 0;  horizontalPos < 4; horizontalPos++)
		{			
			c1 = ballsPositions[verticalPos % 5][horizontalPos % 5];
			c2 = ballsPositions[verticalPos % 5][horizontalPos % 5 + 1];
			c3 = ballsPositions[verticalPos % 5 + 1][horizontalPos % 5];
			c4 = ballsPositions[verticalPos % 5 + 1][horizontalPos % 5 + 1];

			int a = c1 + c2 + c3 + c4;
			if (a == 48 * 4 || a == 49 * 4)
				continue;

			State* state = new State(ballsPositions);
			//c2 = c1
			state->ballsPositions[verticalPos % 5][horizontalPos % 5 + 1] = c1;
			//c4 = c2
			state->ballsPositions[verticalPos % 5 + 1][horizontalPos % 5 + 1] = c2;
			//c1 = c3
			state->ballsPositions[verticalPos % 5][horizontalPos % 5] = c3;
			//c3 = c4			
			state->ballsPositions[verticalPos % 5 + 1][horizontalPos % 5] = c4;
			/*std::cout << "Child" << std::endl << *state<< std::endl;		*/						
			state->setGx(this->getGx() + 1);
			state->setHx(hfPtr(state, targetState));
			float Fx = CalculateFx(state->getGx(),state->getHx());	
			state->setFx(Fx);
			if ((Search(O, state) == false) && (Search(C, state) == false))
			{																		
				state->parent = this;	
				GameQuadCells gqc(verticalPos * 5 + horizontalPos, verticalPos * 5 + horizontalPos + 1, (verticalPos + 1) * 5 + horizontalPos, (verticalPos + 1) * 5 + horizontalPos + 1);
				state->setGameQuadCellsNumber(gqc);
				O.push_back(state);								
			}			
			else if (Search(O,state) == true)
			{
				auto tmp = *(getIterator(O, state));
				if (Fx < tmp->getFx())
				{					
					GameQuadCells gqc(verticalPos * 5 + horizontalPos, verticalPos * 5 + horizontalPos + 1, (verticalPos + 1) * 5 + horizontalPos, (verticalPos + 1) * 5 + horizontalPos + 1);
					tmp->setFx(Fx);
					tmp->setGx(state->getGx());
					tmp->setHx(state->getHx());
					tmp->setGameQuadCellsNumber(gqc);
					tmp->parent = this;
				}                
				delete state;
			}
			else if (Search(C,state))
			{
				auto it = getIterator(C, state);
				auto tmp = *it;
				if (Fx < tmp->getFx())
				{
					GameQuadCells gqc(verticalPos * 5 + horizontalPos, verticalPos * 5 + horizontalPos + 1, (verticalPos + 1) * 5 + horizontalPos, (verticalPos + 1) * 5 + horizontalPos + 1);
					/*std::cout << Fx << std::endl;*/
					tmp->setFx(Fx);
					tmp->setGx(state->getGx());
					tmp->setGameQuadCellsNumber(gqc);
					tmp->setHx(state->getHx());
					tmp->parent = this;
					O.push_back(tmp);
					C.erase(it);			
				}
				delete state;
			}
		}
	}

}

State::~State()
{
	if (ballsPositions != nullptr)
	{
		for (size_t i = 0; i < 5; i++)
		{
			delete[] ballsPositions[i];
		}
	}
}

bool State::operator==(const State& rhs) const
{
	if (ballsPositions == nullptr || rhs.ballsPositions == nullptr) return false;
	for (size_t i = 0; i < 5; i++)
	{
		for (size_t j = 0; j < 5; j++)
		{
			if (ballsPositions[i][j] != rhs.ballsPositions[i][j])
				return false;
		}
	}
	return true;
}
bool State::operator!=(const State& rhs) const
{	
	return (!(*this == rhs));
}

int State::Compare(const State* const rhs) const
{
	int counter = 0;
	for (size_t i = 0; i < 5; i++)
	{
		for (size_t j = 0; j < 5; j++)
		{
			if (ballsPositions[i][j] == rhs->ballsPositions[i][j]
				&& ballsPositions[i][j] == 1
				&& rhs->ballsPositions[i][j] == 1)
			{
				counter++;
			}
		}
	}
	
	return 25 - counter;
}

std::ostream& operator<<(std::ostream& out, const State& state)
{
	out << std::endl;
	for (size_t i = 0; i < 5; i++)
	{
		for (size_t j = 0; j < 5; j++)
		{
			out << state.ballsPositions[i][j] << ' ';
		}
		out << std::endl;
	}
	return out;
}

float CalculateFx(int gx, float hx)
{
	return (float)gx + hx;
}
bool findPos(std::vector<pos>& targetFieldBallsPositions, pos& what)
{
	for (size_t i = 0; i < targetFieldBallsPositions.size(); i++)
	{
		if (targetFieldBallsPositions[i].i == what.i && targetFieldBallsPositions[i].j == what.j)
			return true;
	}
	return false;
}
float heuristic(State* currentState, State* targetState)
{
	auto targetBallsPositions = targetState->getBallsPositions();
	auto currentBallsPositions = currentState->getBallsPositions();
	float distance = std::numeric_limits<float>::max();
	float hx = 0;
	float tmp2 = std::numeric_limits<float>::max();
	std::vector<pos> gameFieldBallsPositions;
	std::vector<pos> targetFieldBallsPositions;
	for (size_t i = 0; i < 5; i++)
	{
		for (size_t j = 0; j < 5; j++)
		{
			if (currentBallsPositions[i][j] == '1')
			{
				pos p;
				p.i = i;
				p.j = j;
				gameFieldBallsPositions.push_back(p);
			}
			if (targetBallsPositions[i][j] == '1')
			{
				pos p;
				p.i = i;
				p.j = j;
				targetFieldBallsPositions.push_back(p);
			}
		}
	}
	float shortest = std::numeric_limits<float>::max();
	float longest = std::numeric_limits<float>::min();
	for (size_t i = 0; i < gameFieldBallsPositions.size(); i++)
	{
		if (findPos(targetFieldBallsPositions, gameFieldBallsPositions[i]) == false)
		{
			float dist = 0;
			for (size_t j = 0; j < targetFieldBallsPositions.size(); j++)
			{
				dist += abs(targetFieldBallsPositions[j].i - gameFieldBallsPositions[i].i) + abs(targetFieldBallsPositions[j].i - gameFieldBallsPositions[i].i);				
			}
			hx += dist;
			shortest = std::numeric_limits<float>::max();
			longest = std::numeric_limits<float>::min();
		}
	}
	return hx;	
}

float heuristic2(State* currentState, State* targetState)
{	
	auto targetBallsPositions = targetState->getBallsPositions();
	auto currentBallsPositions = currentState->getBallsPositions();
	float distance = std::numeric_limits<float>::max();
	float hx = 0;
	float tmp2 = std::numeric_limits<float>::max();
	std::vector<pos> gameFieldBallsPositions;
	std::vector<pos> targetFieldBallsPositions;
	for (size_t i = 0; i < 5; i++)
	{
		for (size_t j = 0; j < 5; j++)
		{
			if (currentBallsPositions[i][j] == '1')
			{
				pos p;
				p.i = i;
				p.j = j;
				gameFieldBallsPositions.push_back(p);
			}
			if (targetBallsPositions[i][j] == '1')
			{
				pos p;
				p.i = i;
				p.j = j;
				targetFieldBallsPositions.push_back(p);
			}
		}
	}
	float shortest = std::numeric_limits<float>::max();
	float longest = std::numeric_limits<float>::min();	
	for (size_t i = 0; i < gameFieldBallsPositions.size(); i++)
	{
		if (findPos(targetFieldBallsPositions, gameFieldBallsPositions[i]) == false)
		{
			for (size_t j = 0; j < targetFieldBallsPositions.size(); j++)
			{
				float dist = sqrt(pow(targetFieldBallsPositions[j].j - gameFieldBallsPositions[i].j, 2)
					+ pow(targetFieldBallsPositions[j].i - gameFieldBallsPositions[i].i, 2));
				if (dist < shortest)
					shortest = dist;
				if (dist > longest)
					longest = dist;
			}
			hx += shortest * longest;
			shortest = std::numeric_limits<float>::max();
			longest = std::numeric_limits<float>::min();
		}
	}
	return hx;	
}

std::vector<State*>::const_iterator getIterator(std::vector<State*>& V, State* elem)
{
	std::vector<State*>::iterator tmp = V.begin();
	for (auto i = V.begin(); i < V.end(); i++)
	{
		if (*(*i) == *elem)
			tmp = i;
	}
	return tmp;
}
