#include "PhysicsSystem.h"

#include <iostream>

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
  DynamicEntity& user = entity_system.get_user("kadijah");

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
  Vec3 velocity(user_heading * direction * user.speed);
  Vec3 displacement(velocity * dt);

  resolve_collisions(user, displacement);

  Matrix r;
  r.makeRotate(user.heading, Vec3(0, 0, 1));

  Matrix t;
  t.makeTranslate(user.position);

  user.xform->setMatrix(r * t);
}


void PhysicsSystem::resolve_collisions(DynamicEntity& user, Vec3& displacement)
{
  Vec3 position(user.position);

  double xx = position.x() + displacement.x();
  double yy = position.y() + displacement.y();

  if (displacement.x() > 0)
  {
    if (map_system.is_solid(xx + USER_RADIUS, position.y(), user.floor))
      xx = (int)(xx + USER_RADIUS) - USER_RADIUS;
  }
  else
  {
    if (map_system.is_solid(xx - USER_RADIUS, position.y(), user.floor))
      xx = (int)(xx - USER_RADIUS) + USER_RADIUS;
  }

  if (displacement.y() > 0)
  {
    if (map_system.is_solid(position.x(), yy + USER_RADIUS, user.floor))
      yy = (int)(yy + USER_RADIUS) - USER_RADIUS;
  }
  else
  {
    if (map_system.is_solid(position.x(), yy - USER_RADIUS, user.floor))
      yy = (int)(yy - USER_RADIUS) + USER_RADIUS;
  }

  user.position.set(xx, yy, user.floor);
}
