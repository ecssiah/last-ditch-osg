#include "MapSystem.h"

#include <iostream>
#include "../Constants.h"

using namespace std;
using namespace ld;

MapSystem::MapSystem()
  : tiles(
      MAP_SIZE_X, std::vector<std::vector<Tile>>(
	MAP_SIZE_Y, std::vector<Tile>(
	  MAP_SIZE_Z, Tile())))
{
  build_map();
}


void MapSystem::build_map()
{
  build_room(0, 0, 0, 10, 10, 1);

}

void MapSystem::build_room(
  int x_, int y_, int z_,
  unsigned size_x, unsigned size_y, unsigned size_z)
{
  for (int x = x_; x < size_x; ++x)
  {
    tiles[x][y_][z_].type = "a";
    tiles[x][y_][z_].name = "wall";
    tiles[x][y_][z_].rotation = 90.f;

    for (int y = y_; y < size_y; ++y)
    {

    }
  }
}
