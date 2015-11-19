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
  void scan_collisions(DynamicEntity& user, const osg::Vec2& start);
  void collision_pass(
    DynamicEntity& user, const osg::Vec2& start, int tile_x, int tile_y);
  void resolve_collision(
    DynamicEntity& user, const osg::Vec2& start, const osg::Vec2& tile_pos);
  bool tile_contains(const osg::Vec2& point, const osg::Vec2& tile_pos);

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
