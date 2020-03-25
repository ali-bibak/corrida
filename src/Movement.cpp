#include "Movement.h"

Movement::Movement()
{
	x1 = 0;
	x2 = 0;
	y1 = 0;
	y2 = 0;
}

Movement::Movement(int x1, int y1, int x2, int y2, char wall, bool pull)
{
	this->x1 = x1;
	this->x2 = x2;
	this->y1 = y1;
	this->y2 = y2;
	this->wall = wall;
	this->pull = pull;
}

void Movement::set(int x1, int y1, int x2, int y2, char wall, bool pull)
{
	this->x1 = x1;
	this->x2 = x2;
	this->y1 = y1;
	this->y2 = y2;
	this->wall = wall;
	this->pull = pull;
}

Movement::~Movement()
{

}