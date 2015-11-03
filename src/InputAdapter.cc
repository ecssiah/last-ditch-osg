#include "InputAdapter.h"

#include <iostream>
#include <osgDB/ReadFile>

using namespace ld;

bool InputAdapter::handle(
  const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
  switch(ea.getEventType())
  {
  case osgGA::GUIEventAdapter::KEYDOWN:
    std::cout << "Key Down!" << std::endl;
    return false;
  default:
    return false;
  }
}
