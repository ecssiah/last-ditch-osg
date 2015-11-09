#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include "../components/Input.h"

namespace ld
{

class PhysicsSystem
{
public:
  PhysicsSystem(Input& input);

  void update();

private:
  Input& input;
};

}

#endif /* PHYSICSSYSTEM_H */
