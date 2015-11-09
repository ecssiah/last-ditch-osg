#ifndef DYNAMICENTITY_H
#define DYNAMICENTITY_H

#include <osg/Node>
#include <osg/MatrixTransform>

struct DynamicEntity
{
  DynamicEntity()
    : name(""),
      xform(),
      position(),
      speed(.05),
      x_rot_speed(.003),
      y_rot_speed(.003),
      heading(0),
      pitch(0)
  {}

  std::string name;
  osg::ref_ptr<osg::MatrixTransform> xform;
  osg::Vec3 position;
  double speed, x_rot_speed, y_rot_speed;
  double heading, pitch;
};

#endif /* DYNAMICENTITY_H */
