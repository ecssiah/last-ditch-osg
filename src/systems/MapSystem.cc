#include "MapSystem.h"

#include <chrono>
#include <cmath>
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
    master_rooms[floor].push_back(Room(-5, 5, 10, 10));
  }

  for (auto floor = 0; floor < NUM_FLOORS; ++floor)
    for (auto& master : master_rooms[floor])
      seed_rooms(master, floor);

  for (auto floor = 0; floor < NUM_FLOORS; ++floor)
    for (auto i = 0; i < 10; ++i)
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

  for (auto room_num = 0; room_num < ROOMS_PER_FLOOR; ++room_num)
  {
    for (auto i = 0; i < 10000; ++i)
    {
      std::uniform_int_distribution<> x_dist(master.x, master.x + master.w - 3);
      std::uniform_int_distribution<> y_dist(master.y, master.y + master.h - 3);

      auto x = x_dist(RNG);
      auto y = y_dist(RNG);

      Room candidate(x, y, 3, 3, &master);

      auto collision = false;
      for (auto& room : rooms[floor])
      {
	if (room == candidate) continue;

	collision = room_intersects_room(candidate, room);

	if (collision) break;
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
  const auto master_room = target.master;
  Room test_room(target.x, target.y, target.w, target.h);

  if (test_room.x + test_room.w + 1 <= master_room->x + master_room->w)
  {
    ++test_room.w;

    auto collision = false;
    for (auto& room : rooms[floor])
    {
      if (target == room) continue;
      if (room_intersects_room(test_room, room))
      {
	collision = true;
	break;
      }
    }

    if (!collision)
    {
      target.w = test_room.w;
      printf("Right:\n");
      return;
    }
  }

  if (test_room.y + test_room.h + 1 <= master_room->y + master_room->h)
  {
    ++test_room.h;

    auto collision = false;
    for (auto& room : rooms[floor])
    {
      if (target == room) continue;
      if (room_intersects_room(test_room, room))
      {
	collision = true;
	break;
      }
    }

    if (!collision)
    {
      target.h = test_room.h;
      printf("Up:");
      return;
    }
  }

  if (test_room.x - 1 >= master_room->x)
  {
    --test_room.x;
    ++test_room.w;

    auto collision = false;
    for (auto& room : rooms[floor])
    {
      if (target == room) continue;
      if (room_intersects_room(test_room, room))
      {
	collision = true;
	break;
      }
    }

    if (!collision)
    {
      target.x = test_room.x;
      target.w = test_room.w;
      printf("Left:\n");
      return;
    }
  }

  if (test_room.y - 1 >= master_room->y)
  {
    --test_room.y;
    ++test_room.h;

    auto collision = false;
    for (auto& room : rooms[floor])
    {
      if (target == room) continue;
      if (room_intersects_room(test_room, room))
      {
	collision = true;
	break;
      }
    }

    if (!collision)
    {
      target.y = test_room.y;
      target.h = test_room.h;
      printf("Down:\n");
      return;
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
  for (auto x = x_ + 1; x < x_ + w_ - 1; ++x)
  {
    set_tile(x, y_, floor, type, "wall", 180);
    set_tile(x, y_ + h_ - 1, floor, type, "wall", 0);

    set_ceil_tile(x, y_, floor, type, "floor-edge", 180);
    set_ceil_tile(x, y_ + h_ - 1, floor, type, "floor-edge", 0);
  }

  for (auto y = y_ + 1; y < y_ + h_ - 1; ++y)
  {
    set_tile(x_, y, floor, type, "wall", 90);
    set_tile(x_ + w_ - 1, y, floor, type, "wall", 270);

    set_ceil_tile(x_, y, floor, type, "floor-edge", 90);
    set_ceil_tile(x_ + w_ - 1, y, floor, type, "floor-edge", 270);
  }

  set_tile(x_, y_ + h_ - 1, floor, type, "corner", 0);
  set_tile(x_, y_, floor, type, "corner", 90);
  set_tile(x_ + w_ - 1, y_, floor, type, "corner", 180);
  set_tile(x_ + w_ - 1, y_ + h_ - 1, floor, type, "corner", 270);

  set_ceil_tile(x_, y_ + h_ - 1, floor, type, "floor-edge", 0);
  set_ceil_tile(x_, y_, floor, type, "floor-edge", 90);
  set_ceil_tile(x_ + w_ - 1, y_, floor, type, "floor-edge", 180);
  set_ceil_tile(x_ + w_ - 1, y_ + h_ - 1, floor, type, "floor-edge", 270);
}


void MapSystem::layout_room(
  const std::string& type, const Room& room, int floor)
{
  layout_room(type, room.x, room.y, room.w, room.h, floor);
}


void MapSystem::layout_room(
  const std::string& type, int x_, int y_, int w_, int h_, int floor)
{
  for (auto x = x_ + 1; x < x_ + w_ - 1; ++x)
  {
    set_tile(x, y_ + h_ - 1, floor, type, "int-wall", 0);
    set_tile(x, y_, floor, type, "int-wall", 180);

    set_ceil_tile(x, y_ + h_ - 1, floor, type, "floor-edge", 0);
    set_ceil_tile(x, y_, floor, type, "floor-edge", 180);
  }

  for (auto y = y_ + 1; y < y_ + h_ - 1; ++y)
  {
    set_tile(x_, y, floor, type, "int-wall", 90);
    set_tile(x_ + w_ - 1, y, floor, type, "int-wall", 270);

    set_ceil_tile(x_, y, floor, type, "floor-edge", 90);
    set_ceil_tile(x_ + w_ - 1, y, floor, type, "floor-edge", 270);
  }

  set_tile(x_, y_ + h_ - 1, floor, type, "int-corner", 0);
  set_tile(x_, y_, floor, type, "int-corner", 90);
  set_tile(x_ + w_ - 1, y_, floor, type, "int-corner", 180);
  set_tile(x_ + w_ - 1, y_ + h_ - 1, floor, type, "int-corner", 270);

  set_ceil_tile(x_, y_ + h_ - 1, floor, type, "floor-edge", 0);
  set_ceil_tile(x_, y_, floor, type, "floor-edge", 90);
  set_ceil_tile(x_ + w_ - 1, y_, floor, type, "floor-edge", 180);
  set_ceil_tile(x_ + w_ - 1, y_ + h_ - 1, floor, type, "floor-edge", 270);
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


bool MapSystem::rect_intersects_rect(
  int r1x1, int r1x2, int r1y1, int r1y2,
  int r2x1, int r2x2, int r2y1, int r2y2,
  bool allow_overlap)
{
  bool to_left, to_right, above, below;

  if (allow_overlap)
  {
    to_right = r1x1 >= r2x2 - 1;
    to_left = r1x2 - 1 <= r2x1;
    above = r1y1 >= r2y2 - 1;
    below = r1y2 - 1 <= r2y1;
  }
  else
  {
    to_right = r1x1 > r2x2 - 1;
    to_left = r1x2 - 1 < r2x1;
    above = r1y1 > r2y2 - 1;
    below = r1y2 - 1 < r2y1;
  }

  printf("%d %d %d %d\n", r1x1, r1x2, r1y1, r1y2);
  printf("%d %d %d %d\n", r2x1, r2x2, r2y1, r2y2);

  if (to_left) printf("to left\n");
  else printf("not to left\n");
  if (to_right) printf("to right\n");
  else printf("not to right\n");
  if (above) printf("above\n");
  else printf("not above\n");
  if (below) printf("below\n");
  else printf("not below\n");
  printf("\n");

  auto intersects = !(to_left || to_right || above || below);

  return intersects;
}


bool MapSystem::rect_intersects_room(
  int x1, int x2, int y1, int y2, const Room& room, bool allow_overlap)
{
  return rect_intersects_rect(
    x1, x2, y1, y2,
    room.x, room.x + room.w, room.y, room.y + room.h,
    allow_overlap);
}


bool MapSystem::room_intersects_room(const Room& r1, const Room& r2, bool allow_overlap)
{
  return rect_intersects_rect(
    r1.x, r1.x + r1.w, r1.y, r1.y + r1.h,
    r2.x, r2.x + r2.w, r2.y, r2.y + r2.h,
    allow_overlap);
}
