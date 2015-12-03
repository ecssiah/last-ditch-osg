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
  debug_text_object->setText("This string can't be empty, we don't know why!!!");

  auto geode = new Geode;
  geode->addDrawable(debug_text_object);

  auto stateset = geode->getOrCreateStateSet();
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

  const auto& user = entity_system.get_user("kadijah");

  Vec3 center(user.position + Vec3(0, 0, CAMERA_HEIGHT));
  Quat user_orient(user.pitch, Vec3(1, 0, 0), 0, Vec3(), user.heading, Vec3(0, 0, 1));
  Vec3 eye(center + user_orient * Vec3(0, CAMERA_OFFSET, 0));

  std::ostringstream ss;
  ss.precision(1);
  ss.setf(std::ios::fixed);
  ss <<
    TILE_SIZE * user.position.x() << " " <<
    TILE_SIZE * user.position.y() << " " <<
    FLOOR_HEIGHT * user.position.z();

  Debug::instance().msg = ss.str();
  viewer.getView(MAIN_VIEW)->getCamera()->setViewMatrixAsLookAt(
    eye * TILE_SIZE, center * TILE_SIZE, Vec3(0, 0, 1));

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


void CameraSystem::fix_vertical_axis(Quat& rotation)
{
  Vec3d cam_up = rotation * Vec3d(0, 1, 0);
  Vec3d cam_right = rotation * Vec3d(1, 0, 0);
  Vec3d cam_forward = rotation * Vec3d(0, 0, -1);

  // computed directions
  Vec3d cam_right1 = cam_forward ^ Vec3(0, 0, 1);
  Vec3d cam_right2 = cam_up ^ Vec3(0, 0, 1);
  bool cam1_right_larger = cam_right1.length2() > cam_right2.length2();
  Vec3d new_cam_right = cam1_right_larger ? cam_right1 : cam_right2;

  if (new_cam_right * cam_right < 0) new_cam_right = -new_cam_right;

  // vertical axis correction
  Quat rotation_vertical_axis_correction;
  rotation_vertical_axis_correction.makeRotate(cam_right, new_cam_right);

  // rotate camera
  rotation *= rotation_vertical_axis_correction;

  // make viewer's up vector to be always less than 90 degrees from "up" axis
  Vec3d new_cam_up = new_cam_right ^ cam_forward;
  if (new_cam_up * Vec3(0, 0, 1) < 0)
    rotation = Quat(PI, Vec3d(0, 0, 1)) * rotation;
}
