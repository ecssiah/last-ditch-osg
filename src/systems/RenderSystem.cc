#include "RenderSystem.h"

#include <iostream>
#include <osg/Image>
#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>
#include "../Constants.h"
#include "../components/Tile.h"

using namespace ld;

RenderSystem::RenderSystem(osg::ref_ptr<osg::Group> root_, MapSystem& map_system_)
  : root(root_),
    map_system(map_system_)
{
  using namespace osg;

  osgDB::Registry::instance()->getDataFilePathList().push_back("media/");

  setup_materials();
  root->addChild(setup_test_grid());

  const std::string name = "kadijah";
  users[name] = setup_character(name);
  root->addChild(users[name]);

  build_map();
}


osg::ref_ptr<osg::Node> RenderSystem::setup_test_grid()
{
  using namespace osg;

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
  using namespace osg;

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
  using namespace osg;

  ref_ptr<Node> node = osgDB::readNodeFile("models/" + name + ".fbx");

  StateSet* state_set = node->getOrCreateStateSet();
  state_set->setTextureAttributeAndModes(
    0, textures["clothing1"],StateAttribute::ON | StateAttribute::OVERRIDE);
  state_set->setAttribute(materials["clothing1"]);

  return node;
}


void RenderSystem::setup_materials()
{
  using namespace osg;

  ref_ptr<Image> image;
  image = osgDB::readImageFile("textures/kadijah.png");

  textures["kadijah"] = new Texture2D;
  textures["kadijah"]->setImage(image);
  textures["kadijah"]->setUnRefImageDataAfterApply(true);

  materials["kadijah"] = new Material;
  materials["kadijah"]->setDiffuse(Material::FRONT, Vec4(.2f, .9f, .9f, 1.f));
  materials["kadijah"]->setSpecular(Material::FRONT, Vec4(1.f, 1.f, 1.f, 1.f));
  materials["kadijah"]->setShininess(Material::FRONT, 96.f);

  image = osgDB::readImageFile("textures/clothing1.png");

  textures["clothing1"] = new Texture2D;
  textures["clothing1"]->setImage(image);
  textures["clothing1"]->setUnRefImageDataAfterApply(true);

  materials["clothing1"] = new Material;
  materials["clothing1"]->setDiffuse(Material::FRONT, Vec4(.2f, .9f, .9f, 1.f));
  materials["clothing1"]->setSpecular(Material::FRONT, Vec4(1.f, 1.f, 1.f, 1.f));
  materials["clothing1"]->setShininess(Material::FRONT, 96.f);

  image = osgDB::readImageFile("textures/buildings.png");

  textures["buildings"] = new Texture2D;
  textures["buildings"]->setImage(image);
  textures["buildings"]->setUnRefImageDataAfterApply(true);

  materials["buildings"] = new Material;
  materials["buildings"]->setDiffuse(Material::FRONT, Vec4(.2f, .9f, .9f, 1.f));
  materials["buildings"]->setSpecular(Material::FRONT, Vec4(1.f, 1.f, 1.f, 1.f));
  materials["buildings"]->setShininess(Material::FRONT, 96.f);
}


void RenderSystem::build_map()
{
  using namespace osg;

  for (int x = -MAP_SIZE / 2; x < MAP_SIZE / 2; ++x)
  {
    for (int y = -MAP_SIZE / 2; y < MAP_SIZE / 2; ++y)
    {
      const Tile& tile = map_system.get_tile(x, y, 0);

      if (tile.name == "") continue;

      std::cout << x << " " << y << ":" << tile.name << std::endl;

      ref_ptr<Node> node = osgDB::readNodeFile(
	"models/" + tile.type + "-" + tile.name + ".fbx");

      StateSet* state_set = node->getOrCreateStateSet();
      state_set->setTextureAttributeAndModes(
	0, textures["buildings"], StateAttribute::ON | StateAttribute::OVERRIDE);
      state_set->setAttribute(materials["buildings"]);

      ref_ptr<PositionAttitudeTransform> xform = new PositionAttitudeTransform;
      xform->setPosition(Vec3(x * TILE_SIZE, y * TILE_SIZE, 0));
      xform->setAttitude(Quat(osg::inDegrees(tile.rotation), Vec3(0, 0, 1)));

      xform->addChild(node);
      root->addChild(xform);
    }
  }
}
