#include "PhysicsSystem.h"

#include <iostream>

using namespace ld;

PhysicsSystem::PhysicsSystem(Input& input_, RenderSystem& render_system_)
  : input(input_),
    render_system(render_system_)
{
  user_xform = render_system.get_user_xform();

}


void PhysicsSystem::update()
{
  using namespace osg;

  Vec3 position = user_xform->getPosition();
  Vec3 dir;

  if (input.forward)
    dir += Vec3(0, 1, 0);
  if (input.backward)
    dir += Vec3(0, -1, 0);
  if (input.left)
    dir += Vec3(-1, 0, 0);
  if (input.right)
    dir += Vec3(1, 0, 0);
  if (input.up)
    dir += Vec3(0, 0, 1);
  if (input.down)
    dir += Vec3(0, 0, -1);

  dir.normalize();
  dir *= .01;

  user_xform->setPosition(position + dir);
}
