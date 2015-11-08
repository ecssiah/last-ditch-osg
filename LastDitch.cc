#include "LastDitch.h"

#include <osg/Group>
#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgGA/TrackballManipulator>
#include "src/Constants.h"

using namespace ld;

LastDitch::LastDitch()
  : root(new osg::Group),
    input(),
    map_system(),
    render_system(root, map_system),
    physics_system(input),
    camera_system(root, input)
{
  using namespace osg;

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
