#ifndef DYNAMICENTITY_H
#define DYNAMICENTITY_H

#include <osg/Node>
#include <osg/MatrixTransform>
#include "../Constants.h"

struct DynamicEntity
{
  DynamicEntity()
    : name(""),
      xform(),
      position(),
      floor(0),
      speed(USER_SPEED),
      x_rot_speed(USER_X_ROT_SPEED),
      y_rot_speed(USER_Y_ROT_SPEED),
      heading(M_PI),
      pitch(0)
  {}

  std::string name;
  osg::ref_ptr<osg::MatrixTransform> xform;
  osg::Vec3 position;
  int floor;
  double speed, x_rot_speed, y_rot_speed;
  double heading, pitch;
};

#endif /* DYNAMICENTITY_H */
