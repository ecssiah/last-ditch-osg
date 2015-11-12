#include "Constants.h"

#include <yaml-cpp/yaml.h>

const YAML::Node constants = YAML::LoadFile("scripts/constants.yml");

// User
const double USER_SPEED = constants["user speed"].as<double>();
const double USER_X_ROT_SPEED = constants["user x rot speed"].as<double>();
const double USER_Y_ROT_SPEED = constants["user y rot speed"].as<double>();

// Rendering
const double FIXED_TIMESTEP = constants["fixed timestep"].as<double>();

// Map
const int MAP_SIZE = constants["map size"].as<int>();
const int NUM_FLOORS = constants["num floors"].as<int>();
const int TILE_SIZE = constants["tile size"].as<int>();
