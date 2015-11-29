#include "CameraSystem.h"

#include <iostream>
#include <osg/PositionAttitudeTransform>
#include <osgGA/GUIEventHandler>
#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>
#include "../Constants.h"
#include "../DebugOut.h"
#include "../InputAdapter.h"
#include "../callbacks/DebugTextCallback.h"
#include "../components/DynamicEntity.h"

using namespace ld;
using namespace osg;

CameraSystem::CameraSystem(
  osg::ref_ptr<osg::Group> root,
  Input& input,
  EntitySystem& entity_system_
)
  : running(true),
    active_cursor(true),
    entity_system(entity_system_),
    viewer(),
    debug_text_object(new osgText::Text)
{
  auto* view = new osgViewer::View;
  viewer.addView(view);

  view->setSceneData(root);
  view->setUpViewAcrossAllScreens();
  view->getCamera()->setProjectionMatrixAsPerspective(
    40, (float)FULLSCREEN_SIZE_X / (float)FULLSCREEN_SIZE_Y, 0.1, 100.0);
  view->addEventHandler(new InputAdapter(input, entity_system, *this));

  auto* stats_handler = new osgViewer::StatsHandler;
  stats_handler->setKeyEventTogglesOnScreenStats(osgGA::GUIEventAdapter::KEY_O);
  view->addEventHandler(stats_handler);

  osgViewer::Viewer::Windows windows;
  viewer.getWindows(windows);

  Camera* hud_cam = setup_HUD(windows);
  auto* hud_view = new osgViewer::View;
  hud_view->setCamera(hud_cam);

  viewer.addView(hud_view);
}


Camera* CameraSystem::setup_HUD(osgViewer::Viewer::Windows& windows)
{
  auto* camera = new Camera;

  camera->setProjectionMatrixAsOrtho2D(0, FULLSCREEN_SIZE_X, 0, FULLSCREEN_SIZE_Y);
  camera->setReferenceFrame(Transform::ABSOLUTE_RF);
  camera->setViewMatrix(Matrix::identity());
  camera->setClearMask(GL_DEPTH_BUFFER_BIT);
  camera->setRenderOrder(Camera::POST_RENDER);
  camera->setAllowEventFocus(false);

  debug_text_object->setText("this string can't be empty, we don't know why!!!");
  debug_text_object->setFont("fonts/Vera.ttf");
  debug_text_object->setDataVariance(Object::DYNAMIC);
  debug_text_object->setDrawCallback(new DebugTextCallback);

  auto* geode = new Geode;
  geode->addDrawable(debug_text_object);

  auto* stateset = geode->getOrCreateStateSet();
  stateset->setMode(GL_LIGHTING, StateAttribute::OFF);

  camera->addChild(geode);

  camera->setGraphicsContext(windows[0]);
  camera->setViewport(
    0, 0,
    windows[0]->getTraits()->width,
    windows[0]->getTraits()->height);

  return camera;
}


void CameraSystem::update()
{
  if (viewer.done())
  {
    running = false;
    return;
  }

  auto& user = entity_system.get_user("kadijah");

  Vec3 offset(0, 0, CAMERA_HEIGHT);
  Vec3 direction(-sin(user.heading), cos(user.heading), .1);
  Vec3 start = user.position + offset + direction * CAMERA_OFFSET;
  Vec3 center = user.position + offset;

  using namespace std;

  DebugOut::instance().text =
    to_string(user.position.x()) +
    " " + to_string(user.position.y());

  viewer.getView(MAIN_VIEW)->getCamera()->setViewMatrixAsLookAt(
    start * 2 * TILE_RADIUS, center * 2 * TILE_RADIUS, Vec3(0, 0, 1));

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
