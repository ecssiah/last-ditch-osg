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

  Quat user_heading_quat(user.heading, Vec3(0, 0, 1));
  Vec3 velocity(user_heading_quat * direction * user.speed);
  Vec3 displacement(velocity * dt);

  Vec2 start(user.position.x(), user.position.y());

  user.position += displacement;

  scan_collisions(user, start);

  Matrix r;
  r.makeRotate(user_heading_quat);

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
	  float remaining_time = 1.f - inter->time;
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
  float R = tile_x + .5;
  float L = tile_x - .5;
  float T = tile_y + .5;
  float B = tile_y - .5;

  bool bounding_box_check =
    (std::max(start.x(), user.position.x()) + USER_RADIUS < L) ||
    (std::min(start.x(), user.position.x()) - USER_RADIUS > R) ||
    (std::max(start.y(), user.position.y()) + USER_RADIUS < T) ||
    (std::min(start.y(), user.position.y()) - USER_RADIUS > B);

  if (bounding_box_check) return nullptr;

  float dx = user.position.x() - start.x();
  float dy = user.position.y() - start.y();
  float invdx = (dx == 0.f ? 0.f : 1.f / dx);
  float invdy = (dy == 0.f ? 0.f : 1.f / dy);
  float cornerX = std::numeric_limits<float>::max();
  float cornerY = std::numeric_limits<float>::max();

  // left side
  if (start.x() + USER_RADIUS < L && user.position.x() + USER_RADIUS > L)
  {
    float time = (L - start.x()) * invdx;

    if (time >= 0.f && time <= 1.f)
    {
      float x = dx * time + start.x();
      float y = dy * time + start.y();

      if (y <= T && y >= B) return new Intersection(time, x, y, -1, 0, L, y);
    }
    cornerX = L;
  }

  // right side
  if (start.x() - USER_RADIUS > R && user.position.x() - USER_RADIUS < R)
  {
    float time = (start.x() - R) * -invdx;

    if (time >= 0.f && time <= 1.f)
    {
      float x = dx * time + start.x();
      float y = dy * time + start.y();

      if (y <= T && y >= B) return new Intersection(time, x, y, 1, 0, R, y);
    }
    cornerX = R;
  }

  // bottom side
  if (start.y() + USER_RADIUS < B && user.position.y() + USER_RADIUS > B)
  {
    float time = (B - start.y()) * invdy;

    if (time >= 0.f && time <= 1.f)
    {
      float x = dx * time + start.x();
      float y = dy * time + start.y();

      if (x >= L && x <= R) return new Intersection(time, x, y, 0, -1, x, B);
    }
    cornerY = B;
  }

  // top side
  if (start.y() - USER_RADIUS > T && user.position.y() - USER_RADIUS < T)
  {
    float time = (start.y() - T) * -invdy;

    if (time >= 0.f && time <= 1.f)
    {
      float x = dx * time + start.x();
      float y = dy * time + start.y();

      if (x >= L && x <= R) return new Intersection(time, x, y, 0, 1, x, T);
    }
    cornerY = T;
  }

  bool no_intersection =
    cornerX == std::numeric_limits<float>::max() &&
    cornerY == std::numeric_limits<float>::max();

  if (no_intersection) return nullptr;

  bool no_x_intersection =
    cornerX != std::numeric_limits<float>::max() &&
    cornerY == std::numeric_limits<float>::max();

  if (no_x_intersection) cornerY = (dy > 0 ? B : T);

  bool no_y_intersection =
    cornerY != std::numeric_limits<float>::max() &&
    cornerX == std::numeric_limits<float>::max();

  if (no_y_intersection) cornerX = (dx > 0 ? L : R);

  double inverse_radius = 1.0 / USER_RADIUS;
  double line_length = sqrt(dx * dx + dy * dy);
  double corner_dx = cornerX - user.position.x();
  double corner_dy = cornerY - user.position.y();
  double corner_distance = sqrt(corner_dx * corner_dx + corner_dy * corner_dy);
  double inner_angle =
    acos((corner_dx * dx + corner_dy * dy) / (line_length * corner_distance));

  if (corner_distance < USER_RADIUS) return nullptr;

  if (inner_angle == 0.f)
  {
    float time = (float)((corner_distance - USER_RADIUS) / line_length);

    if (time > 1.f || time < 0.f) return nullptr;

    float ix = time * dx + start.x();
    float iy = time * dy + start.y();
    float nx = (float)(corner_dx / corner_distance);
    float ny = (float)(corner_dy / corner_distance);

    return new Intersection(time, ix, iy, nx, ny, cornerX, cornerY);
  }

  double inner_angle_sin = sin(inner_angle);
  double angle1_sin = inner_angle_sin * corner_distance * inverse_radius;

  if (fabs(angle1_sin) > 1.f) return nullptr;

  double angle1 = M_PI - asin(angle1_sin);
  double angle2 = M_PI - inner_angle - angle1;
  double intersection_distance = USER_RADIUS * sin(angle2) / inner_angle_sin;

  float time = (float)(intersection_distance / line_length);

  if (time > 1.f || time < 0.f) return nullptr;

  float ix = time * dx + start.x();
  float iy = time * dy + start.y();
  float nx = (float)((ix - cornerX) * inverse_radius);
  float ny = (float)((iy - cornerY) * inverse_radius);

  return new Intersection(time, ix, iy, nx, ny, cornerX, cornerY);
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
