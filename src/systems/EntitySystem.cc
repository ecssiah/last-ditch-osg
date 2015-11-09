#include "EntitySystem.h"

#include <iostream>
#include "../components/DynamicEntity.h"

using namespace ld;

EntitySystem::EntitySystem(RenderSystem& render_system_)
  : render_system(render_system_)
{
  DynamicEntity user;
  user.name = "kadijah";
  user.xform = render_system.get_user_xform(user.name);

  users[user.name] = user;
}


void EntitySystem::add_user(DynamicEntity user)
{
  users[user.name] = user;
}
