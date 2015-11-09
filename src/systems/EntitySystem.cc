#include "EntitySystem.h"

EntitySystem::EntitySystem()
{

}


void EntitySystem::add_user(
  const std::string& name, DynamicEntity entity)
{
  users[name] = entity;
}
