#include "MapSystem.h"

#include <cmath>
#include <iostream>
#include "../Constants.h"

using namespace std;
using namespace ld;

MapSystem::MapSystem()
{
  layout_map();

  printf(" Map System finished\n");
}


void MapSystem::layout_map()
{
  for (auto floor = 0; floor < NUM_FLOORS; ++floor)
  {
    layout_room("a", 5, 5, 5, 5, floor);
    layout_room("a", 5, -5, 5, 5, floor);
    layout_room("a", -5, 5, 5, 5, floor);
    layout_room("a", -5, -5, 5, 5, floor);
  }
}


void MapSystem::layout_room(
  const std::string& type, int x_, int y_, int size_x, int size_y, int floor)
{
  for (auto x = x_ - size_x / 2 + 1; x <= x_ + size_x / 2 - 1; ++x)
  {
    set_tile(x, y_ - size_y / 2, floor, type, "wall", 180);
    set_tile(x, y_ + size_y / 2, floor, type, "wall", 0);

    set_ceil_tile(x, y_ - size_y / 2, floor, type, "floor-edge", 180);
    set_ceil_tile(x, y_ + size_y / 2, floor, type, "floor-edge", 0);
  }

  for (auto y = y_ - size_y / 2 + 1; y <= y_ + size_y / 2 - 1; ++y)
  {
    set_tile(x_ - size_x / 2, y, floor, type, "wall", 90);
    set_tile(x_ + size_x / 2, y, floor, type, "wall", 270);

    set_ceil_tile(x_ - size_x / 2, y, floor, type, "floor-edge", 90);
    set_ceil_tile(x_ + size_x / 2, y, floor, type, "floor-edge", 270);
  }

  for (auto x = x_ - size_x / 2 + 1; x <= x_ + size_x / 2 - 1; ++x)
    for (auto y = y_ - size_y / 2 + 1; y <= y_ + size_x / 2 - 1; ++y)
      set_ceil_tile(x, y, floor, type, "floor");

  set_tile(x_ - size_x / 2, y_ - size_y / 2, floor, type, "corner", 90);
  set_tile(x_ + size_x / 2, y_ - size_y / 2, floor, type, "corner", 180);
  set_tile(x_ + size_x / 2, y_ + size_y / 2, floor, type, "corner", 270);
  set_tile(x_ - size_x / 2, y_ + size_y / 2, floor, type, "corner", 0);

  set_ceil_tile(x_ - size_x / 2, y_ - size_y / 2, floor, type, "floor-edge", 90);
  set_ceil_tile(x_ + size_x / 2, y_ - size_y / 2, floor, type, "floor-edge", 180);
  set_ceil_tile(x_ + size_x / 2, y_ + size_y / 2, floor, type, "floor-edge", 270);
  set_ceil_tile(x_ - size_x / 2, y_ + size_y / 2, floor, type, "floor-edge", 0);

  set_tile(x_ - size_x / 2, y_, floor, type, "door", 90, false);
  set_tile(x_, y_, floor, type, "transporter", 0, false);
}


void MapSystem::set_tile(
  int x, int y, int floor,
  const std::string& type, const std::string& name,
  double rotation,
  bool solid)
{
  auto& tile = get_tile(x, y, floor);

  tile.type = type;
  tile.name = name;
  tile.position = osg::Vec3(x, y, floor);
  tile.rotation = rotation;
  tile.solid = solid;
}


void MapSystem::set_ceil_tile(
  int x, int y, int floor,
  const std::string& type, const std::string& name,
  double rotation)
{
  auto& tile = get_tile(x, y, floor);

  tile.ceil_type = type;
  tile.ceil_name = name;
  tile.ceil_rotation = rotation;
}


Tile& MapSystem::get_tile(int x, int y, int floor)
{
  auto xx = x + MAP_SIZE / 2;
  auto yy = y + MAP_SIZE / 2;

  return tiles[floor][xx][yy];
}


const Tile& MapSystem::get_tile(int x, int y, int floor) const
{
  return get_tile(x, y, floor);
}


Tile& MapSystem::get_tile(double x, double y, int floor)
{
  auto xx = round(x);
  auto yy = round(y);

  return get_tile(xx, yy, floor);
}


bool MapSystem::is_solid(double x, double y, int floor)
{
  return get_tile(x, y, floor).solid;
}
