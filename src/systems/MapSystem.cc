#include "MapSystem.h"

#include <iostream>
#include "../Constants.h"

using namespace std;
using namespace ld;

MapSystem::MapSystem()
  : tiles(
      MAP_SIZE_X, std::vector<std::vector<Tile>>(
	MAP_SIZE_Y, std::vector<Tile>(
	  MAP_SIZE_Z, Tile())))
{
}


void MapSystem::init()
{

}


void MapSystem::update()
{

}
