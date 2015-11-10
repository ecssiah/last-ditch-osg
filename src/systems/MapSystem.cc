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
  layout_room(-3, -3, 7, 7, 0);
}


void MapSystem::layout_room(int x_, int y_, int size_x, int size_y, int floor)
{
  for (int x = x_ + 1; x < x_ + size_x - 1; ++x)
  {
    set_tile(x, y_, floor, "a", "wall", 180);
    set_tile(x, y_ + size_y - 1, floor, "a", "wall", 0);
  }

  for (int y = y_ + 1; y < y_ + size_y - 1; ++y)
  {
    set_tile(x_, y, floor, "a", "wall", 90);
    set_tile(x_ + size_x - 1, y, floor, "a", "wall", 270);
  }

  set_tile(x_, y_, floor, "a", "corner", 90);
  set_tile(x_ + size_x - 1, y_, floor, "a", "corner", 180);
  set_tile(x_ + size_x - 1, y_ + size_y - 1, floor, "a", "corner", 270);
  set_tile(x_, y_ + size_y - 1, floor, "a", "corner", 0);
}


void MapSystem::set_tile(
  int x, int y, int floor,
  const std::string& type, const std::string& name,
  double rotation)
{
  Tile& tile = get_tile(x, y, floor);

  tile.type = type;
  tile.name = name;
  tile.rotation = rotation;
}


Tile& MapSystem::get_tile(int x, int y, int floor)
{
  return tiles[x + MAP_SIZE / 2][y + MAP_SIZE / 2][floor];
}


const Tile& MapSystem::get_tile(int x, int y, int floor) const
{
  return tiles[x + MAP_SIZE / 2][y + MAP_SIZE / 2][floor];
}
