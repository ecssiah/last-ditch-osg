#ifndef MAPSYSTEM_H
#define MAPSYSTEM_H

#include <string>
#include <array>
#include <vector>
#include "../Constants.h"
#include "../components/Tile.h"

namespace ld
{

class MapSystem
{
  static constexpr unsigned Num_Chunks = 5;
  static constexpr unsigned Num_Floors = 3;
  static constexpr unsigned Tile_Size = 2;
  static constexpr unsigned Chunk_Size = 34;
  static constexpr unsigned Map_Size = Chunk_Size * Num_Chunks;

  void print_map();

  std::array<std::array<std::array<Tile, Map_Size>, Map_Size>, Num_Floors> tiles;

public:
  MapSystem();

  void layout_map();
  void layout_room(
    const std::string& type, int x, int y, int size_x, int size_y, int floor);

  void set_tile(
    int x, int y, int floor,
    const std::string& type, const std::string& name,
    double rotation,
    bool solid = true);

  Tile& get_tile(int x, int y, int floor);
  const Tile& get_tile(int x, int y, int floor) const;
  Tile& get_tile(double x, double y, int floor);

  bool is_solid(double x, double y, int floor);

  int get_num_chunks() const { return Num_Chunks; }
  int get_num_floors() const { return Num_Floors; }
  int get_size() const { return Map_Size; }
  int get_tile_size() const { return Tile_Size; }
  int get_chunk_size() const { return Chunk_Size; }
};

}

#endif /* MAPSYSTEM_H */
