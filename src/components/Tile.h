#ifndef TILE_H
#define TILE_H

#include <iostream>
#include <osg/Vec3>

namespace ld
{

struct Tile
{
  bool solid = false;

  std::string type = "a";
  std::string name = "";

  osg::Vec3 position = osg::Vec3();

  float rotation = 0;
};

}

#endif /* TILE_H */
