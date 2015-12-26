#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <string>
#include <osg/Group>
#include <osg/Material>
#include <osg/MatrixTransform>
#include <osg/Texture2D>
#include <osgText/Text>
#include <osgViewer/Viewer>
#include "MapSystem.h"

namespace ld
{

class RenderSystem
{
  void build_map();
  void build_objects();
  void setup_materials();
  void setup_material(const std::string& name);

  osg::Node* setup_foundation();
  osg::Node* setup_test_grid();
  osg::Node* setup_accessory(const std::string& name);
  osg::MatrixTransform* setup_character(const std::string& name);

  osg::ref_ptr<osg::Group> root;

  MapSystem& map_system;

  std::map<std::string, osg::ref_ptr<osg::MatrixTransform>> users;
  std::map<std::string, osg::ref_ptr<osg::Texture2D>> textures;
  std::map<std::string, osg::ref_ptr<osg::Material>> materials;

public:
  RenderSystem(osg::ref_ptr<osg::Group> root, MapSystem& map_system);

  osg::ref_ptr<osg::MatrixTransform> get_user_xform(const std::string& name)
    { return users[name]; }
};

}

#endif /* RENDERSYSTEM_H */
