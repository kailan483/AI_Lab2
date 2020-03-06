#include "State.h"
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

State::State(std::vector<int> ballsPositions, int gx, GameQuadCells gameQuadCells)
{
	for (auto i : ballsPositions)
	{
		this->ballsPositions.push_back(i);
	}	
	this->gx = gx;
	this->gameQuadCells = gameQuadCells;
	std::sort(this->ballsPositions.begin(), this->ballsPositions.end());
}

State::State(const State& state)
{
	for (size_t i = 0; i < state.ballsPositions.size(); i++)
	{
		this->ballsPositions.push_back(state.ballsPositions[i]);
	}
	this->gx = state.gx;
	this->fx = state.fx;
	this->hx = state.hx;
	this->gameQuadCells = state.gameQuadCells;
	std::sort(this->ballsPositions.begin(), this->ballsPositions.end());
}
State& State::operator=(const State& state)
{
	if (&state == this)
		return *this;

	for (size_t i = 0; i < state.ballsPositions.size(); i++)
	{
		this->ballsPositions.push_back(state.ballsPositions[i]);
	}
	this->gx = state.gx;
	this->hx = state.hx;
	this->fx = state.fx;
	this->gameQuadCells = state.gameQuadCells;
	std::sort(this->ballsPositions.begin(), this->ballsPositions.end());
	return *this;
}

std::vector<int>& State::getBallsPositions()
{
	return ballsPositions;
}

void State::Expand(std::vector<State*>& O, std::vector<State*>& C, State* targetState, HeuristicFunctionPtr hfPtr)
{
	/*std::cout << this << std::endl;*/
	int verticalPos = 0, horizontalPos = 0;
	int c1 = verticalPos * 5 + horizontalPos;
	int c2 = verticalPos * 5 + horizontalPos + 1;
	int c3 = (verticalPos + 1) * 5 + horizontalPos;
	int c4 = (verticalPos + 1) * 5 + horizontalPos + 1;
	for (verticalPos = 0; verticalPos < 4; verticalPos++)
	{
		
		for (horizontalPos = 0;  horizontalPos < 4; horizontalPos++)
		{			
			c1 = verticalPos * 5 + horizontalPos;
			c2 = verticalPos * 5 + horizontalPos + 1;
			c3 = (verticalPos + 1) * 5 + horizontalPos;
			c4 = (verticalPos + 1) * 5 + horizontalPos + 1;
			State* state = new State(ballsPositions);
			for (size_t i = 0; i < state->ballsPositions.size(); i++)
			{			
				if (state->ballsPositions[i] == c1) {
					state->ballsPositions[i]++;
					continue;
				}
				if (state->ballsPositions[i] == c2) {
					state->ballsPositions[i] += 5;
					continue;
				}
				if (state->ballsPositions[i] == c3) {
					state->ballsPositions[i] -= 5;
					continue;
				}
				if (state->ballsPositions[i] == c4) {
					state->ballsPositions[i]--;
					continue;
				}
			}	
			state->setGameQuadCellsNumber(GameQuadCells(c1, c2, c3, c4));
			std::sort(state->ballsPositions.begin(), state->ballsPositions.end());
			state->setGx(this->getGx() + 1);
			state->setHx(hfPtr(state, targetState));
			float Fx = CalculateFx(state->getGx(),state->getHx());	
			state->setFx(Fx);
			if ((Search(O, state) == false) && (Search(C, state) == false))
			{																		
				state->parent = this;		
				O.push_back(state);								
			}			
			else if (Search(O,state) == true)
			{
				auto tmp = *(getIterator(O, state));
				if (Fx < tmp->getFx())
				{					
					tmp->setFx(Fx);
					tmp->setGx(state->getGx());
					tmp->setHx(state->getHx());
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
					std::cout << Fx << std::endl;
					tmp->setFx(Fx);
					tmp->setGx(state->getGx());
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
	ballsPositions.clear();
}

bool State::operator==(const State& rhs) const
{
	if (ballsPositions.size() != rhs.ballsPositions.size()) return false;
	for (size_t i = 0; i < rhs.ballsPositions.size(); i++)
	{
		if (ballsPositions[i] != rhs.ballsPositions[i])
			return false;
	}
	return true;
}
bool State::operator!=(const State& rhs) const
{	
	if (ballsPositions.size() != rhs.ballsPositions.size()) return true;
	for (size_t i = 0; i < rhs.ballsPositions.size(); i++)
	{
		if (ballsPositions[i] != rhs.ballsPositions[i])
			return true;
	}
	return false;
}

int State::Compare(const State* const rhs) const
{
	int counter = 0;
	for (size_t i = 0; i < ballsPositions.size(); i++)
	{
		for (size_t j = 0; j < rhs->ballsPositions.size(); j++)
		{
			counter = ballsPositions[i] == rhs->ballsPositions[j] ? counter + 1 : counter;
		}
	}
	
	return ballsPositions.size() - counter;
}

std::ostream& operator<<(std::ostream& out, const State& state)
{
	out << "Balls Positions: ";
	for (auto& i : state.ballsPositions)
	{
		out << i << "\t";
	}
	out << std::endl 
		<< "Length from beginning: " << state.gx << std::endl
		<< "Heuristics function result: " << state.hx << std::endl
		<< "f(x) = " << state.fx << std::endl << std::endl;;

	return out;
}

float CalculateFx(int gx, float hx)
{
	return (float)gx + hx;
}

float heuristic(State* currentState, State* targetState)
{
	float hx = (float)currentState->Compare(targetState) / (currentState->getBallsPositions().size() - (float)currentState->Compare(targetState));
	return hx;
}

float heuristic2(State* currentState, State* targetState)
{
	auto targetBallsPositions = targetState->getBallsPositions();
	auto currentBallsPositions = currentState->getBallsPositions();
	float distance = std::numeric_limits<float>::max();
	float hx = 0;
	float tmp2 = std::numeric_limits<float>::max();
	for (size_t i = 0; i < currentBallsPositions.size(); i++)
	{
		int hPosSrc = currentBallsPositions[i] % 5;
		int vPosSrc = currentBallsPositions[i] / 5;
		bool flag = false;
		for (size_t j = 0; j < targetBallsPositions.size(); j++)
		{
			if (std::find(targetBallsPositions.begin(), targetBallsPositions.end(), currentBallsPositions[i]) == targetBallsPositions.end())
			{
				flag = true;
				int hPosDest = targetBallsPositions[j] % 5;
				int vPosDest = targetBallsPositions[j] / 5;
				float tmp = sqrt(pow(hPosDest - hPosSrc, 2) + pow(vPosDest - vPosSrc, 2));
				if (tmp2 > tmp)
				{
					tmp2 = tmp;
				}
				if (tmp < distance)
				{
					distance = tmp;
				}
			}
			else break;
		}
		if (flag == true)
		{
			hx += (distance * tmp2);
			tmp2 = std::numeric_limits<float>::max();
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
