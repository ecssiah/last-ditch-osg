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
<<<<<<< HEAD
  CameraSystem(osg::ref_ptr<osg::Group> root);
=======
  CameraSystem(osg::ref_ptr<osg::Group> root, Input& input);
>>>>>>> b3522d9cbabef47e9f42d6e8c51d8a1ecb9d1fc1

  void update();

  bool is_running() const { return running; }
  bool has_active_cursor() const { return active_cursor; }
  void show_cursor(bool show);
  void toggle_cursor();

  void add_event_handler(InputAdapter* input_adapter);

private:
  bool running;
  bool active_cursor;

<<<<<<< HEAD
=======
  Input& input;

>>>>>>> b3522d9cbabef47e9f42d6e8c51d8a1ecb9d1fc1
  osgViewer::Viewer viewer;
};

}

#endif /* CAMERASYSTEM_H */
