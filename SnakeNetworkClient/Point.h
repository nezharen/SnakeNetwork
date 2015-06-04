#ifndef POINT_H
#define POINT_H

class Point
{
public:
	Point(unsigned char x = -1, unsigned char y = -1);
	~Point();
	unsigned char x, y;
};

#endif
