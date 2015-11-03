#ifndef LASTDITCH_H
#define LASTDITCH_H

#include <osg/Group>
#include <osgViewer/Viewer>
#include "src/systems/MapSystem.h"
#include "src/systems/RenderSystem.h"

namespace ld
{

class LastDitch
{
public:
  LastDitch();
  ~LastDitch();

private:
  osg::ref_ptr<osg::Group> root;
  osgViewer::Viewer viewer;

  MapSystem map_system;
  RenderSystem render_system;
};

}

#endif /* LASTDITCH_H */
