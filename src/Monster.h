#include "Movement.h"
#include "Map.h"

#ifndef MONSTER
#define MONSTER

class Monster
{
protected:
	Map map;

public:
	Monster(){}
	virtual Movement next(Map&) = 0;
	~Monster(){}
};

#endif
