#include "PhysicsSystem.h"

#include <iostream>
#include <limits>
#include <algorithm>

using namespace ld;
using namespace osg;

PhysicsSystem::PhysicsSystem(
  Input& input_,
  EntitySystem& entity_system_,
  MapSystem& map_system_
)
  : input(input_),
    entity_system(entity_system_),
    map_system(map_system_)
{
}


void PhysicsSystem::update(double dt)
{
  auto& user = entity_system.get_user("kadijah");

  simulate(user, dt);
}


void PhysicsSystem::simulate(DynamicEntity& user, double dt)
{
  Vec3 direction;
  if (input.forward) direction += Vec3(0, -1, 0);
  if (input.backward) direction += Vec3(0, 1, 0);
  if (input.left) direction += Vec3(1, 0, 0);
  if (input.right) direction += Vec3(-1, 0, 0);
  if (input.up) direction += Vec3(0, 0, 1);
  if (input.down) direction += Vec3(0, 0, -1);

  direction.normalize();

  Quat user_heading(user.heading, Vec3(0, 0, 1));
  Vec2 start(user.position.x(), user.position.y());
  Vec3 velocity(user_heading * direction * user.speed);

  user.position += velocity * dt;

  scan_collisions(user, start);

  Matrix r, t;
  r.makeRotate(user_heading);
  t.makeTranslate(user.position);

  user.xform->setMatrix(r * t);
}


void PhysicsSystem::scan_collisions(DynamicEntity& user, const Vec2& start)
{
  int x = round(user.position.x());
  int y = round(user.position.y());

  for (int xx = x - 1; xx <= x + 1; ++xx)
  {
    for (int yy = y - 1; yy <= y + 1; ++yy)
    {
      if (map_system.get_tile(xx, yy, 0).solid)
      {
	resolve_collision(user, start, xx, yy);
      }
    }
  }
}


void PhysicsSystem::resolve_collision(
  DynamicEntity& user, const osg::Vec2& start, int x, int y)
{

}
