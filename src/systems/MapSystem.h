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

  void layout_foundation();
  void layout_map();
  void layout_room(
    const std::string& type, int x, int y, int size_x, int size_y, int floor);

  void set_tile(
    int x, int y, int floor,
    const std::string& type, const std::string& name,
    double rotation);

  Tile& get_tile(int x, int y, int floor);
  const Tile& get_tile(int x, int y, int floor) const;

private:
  std::vector<std::vector<std::vector<Tile>>> tiles;
};

}

#endif /* MAPSYSTEM_H */
