#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include "../Constants.h"
#include "../components/Input.h"
#include "../systems/EntitySystem.h"

namespace ld
{

class PhysicsSystem
{
public:
  PhysicsSystem(Input& input, EntitySystem& entity_system);

  void update(double dt);

private:
  Input& input;
  EntitySystem& entity_system;
};

}

#endif /* PHYSICSSYSTEM_H */
