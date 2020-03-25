/***********************************************

  My_Graphic.h
  Graphic header for Corrida
  by: Ali Bibak
  last modified: Jul 20, 2014

***********************************************/

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "Graphic.h"
#include <sstream>
#include <cstring>

#ifndef MY_GRAPHIC_H
#define MY_GRAPHIC_H

using std::string;
using std::stringstream;

sf::RenderWindow App(sf::VideoMode(600, 700, 64), "Corrida!", sf::Style::Close);
sf::Clock myClock;
sf::Color bgColor = sf::Color(0, 0, 0), myRed(204, 0, 0);

double TimeToWait = 2;
bool southPark = false, gotNames = false, defServer = false, isOpen = true;
int walls1 = 10, walls2 = 10, pieces1 = 2, pieces2 = 2, player1Ind = 0, player2Ind = 1, monsterInd = 0, boxInd = 0, starInd = 0;

// graphical works of the program
class My_Graphic : public Graphic{
public:
	// set players' names
	virtual void set_name(string s1, string s2){
		player1_name = s1;
		player2_name = s2;
	}
	// set players' score
	virtual void set_score(unsigned int s1, unsigned int s2){
		player1_score = s1;
		player2_score = s2;
	}
	// set map
	virtual void set_map(Map &m){
		map.set(m);
	}
	// set time
	virtual void set_time(unsigned int t){
		if(time >= 0)
			time = t;
		else
			time = 0;
	}
	// paint the current state based on attributes
	virtual void repaint(){
		App.Clear(bgColor);
		// south park
		if(southPark){
			sf::Image bg;
			bg.LoadFromFile("../data/images/other/sp_background.jpg");
			sf::Sprite BG;
			BG.SetImage(bg);
			BG.Resize(600, 700);
			BG.SetPosition(0, 0);
			App.Draw(BG);
		}
		// initializing for drawing the board
		sf::Image board;
		board.LoadFromFile("../data/images/other/square.jpg");
		sf::Sprite Board[8][8];
		sf::Sprite Content[8][8];
		const int boardX = 70, boardY = 150, scoreX = boardX, scoreY = 75;
		stringstream ss;
		string path;
		sf::Image p1;
		string p1S;
		ss.clear();
		ss << player1Ind+1;
		ss >> p1S;
		path = string("../data/images/player/player") + p1S + ".jpg";
		if(southPark)
			path = string("../data/images/player/sp_player1_1.jpg");
		p1.LoadFromFile(path.c_str());
		sf::Sprite P1;
		P1.SetImage(p1);
		P1.SetPosition(scoreX, scoreY);
		App.Draw(P1);
		sf::Image p2;
		string p2S;
		ss.clear();
		ss << player2Ind+1;
		ss >> p2S;
		path = string("../data/images/player/player") + p2S + ".jpg";
		if(southPark)
			path = string("../data/images/player/sp_player2_1.jpg");
		p2.LoadFromFile(path.c_str());
		sf::Sprite P2;
		P2.SetImage(p2);
		P2.SetPosition(scoreX + 8*57 - 55, scoreY);
		App.Draw(P2);
		sf::Shape Rect1 = sf::Shape::Rectangle(scoreX + 55, scoreY + 1, scoreX + 55 + 115, scoreY + 55, sf::Color::White);
		App.Draw(Rect1);
		sf::Shape Rect2 = sf::Shape::Rectangle(scoreX + 57*8 - 55 + 1, scoreY + 1, scoreX + 57*8 - 55 - 115, scoreY + 55, sf::Color::White);
		App.Draw(Rect2);
		string c1, c2, sc1, sc2, w1, w2, pc1, pc2;
		ss.clear();
		ss << player1_score;
		ss >> sc1;
		ss.clear();
		ss << player2_score;
		ss >> sc2;
		ss.clear();
		ss << walls1;
		ss >> w1;
		ss.clear();
		ss << walls2;
		ss >> w2;
		ss.clear();
		ss << pieces1;
		ss >> pc1;
		ss.clear();
		ss << pieces2;
		ss >> pc2;
		c1 = string("score: ") + sc1 + "\nwalls: " + w1 + "\npieces: " + pc1;
		c2 = string("score: ") + sc2 + "\nwalls: " + w2 + "\npieces: " + pc2;
		sf::String C1, C2;
		C1.SetText(c1);
		C1.SetSize(15);
		C1.SetPosition(scoreX + 55, scoreY + 1);
		C1.SetColor(sf::Color::Black);
		C2.SetText(c2);
		C2.SetSize(15);
		C2.SetPosition(scoreX + 57*8 - 165, scoreY + 1);
		C2.SetColor(sf::Color::Black);
		App.Draw(C1);
		App.Draw(C2);
		// drawing the board
		for(int i = 0; i < 8; i++)
			for(int j = 0; j < 8; j++){
				Board[i][j].SetImage(board);
				Board[i][j].SetPosition(boardX + 57*j, boardY + 57*i);
				App.Draw(Board[i][j]);
				if(map.get_cell(i, j) != '.'){
					sf::Image content;
					string path, S;
					stringstream ss;
					// drawing the pieces
					switch(map.get_cell(i, j)){
						case 'p':
							if(southPark){
								content.LoadFromFile("../data/images/player/sp_player1_1.jpg");
								break;
							}
						case 'P':
							if(southPark){
								content.LoadFromFile("../data/images/player/sp_player1_2.jpg");
								break;
							}
							ss.clear();
							ss << player1Ind+1;
							ss >> S;
							path = string("../data/images/player/player") + S + ".jpg";
							content.LoadFromFile(path.c_str());
							break;
						case 'q':
							if(southPark){
								content.LoadFromFile("../data/images/player/sp_player2_1.jpg");
								break;
							}
						case 'Q':
							if(southPark){
								content.LoadFromFile("../data/images/player/sp_player2_2.jpg");
								break;
							}
							ss.clear();
							ss << player2Ind+1;
							ss >> S;
							path = string("../data/images/player/player") + S + ".jpg";
							content.LoadFromFile(path.c_str());
							break;
						case 'M':
							if(southPark){
								content.LoadFromFile("../data/images/monster/sp_monster.jpg");
								break;
							}
							ss.clear();
							ss << monsterInd+1;
							ss >> S;
							path = string("../data/images/monster/monster") + S + ".jpg";
							content.LoadFromFile(path.c_str());
							break;
						case 's':
							if(southPark){
								content.LoadFromFile("../data/images/star/sp_star.jpg");
								break;
							}
							ss.clear();
							ss << starInd+1;
							ss >> S;
							path = string("../data/images/star/star") + S + ".jpg";
							content.LoadFromFile(path.c_str());
							break;
						case '#':
							if(southPark){
								content.LoadFromFile("../data/images/box/sp_box.jpg");
								break;
							}
							ss.clear();
							ss << boxInd+1;
							ss >> S;
							path = string("../data/images/box/box") + S + ".jpg";
							content.LoadFromFile(path);
							break;
					}
					Content[i][j].SetImage(content);
					Content[i][j].SetPosition(boardX + 57*j + 1, boardY + 57*i + 1);
					App.Draw(Content[i][j]);
				}
			}
		// drawing the walls
		for(int i = 0; i < 7; i++)
			for(int j = 0; j < 8; j++){
				if(map.is_h_wall(i, j)){
					sf::Shape Line = sf::Shape::Line(boardX + 57*j, boardY + 57*i + 57, boardX + 57*j + 57, boardY + 57*i + 57, 4, sf::Color::Black);
					App.Draw(Line);
				}
				if(map.is_v_wall(j, i)){
					sf::Shape Line = sf::Shape::Line(boardX + 57*i + 57, boardY + 57*j, boardX + 57*i + 57, boardY + 57*j + 57, 4, sf::Color::Black);
					App.Draw(Line);
				}
			}
		// time left
		sf::Shape Rect = sf::Shape::Rectangle(scoreX + 200, scoreY + 1, scoreX + 200 + 55, scoreY + 55, sf::Color::White);
		App.Draw(Rect);
		int left = 40 - time;
		int timeX = 280, timeY = 78;
		string leftStr;
		if(left > 0)
			switch(left/10){
				case 4:
					timeX -= 2;
				case 3:
				case 2:
					timeX++;
					break;
				case 1:
					timeX += 5;
					break;
				case 0:
					timeX--;
			}
		if(left >= 0){
			ss.clear();
			ss << left;
			ss >> leftStr;
		}
		if(left < 10)
			leftStr = string("0") + leftStr;
		sf::String Time;
		sf::Font font;
		font.LoadFromFile("../data/fonts/display.ttf");
		Time.SetText(leftStr);
		if(left < 0){
			leftStr = "--";
			timeX += 5;
		}
		Time.SetColor(myRed);
		Time.SetPosition(timeX, timeY);
		Time.SetSize(40);
		Time.SetFont(font);
		App.Draw(Time);
		App.Display();
	}
	// move
	virtual void move(Movement &m){
		char before = map.get_cell(m.x1, m.y1), after = map.get_cell(m.x2, m.y2);
		string str;
		switch(before){
			case 'M':
				str = "Monster's turn to move";
				switch(after){
					case 'p':
					case 'P':
						pieces1--;
						break;
					case 'q':
					case 'Q':
						pieces2--;
						break;
				}
				break;
			case 'p':
			case 'P':
				str = string("First player's turn to move its ") + (before == 'p' ? "first" : "second") + " piece";
				if(m.wall != 'n')
					walls1--;
				break;
			case 'q':
			case 'Q':
				str = string("Second player's turn to move its ") + (before == 'q' ? "first" : "second") + " piece";
				if(m.wall != 'n')
					walls2--;
				break;
		}
		sf::Shape Rect = sf::Shape::Rectangle(0, 620, 600, 620 + 50, sf::Color::White);
		App.Draw(Rect);
		sf::String move;
		sf::Font font;
		move.SetText(str);
		font.LoadFromFile("../data/fonts/code.otf");
		move.SetFont(font);
		move.SetSize(18);
		move.SetColor(myRed);
		move.SetPosition(70, 633);
		App.Draw(move);
		App.Display();
		sf::Event event;
		myClock.Reset();
		while(myClock.GetElapsedTime() < TimeToWait)
			if(App.GetEvent(event) && event.Type == sf::Event::Closed){
				App.Close();
				isOpen = false;
				return;
			}
	}
	// check if window is open
	virtual bool is_open(){
		return isOpen;
	}
	// menu
	virtual void menu(){
		sf::Font font;
		sf::Event event;
		// initializing title logo values
		int titleX = 147, titleY = 20;
		sf::String Title;
		sf::Font TitleFont, AliFont;
		TitleFont.LoadFromFile("../data/fonts/tetri.ttf");
		Title.SetText("CORRIDA");
		Title.SetFont(TitleFont);
		Title.SetSize(70);
		Title.SetPosition(titleX, titleY);
		Title.SetColor(myRed);
		App.Draw(Title);
		AliFont.LoadFromFile("../data/fonts/datacontrol.ttf");
		sf::String Ali;
		Ali.SetText("by Ali Bibak");
		Ali.SetFont(AliFont);
		Ali.SetStyle(sf::String::Bold);
		Ali.SetSize(30);
		Ali.SetPosition(titleX+64, titleY+85);
		sf::Shape Rect = sf::Shape::Rectangle(0, 100, 600, 150, sf::Color(204, 0, 0));
		while(true){
			App.Clear(bgColor);
			// south park
			if(southPark){
				sf::Image bg;
				bg.LoadFromFile("../data/images/other/sp_background.jpg");
				sf::Sprite BG;
				BG.SetImage(bg);
				BG.Resize(600, 700);
				BG.SetPosition(0, 0);
				App.Draw(BG);
			}
			// showing the title
			App.Draw(Title);
			App.Draw(Rect);
			App.Draw(Ali);
			// getting the names (if applicable)
			if(!gotNames){
				const int textX = 10, textY = 620;
				sf::String Text1, Text2;
				// getting the first player's name
				font.LoadFromFile("../data/fonts/dearjoe.ttf");
				Text1.SetText("Enter the name of the first player");
				Text1.SetFont(font);
				Text1.SetSize(25);
				Text1.SetPosition(textX, textY);
				App.Draw(Text1);
				App.Display();
				string str1;
				sf::String text1;
				text1.SetColor(myRed);
				text1.SetFont(font);
				text1.SetPosition(textX + 390, textY);
				App.EnableKeyRepeat(false);
				char prev, next;
				bool up = true;
				while(true)
					if(App.GetEvent(event)){
						if(event.Type == sf::Event::Closed){
    		    			App.Close();
							isOpen = false;
							return;
						}
						if(event.Type == sf::Event::TextEntered){
    						if(event.Text.Unicode < 128){
								next = static_cast<char>(event.Text.Unicode);
								if(next == 13)
									break;
    		   					if(up || next != prev){
									str1 += static_cast<char>(event.Text.Unicode);
    		    					text1.SetText(str1);
								}
								prev = next;
							}
							up = false;
						}
						if(event.Type == sf::Event::KeyReleased)
							up = true;
					}
				App.Clear(bgColor);
				App.Draw(Text1);
				App.Draw(text1);
				player1_name = str1;
				// getting the second player's name
				font.LoadFromFile("../data/fonts/dearjoe.ttf");
				Text2.SetText("Enter the name of the second player");
				Text2.SetFont(font);
				Text2.SetSize(25);
				Text2.SetPosition(textX, textY + 35);
				App.Draw(Text2);
				App.Draw(Title);
				App.Draw(Rect);
				App.Draw(Ali);
				App.Display();
				string str2;
				sf::String text2;
				text2.SetColor(myRed);
				text2.SetFont(font);
				text2.SetPosition(textX + 410, textY + 35);
				up = true;
				bool first = true;
				while(true)
					if(App.GetEvent(event)){
						if(event.Type == sf::Event::Closed){
    			    		App.Close();
							isOpen = false;
							return;
						}
						if(event.Type == sf::Event::TextEntered){
    						if(event.Text.Unicode < 128){
								next = static_cast<char>(event.Text.Unicode);
								if(next == 13)
									if(first)
										continue;
									else
										break;
    		   					if(up || next != prev){
									str2 += static_cast<char>(event.Text.Unicode);
    			    				text2.SetText(str2);
								}
								prev = next;
							}
							up = false;
							first = false;
						}
						if(event.Type == sf::Event::KeyReleased){
							up = true;
							first = false;
						}
					}
				player2_name = str2;
				App.Clear(bgColor);
				App.Draw(Title);
				App.Draw(Rect);
				App.Draw(Ali);
				App.Draw(Text1);
				App.Draw(text1);
				App.Draw(Text2);
				App.Draw(text2);
			}
			gotNames = true;
			// show background color choices
			const int numColors = 7, bgX = 50, bgY = 520;
			sf::Color bgColors[numColors] = {sf::Color::Black, sf::Color(50, 50, 50), sf::Color(0, 50, 0), sf::Color(50, 0, 50), sf::Color(50, 50, 0), sf::Color(20, 10, 0), sf::Color(0, 0, 50)};
			if(!southPark){
				for(int i = 0; i < numColors; i++){
					sf::Shape Rect = sf::Shape::Rectangle(bgX + i*75, bgY, bgX + i*75 + 50, bgY + 50, bgColors[i], 2, sf::Color::White);
					App.Draw(Rect);
				}
			}
			// showing menu buttons
			const int numItems = 4, buttonsX = 200, buttonsY = 200 + southPark * 100;
			string menuStrings[numItems] = {"new_game", string("levels") + (!defServer ? "_off" : ""), "customize", "quit"};
			sf::Sprite menuButtons[numItems];
			for(int i = 0; i < numItems; i++){
				sf::Image image;
				string path = string("../data/images/menu/") + string(southPark ? "sp_" : "") + menuStrings[i] + ".jpeg";
				image.LoadFromFile(path.c_str());
				menuButtons[i].SetImage(image);
				menuButtons[i].SetPosition(buttonsX, buttonsY + 75 * i);
				App.Draw(menuButtons[i]);
			}
			App.Display();
			// choosing a menu option
			const sf::Input& Input = App.GetInput();
			if(Input.IsMouseButtonDown(sf::Mouse::Left))
				while(true)
					if(App.GetEvent(event) && event.Type == sf::Event::MouseButtonReleased)
						break;
			while(true)
				if(App.GetEvent(event)){
					if(event.Type == sf::Event::Closed){
    	    			App.Close();
						isOpen = false;
						return;
					}
					if(event.Type == sf::Event::MouseButtonPressed && Input.IsMouseButtonDown(sf::Mouse::Left)){
						int x = Input.GetMouseX(), y = Input.GetMouseY(), choice = -1;
						// menu buttons
						if(buttonsX <= x && x <= buttonsX + 200){
							for(int i = 0; i < numItems; i++)
								if(buttonsY + i*75 <= y && y <= buttonsY + i*75 + 50){
									choice = i;
									break;
								}
						}
						// background color choices
						if(!southPark && bgY <= y && y <= bgY + 50){
							for(int i = 0; i < numColors; i++)
								if(bgX + i*75 <= x && x <= bgX + i*75 + 50){
									choice = 10 + i;
									break;
								}
						}
						if(choice >= 10){
							int ind = choice - 10;
							bgColor = bgColors[ind];
							break;
						}
						bool restartMenu = false;
						int ret;
						switch(choice){
							// new game
							case 0:
								return;
							// levels
							case 1:
								if(!defServer)
									break;
								levels();
								if(!isOpen)
									return;
								restartMenu = true;
								break;
							// customize
							case 2:
								customize();
								if(!isOpen)
									return;
								restartMenu = true;
								break;
							// quit
							case 3:
								App.Close();
								isOpen = false;
								return;
						}
						if(restartMenu)
							break;
					}
				}
		}
	}
	// choose a level
	void levels(){
		while(true){
			const int levelsX = 100, levelsY = 50;
			App.Clear(bgColor);
			// south park
			if(southPark){
				sf::Image bg;
				bg.LoadFromFile("../data/images/other/sp_background.jpg");
				sf::Sprite BG;
				BG.SetImage(bg);
				BG.Resize(600, 700);
				BG.SetPosition(0, 0);
				App.Draw(BG);
			}
			// showing levels
			for(int i = 0; i < 4; i++)
				for(int j = 0; j < 3; j++){
					int ind = i*3 + j;
					sf::Image image;
					stringstream ss;
					ss << ind+1;
					string indS;
					ss >> indS;
					string path = string("../data/images/level/level") + indS + ".jpg";
					image.LoadFromFile(path);
					sf::Sprite Image;
					Image.SetImage(image);
					Image.Resize(100, 100);
					Image.SetPosition(levelsX + 150 * j, levelsY + 150 * i);
					App.Draw(Image);
				}
			for(int i = 0; i < 4; i++)
				for(int j = 0; j < 3; j++){
					int ind = i*3 + j;
					if(level == ind + 1){
						sf::Image image;
						image.LoadFromFile("../data/images/other/black_border.png");
						sf::Sprite Image;
						Image.SetImage(image);
						Image.Resize(100, 100);
						Image.SetPosition(levelsX + 150*j, levelsY + 150*i);
						App.Draw(Image);
						break;
					}
				}
			// showing buttons
			sf::Image image;
			string path = string("../data/images/menu/") + (southPark ? "sp_" : "") + "submit.jpeg";
			image.LoadFromFile(path.c_str());
			sf::Sprite Image1;
			Image1.SetImage(image);
			Image1.SetPosition(levelsX - 50, levelsY + 575);
			App.Draw(Image1);
			path = string("../data/images/menu/") + (southPark ? "sp_" : "") + "use_map.jpeg";
			image.LoadFromFile(path.c_str());
			sf::Sprite Image2;
			Image2.SetImage(image);
			Image2.SetPosition(levelsX + 250, levelsY + 575);
			App.Draw(Image2);
			sf::String Text;
			sf::Font font;
			font.LoadFromFile("../data/fonts/code.otf");
			Text.SetText("Please choose your map");
			Text.SetSize(25);
			Text.SetFont(font);
			Text.SetPosition(levelsX, levelsY - 35);
			for(int i = 0; i < 2; i++){
				sf::Shape Line = sf::Shape::Line(levelsX + 150*i + 125, levelsY, levelsX + 150*i + 125, levelsY + 550, 20, myRed);
				App.Draw(Line);
			}
			for(int i = 0; i < 3; i++){
				sf::Shape Line = sf::Shape::Line(levelsX, levelsY + 150*i + 125, levelsX + 400, levelsY + 150*i + 125, 20, myRed);
				App.Draw(Line);
			}
			App.Draw(Text);
			Text.SetText("OR");
			Text.SetSize(20);
			Text.SetFont(font);
			Text.SetPosition(levelsX + 190, levelsY + 587);
			App.Draw(Text);
			App.Display();
			// choosing levels
			sf::Event event;
			const sf::Input& Input = App.GetInput();
			if(Input.IsMouseButtonDown(sf::Mouse::Left))
				while(true)
					if(App.GetEvent(event) && event.Type == sf::Event::MouseButtonReleased)
						break;
			while(true)
				if(App.GetEvent(event)){
					if(event.Type == sf::Event::Closed){
    	    			App.Close();
						isOpen = false;
						return;
					}
					if(event.Type == sf::Event::MouseButtonPressed){
						int x = Input.GetMouseX(), y = Input.GetMouseY(), choice = -1;
						// levels
						for(int i = 0; i < 4; i++)
							for(int j = 0; j < 3; j++)
								if(levelsX + 150*j <= x && x <= levelsX + 150*j + 100
								&& levelsY + 150*i <= y && y <= levelsY + 150*i + 100){
									choice = i*3 + j;
									break;
								}
						if(choice != -1){
							level = choice + 1;
							break;
						}
						// submit
						if(levelsX-50 <= x && x <= levelsX-50 + 200
						&& levelsY+575 <= y && y <= levelsY+575 + 50)
							return;
						// use map.txt
						if(levelsX+250 <= x && x <= levelsX+250 + 200
						&& levelsY+575 <= y && y <= levelsY+575 + 50){
							level = 0;
							return;
						}
					}
				}
		}
	}	
	// customize the graphics
	void customize(){
		while(true){
			App.Clear(bgColor);
			// south park
			if(southPark){
				sf::Image bg;
				bg.LoadFromFile("../data/images/other/sp_background.jpg");
				sf::Sprite BG;
				BG.SetImage(bg);
				BG.Resize(600, 700);
				BG.SetPosition(0, 0);
				App.Draw(BG);
			}
			int customX = 25, customY = 50;
			sf::String Text;
			sf::Font font;
			font.LoadFromFile("../data/fonts/code.otf");
			Text.SetText("Customize graphics");
			Text.SetSize(20);
			Text.SetFont(font);
			Text.SetPosition(customX, customY);
			App.Draw(Text);
			// showing players
			customX = 275, customY = 50;
			Text.SetText("first player");
			Text.SetSize(15);
			Text.SetFont(font);
			Text.SetPosition(customX, customY-20);
			App.Draw(Text);
			Text.SetText("second player");
			Text.SetSize(15);
			Text.SetFont(font);
			Text.SetPosition(customX, customY+100-20);
			App.Draw(Text);
			for(int i = 0; i < 6; i++){
				sf::Image image;
				string path = string("../data/images/player/player" + string(1, '0'+i+1) + ".jpg");
				image.LoadFromFile(path.c_str());
				sf::Sprite Image;
				Image.SetImage(image);
				Image.Resize(50, 50);
				Image.SetPosition(customX + 50*i, customY);
				App.Draw(Image);
				Image.SetPosition(customX + 50*i, customY + 100);
				App.Draw(Image);
			}
			for(int i = 0; i < 6; i++)
				if(player1Ind == i){
					sf::Image image;
					image.LoadFromFile("../data/images/other/border.png");
					sf::Sprite Image;
					Image.SetImage(image);
					Image.Resize(50, 50);
					Image.SetPosition(customX + 50*i, customY);
					App.Draw(Image);
					break;
				}
			customY += 100;
			for(int i = 0; i < 6; i++)
				if(player2Ind == i){
					sf::Image image;
					image.LoadFromFile("../data/images/other/border.png");
					sf::Sprite Image;
					Image.SetImage(image);
					Image.Resize(50, 50);
					Image.SetPosition(customX + 50*i, customY);
					App.Draw(Image);
					break;
				}
			// showing monsters
			customX = 25;
			customY = 150;
			Text.SetText("monster");
			Text.SetSize(15);
			Text.SetFont(font);
			Text.SetPosition(customX, customY-20);
			App.Draw(Text);
			for(int i = 0; i < 4; i++){
				sf::Image image;
				string path = string("../data/images/monster/monster" + string(1, '0'+i+1) + ".jpg");
				image.LoadFromFile(path.c_str());
				sf::Sprite Image;
				Image.SetImage(image);
				Image.Resize(50, 50);
				Image.SetPosition(customX + 50*i, customY);
				App.Draw(Image);
			}
			for(int i = 0; i < 4; i++)
				if(monsterInd == i){
					sf::Image image;
					image.LoadFromFile("../data/images/other/border.png");
					sf::Sprite Image;
					Image.SetImage(image);
					Image.Resize(50, 50);
					Image.SetPosition(customX + 50*i, customY);
					App.Draw(Image);
					break;
				}
			// showing boxes
			customX = 25;
			customY = 250;
			Text.SetText("box");
			Text.SetSize(15);
			Text.SetFont(font);
			Text.SetPosition(customX, customY-20);
			App.Draw(Text);
			for(int i = 0; i < 4; i++)
				for(int j = 0; j < 5; j++){
					int ind = i*5 + j;
					sf::Image image;
					stringstream ss;
					ss << ind+1;
					string indS;
					ss >> indS;
					string path = string("../data/images/box/box" + indS + ".jpg");
					image.LoadFromFile(path.c_str());
					sf::Sprite Image;
					Image.SetImage(image);
					Image.Resize(50, 50);
					Image.SetPosition(customX + 50*j, customY + 50*i);
					App.Draw(Image);
				}
			bool flag = true;
			for(int i = 0; i < 4 && flag; i++)
				for(int j = 0; j < 5 && flag; j++){
					int ind = i*5 + j;
					if(boxInd == ind){
						sf::Image image;
						image.LoadFromFile("../data/images/other/border.png");
						sf::Sprite Image;
						Image.SetImage(image);
						Image.Resize(50, 50);
						Image.SetPosition(customX + 50*j, customY + 50*i);
						App.Draw(Image);
						flag = false;
					}
				}
			// showing stars
			customX = 325;
			customY = 250;
			Text.SetText("star");
			Text.SetSize(15);
			Text.SetFont(font);
			Text.SetPosition(customX, customY-20);
			App.Draw(Text);
			for(int i = 0; i < 4; i++)
				for(int j = 0; j < 5; j++){
					int ind = i*5 + j;
					sf::Image image;
					stringstream ss;
					ss << ind+1;
					string indS;
					ss >> indS;
					string path = string("../data/images/star/star" + indS + ".jpg");
					image.LoadFromFile(path.c_str());
					sf::Sprite Image;
					Image.SetImage(image);
					Image.Resize(50, 50);
					Image.SetPosition(customX + 50*j, customY + 50*i);
					App.Draw(Image);
				}
			flag = true;
			for(int i = 0; i < 4 && flag; i++)
				for(int j = 0; j < 5 && flag; j++){
					int ind = i*5 + j;
					if(starInd == ind){
						sf::Image image;
						image.LoadFromFile("../data/images/other/border.png");
						sf::Sprite Image;
						Image.SetImage(image);
						Image.Resize(50, 50);
						Image.SetPosition(customX + 50*j, customY + 50*i);
						App.Draw(Image);
						flag = false;
					}
				}
			// showing banner
			customX = 0;
			customY = 600;
			sf::Image image;
			string path = "../data/images/other/sp_banner.jpg";
			image.LoadFromFile(path);
			sf::Sprite Image;
			Image.SetImage(image);
			Image.Resize(600, 100);
			Image.SetPosition(customX, customY);
			App.Draw(Image);
			// showing buttons
			customX = 25;
			customY = 500;
			path = "../data/images/menu/submit.jpeg";
			if(southPark)
				path = "../data/images/menu/sp_submit.jpeg";
			image.LoadFromFile(path);
			sf::Sprite Image2;
			Image2.SetImage(image);
			Image2.SetPosition(customX, customY);
			App.Draw(Image2);
			customX = 375;
			customY = 500;
			path = "../data/images/menu/go_southpark.jpeg";
			if(southPark)
				path = "../data/images/menu/sp_go_southpark.jpeg";
			image.LoadFromFile(path);
			sf::Sprite Image3;
			Image3.SetImage(image);
			Image3.SetPosition(customX, customY);
			App.Draw(Image3);
			customX = 290;
			customY = 515;
			Text.SetText("OR");
			Text.SetSize(20);
			Text.SetFont(font);
			Text.SetPosition(customX, customY);
			App.Draw(Text);
			App.Display();
			// customizing
			sf::Event event;
			const sf::Input& Input = App.GetInput();
			if(Input.IsMouseButtonDown(sf::Mouse::Left))
				while(true)
					if(App.GetEvent(event) && event.Type == sf::Event::MouseButtonReleased)
						break;
			while(true)
				if(App.GetEvent(event)){
					if(event.Type == sf::Event::Closed){
	    	    		App.Close();
						isOpen = false;
						return;
					}
					if(event.Type == sf::Event::MouseButtonPressed){
						int x = Input.GetMouseX(), y = Input.GetMouseY(), choice = -1;
						// first player
						customX = 275;
						customY = 50;
						if(customY <= y && y <= customY + 50)
							for(int i = 0; i < 8; i++)
								if(customX + i*50 <= x && x <= customX + i * 50 + 50){
									choice = i;
									break;
								}
						if(choice != -1){
							player1Ind = choice;
							break;
						}
						// second player
						customY += 100;
						if(customY <= y && y <= customY + 50)
							for(int i = 0; i < 8; i++)
								if(customX + i*50 <= x && x <= customX + i * 50 + 50){
									choice = i;
									break;
								}
						if(choice != -1){
							player2Ind = choice;
							break;
						}
						// monster
						customX = 25;
						customY = 150;
						if(customY <= y && y <= customY + 50)
							for(int i = 0; i < 4; i++)
								if(customX + i*50 <= x && x <= customX + i * 50 + 50){
									choice = i;
									break;
								}
						if(choice != -1){
							monsterInd = choice;
							break;
						}
						// box
						customY += 100;
						for(int i = 0; i < 4; i++)
							for(int j = 0; j < 5; j++){
								int ind = i*5 + j;
								if((customX + j*50 <= x && x <= customX + j * 50 + 50)
								&& (customY + i*50 <= y && y <= customY + i * 50 + 50)){
									choice = ind;
									break;
								}
							}
						if(choice != -1){
							boxInd = choice;
							break;
						}
						// star
						customX = 325;
						for(int i = 0; i < 4; i++)
							for(int j = 0; j < 5; j++){
								int ind = i*5 + j;
								if((customX + j*50 <= x && x <= customX + j * 50 + 50)
								&& (customY + i*50 <= y && y <= customY + i * 50 + 50)){
									choice = ind;
									break;
								}
							}
						if(choice != -1){
							starInd = choice;
							break;
						}
						// submit
						customX = 25;
						customY = 500;
						if(customY <= y && y <= customY + 50
						&& customX <= x && x <= customX + 200){
							southPark = false;
							return;
						}
						// south park
						customX = 375;
						if(customY <= y && y <= customY + 50
						&& customX <= x && x <= customX + 200){
							southPark = true;
							return;
						}
					}
				}
		}
	}
	// default server
	void defaultServer(){
		defServer = true;
	}
	// return the chosen level
	int getLevel() const{
		return level;
	}
	// game over
	virtual void game_over(int winner){
		const int timeX = 285, timeY = 78, scoreX = 70, scoreY = 75;
		sf::Shape Rect = sf::Shape::Rectangle(scoreX + 200, scoreY + 1, scoreX + 200 + 55, scoreY + 55, sf::Color::White);
		App.Draw(Rect);
		sf::String Time;
		sf::Font font;
		font.LoadFromFile("../data/fonts/display.ttf");
		Time.SetText("--");
		Time.SetColor(myRed);
		Time.SetPosition(timeX, timeY);
		Time.SetSize(40);
		Time.SetFont(font);
		App.Draw(Time);
		string str;
		if(winner == 0)
			str = "WE HAVE A TIE!";
		else if(winner == 1){
			for(int i = 0; i < player1_name.length(); i++)
				str += toupper(player1_name[i]);
			str += " WON!";
		}
		else if(winner == 2){
			for(int i = 0; i < player2_name.length(); i++)
				str += toupper(player2_name[i]);
			str += " WON!";
		}
		else
			str = "WRONG INFORMATION FROM THE SERVER!";
		sf::String Text;
		font.LoadFromFile("../data/fonts/code.otf");
		Text.SetText(str);
		Text.SetPosition(75, 25);
		Text.SetFont(font);
		Text.SetColor(myRed);
		Text.SetSize(25);
		App.Draw(Text);
		App.Display();
		sf::Event event;
		while(true)
			if(App.GetEvent(event) && event.Type == sf::Event::Closed){
				isOpen = false;
				return;
			}
	}
};

#endif
