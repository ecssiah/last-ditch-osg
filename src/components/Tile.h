#ifndef TILE_H
#define TILE_H

#include <iostream>
#include <osg/Vec3>

namespace ld
{

struct Tile
{
  Tile()
    : solid(false),
      position(osg::Vec3()),
      type("a"),
      name(""),
      rotation(0.0),
      ceil_type("a"),
      ceil_name(""),
      ceil_rotation(0.0)
  {}

  bool solid;
  osg::Vec3 position;

  std::string type;
  std::string name;
  float rotation;

  std::string ceil_type;
  std::string ceil_name;
  float ceil_rotation;
};

}

#endif /* TILE_H */
