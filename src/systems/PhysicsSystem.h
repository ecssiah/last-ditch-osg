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

struct Intersection
{
  Intersection(float time_, float x, float y, float nx, float ny, float ix, float iy)
    : time(time_),
      position(x, y),
      normal(nx, ny),
      intersection(ix, iy)
  {}

  float time;
  osg::Vec2 position;
  osg::Vec2 normal;
  osg::Vec2 intersection;
};


class PhysicsSystem
{
  void simulate(DynamicEntity& user, double dt);
  void scan_collisions(DynamicEntity& user, const osg::Vec2& start);
  Intersection* collision_pass(
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
