#ifndef ENTITYSYSTEM_H
#define ENTITYSYSTEM_H

#include <osg/Node>
#include "../components/DynamicEntity.h"
#include "../systems/RenderSystem.h"

namespace ld
{

class EntitySystem
{
  std::map<std::string, DynamicEntity> users;

  RenderSystem& render_system;

public:
  EntitySystem(RenderSystem& render_system);

  DynamicEntity& get_user(const std::string& name) { return users[name]; }
};

}


#endif /* ENTITYSYSTEM_H */
