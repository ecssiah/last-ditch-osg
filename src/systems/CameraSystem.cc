#include "CameraSystem.h"

#include <iostream>
#include <osg/PositionAttitudeTransform>
#include <osgGA/GUIEventHandler>
#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>
#include "../Constants.h"
#include "../InputAdapter.h"
#include "../components/DynamicEntity.h"

using namespace ld;
using namespace osg;

CameraSystem::CameraSystem(
  osg::ref_ptr<osg::Group> root,
  Input& input, EntitySystem& entity_system_
)
  : running(true),
    active_cursor(true),
    debug_text("Testing Debug Text"),
    entity_system(entity_system_),
    viewer()
{
  osgViewer::View* view = new osgViewer::View;
  viewer.addView(view);

  view->setSceneData(root);
  view->setUpViewAcrossAllScreens();
  view->addEventHandler(new InputAdapter(input, entity_system, *this));

  osgViewer::StatsHandler* stats_handler = new osgViewer::StatsHandler;
  stats_handler->setKeyEventTogglesOnScreenStats(osgGA::GUIEventAdapter::KEY_O);
  view->addEventHandler(stats_handler);

  osgViewer::Viewer::Windows windows;
  viewer.getWindows(windows);

  Camera* hud_cam = setup_HUD();
  hud_cam->setGraphicsContext(windows[0]);
  hud_cam->setViewport(
    0, 0,
    windows[0]->getTraits()->width, windows[0]->getTraits()->height);

  osgViewer::View* hud_view = new osgViewer::View;
  hud_view->setCamera(hud_cam);

  viewer.addView(hud_view);
}


Camera* CameraSystem::setup_HUD()
{
  Camera* camera = new Camera;

  camera->setProjectionMatrix(Matrix::ortho2D(0, FULLSCREEN_SIZE_X, 0, FULLSCREEN_SIZE_Y));
  camera->setReferenceFrame(Transform::ABSOLUTE_RF);
  camera->setViewMatrix(Matrix::identity());
  camera->setClearMask(GL_DEPTH_BUFFER_BIT);
  camera->setRenderOrder(Camera::POST_RENDER);
  camera->setAllowEventFocus(false);

  Geode* geode = new Geode;

  StateSet* stateset = geode->getOrCreateStateSet();
  stateset->setMode(GL_LIGHTING, StateAttribute::OFF);

  osgText::Text* debug_text_object = new osgText::Text;
  geode->addDrawable(debug_text_object);

  debug_text_object->setFont("fonts/Vera.ttf");
  debug_text_object->setPosition(Vec3(0, 0, 0));
  debug_text_object->setText(debug_text);

  camera->addChild(geode);

  return camera;
}


void CameraSystem::update()
{
  if (viewer.done())
  {
    running = false;
    return;
  }

  DynamicEntity& user = entity_system.get_user("kadijah");

  Vec3 offset(0, 0, 1.4);
  Vec3 direction(-sin(user.heading), cos(user.heading), .2);

  Vec3 start = user.position + offset + direction;
  Vec3 center = user.position + offset;

  viewer.getView(MAIN_VIEW)->getCamera()->setViewMatrixAsLookAt(
    start, center, Vec3(0, 0, 1));

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
