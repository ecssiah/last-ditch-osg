#include "EntitySystem.h"

EntitySystem::EntitySystem()
{

}


void EntitySystem::add_user(DynamicEntity user)
{
  users[user.name] = user;
}
