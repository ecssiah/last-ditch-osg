#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include <osg/Vec2>
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
  void scan_collisions(DynamicEntity& user);
  void resolve_collision(DynamicEntity& user, int x, int y);

  double cosine_interp(double v1, double v2, double t);
  osg::Vec3d cosine_interp(osg::Vec3 v1, osg::Vec3 v2, double t);

  const double tile_radius;

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
