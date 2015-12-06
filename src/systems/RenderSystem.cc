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

  root->addChild(setup_foundation());

  build_map();

  auto name = "kadijah";
  users[name] = setup_character(name);
  root->addChild(users[name]);

  auto stateset = root->getOrCreateStateSet();
  stateset->setMode(GL_LIGHTING, StateAttribute::ON);
  stateset->setMode(GL_LIGHT0, StateAttribute::ON);

  auto light0 = new Light;
  light0->setAmbient(Vec4(.1f, .1f, .1f, 1.f));
  light0->setDiffuse(Vec4(.5f, .5f, .5f, 1.f));
  light0->setSpecular(Vec4(.5f, .5f, .5f, 1.f));
  light0->setPosition(Vec4(0.f, 0.f, 0.f, 1.f));
  light0->setDirection(Vec3(0.f, 0.f, -1.f));

  auto xform = new PositionAttitudeTransform;
  xform->setPosition(Vec3(0.f, 0.f, 10.f));

  auto ls0 = new LightSource;
  ls0->setLight(light0);

  xform->addChild(ls0);
  root->addChild(xform);

  printf(" Render System finished\n");
}


osg::Node* RenderSystem::setup_foundation()
{
  auto group = new Group;

  auto state_set = group->getOrCreateStateSet();
  state_set->setTextureAttributeAndModes(
    0, textures["buildings"],StateAttribute::ON | StateAttribute::OVERRIDE);
  state_set->setAttribute(materials["buildings"]);

  auto foundation = osgDB::readNodeFile("models/a-foundation.fbx");

  for (int x = -NUM_CHUNKS / 2; x <= NUM_CHUNKS / 2; ++x)
  {
    for (int y = -NUM_CHUNKS / 2; y <= NUM_CHUNKS / 2; ++y)
    {
      auto xform = new PositionAttitudeTransform;

      xform->setPosition(Vec3(x * CHUNK_SIZE, y * CHUNK_SIZE, 0));
      xform->addChild(foundation);
      group->addChild(xform);
    }
  }

  return group;
}


osg::Node* RenderSystem::setup_test_grid()
{
  auto node = osgDB::readNodeFile("models/grid.fbx");

  auto state_set = node->getOrCreateStateSet();
  state_set->setTextureAttributeAndModes(
    0, textures["buildings"],StateAttribute::ON | StateAttribute::OVERRIDE);
  state_set->setAttribute(materials["buildings"]);

  return node;
}


osg::MatrixTransform* RenderSystem::setup_character(const std::string& name)
{
  auto character_group = new Group;

  auto character = osgDB::readNodeFile("models/" + name + ".fbx");

  auto state_set = character->getOrCreateStateSet();
  state_set->setTextureAttributeAndModes(
    0, textures[name], StateAttribute::ON | StateAttribute::OVERRIDE);
  state_set->setAttribute(materials[name]);

  character_group->addChild(character);
  character_group->addChild(setup_accessory("hair"));
  character_group->addChild(setup_accessory("jacket"));
  character_group->addChild(setup_accessory("tanktop"));
  character_group->addChild(setup_accessory("pants"));
  character_group->addChild(setup_accessory("boots"));

  auto xform = new MatrixTransform;
  xform->addChild(character_group);

  return xform;
}


osg::Node* RenderSystem::setup_accessory(const std::string& name)
{
  auto node = osgDB::readNodeFile("models/" + name + ".fbx");

  auto state_set = node->getOrCreateStateSet();
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
  auto image = osgDB::readImageFile("textures/" + name + ".png");

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
  for (int floor = 0; floor < NUM_FLOORS; ++floor)
  {
    for (int x = -MAP_SIZE / 2; x <= MAP_SIZE / 2; ++x)
    {
      for (int y = -MAP_SIZE / 2; y <= MAP_SIZE / 2; ++y)
      {
	const auto& tile = map_system.get_tile(x, y, floor);

	if (tile.name != "")
	{
	  auto node = osgDB::readNodeFile(
	    "models/" + tile.type + "-" + tile.name + ".fbx");

	  auto state_set = node->getOrCreateStateSet();
	  state_set->setTextureAttributeAndModes(
	    0, textures["buildings"], StateAttribute::ON | StateAttribute::OVERRIDE);
	  state_set->setAttribute(materials["buildings"]);

	  auto xform = new MatrixTransform;

	  Matrix r, t;
	  r.makeRotate(inDegrees(tile.rotation), Vec3(0, 0, 1));
	  t.makeTranslate(
	    Vec3(
	      TILE_SIZE * x,
	      TILE_SIZE * y,
	      FLOOR_HEIGHT * tile.position.z()));

	  xform->setMatrix(r * t);
	  xform->addChild(node);
	  root->addChild(xform);
	}

	if (tile.ceil_name != "")
	{
	  auto node = osgDB::readNodeFile(
	    "models/" + tile.ceil_type + "-" + tile.ceil_name + ".fbx");

	  auto state_set = node->getOrCreateStateSet();
	  state_set->setTextureAttributeAndModes(
	    0, textures["buildings"], StateAttribute::ON | StateAttribute::OVERRIDE);
	  state_set->setAttribute(materials["buildings"]);

	  auto xform = new MatrixTransform;

	  Matrix r, t;
	  r.makeRotate(inDegrees(tile.ceil_rotation), Vec3(0, 0, 1));
	  t.makeTranslate(
	    Vec3(
	      TILE_SIZE * x,
	      TILE_SIZE * y,
	      FLOOR_HEIGHT * (tile.position.z() + 1)));

	  xform->setMatrix(r * t);
	  xform->addChild(node);
	  root->addChild(xform);
	}
      }
    }
  }
}
