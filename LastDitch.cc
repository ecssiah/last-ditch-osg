#include "LastDitch.h"

#include "src/InputAdapter.h"

using namespace ld;

LastDitch::LastDitch()
  : root(new osg::Group),
    input(),
    entity_system(),
    map_system(),
    render_system(root, map_system),
    physics_system(input, entity_system),
    camera_system(root)
{
  using namespace osg;

  DynamicEntity user;
  user.name = "kadijah";
  user.xform = render_system.get_user(user.name);

  entity_system.add_user(user.name, user);

  camera_system.add_event_handler(new InputAdapter(input, camera_system));

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
