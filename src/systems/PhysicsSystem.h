#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include <osg/Vec3>
#include "../Constants.h"
#include "../components/Input.h"
#include "../components/DynamicEntity.h"
#include "../systems/EntitySystem.h"
#include "../systems/MapSystem.h"

namespace ld
{

class PhysicsSystem
{
  void simulate(DynamicEntity& user, double dt);
  void resolve_collisions(DynamicEntity& user, osg::Vec3& displacement);

  Input& input;
  EntitySystem& entity_system;
  MapSystem& map_system;

public:
  PhysicsSystem(
    Input& input,
    EntitySystem& entity_system,
    MapSystem& map_system);

  void update(double dt);
};

}

#endif /* PHYSICSSYSTEM_H */
