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
    master_rooms[floor].push_back(Room(-5, 10, 20, 20));

    for (const auto& master : master_rooms[floor])
      seed_rooms(master, floor);

    for (auto i = 0; i < 100; ++i)
      for (auto& room : rooms[floor])
	extend_room(room, floor);

    for (const auto& room : rooms[floor])
      layout_room("a", room, floor);

    for (const auto& master : master_rooms[floor])
      layout_master("a", master, floor);
  }
}


void MapSystem::seed_rooms(const Room& master, int floor)
{
  RNG.seed(seed);

  for (auto room_num = 0; room_num < ROOMS_PER_FLOOR; ++room_num)
  {
    for (auto i = 0; i < 10000; ++i)
    {
      const auto min_room_size = 3;

      std::uniform_int_distribution<> x_dist(master.x, master.x + master.w - min_room_size);
      std::uniform_int_distribution<> y_dist(master.y, master.y + master.h - min_room_size);

      const auto x = x_dist(RNG);
      const auto y = y_dist(RNG);

      Room candidate(x, y, min_room_size, min_room_size, &master);

      if (room_is_clear(candidate, floor))
      {
	rooms[floor].push_back(candidate);
	break;
      }
    }
  }
}


void MapSystem::extend_room(Room& target, int floor)
{
  const auto master = target.master;
  Room test_room(target.x, target.y, target.w, target.h);

  if (test_room.x + test_room.w + 1 <= master->x + master->w)
  {
    ++test_room.w;

    if (room_is_clear(test_room, target, floor))
      target.w = test_room.w;
    else
      --test_room.w;
  }

  if (test_room.y + test_room.h + 1 <= master->y + master->h)
  {
    ++test_room.h;

    if (room_is_clear(test_room, target, floor))
      target.h = test_room.h;
    else
      --test_room.h;
  }

  if (test_room.x - 1 >= master->x)
  {
    --test_room.x;
    ++test_room.w;

    if (room_is_clear(test_room, target, floor))
    {
      target.x = test_room.x;
      target.w = test_room.w;
    }
    else
    {
      ++test_room.x;
      --test_room.w;
    }
  }

  if (test_room.y - 1 >= master->y)
  {
    --test_room.y;
    ++test_room.h;

    if (room_is_clear(test_room, target, floor))
    {
      target.y = test_room.y;
      target.h = test_room.h;
    }
    else
    {
      ++test_room.y;
      --test_room.h;
    }
  }
}


void MapSystem::layout_master(
  const std::string& type, const Room& master, int floor)
{
  const auto mx = master.x, my = master.y;
  const auto mw = master.w, mh = master.h;

  for (auto x = mx + 1; x < mx + mw - 1; ++x)
  {
    set_tile(x, my, floor, type, "wall", 180);
    set_tile(x, my + mh - 1, floor, type, "wall", 0);

    set_ceil_tile(x, my, floor, type, "floor-edge", 180);
    set_ceil_tile(x, my + mh - 1, floor, type, "floor-edge", 0);
  }

  for (auto y = my + 1; y < my + mh - 1; ++y)
  {
    set_tile(mx, y, floor, type, "wall", 90);
    set_tile(mx + mw - 1, y, floor, type, "wall", 270);

    set_ceil_tile(mx, y, floor, type, "floor-edge", 90);
    set_ceil_tile(mx + mw - 1, y, floor, type, "floor-edge", 270);
  }

  set_tile(mx, my + mh - 1, floor, type, "corner", 0);
  set_tile(mx, my, floor, type, "corner", 90);
  set_tile(mx + mw - 1, my, floor, type, "corner", 180);
  set_tile(mx + mw - 1, my + mh - 1, floor, type, "corner", 270);

  set_ceil_tile(mx, my + mh - 1, floor, type, "floor-edge", 0);
  set_ceil_tile(mx, my, floor, type, "floor-edge", 90);
  set_ceil_tile(mx + mw - 1, my, floor, type, "floor-edge", 180);
  set_ceil_tile(mx + mw - 1, my + mh - 1, floor, type, "floor-edge", 270);
}


void MapSystem::layout_room(
  const std::string& type, const Room& room, int floor)
{
  const auto rx = room.x, ry = room.y;
  const auto rw = room.w, rh = room.h;

  for (auto x = rx + 1; x < rx + rw - 1; ++x)
  {
    set_tile(x, ry + rh - 1, floor, type, "int-wall", 0);
    set_tile(x, ry, floor, type, "int-wall", 180);

    set_ceil_tile(x, ry + rh - 1, floor, type, "floor-edge", 0);
    set_ceil_tile(x, ry, floor, type, "floor-edge", 180);
  }

  for (auto y = ry + 1; y < ry + rh - 1; ++y)
  {
    set_tile(rx, y, floor, type, "int-wall", 90);
    set_tile(rx + rw - 1, y, floor, type, "int-wall", 270);

    set_ceil_tile(rx, y, floor, type, "floor-edge", 90);
    set_ceil_tile(rx + rw - 1, y, floor, type, "floor-edge", 270);
  }

  set_tile(rx, ry + rh - 1, floor, type, "int-corner", 0);
  set_tile(rx, ry, floor, type, "int-corner", 90);
  set_tile(rx + rw - 1, ry, floor, type, "int-corner", 180);
  set_tile(rx + rw - 1, ry + rh - 1, floor, type, "int-corner", 270);

  set_ceil_tile(rx, ry + rh - 1, floor, type, "floor-edge", 0);
  set_ceil_tile(rx, ry, floor, type, "floor-edge", 90);
  set_ceil_tile(rx + rw - 1, ry, floor, type, "floor-edge", 180);
  set_ceil_tile(rx + rw - 1, ry + rh - 1, floor, type, "floor-edge", 270);
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


Tile& MapSystem::get_tile(int x_, int y_, int floor)
{
  const auto x = x_ + MAP_SIZE / 2;
  const auto y = y_ + MAP_SIZE / 2;

  return tiles[floor][x][y];
}


const Tile& MapSystem::get_tile(int x, int y, int floor) const
{
  return get_tile(x, y, floor);
}


Tile& MapSystem::get_tile(double x_, double y_, int floor)
{
  const auto x = std::round(x_);
  const auto y = std::round(y_);

  return get_tile(x, y, floor);
}


bool MapSystem::is_solid(double x, double y, int floor) const
{
  return get_tile(x, y, floor).solid;
}


bool MapSystem::rect_intersects_rect(
  int r1x1, int r1x2, int r1y1, int r1y2,
  int r2x1, int r2x2, int r2y1, int r2y2,
  bool allow_overlap) const
{
  const auto to_right = allow_overlap ? r1x1 >= r2x2 - 1 : r1x1 > r2x2;
  const auto to_left= allow_overlap ? r1x2 - 1 <= r2x1 : r1x2 - 1 < r2x1;
  const auto above = allow_overlap ? r1y1 >= r2y2 - 1 : r1y1 > r2y2 - 1;
  const auto below = allow_overlap ? r1y2 - 1 <= r2y1 : r1y2 - 1 < r2y1;

  return !(to_left || to_right || above || below);
}


bool MapSystem::rect_intersects_room(
  int x1, int x2, int y1, int y2, const Room& room, bool allow_overlap) const
{
  return rect_intersects_rect(
    x1, x2, y1, y2,
    room.x, room.x + room.w, room.y, room.y + room.h,
    allow_overlap);
}


bool MapSystem::room_intersects_room(
  const Room& r1, const Room& r2, bool allow_overlap) const
{
  return rect_intersects_rect(
    r1.x, r1.x + r1.w, r1.y, r1.y + r1.h,
    r2.x, r2.x + r2.w, r2.y, r2.y + r2.h,
    allow_overlap);
}


bool MapSystem::room_is_clear(
  const Room& modded_room, const Room& original_room, int floor) const
{
  for (const auto& room : rooms[floor])
  {
    if (original_room == room) continue;
    if (room_intersects_room(modded_room, room)) return false;
  }

  return true;
}


bool MapSystem::room_is_clear(const Room& test_room, int floor) const
{
  for (const auto& room : rooms[floor])
  {
    if (test_room == room) continue;
    if (room_intersects_room(test_room, room)) return false;
  }

  return true;
}
