#include "LastDitch.h"

#include "src/InputAdapter.h"

using namespace ld;
using namespace osg;

LastDitch::LastDitch()
  : root(new osg::Group),
    input(),
    entity_system(),
    map_system(),
    render_system(root, map_system),
    physics_system(input, entity_system),
    camera_system(root)
{
  camera_system.add_event_handler(
    new InputAdapter(input, entity_system, camera_system));

  DynamicEntity user;
  user.name = "kadijah";
  user.xform = render_system.get_user_xform(user.name);

  entity_system.add_user(user);

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
