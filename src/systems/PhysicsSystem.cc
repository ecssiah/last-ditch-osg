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
  Vec2 input_direction;

  if (input.forward) input_direction += Vec2(0, -1);
  if (input.backward) input_direction += Vec2(0, 1);
  if (input.left) input_direction += Vec2(1, 0);
  if (input.right) input_direction += Vec2(-1, 0);

  Vec2 direction(
    input_direction.x() * cos(user.heading) - input_direction.y() * sin(user.heading),
    input_direction.x() * sin(user.heading) + input_direction.y() * cos(user.heading));

  Vec2 velocity(direction * user.speed);
  Vec2 displacement(velocity * dt);
  Vec2 start(user.position);

  user.position += displacement;

  scan_collisions(user, start);

  Matrix r;
  r.makeRotate(user.heading, Vec3(0, 0, 1));

  Vec3 position(
    user.position.x() * TILE_SIZE,
    user.position.y() * TILE_SIZE,
    user.floor * FLOOR_HEIGHT);

  Matrix t;
  t.makeTranslate(position);

  user.xform->setMatrix(r * t);
}


void PhysicsSystem::scan_collisions(DynamicEntity& user, const Vec2& start)
{
  const int x = round(user.position.x());
  const int y = round(user.position.y());
}

void PhysicsSystem::collision_pass(
  DynamicEntity& user, const Vec2& start, int tile_x, int tile_y)
{
}


void PhysicsSystem::resolve_collision(
  DynamicEntity& user, const Vec2& start, const Vec2& tile_pos)
{
}


bool PhysicsSystem::tile_contains(const Vec2& point, const Vec2& tile_pos)
{
  bool contained =
    (point.x() > tile_pos.x() - TILE_SIZE / 2.0) &&
    (point.x() < tile_pos.x() + TILE_SIZE / 2.0) &&
    (point.y() > tile_pos.y() - TILE_SIZE / 2.0) &&
    (point.y() < tile_pos.y() + TILE_SIZE / 2.0);

  return contained;
}
