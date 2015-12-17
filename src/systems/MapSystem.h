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
static constexpr double TILE_SIZE = 2.0;
static constexpr double FLOOR_HEIGHT = 4.0;

class MapSystem
{
  void seed_rooms(Room& master, int floor);
  void extend_room(Room& target, int floor);
  void layout_map();
  void layout_master(const std::string& type, const Room& master, int floor);
  void layout_master(const std::string& type, int x, int y, int w, int h, int floor);
  void layout_room(const std::string& type, const Room& room, int floor);
  void layout_room(const std::string& type, int x, int y, int w, int h, int floor);
  bool intersects(Room& r1, Room& r2);

  unsigned long seed;

  std::array<std::vector<Room>, NUM_FLOORS> rooms;
  std::array<std::vector<Room>, NUM_FLOORS> master_rooms;
  std::array<std::array<std::array<Tile, MAP_SIZE + 1>, MAP_SIZE + 1>, NUM_FLOORS> tiles;

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
  Tile& get_tile(int x, int y, int floor);
  const Tile& get_tile(int x, int y, int floor) const;
  Tile& get_tile(double x, double y, int floor);
  bool is_solid(double x, double y, int floor);
};

}

#endif /* MAPSYSTEM_H */
