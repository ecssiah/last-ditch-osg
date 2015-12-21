#ifndef MAPSYSTEM_H
#define MAPSYSTEM_H

#include <string>
#include <array>
#include <random>
#include <vector>
#include "../Constants.h"
#include "../components/Room.h"
#include "../components/Tile.h"

namespace ld
{

static constexpr int NUM_CHUNKS = 5;
static constexpr int CHUNK_SIZE = 34;
static constexpr int MAP_SIZE = CHUNK_SIZE * NUM_CHUNKS;
static constexpr int NUM_FLOORS = 1;
static constexpr int ROOMS_PER_FLOOR = 8;
static constexpr double TILE_SIZE = 2.0;
static constexpr double FLOOR_HEIGHT = 4.0;

class MapSystem
{
  void layout_map();

  void seed_rooms(const Room& master, int floor);
  void extend_room(Room& target, int floor);

  void layout_master(const std::string& type, const Room& master, int floor);
  void layout_room(const std::string& type, const Room& room, int floor);

  bool rect_intersects_rect(
    int r1x1, int r1x2, int r1y1, int r1y2,
    int r2x1, int r2x2, int r2y1, int r2y2,
    bool allow_overlap = true) const;
  bool rect_intersects_room(
    int x1, int x2, int y1, int y2,
    const Room& room, bool allow_overlap = true) const;
  bool room_intersects_room(
    const Room& r1, const Room& r2, bool allow_overlap = true) const;
  bool room_is_clear(Room& test_room, int floor) const;
  bool room_is_clear(Room& modded_room, Room& original_room, int floor) const;

  std::array<std::vector<Room>, NUM_FLOORS> rooms;
  std::array<std::vector<Room>, NUM_FLOORS> master_rooms;
  std::array<std::array<std::array<Tile, MAP_SIZE+1>, MAP_SIZE+1>, NUM_FLOORS> tiles;

  unsigned long long seed;

public:
  MapSystem();

  void set_tile(
    int x, int y, int floor,
    const std::string& type, const std::string& name,
    double rotation = 0.0, bool solid = true);
  void set_ceil_tile(
    int x, int y, int floor,
    const std::string& type, const std::string& name,
    double rotation = 0.0);

  Tile& get_tile(double x, double y, int floor);
  Tile& get_tile(int x, int y, int floor);
  const Tile& get_tile(int x, int y, int floor) const;

  bool is_solid(double x, double y, int floor) const;
};

}

#endif /* MAPSYSTEM_H */
