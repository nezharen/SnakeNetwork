#ifndef SNAKE_H
#define SNAKE_H

enum Direction {directionUp, directionDown, directionLeft, directionRight};

class Point;

class SnakeBody
{
public:
	SnakeBody(Point *point);
	~SnakeBody();
	Point *point;
	SnakeBody *front;
	SnakeBody *next;
};

class Snake
{
public:
	Snake(SnakeBody *head, SnakeBody *tail, Direction direction);
	~Snake();
	SnakeBody *head, *tail;
	Direction direction;
	unsigned short length;
};

#endif
