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
  case osgGA::GUIEventAdapter::KEYDOWN:
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
  case osgGA::GUIEventAdapter::KEYUP:
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
  default:
    return false;
  }
}
