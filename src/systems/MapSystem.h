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

  void init();
  void update();

  const Tile& get_tile(int x, int y, int z) const { return tiles[x][y][z]; }

private:
  std::vector<std::vector<std::vector<Tile>>> tiles;
};

}

#endif /* MAPSYSTEM_H */
