#include "EntitySystem.h"

EntitySystem::EntitySystem()
{

}


void EntitySystem::add_entity(
  const std::string& name, DynamicEntity entity)
{
  dynamic_entities[name] = entity;
}
