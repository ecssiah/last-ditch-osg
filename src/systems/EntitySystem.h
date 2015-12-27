#ifndef ENTITYSYSTEM_H
#define ENTITYSYSTEM_H

#include <random>
#include <osg/Node>
#include "MapSystem.h"
#include "../components/Door.h"
#include "../components/DynamicEntity.h"

namespace ld
{

class EntitySystem
{
  void update();
  void setup_users();
  void setup_doors();

  std::mt19937& rng;

  std::map<std::string, DynamicEntity> users;
  std::array<std::vector<Door>, NUM_FLOORS> doors;

  MapSystem& map_system;

public:
  EntitySystem(std::mt19937& rng, MapSystem& map_system);

  DynamicEntity& get_user(const std::string& name) { return users[name]; }

  const std::array<std::vector<Door>, NUM_FLOORS>& get_doors() const { return doors; }
};

}


#endif /* ENTITYSYSTEM_H */
