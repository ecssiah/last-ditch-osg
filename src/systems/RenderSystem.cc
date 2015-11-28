#include "RenderSystem.h"

#include <iostream>
#include <osg/Image>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>
#include "../Constants.h"
#include "../callbacks/TestCallback.h"
#include "../components/Tile.h"

using namespace ld;
using namespace osg;

RenderSystem::RenderSystem(osg::ref_ptr<osg::Group> root_, MapSystem& map_system_)
  : root(root_),
    map_system(map_system_)
{
  osgDB::Registry::instance()->getDataFilePathList().push_back("media/");

  setup_materials();

  root->addChild(setup_foundation());

  build_map();

  auto name = "kadijah";
  users[name] = setup_character(name);
  root->addChild(users[name]);
}


osg::Node* RenderSystem::setup_foundation()
{
  auto* group = new Group;

  auto* state_set = group->getOrCreateStateSet();
  state_set->setTextureAttributeAndModes(
    0, textures["buildings"],StateAttribute::ON | StateAttribute::OVERRIDE);
  state_set->setAttribute(materials["buildings"]);

  auto* foundation = osgDB::readNodeFile("models/a-foundation.fbx");

  int num_chunks = map_system.get_num_chunks();

  for (int x = -num_chunks / 2; x <= num_chunks / 2; ++x)
  {
    for (int y = -num_chunks / 2; y <= num_chunks / 2; ++y)
    {
      ref_ptr<PositionAttitudeTransform> xform = new PositionAttitudeTransform;
      xform->setPosition(
	Vec3(
	  x * map_system.get_chunk_size(),
	  y * map_system.get_chunk_size(),
	  0));
      xform->addChild(foundation);
      group->addChild(xform);
    }
  }

  return group;
}


osg::Node* RenderSystem::setup_test_grid()
{
  auto* node = osgDB::readNodeFile("models/grid.fbx");

  auto* state_set = node->getOrCreateStateSet();
  state_set->setTextureAttributeAndModes(
    0, textures["buildings"],StateAttribute::ON | StateAttribute::OVERRIDE);
  state_set->setAttribute(materials["buildings"]);

  return node;
}


osg::MatrixTransform* RenderSystem::setup_character(const std::string& name)
{
  auto* character_group = new Group;

  auto* character = osgDB::readNodeFile("models/" + name + ".fbx");

  auto* state_set = character->getOrCreateStateSet();
  state_set->setTextureAttributeAndModes(
    0, textures[name], StateAttribute::ON | StateAttribute::OVERRIDE);
  state_set->setAttribute(materials[name]);

  character_group->addChild(character);
  character_group->addChild(setup_accessory("hair"));
  character_group->addChild(setup_accessory("jacket"));
  character_group->addChild(setup_accessory("tanktop"));
  character_group->addChild(setup_accessory("pants"));
  character_group->addChild(setup_accessory("boots"));

  auto* xform = new MatrixTransform;
  xform->addChild(character_group);

  return xform;
}


osg::Node* RenderSystem::setup_accessory(const std::string& name)
{
  auto* node = osgDB::readNodeFile("models/" + name + ".fbx");

  auto* state_set = node->getOrCreateStateSet();
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
  auto* image = osgDB::readImageFile("textures/" + name + ".png");

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
  int map_size = map_system.get_size();

  for (int x = -map_size / 2; x < map_size / 2; ++x)
  {
    for (int y = -map_size / 2; y < map_size / 2; ++y)
    {
      const auto& tile = map_system.get_tile(x, y, 0);

      if (tile.name == "") continue;

      auto* node = osgDB::readNodeFile(
	"models/" + tile.type + "-" + tile.name + ".fbx");

      auto* state_set = node->getOrCreateStateSet();
      state_set->setTextureAttributeAndModes(
	0, textures["buildings"], StateAttribute::ON | StateAttribute::OVERRIDE);
      state_set->setAttribute(materials["buildings"]);

      auto* xform = new MatrixTransform;

      Matrix r, t;
      r.makeRotate(
	osg::inDegrees(tile.rotation), Vec3(0, 0, 1));
      t.makeTranslate(
	Vec3(
	  TILE_SIZE * x,
	  TILE_SIZE * y,
	  0));

      xform->setMatrix(r * t);

      xform->addChild(node);
      root->addChild(xform);
    }
  }
}
