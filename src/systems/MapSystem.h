#ifndef MAPSYSTEM_H
#define MAPSYSTEM_H

#include <string>
#include <vector>
#include "../components/Tile.h"

namespace ld
{

class MapSystem
{
public:
  MapSystem();

  void build_map();
  void build_room(
    int x, int y, int z,
    unsigned size_x, unsigned size_y, unsigned size_z);

  const Tile& get_tile(int x, int y, int z) const { return tiles[x][y][z]; }

private:
  std::vector<std::vector<std::vector<Tile>>> tiles;
};

}

#endif /* MAPSYSTEM_H */
