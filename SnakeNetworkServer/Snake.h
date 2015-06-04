#ifndef SNAKE_H
#define SNAKE_H

enum Direction {directionUp, directionDown, directionLeft, directionRight};

class Point;

class SnakeBody
{
public:
	SnakeBody(Point *point);
	Point *point;
	SnakeBody *front;
	SnakeBody *next;
};

class Snake
{
public:
	Snake(SnakeBody *head, SnakeBody *tail, Direction direction);
	bool forward(Point *food);
	SnakeBody *head, *tail;
	Direction direction;
	unsigned short length;
	bool alive, directionChanged;
};

#endif
