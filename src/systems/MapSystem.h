#ifndef MAPSYSTEM_H
#define MAPSYSTEM_H

#include <string>
#include <array>
#include <vector>
#include "../Constants.h"
#include "../components/Tile.h"

namespace ld
{

static constexpr int NUM_CHUNKS = 5;
static constexpr int NUM_FLOORS = 2;
static constexpr double FLOOR_HEIGHT = 4.0;
static constexpr int TILE_SIZE = 2;
static constexpr int CHUNK_SIZE = 34;
static constexpr int MAP_SIZE = CHUNK_SIZE * NUM_CHUNKS;

class MapSystem
{
  std::array<std::array<std::array<Tile, MAP_SIZE>, MAP_SIZE>, NUM_FLOORS> tiles;

public:
  MapSystem();

  void layout_map();
  void layout_room(
    const std::string& type, int x, int y, int size_x, int size_y, int floor);

  void set_tile(
    int x, int y, int floor,
    const std::string& type, const std::string& name,
    double rotation = 0.0,
    bool solid = true);

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
