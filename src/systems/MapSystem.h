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

  void layout_map();
  void layout_room(
    int x, int y,
    unsigned size_x, unsigned size_y,
    int floor);

  const Tile& get_tile(int x, int y, int floor) const;

private:
  std::vector<std::vector<std::vector<Tile>>> tiles;
};

}

#endif /* MAPSYSTEM_H */
