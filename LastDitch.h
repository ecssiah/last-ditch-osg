#ifndef LASTDITCH_H
#define LASTDITCH_H

#include <random>
#include <osg/Group>
#include "src/components/Input.h"
#include "src/systems/TimeSystem.h"
#include "src/systems/MapSystem.h"
#include "src/systems/EntitySystem.h"
#include "src/systems/PhysicsSystem.h"
#include "src/systems/RenderSystem.h"
#include "src/systems/CameraSystem.h"

namespace ld
{

class LastDitch
{
  osg::ref_ptr<osg::Group> root;

  Input input;

  std::mt19937 rng;

  TimeSystem time_system;
  MapSystem map_system;
  EntitySystem entity_system;
  PhysicsSystem physics_system;
  RenderSystem render_system;
  CameraSystem camera_system;

public:
  LastDitch();
};

}

#endif /* LASTDITCH_H */
