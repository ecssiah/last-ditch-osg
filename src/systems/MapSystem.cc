#include "MapSystem.h"

#include <chrono>
#include <cmath>
#include <functional>
#include <random>
#include <iostream>
#include "../Constants.h"

using namespace std;
using namespace osg;
using namespace ld;

static mt19937 RNG;

MapSystem::MapSystem()
  : seed(SEED > 0 ? SEED : chrono::high_resolution_clock::now().time_since_epoch().count())
{
  layout_map();

  printf(" Map System finished\n");
}


void MapSystem::layout_map()
{
  for (auto floor = 0; floor < NUM_FLOORS; ++floor)
  {
    master_rooms[floor].push_back(Room(0, 16, 10, 10));
  }

  for (auto floor = 0; floor < NUM_FLOORS; ++floor)
    for (auto& master : master_rooms[floor])
      seed_rooms(master, floor);

  for (auto i = 0; i < 1000; ++i)
    for (auto floor = 0; floor < NUM_FLOORS; ++floor)
      for (auto& room : rooms[floor])
	extend_room(room, floor);

  for (auto floor = 0; floor < NUM_FLOORS; ++floor)
    for (auto& room : rooms[floor])
      layout_room("a", room, floor);

  for (auto floor = 0; floor < NUM_FLOORS; ++floor)
    for (auto& master : master_rooms[floor])
      layout_master("a", master, floor);
}


void MapSystem::seed_rooms(Room& master, int floor)
{
  RNG.seed(seed);

  for (auto room_num = 0; room_num < 4; ++room_num)
  {
    for (auto i = 0; i < 100; ++i)
    {
      std::uniform_int_distribution<> x_dist(
	master.x - master.w / 2 + 1, master.x + master.w / 2 - 1);

      std::uniform_int_distribution<> y_dist(
	master.y - master.h / 2 + 1, master.y + master.h / 2 - 1);

      auto x = x_dist(RNG);
      auto y = y_dist(RNG);

      Room candidate(x, y, 3, 3, &master);

      bool collision = false;
      for (auto& room : rooms[floor])
      {
	if (intersects(candidate, room))
	{
	  collision = true;
	  break;
	}
      }

      if (!collision)
      {
	rooms[floor].push_back(candidate);
	break;
      }
    }
  }
}


void MapSystem::extend_room(Room& target, int floor)
{
  RNG.seed(seed);

  for (auto i = 0; i < 10000; ++i)
  {
    std::uniform_int_distribution<> choice(0, 4);
    auto direction = choice(RNG);

    Room test_room(target.x, target.y, target.w, target.h);
    const Room* master_room = target.master;

    if (direction == 0)
    {
      if (test_room.x + test_room.w / 2 + 1 < master_room->x + master_room->w / 2)
      {
	test_room.w += 2;
      }
    }
    else if (direction == 1)
    {
      if (test_room.y + test_room.h / 2 + 1 < master_room->y + master_room->h / 2)
      {
	test_room.h += 2;
      }
    }
    else if (direction == 2)
    {
      if (test_room.x - test_room.w / 2 - 1 > master_room->x - master_room->w / 2)
      {
	test_room.w += 2;
      }
    }
    else if (direction == 3)
    {
      if (test_room.y - test_room.h / 2 - 1 > master_room->y - master_room->h / 2)
      {
	test_room.h += 2;
      }
    }

    bool collision = false;
    for (auto& room : rooms[floor])
    {
      if (room == target) continue;

      if (intersects(room, test_room))
      {
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

      break;
    }
  }
}


void MapSystem::layout_master(
  const std::string& type, const Room& master, int floor)
{
  layout_master(type, master.x, master.y, master.w, master.h, floor);
}


void MapSystem::layout_master(
  const std::string& type, int x_, int y_, int w_, int h_, int floor)
{
  for (auto x = x_ - w_ / 2 + 1; x <= x_ + w_ / 2 - 1; ++x)
  {
    set_tile(x, y_ - h_ / 2, floor, type, "wall", 180);
    set_tile(x, y_ + h_ / 2, floor, type, "wall", 0);

    set_ceil_tile(x, y_ - h_ / 2, floor, type, "floor-edge", 180);
    set_ceil_tile(x, y_ + h_ / 2, floor, type, "floor-edge", 0);
  }

  for (auto y = y_ - h_ / 2 + 1; y <= y_ + h_ / 2 - 1; ++y)
  {
    set_tile(x_ - w_ / 2, y, floor, type, "wall", 90);
    set_tile(x_ + w_ / 2, y, floor, type, "wall", 270);

    set_ceil_tile(x_ - w_ / 2, y, floor, type, "floor-edge", 90);
    set_ceil_tile(x_ + w_ / 2, y, floor, type, "floor-edge", 270);
  }

  set_tile(x_ - w_ / 2, y_ + h_ / 2, floor, type, "corner", 0);
  set_tile(x_ - w_ / 2, y_ - h_ / 2, floor, type, "corner", 90);
  set_tile(x_ + w_ / 2, y_ - h_ / 2, floor, type, "corner", 180);
  set_tile(x_ + w_ / 2, y_ + h_ / 2, floor, type, "corner", 270);

  set_ceil_tile(x_ - w_ / 2, y_ + h_ / 2, floor, type, "floor-edge", 0);
  set_ceil_tile(x_ - w_ / 2, y_ - h_ / 2, floor, type, "floor-edge", 90);
  set_ceil_tile(x_ + w_ / 2, y_ - h_ / 2, floor, type, "floor-edge", 180);
  set_ceil_tile(x_ + w_ / 2, y_ + h_ / 2, floor, type, "floor-edge", 270);

  // set_tile(x_, y_ - h_ / 2, floor, type, "door", 0, false);
  // set_tile(x_ - w_ / 2, y_, floor, type, "door", 90, false);
  // set_tile(x_, y_ + h_ / 2, floor, type, "door", 180, false);
  // set_tile(x_ + w_ / 2, y_, floor, type, "door", 270, false);
}


void MapSystem::layout_room(
  const std::string& type, const Room& room, int floor)
{
  layout_room(type, room.x, room.y, room.w, room.h, floor);
}


void MapSystem::layout_room(
  const std::string& type, int x_, int y_, int w_, int h_, int floor)
{
  for (auto x = x_ - w_ / 2 + 1; x <= x_ + w_ / 2 - 1; ++x)
  {
    set_tile(x, y_ + h_ / 2, floor, type, "int-wall", 0);
    set_tile(x, y_ - h_ / 2, floor, type, "int-wall", 180);

    set_ceil_tile(x, y_ + h_ / 2, floor, type, "floor-edge", 0);
    set_ceil_tile(x, y_ - h_ / 2, floor, type, "floor-edge", 180);
  }

  for (auto y = y_ - h_ / 2 + 1; y <= y_ + h_ / 2 - 1; ++y)
  {
    set_tile(x_ - w_ / 2, y, floor, type, "int-wall", 90);
    set_tile(x_ + w_ / 2, y, floor, type, "int-wall", 270);

    set_ceil_tile(x_ - w_ / 2, y, floor, type, "floor-edge", 90);
    set_ceil_tile(x_ + w_ / 2, y, floor, type, "floor-edge", 270);
  }

  // for (auto x = x_ - w_ / 2 + 1; x <= x_ + w_ / 2 - 1; ++x)
  //   for (auto y = y_ - h_ / 2 + 1; y <= y_ + w_ / 2 - 1; ++y)
  //     set_ceil_tile(x, y, floor, type, "floor");

  set_tile(x_ - w_ / 2, y_ + h_ / 2, floor, type, "int-corner", 0);
  set_tile(x_ - w_ / 2, y_ - h_ / 2, floor, type, "int-corner", 90);
  set_tile(x_ + w_ / 2, y_ - h_ / 2, floor, type, "int-corner", 180);
  set_tile(x_ + w_ / 2, y_ + h_ / 2, floor, type, "int-corner", 270);

  set_ceil_tile(x_ - w_ / 2, y_ + h_ / 2, floor, type, "floor-edge", 0);
  set_ceil_tile(x_ - w_ / 2, y_ - h_ / 2, floor, type, "floor-edge", 90);
  set_ceil_tile(x_ + w_ / 2, y_ - h_ / 2, floor, type, "floor-edge", 180);
  set_ceil_tile(x_ + w_ / 2, y_ + h_ / 2, floor, type, "floor-edge", 270);

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
  auto intersects =
    !(r1.x + r1.w / 2 <= r2.x - r2.w / 2 ||
      r2.x + r2.w / 2 <= r1.x - r1.w / 2 ||
      r1.y + r1.h / 2 <= r2.y - r2.h / 2 ||
      r2.y + r2.h / 2 <= r1.y - r1.h / 2);

  return intersects;
}
