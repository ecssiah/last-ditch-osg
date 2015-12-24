#ifndef DOOR_H
#define DOOR_H

#include <string>
#include "UsableObject.h"

namespace ld
{

struct Door : public UsableObject
{
  Door(int x_, int y_, std::string type_, std::string name_, double rotation_)
    : x(x_),
      y(y_),
      type(type_),
      name(name_),
      rotation(rotation_),
      locked(false),
      open(false)
  {}

  int x, y;
  std::string type, name;
  float rotation;
  bool locked, open;
};

}

#endif /* DOOR_H */
