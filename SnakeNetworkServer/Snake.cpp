#include <iostream>
#include "Point.h"
#include "Snake.h"

SnakeBody::SnakeBody(Point *point)
{
	this->point = point;
	front = NULL;
	next = NULL;
}

SnakeBody::~SnakeBody()
{
	front = NULL;
	if (point != NULL)
		delete point;
	if (next != NULL)
		delete next;
}

Snake::Snake(SnakeBody *head, SnakeBody *tail, Direction direction)
{
	head->next = tail;
	tail->front = head;
	this->head = head;
	this->tail = tail;
	this->direction = direction;
	length = 2;
}

Snake::~Snake()
{
	if (head != NULL)
		delete head;
	this->tail = NULL;
}

