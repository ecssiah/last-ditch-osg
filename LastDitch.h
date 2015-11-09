#ifndef LASTDITCH_H
#define LASTDITCH_H

#include <osg/Group>
#include "src/components/Input.h"
#include "src/systems/MapSystem.h"
#include "src/systems/RenderSystem.h"
#include "src/systems/EntitySystem.h"
#include "src/systems/PhysicsSystem.h"
#include "src/systems/CameraSystem.h"

namespace ld
{

class LastDitch
{
public:
  LastDitch();

private:
  osg::ref_ptr<osg::Group> root;

  Input input;

  MapSystem map_system;
  RenderSystem render_system;
  EntitySystem entity_system;
  PhysicsSystem physics_system;
  CameraSystem camera_system;
};

}

#endif /* LASTDITCH_H */
