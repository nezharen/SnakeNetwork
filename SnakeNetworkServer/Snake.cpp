#include <iostream>
#include "Point.h"
#include "Snake.h"

SnakeBody::SnakeBody(Point *point)
{
	this->point = point;
	front = NULL;
	next = NULL;
}

Snake::Snake(SnakeBody *head, SnakeBody *tail, Direction direction)
{
	head->next = tail;
	tail->front = head;
	this->head = head;
	this->tail = tail;
	this->direction = direction;
	length = 2;
	alive = true;
}

bool Snake::forward(Point *food)
{
	SnakeBody *p = new SnakeBody(new Point(head->point->x, head->point->y));
	p->next = head;
	head->front = p;
	head = p;

	switch (direction)
	{
	case directionUp:
		(head->point->y)--;
		break;
	case directionDown:
		(head->point->y)++;
		break;
	case directionLeft:
		(head->point->x)--;
		break;
	case directionRight:
		(head->point->x)++;
		break;
	}

	if (((head->point->x) == (food->x)) && ((head->point->y) == (food->y)))
	{
		length++;
		return true;
	}
	else
	{
		tail = tail->front;
		tail->next = NULL;
		return false;
	}
}

