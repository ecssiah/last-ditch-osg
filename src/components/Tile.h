#ifndef TILE_H
#define TILE_H

#include <iostream>

namespace ld
{

struct Tile
{
  bool solid = false;

  std::string type = "a";
  std::string name = "";

  float rotation = 0;
};

}

#endif /* TILE_H */
