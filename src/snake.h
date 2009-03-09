#ifndef __SNAKE_H
#define __SNAKE_H

#include <points.h>

#include <list>

class Snake
{
public:
	Snake(const int inc = 6, const int size = 6, const int plateau = 48);
	virtual ~Snake();
	void init(const int inc = 6, const int size = 6, const int plateau = 48);
	void reinit();
	void move();
	void randomizeFood();
	void setMovement(const int x = 0, const int y = 0);
	void setMovement(const Point &move);
	bool mort() const;
	bool vivant() const;
	int score() const;
	const list <Point>* snakePoints() const;
	const Point* foodPoint() const;
	int xMove() const;
	int yMove() const;
	
protected:
	list <Point> _snake;
	Point _food;
	Point _move;
	
	bool _inMove;
	int _incBase;
	int _plateau;
	int _score;
	bool _mort;
	int _startSize;
};

#endif
