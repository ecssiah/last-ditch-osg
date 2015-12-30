#ifndef ENTITYSYSTEM_H
#define ENTITYSYSTEM_H

#include <string>
#include <random>
#include <osg/Node>
#include "MapSystem.h"
#include "../components/Door.h"
#include "../components/DynamicEntity.h"
#include "../components/Input.h"

namespace ld
{

class EntitySystem
{
  void setup_users();
  void setup_doors();
  void create_door(
    int x, int y, int floor, std::string type, std::string name, double rotation);

  std::mt19937& rng;

  std::map<std::string, DynamicEntity> users;
  std::array<std::vector<Door>, NUM_FLOORS> doors;

  Input& input;
  MapSystem& map_system;

public:
  EntitySystem(std::mt19937& rng, Input& input, MapSystem& map_system);

  void update();

  DynamicEntity& get_user(const std::string& name) { return users[name]; }

  const std::array<std::vector<Door>, NUM_FLOORS>& get_doors() const { return doors; }
};

}


#endif /* ENTITYSYSTEM_H */
