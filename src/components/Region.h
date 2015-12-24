#ifndef REGION_H
#define REGION_H

#include <osg/Vec3>

namespace ld
{

struct Region
{
  Region(int x_, int y_, int w_, int h_)
    : x(x_),
      y(y_),
      w(w_),
      h(h_)
  {}

  int x, y;
  int w, h;
};

}

#endif /* REGION_H */
