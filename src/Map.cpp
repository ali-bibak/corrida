#include "Map.h"

Map::Map()
{
	cells = new char*[8];
	for (int i = 0; i < 8; i++)
		cells[i] = new char[8];

	horizontal_walls = new bool*[7];
	for (int i = 0; i < 7; i++)
		horizontal_walls[i] = new bool[8];

	vertical_walls = new bool*[8];
	for (int i = 0; i < 8; i++)
		vertical_walls[i] = new bool[7];

	for (int i = 0; i < 8; i++)
	for (int j = 0; j < 8; j++)
		cells[i][j] = '.';

	for (int i = 0; i < 7; i++)
	for (int j = 0; j < 8; j++)
	{
		vertical_walls[j][i] = false;
		horizontal_walls[i][j] = false;
	}
}

void Map::set(Map& map)
{
	for (int i = 0; i < 8; i++)
	for (int j = 0; j < 8; j++)
		cells[i][j] = map.cells[i][j];

	for (int i = 0; i < 7; i++)
	for (int j = 0; j < 8; j++)
	{
		vertical_walls[j][i] = map.vertical_walls[j][i];
		horizontal_walls[i][j] = map.horizontal_walls[i][j];
	}
}

char Map::get_cell(int x, int y) const
{
	return cells[x][y];
}

void Map::set_cell(int x, int y, char c)
{
	cells[x][y] = c;
}

bool Map::is_h_wall(int x, int y) const
{
	return horizontal_walls[x][y];
}

bool Map::is_v_wall(int x, int y) const
{
	return vertical_walls[x][y];
}

void Map::set_h_wall(int x, int y, bool w)
{
	horizontal_walls[x][y] = w;
}

void Map::set_v_wall(int x, int y, bool w)
{
	vertical_walls[x][y] = w;
}

Map::~Map()
{

}