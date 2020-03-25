/***********************************************

  My_Monster.h
  Monster header for Corrida
  by: Ali Bibak
  last modified: Jul 19, 2014

***********************************************/

#include "Monster.h"
#include "Intelligence.h"

#ifndef MONSTER_H
#define MONSTER_H

// monster class
class My_Monster : public Monster{
protected:
	char target;
	bool shouldPause;
public:
	My_Monster() : Monster(){
		target = 'n';
		shouldPause = false;
	}
	// return the next movement
	virtual Movement next(Map& map){
		Movement m = monster_next(map, target, shouldPause);
		char after = map.get_cell(m.x2, m.y2);
		if(m.wall != 'n' || after == 'p' || after == 'P' || after == 'q' || after == 'Q')
			shouldPause = true;
		else
			shouldPause = false;
		return m;
	}
};

#endif
