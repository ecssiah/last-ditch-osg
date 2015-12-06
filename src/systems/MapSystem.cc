#include "MapSystem.h"

#include <cmath>
#include <random>
#include <iostream>
#include "../Constants.h"

using namespace std;
using namespace osg;
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
    master_rooms[floor].push_back(Room(-10, -10, 10, 10));
    master_rooms[floor].push_back(Room(-10, 10, 10, 10));
    master_rooms[floor].push_back(Room(10, 10, 10, 10));
    master_rooms[floor].push_back(Room(10, -10, 10, 10));
  }

  for (int floor = 0; floor < NUM_FLOORS; ++floor)
    for (auto master : master_rooms[floor])
      seed_rooms(master, floor);

  for (int iterations = 0; iterations < 10; ++iterations)
    for (int floor = 0; floor < NUM_FLOORS; ++floor)
      for (auto room : rooms[floor])
	extend_room(room, floor);

  for (int floor = 0; floor < NUM_FLOORS; ++floor)
    for (auto& room : rooms[floor])
      layout_room("a", room, floor);
}


void MapSystem::seed_rooms(Room& master, int floor)
{
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<int> x_dist(master.x, master.x + master.w);
  std::uniform_int_distribution<int> y_dist(master.y, master.y + master.h);

  for (int room_num = 0; room_num < 4; ++room_num)
  {
    int x = x_dist(mt);
    int y = y_dist(mt);

    rooms[floor].push_back(Room(x, y, 2, 2));
  }
}


void MapSystem::extend_room(Room& target, int floor)
{
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<int> dist(0, 4);

  int i = 0;
  while (i++ < 100)
  {
    int direction = dist(mt);
    Room test_room(target.x, target.y, target.w, target.h);

    if (direction == 0) ++test_room.x;
    if (direction == 1) --test_room.x;
    if (direction == 2) ++test_room.y;
    if (direction == 3) --test_room.y;

    bool collision = false;
    for (auto room : rooms[floor])
    {
      if (room == target) continue;

      if (intersects(room, test_room))
      {
	std::cout << "Collision!" << std::endl;
	collision = true;
	break;
      }
    }

    if (!collision)
    {
      target.x = test_room.x;
      target.y = test_room.y;
      target.w = test_room.w;
      target.h = test_room.h;

      std::cout << "Expansion!" << std::endl;

      break;
    }
  }
}


void MapSystem::layout_room(
  const std::string& type, const Room& room, int floor)
{
  layout_room(type, room.x, room.y, room.w, room.h, floor);
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

  tile.position = Vec3(x, y, floor);
  tile.type = type;
  tile.name = name;
  tile.rotation = rotation;
  tile.solid = solid;
}


void MapSystem::set_ceil_tile(
  int x, int y, int floor,
  const std::string& type, const std::string& name,
  double rotation)
{
  auto& tile = get_tile(x, y, floor);

  tile.position = Vec3(x, y, floor);
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
  auto xx = std::round(x);
  auto yy = std::round(y);

  return get_tile(xx, yy, floor);
}


bool MapSystem::is_solid(double x, double y, int floor)
{
  return get_tile(x, y, floor).solid;
}


bool MapSystem::intersects(Room& r1, Room& r2)
{
  bool intersects =
    !(r1.x > r2.x + r2.w || r2.x > r1.x + r1.w ||
      r1.y > r2.y + r2.h || r2.y > r1.y + r1.h);

  return intersects;
}
