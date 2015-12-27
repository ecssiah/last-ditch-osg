#include "LastDitch.h"

#include <chrono>
#include "src/Constants.h"

using namespace ld;
using namespace osg;
using namespace std;

LastDitch::LastDitch()
  : root(new Group),
    input(),
    rng(SEED > 0 ? SEED : chrono::high_resolution_clock::now().time_since_epoch().count()),
    time_system(),
    map_system(rng),
    entity_system(rng, map_system),
    physics_system(input, entity_system, map_system),
    render_system(root, entity_system, map_system),
    camera_system(root, input, entity_system)
{
  printf("Last Ditch starting...\n");

  while (camera_system.is_running())
  {
    auto dt = time_system.tick();

    physics_system.update(dt);
    render_system.update();
    camera_system.update();
  }
}


int main()
{
  setbuf(stdout, NULL);

  LastDitch app;
}
