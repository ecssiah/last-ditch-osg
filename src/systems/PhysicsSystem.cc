#include "PhysicsSystem.h"

#include <iostream>

using namespace ld;
using namespace osg;

PhysicsSystem::PhysicsSystem(Input& input_, EntitySystem& entity_system_)
  : input(input_),
    entity_system(entity_system_)
{
}


void PhysicsSystem::update(double dt)
{
  DynamicEntity& user = entity_system.get_user("kadijah");
  Vec3 direction;

  if (input.forward) direction += Vec3(0, -1, 0);
  if (input.backward) direction += Vec3(0, 1, 0);
  if (input.left) direction += Vec3(1, 0, 0);
  if (input.right) direction += Vec3(-1, 0, 0);
  if (input.up) direction += Vec3(0, 0, 1);
  if (input.down) direction += Vec3(0, 0, -1);

  direction.normalize();

  Quat user_heading(user.heading, Vec3(0, 0, 1));
  user.position += user_heading * direction * user.speed * dt;

  Matrix r;
  r.makeRotate(user.heading, Vec3(0, 0, 1));

  Matrix t;
  t.makeTranslate(user.position);

  user.xform->setMatrix(r * t);
}
