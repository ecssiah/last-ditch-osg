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
    viewer(),
    map_system(),
    render_system(root, map_system)
{
  using namespace osg;

  map_system.init();
  render_system.init();

  viewer.setSceneData(root);
  viewer.addEventHandler(new InputAdapter);
  viewer.setCameraManipulator(new osgGA::TrackballManipulator);

  while (!viewer.done())
  {
    viewer.frame();
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
