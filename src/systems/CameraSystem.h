#ifndef CAMERASYSTEM_H
#define CAMERASYSTEM_H

#include <osg/Group>
#include <osg/PositionAttitudeTransform>
#include <osg/Timer>
#include <osgViewer/Viewer>
#include "EntitySystem.h"
#include "../components/Input.h"

namespace ld
{

class CameraSystem
{
  bool running;
  bool active_cursor;

  EntitySystem& entity_system;

  osgViewer::Viewer viewer;

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
