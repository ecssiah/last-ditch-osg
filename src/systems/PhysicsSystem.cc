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
  t.makeTranslate(user.position * TILE_SIZE);

  user.xform->setMatrix(r * t);
}


void PhysicsSystem::resolve_collisions(DynamicEntity& user, Vec3& displacement)
{
  int x = round(user.position.x());
  int y = round(user.position.y());

  user.position += displacement;

  if (displacement.x() > 0)
  {
    for (int yy = y - 1; yy <= y + 1; ++yy)
    {
      if (map_system.is_solid(x + 1, yy, user.floor))
      {
	collision_pass(user, x + 1, yy);
      }
    }
  }
  else
  {
    for (int yy = y - 1; yy <= y + 1; ++yy)
    {
      if (map_system.is_solid(x - 1, yy, user.floor))
      {
	collision_pass(user, x - 1, yy);
      }
    }
  }

  if (displacement.y() > 0)
  {
    for (int xx = x - 1; xx <= x + 1; ++xx)
    {
      if (map_system.is_solid(xx, y + 1, user.floor))
      {
	collision_pass(user, xx, y + 1);
      }
    }
  }
  else
  {
    for (int xx = x - 1; xx <= x + 1; ++xx)
    {
      if (map_system.is_solid(xx, y - 1, user.floor))
      {
	collision_pass(user, xx, y - 1);
      }
    }
  }
}


void PhysicsSystem::collision_pass(
  DynamicEntity& user, int center_x, int center_y)
{
  std::cout << center_x << " " << center_y << std::endl;

  Vec2 tile_pos(center_x, center_y);
  Vec2 user_pos(user.position.x(), user.position.y());

  double sq_length = (tile_pos - user_pos).length2();

  if (sq_length > pow(TILE_OUTER_RADIUS + USER_RADIUS, 2)) return;

  if (sq_length < pow(TILE_INNER_RADIUS + USER_RADIUS, 2))
  {
    Vec2 ctr_2_ctr = user_pos - tile_pos;
    ctr_2_ctr.normalize();

    Vec2 point = user_pos + ctr_2_ctr * USER_RADIUS;

    if (tile_contains(point, tile_pos))
    {
      resolve_collision(user, tile_pos);
    }
  }
}


void PhysicsSystem::resolve_collision(DynamicEntity& user, Vec2& tile_pos)
{
  std::cout << tile_pos.x() << " " << tile_pos.y() << std::endl;
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
