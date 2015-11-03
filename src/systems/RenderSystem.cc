#include "RenderSystem.h"

#include <iostream>
#include <osg/Image>
#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include "../Constants.h"

using namespace ld;


RenderSystem::RenderSystem(osg::ref_ptr<osg::Group> root_, MapSystem& map_system_)
  : root(root_),
    map_system(map_system_)
{}


void RenderSystem::init()
{
  using namespace osg;

  osgDB::Registry::instance()->getDataFilePathList().push_back("media/");

  setup_materials();

  ref_ptr<Node> node = osgDB::readNodeFile("models/grid.fbx");

  StateSet* state_set = node->getOrCreateStateSet();
  state_set->setTextureAttributeAndModes(
    0, textures["buildings"],StateAttribute::ON | StateAttribute::OVERRIDE);
  state_set->setAttribute(materials["buildings"]);

  root->addChild(node);

  root->addChild(setup_room("a", 0, 0, 0, 10, 10, 1));
  // root->addChild(setup_character("kadijah"));

  std::cout << map_system.get_tile(10, 10, 0).type << std::endl;
}


void RenderSystem::update()
{
}


osg::ref_ptr<osg::Group> RenderSystem::setup_character(const std::string& name)
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

  return character_group;
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


osg::ref_ptr<osg::Node> RenderSystem::setup_tile(
  const std::string& type, const std::string& name)
{
  using namespace osg;

  ref_ptr<Node> node = osgDB::readNodeFile("models/" + type + "-" + name + ".fbx");

  StateSet* state_set = node->getOrCreateStateSet();
  state_set->setTextureAttributeAndModes(
    0, textures["buildings"],StateAttribute::ON | StateAttribute::OVERRIDE);
  state_set->setAttribute(materials["buildings"]);

  return node;
}

osg::ref_ptr<osg::Group> RenderSystem::setup_room(
  const std::string& type,
  int x, int y, int floor,
  unsigned x_size, unsigned y_size, unsigned num_floors)
{
  using namespace osg;

  ref_ptr<Group> group = new Group;

  for (unsigned i = 0; i < x_size; ++i)
  {
    ref_ptr<Node> node = setup_tile(type, "wall");

    ref_ptr<PositionAttitudeTransform> xform = new PositionAttitudeTransform;
    xform->setPosition(Vec3(i * TILE_SIZE, 0, 0));
    xform->addChild(node);
    group->addChild(xform);
  }

  return group;
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
