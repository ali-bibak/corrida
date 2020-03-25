/***********************************************

  My_Player.h
  Player header for Corrida
  by: Ali Bibak
  last modified: Jul 20, 2014

***********************************************/

#include "Player.h"
#include "Intelligence.h"

#ifndef MY_PLAYER_H
#define MY_PLAYER_H

using std::string;

// player class
class My_Player : public Player{
public:
	int numCycles, numWalls;
	My_Player(bool color) : Player(color){
		numWalls = 10;
	}
	// return the next movement (using backtrack)
	virtual Movement next(Map &map, int ind){
		Movement m = player_next(map, color, ind, numWalls);
		if(m.wall != 'n')
			numWalls--;
		return m;
	}
};

#endif
