#ifndef INPUTADAPTER_H
#define INPUTADAPTER_H

#include <osgGA/GUIEventHandler>

namespace ld
{

class InputAdapter : public osgGA::GUIEventHandler
{
public:
  bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
};

}

#endif /* INPUTADAPTER_H */
