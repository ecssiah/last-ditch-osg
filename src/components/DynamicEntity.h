#ifndef DYNAMICENTITY_H
#define DYNAMICENTITY_H

#include <osg/Node>
#include <osg/MatrixTransform>
#include "../Constants.h"

namespace ld
{

struct DynamicEntity
{
  DynamicEntity()
    : name(""),
      xform(),
      position(),
      start(),
      target(),
      speed(USER_SPEED),
      x_rot_speed(USER_X_ROT_SPEED),
      y_rot_speed(USER_Y_ROT_SPEED),
      heading(M_PI),
      pitch(0.0),
      inactive_time(0.0),
      collision_active(true)
  {}

  bool collision_active;
  std::string name;
  osg::ref_ptr<osg::MatrixTransform> xform;
  osg::Vec3 position;
  osg::Vec3 start, target;
  double speed, x_rot_speed, y_rot_speed;
  double heading, pitch;
  double inactive_time;
};

}

#endif /* DYNAMICENTITY_H */
