#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <string>
#include <osg/Group>
#include <osg/Material>
#include <osg/MatrixTransform>
#include <osg/Texture2D>
#include <osgViewer/Viewer>
#include "MapSystem.h"

namespace ld
{

class RenderSystem
{
public:
  RenderSystem(osg::ref_ptr<osg::Group> root, MapSystem& map_system);

<<<<<<< HEAD
  osg::ref_ptr<osg::MatrixTransform> setup_character(const std::string& name);

  osg::ref_ptr<osg::MatrixTransform> get_user(const std::string& name)
    { return users[name]; }
=======
  osg::ref_ptr<osg::Group> setup_character(const std::string& name);
>>>>>>> b3522d9cbabef47e9f42d6e8c51d8a1ecb9d1fc1

private:
  void build_map();
  void setup_materials();
<<<<<<< HEAD
  osg::ref_ptr<osg::Node> setup_test_grid();
=======
>>>>>>> b3522d9cbabef47e9f42d6e8c51d8a1ecb9d1fc1
  osg::ref_ptr<osg::Node> setup_accessory(const std::string& name);

  osg::ref_ptr<osg::Group> root;

  MapSystem& map_system;

  std::map<std::string, osg::ref_ptr<osg::MatrixTransform>> users;
  std::map<std::string, osg::ref_ptr<osg::Texture2D>> textures;
  std::map<std::string, osg::ref_ptr<osg::Material>> materials;
};

}

#endif /* RENDERSYSTEM_H */
