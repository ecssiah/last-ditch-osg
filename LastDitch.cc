#include "LastDitch.h"

<<<<<<< HEAD
#include "src/InputAdapter.h"
=======
#include <osg/Group>
#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgGA/TrackballManipulator>
#include "src/Constants.h"
>>>>>>> b3522d9cbabef47e9f42d6e8c51d8a1ecb9d1fc1

using namespace ld;

LastDitch::LastDitch()
  : root(new osg::Group),
    input(),
    entity_system(),
    map_system(),
    render_system(root, map_system),
    physics_system(input),
<<<<<<< HEAD
    camera_system(root)
{
  using namespace osg;

  DynamicEntity user;
  user.name = "kadijah";
  user.xform = render_system.get_user(user.name);

  entity_system.add_entity(user.name, user);

  camera_system.add_event_handler(new InputAdapter(input, camera_system));

=======
    camera_system(root, input)
{
  using namespace osg;

>>>>>>> b3522d9cbabef47e9f42d6e8c51d8a1ecb9d1fc1
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
