#include "InputAdapter.h"

#include <iostream>
#include <osgDB/ReadFile>
#include "systems/CameraSystem.h"

using namespace ld;

bool InputAdapter::handle(
  const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
  switch(ea.getEventType())
  {
  case osgGA::GUIEventAdapter::PUSH:
  {
    switch (ea.getButtonMask())
    {
    case osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON:
      camera_system.toggle_cursor(); break;
    default:
      break;
    }
    return false;
  }
  case osgGA::GUIEventAdapter::MOVE:
  {
    if (camera_system.has_active_cursor())
      return false;
    else
      return handle_mouse_delta(ea, aa);
  }
  case osgGA::GUIEventAdapter::KEYDOWN:
    return handle_key_down(ea, aa);
  case osgGA::GUIEventAdapter::KEYUP:
    return handle_key_up(ea, aa);
  default:
    return false;
  }
}


bool InputAdapter::handle_key_down(
  const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
  switch (ea.getKey())
  {
  case 'w':
    input.forward = true; break;
  case 'a':
    input.left = true; break;
  case 's':
    input.backward = true; break;
  case 'd':
    input.right = true; break;
  case 'e':
    input.up = true; break;
  case 'q':
    input.down = true; break;
  default:
    break;
  }
  return false;
}


bool InputAdapter::handle_key_up(
  const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
  switch (ea.getKey())
  {
  case 'w':
    input.forward = false; break;
  case 'a':
    input.left = false; break;
  case 's':
    input.backward = false; break;
  case 'd':
    input.right = false; break;
  case 'e':
    input.up = false; break;
  case 'q':
    input.down = false; break;
  default:
    break;
  }
  return false;
}


bool InputAdapter::handle_mouse_delta(
  const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
  float dx = ea.getX() - mouse_center.x();
  float dy = ea.getY() - mouse_center.y();

  if (dx == 0 && dy == 0) return false;

  center_mouse(ea, aa);

  return false;
}


void InputAdapter::center_mouse(
  const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
  mouse_center.set(
    (ea.getXmin() + ea.getXmax()) / 2.f,
    (ea.getYmin() + ea.getYmax()) / 2.f);

  aa.requestWarpPointer(mouse_center.x(), mouse_center.y());
}
