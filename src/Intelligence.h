/***********************************************

  Intelligence.h
  Artificial Intelligence header for Corrida
  by: Ali Bibak
  last modified: Jul 20, 2014

***********************************************/

#include "Map.h"
#include "Movement.h"
#include <string>
#include <complex>
#include <algorithm>
#include <queue>
#include <cstdlib>
#include <ctime>

#ifndef INTELLIGENCE_H
#define INTELLIGENCE_H

using std::min;
using std::max;
using std::pair;
using std::make_pair;
using std::queue;
using std::complex;

// using complex for points/vectors for easier operations (like transport)
typedef complex<int> point;
#define X() real()
#define Y() imag()

// constant points/vectors for furthur use
const point steady(0, 0), up(-1, 0), left(0, -1), down = -up, right = -left;

// constant for (practical) infinity
const int inf = 100 * 1000 * 1000 + 13;

// check if the requested move is valid
bool check_validity(int, int, const Movement &, const Map &, int, int, bool);

// find a character in a map
bool findChar(const Map &, char, int &, int &);

// bfs from a cell to find the best path for monster/stars
void bfs(int, int, const Map &, bool, int [8][8], bool = false);

// bfs from a cell to find the best path for monster
void bfsMonster(int, int, const Map &, bool = false);

// bfs from a cell to find the best path for stars
void bfsStars(int, int, const Map &);

// cmp for sorting moves according to their goodness
bool cmp(const pair<pair<int, Movement>, char> &, const pair<pair<int, Movement>, char> &);

// next movement for monster
Movement monster_next(const Map &, char &, bool);

// change the map after a valid movement
void change(const Movement &, Map &);

// update server variables after a move
void update(const Movement &, const Map &, bool &, bool &, bool &, bool &, bool &, int &, int &, int &, int &);

// calculating value of a board for a player
double evaluate(bool, const Map &, bool, bool, bool, bool, int);

// calculate the best next move for a player
Movement player_next(const Map &, bool, int, int);
				
#endif
