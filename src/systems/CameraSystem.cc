#include "CameraSystem.h"

#include <iostream>
#include <sstream>
#include <osg/PositionAttitudeTransform>
#include <osgGA/GUIEventHandler>
#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>
#include "../Constants.h"
#include "../InputAdapter.h"
#include "../callbacks/DebugTextCallback.h"
#include "../components/DynamicEntity.h"

using namespace ld;
using namespace osg;

CameraSystem::CameraSystem(
  ref_ptr<Group> root,
  Input& input,
  EntitySystem& entity_system_
)
  : running(true),
    active_cursor(true),
    entity_system(entity_system_),
    viewer(),
    debug_text_object(new osgText::Text)
{
  auto view = new osgViewer::View;
  viewer.addView(view);

  view->setSceneData(root);
  view->setUpViewAcrossAllScreens();
  view->getCamera()->setProjectionMatrixAsPerspective(
    FOV, ASPECT_RATIO, NEAR_CLIP, FAR_CLIP);
  view->addEventHandler(new InputAdapter(input, entity_system, *this));

  auto stats_handler = new osgViewer::StatsHandler;
  stats_handler->setKeyEventTogglesOnScreenStats(osgGA::GUIEventAdapter::KEY_O);
  view->addEventHandler(stats_handler);

  osgViewer::Viewer::Windows windows;
  viewer.getWindows(windows);

  auto hud_cam = setup_HUD(windows);
  auto hud_view = new osgViewer::View;
  hud_view->setCamera(hud_cam);

  viewer.addView(hud_view);

  printf(" Camera System finished\n");
}


Camera* CameraSystem::setup_HUD(osgViewer::Viewer::Windows& windows)
{
  auto camera = new Camera;

  camera->setProjectionMatrixAsOrtho2D(0, FULLSCREEN_SIZE_X, 0, FULLSCREEN_SIZE_Y);
  camera->setReferenceFrame(Transform::ABSOLUTE_RF);
  camera->setViewMatrix(Matrix::identity());
  camera->setClearMask(GL_DEPTH_BUFFER_BIT);
  camera->setRenderOrder(Camera::POST_RENDER);
  camera->setAllowEventFocus(false);

  debug_text_object->setFont("fonts/Vera.ttf");
  debug_text_object->setDataVariance(Object::DYNAMIC);
  debug_text_object->setDrawCallback(new DebugTextCallback);
  debug_text_object->setText("Must not be empty!!!");

  auto geode = new Geode;
  geode->addDrawable(debug_text_object);

  auto stateset = geode->getOrCreateStateSet();
  stateset->setMode(GL_LIGHTING, StateAttribute::OFF);

  camera->addChild(geode);
  camera->setGraphicsContext(windows[0]);
  camera->setViewport(0, 0, FULLSCREEN_SIZE_X, FULLSCREEN_SIZE_Y);

  return camera;
}


void CameraSystem::update()
{
  if (viewer.done())
  {
    running = false;
    return;
  }

  const auto& user = entity_system.get_user("kadijah");

  // Debug user position
  std::ostringstream ss;
  ss.precision(1);
  ss.setf(std::ios::fixed);
  ss << user.position.x() << " " << user.position.y() << " " << user.position.z();

  Debug::instance().msg = ss.str();

  Quat user_orient(user.pitch, Vec3(1, 0, 0), 0, Vec3(), user.heading, Vec3(0, 0, 1));

  Vec3 center(user.position + Vec3(0, 0, CAMERA_HEIGHT));
  Vec3 eye(center + user_orient * Vec3(0, CAMERA_OFFSET, 0));
  Vec3 up(0, 0, 1);

  center.set(TILE_SIZE * center.x(), TILE_SIZE * center.y(), FLOOR_HEIGHT * center.z());
  eye.set(TILE_SIZE * eye.x(), TILE_SIZE * eye.y(), FLOOR_HEIGHT * eye.z());

  viewer.getView(MAIN_VIEW)->getCamera()->setViewMatrixAsLookAt(eye, center, up);

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
