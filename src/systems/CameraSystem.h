#ifndef CAMERASYSTEM_H
#define CAMERASYSTEM_H

#include <osg/Matrix>
#include <osgViewer/Viewer>

namespace ld
{

class CameraSystem
{
public:
  CameraSystem(osg::ref_ptr<osg::Group> root);

  void update();

  bool is_running() const { return running; }

private:
  bool running;

  osg::Matrix matrix;
  osgViewer::Viewer viewer;

};

}

#endif /* CAMERASYSTEM_H */
