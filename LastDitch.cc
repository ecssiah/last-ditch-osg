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
    double dt = time_system.tick();

    for (int i = 0; i < time_system.get_iterations(); ++i)
    {
      physics_system.update(FIXED_TIMESTEP);
    }

    sleep(FIXED_TIMESTEP - dt);

    // physics_system.update(dt);
    camera_system.update();
  }
}


int main()
{
  LastDitch app;
}
