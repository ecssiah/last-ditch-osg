#ifndef CAMERASYSTEM_H
#define CAMERASYSTEM_H

#include <osg/Group>
#include <osgViewer/Viewer>

namespace ld
{

class InputAdapter;

class CameraSystem
{
public:
  CameraSystem(osg::ref_ptr<osg::Group> root);

  void update();

  bool is_running() const { return running; }
  bool has_active_cursor() const { return active_cursor; }
  void show_cursor(bool show);
  void toggle_cursor();

  void add_event_handler(InputAdapter* input_adapter);

private:
  bool running;
  bool active_cursor;

  osgViewer::Viewer viewer;
};

}

#endif /* CAMERASYSTEM_H */
