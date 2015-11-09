#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <osg/MatrixTransform>

struct Entity
{
  Entity()
    : name(""),
      xform()
  {}

  std::string name;
  osg::MatrixTransform xform;
};

#endif /* ENTITY_H */
