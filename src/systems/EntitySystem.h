#ifndef ENTITYSYSTEM_H
#define ENTITYSYSTEM_H

#include <osg/Node>
#include "../components/DynamicEntity.h"
#include "../systems/RenderSystem.h"

namespace ld
{

class EntitySystem
{
public:
  EntitySystem(RenderSystem& render_system);

  void add_user(DynamicEntity entity);
  DynamicEntity& get_user(const std::string& name) { return users[name]; }

private:
  std::map<std::string, DynamicEntity> users;

  RenderSystem& render_system;

};

}


#endif /* ENTITYSYSTEM_H */
