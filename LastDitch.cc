#include "LastDitch.h"

#include <osg/Group>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgGA/TrackballManipulator>
#include "src/Constants.h"
#include "src/InputAdapter.h"

using namespace ld;

LastDitch::LastDitch()
  : root(new osg::Group),
    camera_system(root),
    map_system(),
    render_system(root, map_system)
{
  while (camera_system.is_running())
  {
    camera_system.update();
  }
}

LastDitch::~LastDitch()
{
}

int main()
{
  LastDitch app;

  return 0;
}
