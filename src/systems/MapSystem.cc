#include "MapSystem.h"

#include <cmath>
#include <iostream>
#include "../Constants.h"

using namespace std;
using namespace ld;

MapSystem::MapSystem()
{
  layout_map();
}


void MapSystem::layout_map()
{
  layout_room("a", 5, 5, 5, 5, 0);
  layout_room("a", 5, -5, 5, 5, 0);
  layout_room("a", -5, 5, 5, 5, 0);
  layout_room("a", -5, -5, 5, 5, 0);
}


void MapSystem::layout_room(
  const std::string& type, int x_, int y_, int size_x, int size_y, int floor)
{
  for (int x = x_ - size_x / 2 + 1; x < x_ + size_x / 2; ++x)
  {
    set_tile(x, y_ - size_y / 2, floor, type, "wall", 180);
    set_tile(x, y_ + size_y / 2, floor, type, "wall", 0);
  }

  for (int y = y_ - size_y / 2 + 1; y < y_ + size_y / 2; ++y)
  {
    set_tile(x_ - size_x / 2, y, floor, type, "wall", 90);
    set_tile(x_ + size_x / 2, y, floor, type, "wall", 270);
  }

  set_tile(x_ - size_x / 2, y_ - size_y / 2, floor, type, "corner", 90);
  set_tile(x_ + size_x / 2, y_ - size_y / 2, floor, type, "corner", 180);
  set_tile(x_ + size_x / 2, y_ + size_y / 2, floor, type, "corner", 270);
  set_tile(x_ - size_x / 2, y_ + size_y / 2, floor, type, "corner", 0);

  set_tile(x_ - size_x / 2, y_, floor, type, "door", 90, false);
}


void MapSystem::set_tile(
  int x, int y, int floor,
  const std::string& type, const std::string& name,
  double rotation,
  bool solid)
{
  Tile& tile = get_tile(x, y, floor);

  tile.type = type;
  tile.name = name;
  tile.position = osg::Vec3(x, y, floor);
  tile.rotation = rotation;
  tile.solid = solid;
}


Tile& MapSystem::get_tile(int x, int y, int floor)
{
  int xx = x + Map_Size / 2;
  int yy = y + Map_Size / 2;

  return tiles[floor][xx][yy];
}


const Tile& MapSystem::get_tile(int x, int y, int floor) const
{
  return get_tile(x, y, floor);
}


Tile& MapSystem::get_tile(double x, double y, int floor)
{
  int xx = round(x);
  int yy = round(y);

  return get_tile(xx, yy, floor);
}


bool MapSystem::is_solid(double x, double y, int floor)
{
  return get_tile(x, y, floor).solid;
}
