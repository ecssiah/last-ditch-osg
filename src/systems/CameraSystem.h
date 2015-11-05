#ifndef CAMERASYSTEM_H
#define CAMERASYSTEM_H

#include <osg/Group>
#include <osgViewer/Viewer>
#include "../components/Input.h"

namespace ld
{

class CameraSystem
{
public:
  CameraSystem(osg::ref_ptr<osg::Group> root, Input& input);

  void update();

  bool is_running() const { return running; }
  bool has_active_cursor() const { return active_cursor; }
  void show_cursor(bool show);
  void toggle_cursor();

private:
  bool running;
  bool active_cursor;

  Input& input;

  osgViewer::Viewer viewer;
};

}

#endif /* CAMERASYSTEM_H */
