#ifndef CAMERASYSTEM_H
#define CAMERASYSTEM_H

#include <string>
#include <osg/Group>
#include <osg/PositionAttitudeTransform>
#include <osg/Timer>
#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>
#include "EntitySystem.h"
#include "../components/Input.h"

namespace ld
{

enum Views
{
  MAIN_VIEW,
  HUD_VIEW
};

class CameraSystem
{
  bool running;
  bool active_cursor;

  EntitySystem& entity_system;

  osgViewer::CompositeViewer viewer;
  osg::ref_ptr<osgText::Text> debug_text_object;

  osg::Camera* setup_HUD(osgViewer::Viewer::Windows& windows);

public:
  CameraSystem(
    osg::ref_ptr<osg::Group> root, Input& input, EntitySystem& entity_system);

  void update();
  bool is_running() const { return running; }
  bool has_active_cursor() const { return active_cursor; }
  void show_cursor(bool show);
  void toggle_cursor();
};

}

#endif /* CAMERASYSTEM_H */
