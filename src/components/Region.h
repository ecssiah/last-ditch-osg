#ifndef REGION_H
#define REGION_H

#include "Rect.h"
#include "UsableObject.h"

namespace ld
{

struct Region : public Rect
{
  Region(int x, int y, int w, int h, UsableObject* object_ = nullptr)
    : Rect(x, y, w, h),
      object(object_)
  {}

  UsableObject* object;
};

}

#endif /* REGION_H */
