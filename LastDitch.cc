#include "LastDitch.h"

#include <string>
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
    physics_system(input, entity_system, map_system),
    camera_system(root, input, entity_system)
{
  printf("Complete\n");

  while (camera_system.is_running())
  {
    auto dt = time_system.tick();

    physics_system.update(dt);
    camera_system.update();
  }
}


int main()
{
  setbuf(stdout, NULL);

  printf("Last Ditch starting...\n");

  LastDitch app;
}
