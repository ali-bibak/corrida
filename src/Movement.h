#ifndef MOVEMENT
#define MOVEMENT

class Movement
{
public:
	int x1, y1, x2, y2;
	char wall;
	bool pull;
	Movement();
	Movement(int, int, int, int, char, bool);
	void set(int, int, int, int, char, bool);
	~Movement();
};

#endif