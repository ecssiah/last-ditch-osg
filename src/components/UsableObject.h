#ifndef USABLEOBJECT_H
#define USABLEOBJECT_H

namespace ld
{

struct UsableObject
{
  UsableObject()
    : activated(false)
  {}

  bool activated;
};

}

#endif /* USABLEOBJECT_H */
