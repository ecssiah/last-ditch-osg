#include "LastDitch.h"

#include <unistd.h>
#include "src/Constants.h"
#include "src/InputAdapter.h"

using namespace ld;
using namespace osg;

LastDitch::LastDitch()
  : root(new Group),
    input(),
    time_system(),
    map_system(),
    render_system(root, map_system),
    entity_system(render_system),
    physics_system(input, entity_system),
    camera_system(root, input, entity_system)
{
  while (camera_system.is_running())
  {
    auto dt = time_system.tick();

    physics_system.update(dt);
    camera_system.update();
  }
}


int main()
{
  LastDitch app;
}
