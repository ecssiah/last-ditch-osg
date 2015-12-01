#include "PhysicsSystem.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>
#include <algorithm>
#include "../Debug.h"

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
  Vec3 velocity(user_heading * direction * user.speed);

  user.position += velocity * dt;

  scan_collisions(user);

  std::ostringstream ss;
  ss.precision(1);
  ss.setf(std::ios::fixed);
  ss << TILE_SIZE * user.position.x() << " " << TILE_SIZE * user.position.y();

  Debug::instance().msg = ss.str();

  Matrix r, t;
  r.makeRotate(user_heading);
  t.makeTranslate(user.position * TILE_SIZE);

  user.xform->setMatrix(r * t);
}


void PhysicsSystem::scan_collisions(DynamicEntity& user)
{
  int x = round(user.position.x());
  int y = round(user.position.y());

  for (int xx = x - 1; xx <= x + 1; ++xx)
    for (int yy = y - 1; yy <= y + 1; ++yy)
      if (map_system.get_tile(xx, yy, 0).solid)
	resolve_collision(user, xx, yy);
}


void PhysicsSystem::resolve_collision(
  DynamicEntity& user, int x, int y)
{
  Vec2f tile_pos(x, y);
  Vec2f user_pos(user.position.x(), user.position.y());
  Vec2f nearest(user_pos);
  Vec2f min(tile_pos.x() - TILE_SIZE / 4, tile_pos.y() - TILE_SIZE / 4);
  Vec2f max(tile_pos.x() + TILE_SIZE / 4, tile_pos.y() + TILE_SIZE / 4);

  if (nearest.x() < min.x()) nearest.x() = min.x();
  else if (nearest.x() > max.x()) nearest.x() = max.x();

  if (nearest.y() < min.y()) nearest.y() = min.y();
  else if (nearest.y() > max.y()) nearest.y() = max.y();

  Vec2f norm(user_pos - nearest);
  double dist = norm.normalize();
  double depth = USER_RADIUS - dist;

  if (dist < USER_RADIUS) user.position += Vec3d(norm.x(), norm.y(), 0) * depth;
}
