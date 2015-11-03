#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <string>
#include <osg/Group>
#include <osg/Material>
#include <osg/Texture2D>
#include <osgViewer/Viewer>
#include "MapSystem.h"

namespace ld
{

class RenderSystem
{
public:
  RenderSystem(osg::ref_ptr<osg::Group> root, MapSystem& map_system);

  void init();
  void update();

private:
  void setup_materials();
  osg::ref_ptr<osg::Group> setup_character(const std::string& name);
  osg::ref_ptr<osg::Node> setup_tile(
    const std::string& type, const std::string& name);
  osg::ref_ptr<osg::Group> setup_room(
    const std::string& type,
    int x, int y, int floor,
    unsigned x_size, unsigned y_size, unsigned num_floors);
  osg::ref_ptr<osg::Node> setup_accessory(const std::string& name);

  osg::ref_ptr<osg::Group> root;
  MapSystem& map_system;

  std::map<std::string, osg::ref_ptr<osg::Texture2D>> textures;
  std::map<std::string, osg::ref_ptr<osg::Material>> materials;
};

}

#endif /* RENDERSYSTEM_H */
