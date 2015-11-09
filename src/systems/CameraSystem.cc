#include "CameraSystem.h"

#include <iostream>
#include "../InputAdapter.h"

using namespace ld;

CameraSystem::CameraSystem(osg::ref_ptr<osg::Group> root)
  : running(true),
    active_cursor(true),
    viewer()
{
  using namespace osg;

  viewer.setSceneData(root);

  viewer.getCamera()->setProjectionMatrixAsPerspective(40, 1, .1, 100);
  viewer.getCamera()->setViewMatrixAsLookAt(
    Vec3(0, 4, 1.4), Vec3(0, 0, 1), Vec3(0, 0, 1));

  viewer.realize();
}


void CameraSystem::add_event_handler(InputAdapter* input_adapter)
{
  viewer.addEventHandler(input_adapter);
}


void CameraSystem::update()
{
  using namespace osg;

  if (viewer.done())
  {
    running = false;
    return;
  }

  viewer.frame();
}


void CameraSystem::toggle_cursor()
{
  show_cursor(!active_cursor);
}


void CameraSystem::show_cursor(bool show)
{
  osgViewer::Viewer::Windows windows;
  viewer.getWindows(windows);

  for (auto window : windows)
  {
    if (show)
    {
      window->setCursor(osgViewer::GraphicsWindow::InheritCursor);
      active_cursor = true;
    }
    else
    {
      window->setCursor(osgViewer::GraphicsWindow::NoCursor);
      active_cursor = false;
    }
  }
}
