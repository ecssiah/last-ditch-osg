#include "Constants.h"

#include <yaml-cpp/yaml.h>

const YAML::Node constants = YAML::LoadFile("scripts/constants.yml");

// Camera
const double CAMERA_HEIGHT = constants["camera height"].as<double>();
const double CAMERA_OFFSET = constants["camera offset"].as<double>();

// Rendering
const int FULLSCREEN_SIZE_X = constants["fullscreen size x"].as<int>();
const int FULLSCREEN_SIZE_Y = constants["fullscreen size y"].as<int>();
const double FIXED_TIMESTEP = constants["fixed timestep"].as<double>();

// Map
const double FLOOR_HEIGHT = constants["floor height"].as<double>();
const double TILE_SIZE = constants["tile size"].as<double>();

// Physics
const double USER_RADIUS = constants["user radius"].as<double>();
const double USER_SPEED = constants["user speed"].as<double>();
const double USER_X_ROT_SPEED = constants["user x rot speed"].as<double>();
const double USER_Y_ROT_SPEED = constants["user y rot speed"].as<double>();
