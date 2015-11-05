#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include <osg/PositionAttitudeTransform>
#include "../components/Input.h"
#include "../systems/RenderSystem.h"

namespace ld
{

class PhysicsSystem
{
public:
  PhysicsSystem(Input& input, RenderSystem& render_system);

  void update();

private:
  Input& input;
  RenderSystem& render_system;

  osg::ref_ptr<osg::PositionAttitudeTransform> user_xform;

};

}

#endif /* PHYSICSSYSTEM_H */
