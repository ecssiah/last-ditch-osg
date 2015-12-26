#ifndef ROOM_H
#define ROOM_H

#include "Rect.h"

namespace ld
{

struct Room : public Rect
{
  Room(int x, int y, int w, int h, const Room* master_ = nullptr)
    : Rect(x, y, w, h),
      master(master_)
  {}

  bool operator==(Room& room) const
  {
    return x == room.x && y == room.y && w == room.w && h == room.h;
  }

  bool operator==(const Room& room) const
  {
    return x == room.x && y == room.y && w == room.w && h == room.h;
  }

  const Room* master;
};

}

#endif /* ROOM_H */
