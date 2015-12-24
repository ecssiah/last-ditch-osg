#ifndef RECT_H
#define RECT_H

namespace ld
{

struct Rect
{
  Rect(int x_, int y_, int w_, int h_)
    : x(x_),
      y(y_),
      w(w_),
      h(h_)
  {}

  int x, y;
  int w, h;
};

}

#endif /* RECT_H */
