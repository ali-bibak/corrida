#include <string>
#include <fstream>
#include "Movement.h"
#include "Map.h"

#ifndef GRAPHIC
#define GRAPHIC

using std::string;

class Graphic
{
protected:
	string player1_name;
	string player2_name;
	unsigned int player1_score;
	unsigned int player2_score;
	unsigned int time;
	unsigned int level;
	Map map;

public:
	Graphic()
	{
		player1_score = 0;
		player2_score = 0;
		time = 0;
		level = 0;
	}
	virtual void set_name(string, string) = 0;
	virtual void set_score(unsigned int, unsigned int) = 0;
	virtual void set_map(Map&) = 0;
	virtual void set_time(unsigned int) = 0;
	virtual void menu() = 0;
	virtual void repaint() = 0;
	virtual void game_over(int) = 0;
	virtual void move(Movement&) = 0;
	virtual bool is_open() = 0;
	~Graphic(){};
};

#endif
