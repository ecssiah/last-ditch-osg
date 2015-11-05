#include "Constants.h"

#include <yaml-cpp/yaml.h>

const YAML::Node constants = YAML::LoadFile("scripts/constants.yml");

const int MAP_SIZE = constants["map size"].as<int>();
const int NUM_FLOORS = constants["num floors"].as<int>();
const int TILE_SIZE = constants["tile size"].as<int>();
