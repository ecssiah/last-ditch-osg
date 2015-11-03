#include "CameraSystem.h"

#include <osgGA/TrackballManipulator>
#include "../InputAdapter.h"

using namespace ld;

CameraSystem::CameraSystem(osg::ref_ptr<osg::Group> root)
  : running(true),
    matrix(),
    viewer()
{
  viewer.setSceneData(root);
  viewer.addEventHandler(new InputAdapter);
  viewer.setCameraManipulator(new osgGA::TrackballManipulator);
}


void CameraSystem::update()
{
  if (viewer.done())
  {
    running = false;
    return;
  }

  viewer.frame();
}
