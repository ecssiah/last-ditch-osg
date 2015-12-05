#ifndef ROOM_H
#define ROOM_H

struct Room
{
  Room(int x_, int y_, int w_, int h_)
    : x(x_),
      y(y_),
      w(w_),
      h(h_)
  {}

  int x, y;
  int w, h;
};

#endif /* ROOM_H */
