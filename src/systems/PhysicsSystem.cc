#include "PhysicsSystem.h"

#include <iostream>

using namespace ld;

PhysicsSystem::PhysicsSystem(Input& input_, EntitySystem& entity_system_)
  : input(input_),
    entity_system(entity_system_)
{
}


void PhysicsSystem::update()
{
  using namespace osg;

  DynamicEntity& user = entity_system.get_user("kadijah");
  Vec3 velocity;

  if (input.forward) velocity += Vec3(0, 1, 0);
  if (input.backward) velocity += Vec3(0, -1, 0);
  if (input.left) velocity += Vec3(-1, 0, 0);
  if (input.right) velocity += Vec3(1, 0, 0);
  if (input.up) velocity += Vec3(0, 0, 1);
  if (input.down) velocity += Vec3(0, 0, -1);

  velocity.normalize();

  Matrix m = user.xform->getMatrix();
  Matrix t;
  t.makeTranslate(velocity * .01);

  user.xform->setMatrix(m * t);
}
