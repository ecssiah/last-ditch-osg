#include "MapSystem.h"

#include <iostream>
#include "../Constants.h"

using namespace std;
using namespace ld;

MapSystem::MapSystem()
  : tiles(
      MAP_SIZE, std::vector<std::vector<Tile>>(
	MAP_SIZE, std::vector<Tile>(
	  NUM_FLOORS, Tile())))
{
  layout_map();
}


void MapSystem::layout_map()
{
  layout_room(3, 3, 3, 3, 0);
  layout_room(-3, 3, 3, 3, 0);
  layout_room(3, -3, 3, 3, 0);
  layout_room(-3, -3, 3, 3, 0);
}

void MapSystem::layout_room(
  int x_, int y_,
  unsigned size_x, unsigned size_y,
  int floor)
{
  int xx = x_ + MAP_SIZE / 2;
  int yy = y_ + MAP_SIZE / 2;

  tiles[xx][yy][floor].type = "a";
  tiles[xx][yy][floor].name = "corner";
  tiles[xx][yy][floor].rotation = 90;

  tiles[xx + size_x - 1][yy][floor].type = "a";
  tiles[xx + size_x - 1][yy][floor].name = "corner";
  tiles[xx + size_x - 1][yy][floor].rotation = 180;

  tiles[xx][yy + size_y - 1][floor].type = "a";
  tiles[xx][yy + size_y - 1][floor].name = "corner";
  tiles[xx][yy + size_y - 1][floor].rotation = 270;

  tiles[xx + size_x - 1][yy + size_y - 1][floor].type = "a";
  tiles[xx + size_x - 1][yy + size_y - 1][floor].name = "corner";
  tiles[xx + size_x - 1][yy + size_y - 1][floor].rotation = 0;

  for (int x = x_ + 1; x < x_ + size_x - 1; ++x)
  {
    xx = x + MAP_SIZE / 2;

    tiles[xx][yy + size_y - 1][floor].type = "a";
    tiles[xx][yy + size_y - 1][floor].name = "wall";
    tiles[xx][yy + size_y - 1][floor].rotation = 0;

    tiles[xx][yy][floor].type = "a";
    tiles[xx][yy][floor].name = "wall";
    tiles[xx][yy][floor].rotation = 180;
  }

  for (int y = y_ + 1; y < y_ + size_y - 1; ++y)
  {
    xx = x_ + MAP_SIZE / 2;
    yy = y + MAP_SIZE / 2;

    tiles[xx][yy][floor].type = "a";
    tiles[xx][yy][floor].name = "wall";
    tiles[xx][yy][floor].rotation = 90;

    tiles[xx + size_x - 1][yy][floor].type = "a";
    tiles[xx + size_x - 1][yy][floor].name = "wall";
    tiles[xx + size_x - 1][yy][floor].rotation = 270;
  }
}

const Tile& MapSystem::get_tile(int x, int y, int floor) const
{
  return tiles[x + MAP_SIZE / 2][y + MAP_SIZE / 2][floor];
}
