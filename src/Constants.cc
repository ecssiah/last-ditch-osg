#include "Constants.h"

#include <yaml-cpp/yaml.h>

const YAML::Node constants = YAML::LoadFile("scripts/constants.yml");

const int MAP_SIZE_X = constants["map size x"].as<int>();
const int MAP_SIZE_Y = constants["map size y"].as<int>();
const int MAP_SIZE_Z = constants["map size z"].as<int>();

const int TILE_SIZE = constants["tile size"].as<int>();
