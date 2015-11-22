#include "PhysicsSystem.h"

#include <iostream>
#include <limits>

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

  Vec2 start(user.position.x(), user.position.y());

  user.position += displacement;

  scan_collisions(user, start);

  Matrix r;
  r.makeRotate(user_heading);

  Matrix t;
  t.makeTranslate(user.position);

  user.xform->setMatrix(r * t);
}


void PhysicsSystem::scan_collisions(DynamicEntity& user, const Vec2& start)
{
  const int x = round(user.position.x());
  const int y = round(user.position.y());

  for (int xx = x - 1; xx <= x + 1; ++xx)
  {
    for (int yy = y - 1; yy <= y + 1; ++yy)
    {
      if (map_system.get_tile(xx, yy, 0).solid)
      {
	Intersection* inter = collision_pass(user, start, xx, yy);

	if (inter)
	{
	  float remaining_time = 1 - inter->time;
	  float dx = user.position.x() - start.x();
	  float dy = user.position.y() - start.y();
	  float dot = dx * inter->normal.x() + dy * inter->normal.y();
	  float ndx = dx - 2 * dot * inter->normal.x();
	  float ndy = dy - 2 * dot * inter->normal.y();
	  float newx = inter->position.x() + ndx * remaining_time;
	  float newy = inter->position.y() + ndy * remaining_time;

	  user.position = Vec3(newx, newy, 0);
	}
      }
    }
  }
}

Intersection* PhysicsSystem::collision_pass(
  DynamicEntity& user, const Vec2& start, int tile_x, int tile_y)
{
  float dx = user.position.x() - start.x();
  float dy = user.position.y() - start.y();

  if (dx == 0 || dy == 0) return nullptr;

  float R = tile_x + .5;
  float L = tile_x - .5;
  float T = tile_y + .5;
  float B = tile_y - .5;

  float rtime = std::numeric_limits<float>::max();
  float ltime = std::numeric_limits<float>::max();
  float ttime = std::numeric_limits<float>::max();
  float btime = std::numeric_limits<float>::max();

  if (start.x() - USER_RADIUS < L && user.position.x() + USER_RADIUS > L)
    ltime = ((L - USER_RADIUS) - start.x()) / dx;
  if (start.x() + USER_RADIUS > R && user.position.x() - USER_RADIUS < R)
    rtime = (start.x() - (R + USER_RADIUS)) / -dx;
  if (start.y() - USER_RADIUS < T && user.position.y() + USER_RADIUS > T)
    ttime = ((T - USER_RADIUS) - start.y()) / dy;
  if (start.y() + USER_RADIUS > B && user.position.y() - USER_RADIUS < B)
    btime = (start.y() - (B + USER_RADIUS)) / -dy;

  if (ltime >= 0.f && ltime <= 1.f)
  {
    float ly = dy * ltime + start.y();

    if (ly >= T && ly <= B)
      return new Intersection(ltime, dx * ltime + start.x(), ly, -1, 0);
  }
  else if (rtime >= 0.f && rtime <= 1.f)
  {
    float ry = dy * rtime + start.y();

    if (ry >= T && ry <= B)
      return new Intersection(rtime, dx * rtime + start.x(), ry, 1, 0);
  }

  if (ttime >= 0.f && ttime <= 1.f)
  {
    float tx = dx * ttime + start.x();

    if (tx >= L && tx <= R)
      return new Intersection(ttime, tx, dy * ttime + start.y(), 0, -1);
  }
  else if (btime >= 0.f && btime <= 1.f)
  {
    float bx = dx * btime + start.x();

    if (bx >= L && bx <= R)
      return new Intersection(btime, dx, dy * btime + start.y(), 0, 1);
  }

  float cornerx = std::numeric_limits<float>::max();
  float cornery = std::numeric_limits<float>::max();

  if (ltime != std::numeric_limits<float>::max()) cornerx = L;
  else if (rtime != std::numeric_limits<float>::max()) cornerx = R;

  if (ttime != std::numeric_limits<float>::max()) cornery = T;
  else if (btime != std::numeric_limits<float>::max()) cornery = B;

  bool x_side =
    cornerx != std::numeric_limits<float>::max() &&
    cornery == std::numeric_limits<float>::max();

  if (x_side) cornery = (dy > 0 ? B : T);

  bool y_side =
    cornery != std::numeric_limits<float>::max() &&
    cornerx == std::numeric_limits<float>::max();

  if (y_side) cornerx = (dx > 0 ? R : L);

  double inverse_radius = 1.0 / USER_RADIUS;
  double line_length = sqrt(dx * dx + dy * dy);
  double corner_dx = cornerx - user.position.x();
  double corner_dy = cornery - user.position.y();
  double corner_dist = sqrt(corner_dx * corner_dx + corner_dy * corner_dy);
  double inner_angle = acos((corner_dx * dx + corner_dy * dy) / (line_length * corner_dist));
  double inner_angle_sin = sin(inner_angle);
  double angle_sin = inner_angle_sin * corner_dist * inverse_radius;

  if (fabs(angle_sin) > 1) return nullptr;

  double angle1 = M_PI - asin(angle_sin);
  double angle2 = M_PI - inner_angle - angle1;
  double intersection_distance = USER_RADIUS * sin(angle2) / inner_angle_sin;

  float time = (float)(intersection_distance / line_length);

  if (time > 1 || time < 0) return nullptr;

  float ix = time * dx + start.x();
  float iy = time * dy + start.y();
  float nx = (float)((ix - cornerx) * inverse_radius);
  float ny = (float)((iy - cornery) * inverse_radius);

  return new Intersection(time, ix, iy, nx, ny);
}


void PhysicsSystem::resolve_collision(
  DynamicEntity& user, const Vec2& start, const Vec2& tile_pos)
{
}


bool PhysicsSystem::tile_contains(const Vec2& point, const Vec2& tile_pos)
{
  bool contained =
    (point.x() > tile_pos.x() - .5) &&
    (point.x() < tile_pos.x() + .5) &&
    (point.y() > tile_pos.y() - .5) &&
    (point.y() < tile_pos.y() + .5);

  return contained;
}
