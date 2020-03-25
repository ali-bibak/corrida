#include <string>
#include "Movement.h"
#include "Map.h"

#ifndef PLAYER
#define PLAYER

using std::string;

class Player
{
protected:
	const bool color;
	Map map;

public:
	Player(bool color) : color(color){}
	virtual Movement next(Map&, int) = 0;
	~Player(){}
};

#endif
