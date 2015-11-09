#ifndef INPUTADAPTER_H
#define INPUTADAPTER_H

#include <osgGA/GUIEventHandler>
#include "components/Input.h"
#include "systems/EntitySystem.h"
#include "systems/CameraSystem.h"

namespace ld
{

class InputAdapter : public osgGA::GUIEventHandler
{
public:
  InputAdapter(
    Input& input_, EntitySystem& entity_system_, CameraSystem& camera_system_
  )
    : input(input_),
      entity_system(entity_system_),
      camera_system(camera_system_)
  {}

  bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

private:
  bool handle_mouse_move(
    const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
  bool handle_mouse_click(
    const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
  bool handle_key_up(
    const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
  bool handle_key_down(
    const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
  bool handle_mouse_delta(
    const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

  void center_mouse(
    const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

  Input& input;
  EntitySystem& entity_system;
  CameraSystem& camera_system;

  osg::Vec2 mouse_center;
};

}

#endif /* INPUTADAPTER_H */
