#ifndef ENTITYSYSTEM_H
#define ENTITYSYSTEM_H

#include <osg/Node>
#include "../components/DynamicEntity.h"

class EntitySystem
{
public:
  EntitySystem();

  void add_user(DynamicEntity entity);
  DynamicEntity& get_user(const std::string& name) { return users[name]; }

private:
  std::map<std::string, DynamicEntity> users;

};


#endif /* ENTITYSYSTEM_H */
