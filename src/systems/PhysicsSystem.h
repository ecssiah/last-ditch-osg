#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include "../components/Input.h"
#include "../systems/EntitySystem.h"

namespace ld
{

class PhysicsSystem
{
public:
  PhysicsSystem(Input& input, EntitySystem& entity_system);

  void update();

private:
  Input& input;
  EntitySystem& entity_system;
};

}

#endif /* PHYSICSSYSTEM_H */
