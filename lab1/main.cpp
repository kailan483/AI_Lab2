#include "GameField.h"



void Expand(State* parentState, State* targetState, std::vector<State*>& O, std::vector<State*>& C,HeuristicFunctionPtr hfPtr)
{
	parentState->Expand(O, C,targetState, hfPtr);
}

bool Search(std::vector<State*> q, State* what)
{	
	for (auto i : q)
	{
		if (*i == *what)
			return true;
	}
	return false;
}
std::vector<State*>::const_iterator getIteratorOnMinValue(std::vector<State*>& V)
{
	std::vector<State*>::iterator tmp = V.begin();
	float cmp = std::numeric_limits<float>::max();
	for (size_t i = 0; i < V.size(); i++)
	{
		if (V[i]->getFx() < cmp)
		{
			cmp = V[i]->getFx();
			tmp = V.begin() + i;
		}
	}
	return tmp;
}
State* Solution(State* startState,State* targetState,HeuristicFunctionPtr hfPtr)
{
	using namespace std;
	vector<State*> O, C;
	float hx = hfPtr(startState, targetState);
	startState->setHx(hx);
	startState->setFx(CalculateFx(startState->getGx(), startState->getHx()));
	O.push_back(startState);	
	while (!O.empty())
	{	
		State* current = *getIteratorOnMinValue(O);			
		cout << *current;
		if (*current == *targetState)
			return current;		
		O.erase(getIteratorOnMinValue(O));
		Expand(current,targetState, O, C,hfPtr);
		if (Search(C,current) == false)
		C.push_back(current);
	}
}

int main()
{
	
	using namespace std;
	sf::RenderWindow window(sf::VideoMode(1024, 768), "SFML works!");
	window.setKeyRepeatEnabled(false);	
	State* startState = new State(std::vector<int> {1, 2, 3, 6, 11, 16, 8, 13, 19, 17},0,GameQuadCells(0,1,5,6));
	HeuristicFunctionPtr hfPtr = heuristic;
	State* gameBeginState = new State(*startState);

	GameQuad* gq = new GameQuad(5, 5);
	State* targetState = new State(std::vector<int> {1, 2, 3, 6, 11, 16,8,13,18,17});

	GameField* gf = new GameField(120,80,gq,gameBeginState);		
	GameField* target = new GameField(560,80,nullptr,targetState);
	bool pause = false;
	int i;
	std::vector<State*> states;
	
	bool showSolution = false;	
	float time = 0;
	sf::Clock timer;
	timer.restart();
	bool moveQuad = false, changeBallsPositions = false;
	while (window.isOpen())
	{		
		time += timer.getElapsedTime().asMilliseconds();		
		timer.restart();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::EventType::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Tab)
				{	
					if (hfPtr == heuristic)
					{
						hfPtr = heuristic2;
						cout << "Heuristic function changed on \"heuristic2\"" << endl;
						break;
					}
					if (hfPtr == heuristic2)
					{
						hfPtr = heuristic;
						cout << "Heuristic function changed on \"heuristic\"" << endl;
						break;
					}
				}
				if (event.key.code == sf::Keyboard::Down)
					gf->moveGameQuad(Direction::Down);
				if (event.key.code == sf::Keyboard::Left)
					gf->moveGameQuad(Direction::Left);
				if (event.key.code == sf::Keyboard::Right)
					gf->moveGameQuad(Direction::Right);
				if (event.key.code == sf::Keyboard::Up)
					gf->moveGameQuad(Direction::Up);
				if (event.key.code == sf::Keyboard::Space)
				{					
					gf->turnBalls();
					cout << gf << endl;					
				}
				if (event.key.code == sf::Keyboard::Enter)
				{
					if (showSolution == true)
					{
						gf->setBallsPositions(startState->getBallsPositions());
						gf->setGameQuadCells(startState->getGameQuadCellsNumber());
						showSolution = false;
					}
					else
					{
						
						states.clear();
						time = 0;
						clock_t start = clock();
						State* s = Solution(startState, targetState,hfPtr);
						clock_t end = clock();
						double seconds = (double)(end - start) / CLOCKS_PER_SEC;
						cout << "The time of search is " << seconds << " seconds"<<endl;						
						if (s == nullptr)
						{
							cout << "Solution not found!";
							return 1;
						}

						State* tmp = s;
						while (tmp != nullptr)
						{
							states.push_back(tmp);
							tmp = tmp->getParent();
						}
						if (*gameBeginState != *startState)
							i = states.size() - 1;
						else
							i = states.size() - 2;
						delete tmp;
						showSolution = true;
					}
					
				}
			}									
		}
		window.clear(sf::Color(247, 25, 69, 0));
		if (showSolution == true)
		{
			if (i > -1)
			{
				if (time > 2000 && !moveQuad)
				{
					gf->setGameQuadCells(states[i]->getGameQuadCellsNumber());
					gf->getState()->setGameQuadCellsNumber(states[i]->getGameQuadCellsNumber());
					moveQuad = true;
					changeBallsPositions = false;
				}
				if (time > 4000 && !changeBallsPositions)
				{
					gf->setBallsPositions(states[i]->getBallsPositions());			
					time = 0;
					changeBallsPositions = true;
					moveQuad = false;
					i--;
				}

			}
			else i = states.size() - 1;
		}
		gf->Draw(&window);
		target->Draw(&window);
		window.display();
	}

	return 0;
}