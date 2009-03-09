#include <snake.h>

#include <ctime>
#include <cstdio>
#include <cstdlib>

Snake::Snake(const int inc, const int size, const int plateau)
{
	init(inc, size, plateau);
}

void Snake::init(const int inc, const int size, const int plateau)
{
	srand(time(NULL));
	
	_incBase = inc;
	_plateau = plateau;
	_score = 0;
	_mort = false;
	_inMove = false;
	_startSize = size;
	
	_snake.clear();
	while (_snake.size() < (unsigned int)size)
		_snake.push_back(Point(plateau>>1, plateau>>1));
	
	randomizeFood();
	_move.x(0);
	_move.y(0);
}

void Snake::reinit()
{
	init(_incBase, _startSize, _plateau);
}

Snake::~Snake()
{
	_snake.clear();
}

void Snake::move()
{
	_inMove = true;
	
	//snake
	_snake.push_front(_snake.front());
	_snake.pop_back();
	
	//deplacement
	_snake.front() = _snake.front() + _move;
	if (_snake.front().x() >= _plateau)	_snake.front().x(0);
	else if (_snake.front().x() < 0)	_snake.front().x(_plateau -1);
	if (_snake.front().y() >= _plateau)	_snake.front().y(0);
	else if (_snake.front().y() < 0)	_snake.front().y(_plateau -1);
	
	//collision
	list<Point>::iterator it;
	for (it = _snake.begin(); ;)
	{
		if (++it == _snake.end()) break;
		_mort = (_snake.front() == *it) || _mort;
	}
	
  //nourriture attrappee
	if (_snake.front() == _food)
	{
		for (int i = _incBase;  --i >= 0; )
			_snake.push_back(_snake.back());
		randomizeFood();
		_score++;
	}
	
	_inMove = false;
}

void Snake::randomizeFood()
{
	bool unset = false;
	
	do
	{
		unset = false;
		
		_food.x(rand() % _plateau);
		_food.y(rand() % _plateau);
		
		//verif pas dans snake
		list<Point>::iterator it;
		for (it = _snake.begin(); it != _snake.end(); it++)
			unset = (_food == *it) || unset;
	}
	while (unset == true);
}

void Snake::setMovement(const int x, const int y)
{
	if (!_inMove)
		setMovement(Point(x, y));
}

void Snake::setMovement(const Point &move)
{
	if (!_inMove)
		_move = move;
	_inMove = true;
}

bool Snake::mort() const
{
	return _mort;
}

bool Snake::vivant() const
{
	return (!_mort);
}

int Snake::score() const 
{
	return _score;
}

const list <Point>* Snake::snakePoints() const
{
	return &_snake;
}

const Point* Snake::foodPoint() const
{
	return &_food;
}

int Snake::xMove() const
{
	return _move.x();
}

int Snake::yMove() const
{
	return _move.y();
}
