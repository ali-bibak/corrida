/***********************************************

  server.cpp
  main source code for Corrida
  by: Ali Bibak
  last modified: Jul 20, 2014

***********************************************/

#define FATAL_ERROR_LOG 1	// fatal error logs shown in terminal
#define ERROR_LOG 1			// error logs wriiten in 'log.txt'

#include "My_Graphic.h"
#include "My_Player.h"
#include "My_Monster.h"
#include <fstream>
#include <iostream>

using std::cin;
using std::cerr;
using std::endl;
using std::string;
using std::ifstream;
using std::ofstream;
using std::ios;

ofstream flog("../log/log.txt", ios::out);

// informations
bool p1alive1 = true, p1alive2 = true, p2alive1 = true, p2alive2 = true, monsterPause = false;
int p1w = 10, p2w = 10, numStage = 0, p1score = 0, p2score = 0;

// security temp objects
Map secMap;
Movement secMove;

// used functions
bool p1alive();
bool p2alive();
bool valid(const Movement &, const Map &);
void next_stage(Player *, Player *, Monster *, Graphic *, Map &);

int main(){
	ifstream fin, finLevel;
	Graphic* graphic = new My_Graphic();
	Player* player1 = new My_Player(true);
	Player* player2 = new My_Player(false);
	Monster* monster = new My_Monster();
	Map map;
	int choice = 0;
	string level_path;
	// dynamic casting for level choice - since it defers from graphic to graphic
	My_Graphic* mg = dynamic_cast<My_Graphic*>(graphic);
	if(mg)
		mg->defaultServer();
	// menu
	graphic->menu();
	if(mg)
		choice = mg->getLevel();
	if(!graphic->is_open())
		return 0;
	// addressing the input
	if(choice == 0)
		fin.open("../map/map.txt", ios::in);
	else{
		stringstream ss;
		ss << choice;
		string choiceS;
		ss >> choiceS;
		level_path = "../data/levels/map" + choiceS + ".txt";
		finLevel.open(level_path.c_str(), ios::in);
	}
	// existence of map
	if((choice == 0 && !fin) || (choice != 0 && !finLevel)){
		#ifdef FATAL_ERROR_LOG
			cerr << "fatal error: map could not be opened.\nterminating." << endl;
		#endif
		#ifdef ERROR_LOG
			flog << "fatal error: map could not be opened.\nterminating." << endl;
		#endif
		exit(1);
	}
	// loading the map and checking its validity
	char ch;
	string chars("pPqQ.M#s");
	int cnt[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	for(int i = 0; i < 8; i++)
		for(int j = 0; j < 8; j++){
			if(choice == 0)
				fin >> ch;
			else
				finLevel >> ch;
			if(chars.find(ch) == -1){
				#ifdef FATAL_ERROR_LOG
					cerr << "fatal error: invalid map.\nterminating." << endl;
				#endif
				#ifdef ERROR_LOG
					flog << "fatal error: invalid map.\nterminating." << endl;
				#endif
				exit(1);
			}
			cnt[chars.find(ch)]++;
			map.set_cell(i, j, ch);
		}
	if(cnt[0] != 1 || cnt[1] != 1 || cnt[2] != 1 || cnt[3] != 1 || cnt[5] != 1){
		#ifdef ERROR_LOG
			cerr << "fatal error: invalid map.\nterminating." << endl;
		#endif
		#ifdef ERROR_LOG
			flog << "fatal error: invalid map.\nterminating." << endl;
		#endif
		exit(1);
	}
	secMap.set(map);
	graphic->set_map(secMap);
	graphic->repaint();
	// running the game
	const int stages = 40;
	while(numStage < stages && graphic->is_open()){
		next_stage(player1, player2, monster, graphic, map);
		// ending of the game
		if(!p1alive() || !p2alive())
			break;
	}
	if(!graphic->is_open())
		return 0;
	// choosing the winner
	int winner;
	if(!p2alive())
		winner = 1;
	else if(!p1alive())
		winner = 2;
	else if(p1score > p2score)
		winner = 1;
	else if(p2score > p1score)
		winner = 2;
	else if(p1alive1 + p1alive2 > p2alive1 + p2alive2)
		winner = 1;
	else if(p2alive1 + p2alive2 > p1alive1 + p1alive2)
		winner = 2;
	else
		winner = 0;
	// game over
	graphic->game_over(winner);
	return 0;
}

bool p1alive(){
	return p1alive1 || p1alive2;
}
bool p2alive(){
	return p2alive1 || p2alive2;
}

// check if the requested movement is valid
bool valid(const Movement &m, const Map &map, int player, int ind){
	return check_validity(player, ind, m, map, p1w, p2w, monsterPause);
}

// play the next stage of the game
void next_stage(Player *p1, Player *p2, Monster *monster, Graphic *graphic, Map &map){
	bool alive[4] = {p1alive1, p1alive2, p2alive1, p2alive2};
	char chars[4] = {'p', 'P', 'q', 'Q'};
	// players' movement
	for(int i = 0; i < 2; i++)
		for(int j = 0; j < 2; j++)
			if(alive[i*2+j]){
				char ch = chars[i*2+j];
				Movement m;
				secMap.set(map);
				if(i == 0)
					m = p1->next(secMap, j+1);
				else
					m = p2->next(secMap, j+1);
				// validity
				if(!valid(m, map, i+1, j+1)){
					int x, y;
					#ifdef ERROR_LOG
						flog << "player " << i+1 << " requested an invalid movement: " << endl
							 << "x1=" << m.x1 << " y1=" << m.y1 << " x2=" << m.x2 << " y2=" << m.y2 << " wall=" << m.wall << " pull" << m.pull << endl
							 << "in the map: " << endl;
						for(int k = 0; k < 8; k++, flog << endl)
							for(int l = 0; l < 8; l++)
								flog << map.get_cell(k, l) << " ";
						flog << "when walls=" << (i == 0 ? p1w : p2w) << endl << endl;
					#endif
					findChar(map, ch, x, y);
					m.set(x, y, x, y, 'n', false);
				}
				// updating, moving and repainting the movement
				update(m, map, monsterPause, p1alive1, p1alive2, p2alive1, p2alive2, p1w, p2w, p1score, p2score);
				secMove = m;
				graphic->move(secMove);
				if(!graphic->is_open())
					return;
				change(m, map);
				secMap.set(map);
				graphic->set_map(secMap);
				graphic->set_score(p1score, p2score);
				graphic->repaint();
			}
	// monster's movement
	secMap.set(map);
	Movement m = monster->next(secMap);
	// validity
	if(!valid(m, map, 0, 0)){
		int x, y;
		#ifdef ERROR_LOG
			flog << "monster requested an invalid movement: " << endl
				 << "x1=" << m.x1 << " y1=" << m.y1 << " x2=" << m.x2 << " y2=" << m.y2 << " wall=" << m.wall << " pull" << m.pull << endl
				 << "in the map: " << endl;
			for(int i = 0; i < 8; i++, flog << endl)
				for(int j = 0; j < 8; j++)
					flog << map.get_cell(i, j) << " ";
			if(monsterPause)
				flog << "when it had to stop for a turn." << endl << endl;
		#endif
		findChar(map, 'M', x, y);
		m.set(x, y, x, y, 'n', false);
	}
	// updating, moving and repainting the movement
	update(m, map, monsterPause, p1alive1, p1alive2, p2alive1, p2alive2, p1w, p2w, p1score, p2score);
	secMove = m;
	graphic->move(secMove);
	if(!graphic->is_open())
		return;
	change(m, map);
	numStage++;
	graphic->set_time(numStage);
	secMap.set(map);
	graphic->set_map(secMap);
	graphic->repaint();
}
