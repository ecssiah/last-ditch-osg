#include "RenderSystem.h"

#include <iostream>
#include <osg/Image>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>
#include "../Constants.h"
#include "../components/Tile.h"

using namespace ld;
using namespace osg;

RenderSystem::RenderSystem(osg::ref_ptr<osg::Group> root_, MapSystem& map_system_)
  : root(root_),
    map_system(map_system_)
{
  osgDB::Registry::instance()->getDataFilePathList().push_back("media/");

  setup_materials();
  root->addChild(setup_test_grid());

  const std::string& name = "kadijah";
  users[name] = setup_character(name);
  root->addChild(users[name]);

  build_map();
}


osg::ref_ptr<osg::Node> RenderSystem::setup_test_grid()
{
  ref_ptr<Node> node = osgDB::readNodeFile("models/grid.fbx");

  StateSet* state_set = node->getOrCreateStateSet();
  state_set->setTextureAttributeAndModes(
    0, textures["buildings"],StateAttribute::ON | StateAttribute::OVERRIDE);
  state_set->setAttribute(materials["buildings"]);

  return node;
}


osg::ref_ptr<osg::MatrixTransform> RenderSystem::setup_character(
  const std::string& name)
{
  ref_ptr<Group> character_group = new Group;

  ref_ptr<Node> character = osgDB::readNodeFile("models/" + name + ".fbx");

  StateSet* state_set = character->getOrCreateStateSet();
  state_set->setTextureAttributeAndModes(
    0, textures[name], StateAttribute::ON | StateAttribute::OVERRIDE);
  state_set->setAttribute(materials[name]);

  character_group->addChild(character);
  character_group->addChild(setup_accessory("hair"));
  character_group->addChild(setup_accessory("jacket"));
  character_group->addChild(setup_accessory("tanktop"));
  character_group->addChild(setup_accessory("pants"));
  character_group->addChild(setup_accessory("boots"));

  ref_ptr<MatrixTransform> xform = new MatrixTransform;
  xform->addChild(character_group);

  return xform;
}


osg::ref_ptr<osg::Node> RenderSystem::setup_accessory(const std::string& name)
{
  ref_ptr<Node> node = osgDB::readNodeFile("models/" + name + ".fbx");

  StateSet* state_set = node->getOrCreateStateSet();
  state_set->setTextureAttributeAndModes(
    0, textures["clothing1"],StateAttribute::ON | StateAttribute::OVERRIDE);
  state_set->setAttribute(materials["clothing1"]);

  return node;
}


void RenderSystem::setup_materials()
{
  setup_material("kadijah");
  setup_material("clothing1");
  setup_material("buildings");
}


void RenderSystem::setup_material(const std::string& name)
{
  ref_ptr<Image> image = osgDB::readImageFile("textures/" + name + ".png");

  textures[name] = new Texture2D;
  textures[name]->setImage(image);
  textures[name]->setUnRefImageDataAfterApply(true);

  materials[name] = new Material;
  materials[name]->setDiffuse(Material::FRONT, Vec4(.2f, .9f, .9f, 1.f));
  materials[name]->setSpecular(Material::FRONT, Vec4(1.f, 1.f, 1.f, 1.f));
  materials[name]->setShininess(Material::FRONT, 96.f);
}


void RenderSystem::build_map()
{
  for (int x = -MAP_SIZE / 2; x < MAP_SIZE / 2 + 1; ++x)
  {
    for (int y = -MAP_SIZE / 2; y < MAP_SIZE / 2 + 1; ++y)
    {
      const Tile& tile = map_system.get_tile(x, y, 0);

      if (tile.name == "") continue;

      ref_ptr<Node> node = osgDB::readNodeFile(
	"models/" + tile.type + "-" + tile.name + ".fbx");

      StateSet* state_set = node->getOrCreateStateSet();
      state_set->setTextureAttributeAndModes(
	0, textures["buildings"], StateAttribute::ON | StateAttribute::OVERRIDE);
      state_set->setAttribute(materials["buildings"]);

      ref_ptr<MatrixTransform> xform = new MatrixTransform;

      Matrix t;
      t.makeTranslate(Vec3(TILE_SIZE * x, TILE_SIZE * y, 0));
      Matrix r;
      r.makeRotate(osg::inDegrees(tile.rotation), Vec3(0, 0, 1));

      xform->setMatrix(r * t);

      xform->addChild(node);
      root->addChild(xform);
    }
  }
}
