#ifndef MAP
#define MAP

class Map
{
private:
	char** cells;
	bool** horizontal_walls;
	bool** vertical_walls;

public:
	Map();
	void set(Map&);
	char get_cell(int, int) const;
	void set_cell(int, int, char);
	bool is_h_wall(int, int) const;
	bool is_v_wall(int, int) const;
	void set_h_wall(int, int, bool);
	void set_v_wall(int, int, bool);
	~Map();
};

#endif