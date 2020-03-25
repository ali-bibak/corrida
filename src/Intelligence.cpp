/***********************************************

  Intelligence.cpp
  Artificial Intelligence source for Corrida
  by: Ali Bibak
  last modified: Jul 20, 2014

***********************************************/

#include "Intelligence.h"

// bfi's for bfsMonster and bfsStars
int bfiM[8][8], bfiS[8][8];

// check if the requested move is valid
bool check_validity(int player, int ind, const Movement &m, const Map &map, int walls1, int walls2, bool monsterPause){
	int x1 = m.x1, x2 = m.x2, y1 = m.y1, y2 = m.y2;
	// wrong initial cell
	if(x1 < 0 || x1 > 7 || y1 < 0 || y1 > 7)
		return false;
	char before = map.get_cell(x1,y1), chars[5] = {'M', 'p', 'P', 'q', 'Q'};
	if(player == 0 && before != 'M')
		return false;
	else if(player == 1 && before != chars[ind])
		return false;
	else if(player == 2 && before != chars[2+ind])
		return false;
	// walking out of the grid
	if(x2 < 0 || x2 > 7 || y2 < 0 || y2 > 7)
		return false;
	// wrong wall char
	if(m.wall != 'n' && m.wall != 'U' && m.wall != 'L' && m.wall != 'D' && m.wall != 'R')
		return false;
	char after = map.get_cell(x2,y2);
	bool isMonster = before == 'M';
	point dir(x2-x1, y2-y1);
	// walking in wrong directions
	if(dir != steady && dir != up && dir != left && dir != down && dir != right)
		return false;
	// walking through a wall
	if((dir == up && map.is_h_wall(x1-1,y1))
	|| (dir == left && map.is_v_wall(x1,y1-1))
	|| (dir == down && map.is_h_wall(x1,y1))
	|| (dir == right && map.is_v_wall(x1,y1)))
		return false;
	// monster
	if(isMonster){
		// doing anything when supposed to stop
		if(monsterPause && (dir != steady || m.wall != 'n'))
			return false;
		// moving to a box
		if(after == '#')
			return false;
		// destroying a wall outside of grid or a non-existing wall
		switch(m.wall){
			case 'U':
				if(x2 == 0 || !map.is_h_wall(x2-1,y2))
					return false;
				break;
			case 'L':
				if(y2 == 0 || !map.is_v_wall(x2,y2-1))
					return false;
				break;
			case 'D':
				if(x2 == 7 || !map.is_h_wall(x2,y2))
					return false;
				break;
			case 'R':
				if(y2 == 7 || !map.is_v_wall(x2,y2))
					return false;
				break;
		}
		// otherwise
		return true;
	}
	else{
		// moving to monster
		if(after == 'M')
			return false;
		// moving to another player
		if(dir != steady && (after == 'p' || after == 'q' || after == 'P' || after == 'Q'))
			return false;
		// pushing a box outside of the grid, or to a non-empty cell or through a wall
		if(after == '#')
			if((dir == up && (x2 == 0 || (map.get_cell(x2-1,y2) != '.' && map.get_cell(x2-1,y2) != 's') || map.is_h_wall(x2-1,y2)))
			|| (dir == left && (y2 == 0 || (map.get_cell(x2,y2-1) != '.' && map.get_cell(x2,y2-1) != 's') || map.is_v_wall(x2,y2-1)))
			|| (dir == down && (x2 == 7 || (map.get_cell(x2+1,y2) != '.' && map.get_cell(x2+1,y2) != 's') || map.is_h_wall(x2,y2)))
			|| (dir == right && (y2 == 7 || (map.get_cell(x2,y2+1) != '.' && map.get_cell(x2,y2+1) != 's') || map.is_v_wall(x2,y2))))
				return false;
		// making more walls than possible
		if(m.wall != 'n')
			switch(before){
				case 'p':
				case 'P':
					if(walls1 == 0)
						return false;
					break;
				case 'q':
				case 'Q':
					if(walls2 == 0)
						return false;
					break;
			}
		// pulling a box when steady
		if(dir == steady && m.pull)
			return false;
		// pulling a non-existing box, or a box through a wall
		if(m.pull)
			if((dir == up && (x1 == 7 || map.get_cell(x1+1,y1) != '#' || map.is_h_wall(x1,y1)))
			|| (dir == left && (y1 == 7 || map.get_cell(x1,y1+1) != '#' || map.is_v_wall(x1,y1)))
			|| (dir == down && (x1 == 0 || map.get_cell(x1-1,y1) != '#' || map.is_h_wall(x1-1,y1)))
			|| (dir == right && (y1 == 0 || map.get_cell(x1,y1-1) != '#' || map.is_v_wall(x1,y1-1))))
				return false;
		// making a wall outside of grid or over an existing wall
		switch(m.wall){
			case 'U':
				if(x2 == 0 || map.is_h_wall(x2-1,y2))
					return false;
				break;
			case 'L':
				if(y2 == 0 || map.is_v_wall(x2,y2-1))
					return false;
				break;
			case 'D':
				if(x2 == 7 || map.is_h_wall(x2,y2))
					return false;
				break;
			case 'R':
				if(y2 == 7 || map.is_v_wall(x2,y2))
					return false;
				break;
		}
		// otherwise
		return true;
	}
}

// find a character in a map
bool findChar(const Map &map, char ch, int &x, int &y){
	for(int i = 0; i < 8; i++)
		for(int j = 0; j < 8; j++)
			if(map.get_cell(i,j) == ch){
				x = i;
				y = j;
				return true;
			}
	return false;
}

// bfs from a cell to find the best path for monster/stars
void bfs(int x, int y, const Map &map, bool isMonster, int bfi[8][8], bool monsterPause){
	point move[4] = {up, left, down, right};
	for(int i = 0; i < 8; i++)
		for(int j = 0; j < 8; j++)
			bfi[i][j] = inf;
	bfi[x][y] = monsterPause;
	queue<point> que;
	que.push(point(x, y));
	while(!que.empty()){
		point top = que.front();
		que.pop();
		// considering all possible directions to go
		for(int i = 0; i < 4; i++){
			point to = top + move[i], dir = -move[i];
			if(to.X() >= 0 && to.X() < 8 && to.Y() >= 0 && to.Y() < 8 && (map.get_cell(to.X(),to.Y()) == '.' || map.get_cell(to.X(),to.Y()) == 's' || (map.get_cell(to.X(),to.Y()) == 'M' && isMonster) || ((map.get_cell(to.X(),to.Y()) != '#' && map.get_cell(to.X(),to.Y()) != 'M' && !isMonster)))){
				int add = 1;
				// chekcing for breaking the walls (for monsters)
				if((dir == up && map.is_h_wall(to.X()-1,to.Y()))
				|| (dir == left && map.is_v_wall(to.X(),to.Y()-1))
				|| (dir == down && map.is_h_wall(to.X(),to.Y()))
				|| (dir == right && map.is_v_wall(to.X(),to.Y()))){
					if(isMonster)
						add++;
					else
						continue;
				}
				switch(map.get_cell(top.X(),top.Y())){
					case 'p':
					case 'P':
					case 'q':
					case 'Q':
						if(isMonster)
							add += top != point(x, y);
						else
							continue;
						break;
				}
				if(bfi[to.X()][to.Y()] > (bfi[top.X()][top.Y()] + add)){
					bfi[to.X()][to.Y()] = bfi[top.X()][top.Y()] + add;
					if(isMonster && map.get_cell(to.X(),to.Y()) == 'M')
						return;
					que.push(to);
				}
			}
		}
	}
}

// bfs from a cell to find the best path for monster
void bfsMonster(int x, int y, const Map &map, bool monsterPause){
	bfs(x, y, map, true, bfiM);
}

// bfs from a cell to find the best path for stars
void bfsStars(int x, int y, const Map &map){
	bfs(x, y, map, false, bfiS);
}

// cmp for sorting moves according to their goodness
bool cmp(const pair<pair<int, Movement>, char> &pr1, const pair<pair<int, Movement>, char> &pr2){
	return pr1.first.first < pr2.first.first;
}

// next movement for monster
Movement monster_next(const Map &map, char &target, bool monsterPause){
	int dummy;
	bool p1alive1, p1alive2, p2alive1, p2alive2;
	p1alive1 = findChar(map, 'p', dummy, dummy);
	p1alive2 = findChar(map, 'P', dummy, dummy);
	p2alive1 = findChar(map, 'q', dummy, dummy);
	p2alive2 = findChar(map, 'Q', dummy, dummy);
	pair<pair<int, Movement>, char> bestMove[4];
	int num = 0, x, y, X, Y, x1, y1, x2, y2;
	findChar(map, 'M', X, Y);
	// if it should stop
	if(monsterPause){
		Movement m(X, Y, X, Y, 'n', false);
		return m;
	}
	x1 = X;
	y1 = Y;
	char wall;
	point move[4] = {up, left, down, right};
	bool alive[4] = {p1alive1, p1alive2, p2alive1, p2alive2};
	char chars[4] = {'p', 'P', 'q', 'Q'};
	// analyze all players that are still alive
	for(int i = 0; i < 4; i++)
		if(alive[i]){
			findChar(map, chars[i], x, y);
			bfsMonster(x, y, map);
			int minimum = inf;
			// analyzing the neighbour cells
			x2 = x1;
			y2 = y1;
			for(int j = 0; j < 4; j++){
				point N = point(X, Y) + move[j];
				if(N.X() >= 0 && N.X() < 8 && N.Y() >= 0 && N.Y() < 8 && bfiM[N.X()][N.Y()] <= minimum){
					minimum = bfiM[N.X()][N.Y()];
					x2 = N.X();
					y2 = N.Y();
				}
			}
			minimum = inf;
			int nx, ny;
			char wall = 'n';
			bool stay = true;
			point dir = point(x2, y2) - point(x1, y1);
			// checking if we need to stay (because of a wall) and break it
			if(dir == up && map.is_h_wall(x1-1,y1))
				wall = 'U';
			else if(dir == left && map.is_v_wall(x1,y1-1))
				wall = 'L';
			else if(dir == down && map.is_h_wall(x1,y1))
				wall = 'D';
			else if(dir == right && map.is_v_wall(x1,y1))
				wall = 'R';
			else
				stay = false;
			if(stay){
				Movement m(x1, y1, x1, y1, wall, false);
				bestMove[num++] = make_pair(make_pair(bfiM[X][Y], m), chars[i]);
				continue;
			}
			// if we will reach the player in the next turn
			if(bfiM[x2][y2] == 0){
				Movement m(x1, y1, x2, y2, wall, false);
				bestMove[num++] = make_pair(make_pair(bfiM[X][Y], m), chars[i]);
				continue;
			}
			// analyzing the next wall to break (if any)
			for(int j = 0; j < 4; j++){
				point N = point(x2, y2) + move[j];
				if(N.X() >= 0 && N.X() < 8 && N.Y() >= 0 && N.Y() < 8 && bfiM[N.X()][N.Y()] <= minimum){
					minimum = bfiM[N.X()][N.Y()];
					nx = N.X();
					ny = N.Y();
				}
			}
			dir = point(nx, ny) - point(x2, y2);
			// checking if we need to break a wall after the move
			if(dir == up && map.is_h_wall(x2-1,y2))
				wall = 'U';
			else if(dir == left && map.is_v_wall(x2,y2-1))
				wall = 'L';
			else if(dir == down && map.is_h_wall(x2,y2))
				wall = 'D';
			else if(dir == right && map.is_v_wall(x2,y2))
				wall = 'R';
			Movement m(x1, y1, x2, y2, wall, false);
			bestMove[num++] = make_pair(make_pair(bfiM[X][Y], m), chars[i]);
		}
	// sort moves based on their goodness
	sort(bestMove, bestMove + num, cmp);
	int last = 0;
	while(last < num && bestMove[last].first.first == bestMove[0].first.first){
		// if the last target is one of the best moves
		if(target == bestMove[last].second)
			return bestMove[last].first.second;
		last++;
	}
	// randomly select one of the candidates
	srand(time(0));
	int r = rand() % last;
	// change the target
	target = bestMove[r].second;
	return bestMove[r].first.second;
}

// change the map after a valid movement
void change(const Movement &m, Map &map){
	int x1 = m.x1, y1 = m.y1, x2 = m.x2, y2 = m.y2;
	char before = map.get_cell(x1,y1), after = map.get_cell(x2,y2);
	map.set_cell(x2,y2,before);
	bool isMonster = before == 'M';
	point dir(x2-x1, y2-y1);
	// monster
	if(isMonster){
		// cleaning the previous cell
		if(dir != steady)
			map.set_cell(x1,y1,'.');
	}
	// player
	else{
		// managing the previous cell
		if(dir != steady)
			if(!m.pull)
				map.set_cell(x1,y1,'.');
			// pulling a box
			else{
				map.set_cell(x1,y1,'#');
				map.set_cell(x1-(x2-x1),y2-(y2-y1),'.');
			}
		// pushing a box
		if(after == '#')
			map.set_cell(x2+(x2-x1),y2+(y2-y1),'#');
	}
	// making/destroying a wall
	switch(m.wall){
		case 'U':
			map.set_h_wall(x2-1,y2,!isMonster);
			break;
		case 'L':
			map.set_v_wall(x2,y2-1,!isMonster);
			break;
		case 'D':
			map.set_h_wall(x2,y2,!isMonster);
			break;
		case 'R':
			map.set_v_wall(x2,y2,!isMonster);
			break;
	}
}

// update server variables after a move
void update(const Movement &m, const Map &map, bool &monsterPause, bool &p1alive1, bool &p1alive2, bool &p2alive1, bool &p2alive2, int &walls1, int &walls2, int &p1score, int &p2score){
	int x1 = m.x1, y1 = m.y1, x2 = m.x2, y2 = m.y2;
	char before = map.get_cell(x1,y1), after = map.get_cell(x2,y2);
	bool isMonster = before == 'M';
	point dir(x2-x1, y2-y1);
	// monster
	if(isMonster){
		// should pause the next time
		if(m.wall != 'n' || after == 'p' || after == 'P' || after == 'q' || after == 'Q')
			monsterPause = true;
		else
			monsterPause = false;
		// moving to a player
		switch(after){
			case 'p':
				p1alive1 = false;
				break;
			case 'P':
				p1alive2 = false;
				break;
			case 'q':
				p2alive1 = false;
				break;
			case 'Q':
				p2alive2 = false;
				break;
		}
	}
	// player
	else{
		// number of walls
		if(m.wall != 'n')
			switch(before){
				case 'p':
				case 'P':
					walls1--;
					break;
				case 'q':
				case 'Q':
					walls2--;
					break;
			}
		// getting a star
		if(after == 's')
			switch(before){
				case 'p':
				case 'P':
					p1score++;
					break;
				case 'q':
				case 'Q':
					p2score++;
					break;
			}
	}
}

// calculating value of a board for a player
double evaluate(bool color, const Map &map, bool p1alive1, bool p1alive2, bool p2alive1, bool p2alive2, int star){
	char chars[4] = {'p', 'P', 'q', 'Q'};
	bool alive[5] = {p1alive1, p1alive2, p2alive1, p2alive2};
	int c = 0;
	if(!color)
		c = 2;
	double value = 0.0;
	// considering alive pieces
	int X, Y;
	findChar(map, 'M', X, Y);
	for(int i = 0; i < 2; i++){
		int ind = c + i, x, y;
		if(alive[ind]){
			char ch = chars[ind];
			findChar(map, ch, x, y);
			bfsMonster(x, y, map);
			int numStars = 0;
			for(int j = 0; j < 8; j++)
				for(int k = 0; k < 8; k++)
					if(map.get_cell(j,k) == 's')
						numStars++;
			if(numStars == 0)
				return pow(star+1, 2) * pow(bfiM[X][Y] - 1, 2);
			// gain (player/stars situation)
			double D = 50;
			double gain = 0.0;
			for(int j = 0; j < 8; j++)
				for(int k = 0; k < 8; k++)
					if(map.get_cell(j,k) == 's'){
						bfsStars(j, k, map);
						if(numStars == 1)
							return (pow(star+1, 2) * pow(min(bfiM[X][Y] - 1, 10), 2)) / (double) pow(bfiS[x][y], 2);
						int ds2s_sum = 0;
						for(int u = 0; u < 8; u++)
							for(int v = 0; v < 8; v++)
								if(map.get_cell(u,v) == 's')
									if(bfiS[u][v] != inf)
										ds2s_sum += bfiS[u][v];
						// sparseness (star/stars situation)
						double sparseness = (double) ds2s_sum / numStars, d, thisGain;
						if(bfiS[x][y] != inf){
							d = bfiS[x][y];
							D = min(d, D);
						}
						else{
							d = 2 * (abs(x-j) + abs(y-k)) * (abs(x-j) + abs(y-k));
							D = min(d, D);
						}
						thisGain = pow(d, 3) * sparseness;
						gain += thisGain;
					}
			gain /= numStars;
			// value depending on gain, number of stars taken, and distance from monster
			value += (pow(star+1, 2)  * min(bfiM[X][Y] - 1, 10)) / (gain * pow(D, 2));
		}
	}
	return value;
}

// calculate the best next move for a player
Movement player_next(const Map &const_map, bool color, int ind, int numWalls){
	char chars[4] = {'p', 'P', 'q', 'Q'};
	int c = (1-color)*2 + (ind-1), x, y, x2, y2, X, Y, dm = inf, star;
	Map temporary_map, map;
	for(int i = 0; i < 8; i++)
		for(int j = 0; j < 8; j++)
			map.set_cell(i, j, const_map.get_cell(i, j));
	for(int i = 0; i < 7; i++)
		for(int j = 0; j < 8; j++){
			map.set_h_wall(i, j, const_map.is_h_wall(i, j));
			map.set_v_wall(j, i, const_map.is_v_wall(j, i));
		}
	findChar(map, 'M', X, Y);
	findChar(map, chars[c], x, y);
	point pts[5] = {up, left, down, right, steady};
	char walls[5] = {'U', 'L', 'D', 'R', 'n'};
	Movement mv(x, y, x, y, 'n', false);
	bfsMonster(x, y, map, false);
	dm = bfiM[X][Y];
	// if distance from monster is 1
	if(dm == 1){
		int maxi = 0;
		char andWall = 'n';
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 5; j++)
				for(int p = 0; p < 2; p++){
					x2 = x+pts[i].X(), y2 = y+pts[i].Y();
					andWall = walls[j];
					Movement m = Movement(x, y, x2, y2, andWall, p);
					temporary_map.set(map);
					if(check_validity(!color+1, ind, m, temporary_map, numWalls, numWalls, false)){
						change(m, temporary_map);
						bfsMonster(x2, y2, temporary_map);
						int newD = bfiM[X][Y];
						if(newD == inf)
							newD = 10 * (abs(x2-X) + abs(y2-Y));
						if(newD > maxi){
							maxi = newD;
							mv = m;
						}
				}
			}
		if(maxi > 2)
			return mv;
		if((x != 7 && map.get_cell(x+1,y) == 'M') || (x != 0 && map.get_cell(x-1,y) == 'M')){
			mv = Movement(x, y, x+1, y, 'U', false);
			if(check_validity(!color+1, ind, mv, map, numWalls, numWalls, false))
				return mv;
			mv = Movement(x, y, x+1, y, 'n', false);
			if(check_validity(!color+1, ind, mv, map, numWalls, numWalls, false))
				return mv;
			mv = Movement(x, y, x-1, y, 'D', false);
			if(check_validity(!color+1, ind, mv, map, numWalls, numWalls, false))
				return mv;
			mv = Movement(x, y, x-1, y, 'n', false);
			if(check_validity(!color+1, ind, mv, map, numWalls, numWalls, false))
				return mv;
			mv = Movement(x, y, x, y+1, 'n', false);
			if(check_validity(!color+1, ind, mv, map, numWalls, numWalls, false))
				return mv;
			mv = Movement(x, y, x, y-1, 'n', false);
			if(check_validity(!color+1, ind, mv, map, numWalls, numWalls, false))
				return mv;
			if(x != 7 && map.get_cell(x+1,y) == 'M'){
				mv = Movement(x, y, x, y, 'D', false);
				if(check_validity(!color+1, ind, mv, map, numWalls, numWalls, false))
					return mv;
			}
			else{
				mv = Movement(x, y, x, y, 'U', false);
				if(check_validity(!color+1, ind, mv, map, numWalls, numWalls, false))
					return mv;
			}
		}
		if((y != 7 && map.get_cell(x,y+1) == 'M') || (y != 0 && map.get_cell(x,y-1) == 'M')){
			mv = Movement(x, y, x, y+1, 'L', false);
			if(check_validity(!color+1, ind, mv, map, numWalls, numWalls, false))
				return mv;
			mv = Movement(x, y, x, y+1, 'n', false);
			if(check_validity(!color+1, ind, mv, map, numWalls, numWalls, false))
				return mv;
			mv = Movement(x, y, x, y-1, 'R', false);
			if(check_validity(!color+1, ind, mv, map, numWalls, numWalls, false))
				return mv;
			mv = Movement(x, y, x, y-1, 'n', false);
			if(check_validity(!color+1, ind, mv, map, numWalls, numWalls, false))
				return mv;
			mv = Movement(x, y, x+1, y, 'n', false);
			if(check_validity(!color+1, ind, mv, map, numWalls, numWalls, false))
				return mv;
			mv = Movement(x, y, x-1, y, 'n', false);
			if(check_validity(!color+1, ind, mv, map, numWalls, numWalls, false))
				return mv;
			if(y != 7 && map.get_cell(x,y+1) == 'M'){
				mv = Movement(x, y, x, y, 'R', false);
				if(check_validity(!color+1, ind, mv, map, numWalls, numWalls, false))
					return mv;
			}
			else{
				mv = Movement(x, y, x, y, 'L', false);
				if(check_validity(!color+1, ind, mv, map, numWalls, numWalls, false))
					return mv;
			}
		}
	}
	double maximum = -1, fmaximum = -1;
	int dummy;
	bool p1alive1, p1alive2, p2alive1, p2alive2;
	p1alive1 = findChar(map, 'p', dummy, dummy);
	p1alive2 = findChar(map, 'P', dummy, dummy);
	p2alive1 = findChar(map, 'q', dummy, dummy);
	p2alive2 = findChar(map, 'Q', dummy, dummy);
	Map best;
	bool gotStar = false;
	// calculating the best next move based on evaluation in evaluate()
	for(int i = 0; i < 5; i++)
		for(int p = 0; p < 2; p++){
			x2 = x + pts[i].X();
			y2 = y + pts[i].Y();
			Movement m(x, y, x2, y2, 'n', p);
			temporary_map.set(map);
			if(check_validity(!color+1, ind, m, temporary_map, numWalls, numWalls, false)){
				star = temporary_map.get_cell(m.x2,m.y2) == 's';
				change(m, temporary_map);
				bfsMonster(x, y, temporary_map, false);
				bool nowGotStar = false, inDanger = bfiM[X][Y] <= 2;
				if(star)
					gotStar = nowGotStar = true;
				double value = evaluate(color, temporary_map, p1alive1, p1alive2, p2alive1, p2alive2, star);
				if((!inDanger) && ((gotStar && nowGotStar && value > fmaximum) || (!gotStar && value > maximum))){
					if(gotStar)
						fmaximum = value;
					else
						maximum = value;
					mv = m;
				}
			}
		}
	if(!check_validity(!color+1, ind, mv, map, numWalls, numWalls, false))
		mv = Movement(x, y, x, y, 'n', false);
	Movement save = mv;
	x2 = mv.x2, y2 = mv.y2;
	char andWall = mv.wall;
	bfsMonster(x2, y2, map, false);
	dm = bfiM[X][Y];
	if(bfiM[X][Y] == inf)
		dm = 10 * (abs(X-x2) + abs(Y-y2));
	// if distance from monster will be 1
	if(dm == 1){
		int maxi = 0;
		char andWall = 'n';
		for(int i = 0; i < 5; i++)
			for(int j = 4; j >= 0; j--)
				for(int p = 0; p < 2; p++){
					x2 = x+pts[i].X(), y2 = y+pts[i].Y();
					andWall = walls[j];
					Movement m = Movement(x, y, x2, y2, andWall, p);
					temporary_map.set(map);
					if(check_validity(!color+1, ind, m, temporary_map, numWalls, numWalls, false)){
						change(m, temporary_map);
						bfsMonster(x2, y2, temporary_map);
						int newD = bfiM[X][Y];
						if(newD == inf)
							newD = 10 * (abs(x2-X) + abs(y2-Y));
						if(newD > maxi){
							maxi = newD;
							mv = m;
						}
				}
			}
		if((maxi > 1) || (maxi >= 1 && andWall == 'n'))
			return mv;
		andWall = 'n';
		if(x2 != 0 && map.get_cell(x2-1,y2) == 'M' && !map.is_v_wall(x2-1,y2) && numWalls)
			andWall = 'U';
		else if(x2 != 7 && map.get_cell(x2+1,y2) == 'M' && !map.is_v_wall(x2,y2) && numWalls)
			andWall = 'D';
		else if(y2 != 0 && map.get_cell(x2,y2-1) == 'M' && !map.is_h_wall(x2,y2-1) && numWalls)
			andWall = 'L';
		else if(y2 != 7 && map.get_cell(x2,y2+1) == 'M' && !map.is_h_wall(x2,y2) && numWalls)
			andWall = 'R';
		mv = Movement(x, y, x2, y2, andWall, false);
		return mv;
	}
	// if distance from monster will be 2
	else if(dm == 2){
		int maxi = 0;
		for(int i = 0; i < 5; i++)
			for(int p = 0; p < 2; p++){
				x2 = x+pts[i].X(), y2 = y+pts[i].Y();
				Movement m = Movement(x, y, x2, y2, 'n', p);
				temporary_map.set(map);
				if(check_validity(!color+1, ind, m, temporary_map, numWalls, numWalls, false)){
					change(m, temporary_map);
					bfsMonster(x2, y2, temporary_map);
					int newD = bfiM[X][Y];
					if(newD == inf)
						newD = 10 * (abs(x2-X) + abs(y2-Y));
					if(newD > maxi){
						maxi = newD;
						mv = m;
					}
			}
		}
		if(maxi > 2 || (maxi >= 2 && mv.wall == 'n'))
			return mv;
		x2 = mv.x2;
		y2 = mv.y2;
		mv = Movement(x, y, x, y, 'n', false);
		if(x2 == X){
			if(Y < y2 && !map.is_h_wall(x2,y2-1) && numWalls)
				andWall = 'L';
			else if(Y > y2 && !map.is_h_wall(x2,y2) && numWalls)
				andWall = 'R';
			else
				andWall = 'n';
		}
		else if(y2 == Y){
			if(X < x2 && !map.is_v_wall(x2-1,y2) && numWalls)
				andWall = 'U';
			else if(X > x2 && !map.is_v_wall(x2,y2) && numWalls)
				andWall = 'D';
			else
				andWall = 'n';
		}
		else
			andWall = 'n';
	}
	// if distance from monster will be 3
	else if(dm == 3 && x == x2 && y == y2){
		int maxi = 0;
		for(int i = 0; i < 5; i++)
			for(int p = 0; p < 2; p++){
				x2 = x+pts[i].X(), y2 = y+pts[i].Y();
				Movement m = Movement(x, y, x2, y2, 'n', p);
				temporary_map.set(map);
				if(check_validity(!color+1, ind, m, temporary_map, numWalls, numWalls, false)){
					change(m, temporary_map);
					bfsMonster(x2, y2, temporary_map);
					int newD = bfiM[X][Y];
					if(newD == inf)
						newD = 10 * (abs(x2-X) + abs(y2-Y));
					if(newD > maxi){
						maxi = newD;
						mv = m;
					}
			}
		}
		return mv;
	}
	// not moving or making any walls far from monster
	else if(x == x2 && y == y2 && !andWall){
		int minimumD = inf;
		// go to the nearest star
		for(int i = 0; i < 8; i++)
			for(int j = 0; j < 8; j++)
				if(map.get_cell(i,j) == 's'){
					bfsStars(i, j, map);
					if(bfiS[x][y] < minimumD){
						minimumD = bfiS[x][y];
						int minimum = inf;
						Movement m;
						for(int k = 0; k < 4; k++){
							m = Movement(x, y, x+pts[k].X(), y+pts[k].Y(), 'n', false);
							if(check_validity(!color+1, ind, m, map, numWalls, numWalls, 0) && bfiS[x+pts[k].X()][y+pts[k].Y()] < minimum){
								minimum = bfiS[x+pts[k].X()][y+pts[k].Y()];
								mv = m;
							}
						}
					}
				}
		return mv;
	}
	if(gotStar)
		return save;
	mv = Movement(x, y, mv.x2, mv.y2, andWall, mv.pull);
	return mv;
}
