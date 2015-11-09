#ifndef DYNAMICENTITY_H
#define DYNAMICENTITY_H

#include <osg/Node>
#include <osg/MatrixTransform>

struct DynamicEntity
{
  DynamicEntity()
    : name(""),
      xform(),
      velocity()
  {}

  std::string name;
  osg::ref_ptr<osg::MatrixTransform> xform;
  osg::Vec3 velocity;
};

#endif /* DYNAMICENTITY_H */
