#ifndef ENTITYSYSTEM_H
#define ENTITYSYSTEM_H

#include <osg/Node>
#include "../components/DynamicEntity.h"

class EntitySystem
{
public:
  EntitySystem();

  void add_entity(const std::string& name, DynamicEntity entity);

private:
  std::map<std::string, DynamicEntity> dynamic_entities;

};


#endif /* ENTITYSYSTEM_H */
