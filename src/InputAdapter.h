#ifndef INPUTADAPTER_H
#define INPUTADAPTER_H

#include <osgGA/GUIEventHandler>
#include "components/Input.h"
#include "systems/CameraSystem.h"

namespace ld
{

class InputAdapter : public osgGA::GUIEventHandler
{
public:
  InputAdapter(Input& input_, CameraSystem& camera_system_)
    : input(input_),
      camera_system(camera_system_)
  {}

  bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

private:
  Input& input;

  CameraSystem& camera_system;
};

}

#endif /* INPUTADAPTER_H */
