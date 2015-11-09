#include "LastDitch.h"

#include "src/InputAdapter.h"

using namespace ld;
using namespace osg;

LastDitch::LastDitch()
  : root(new Group),
    input(),
    map_system(),
    render_system(root, map_system),
    entity_system(render_system),
    physics_system(input, entity_system),
    camera_system(root, input, entity_system)
{
  while (camera_system.is_running())
  {
    physics_system.update();
    camera_system.update();
  }
}


int main()
{
  LastDitch app;
}
