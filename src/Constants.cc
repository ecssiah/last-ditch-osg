#include "Constants.h"

#include <yaml-cpp/yaml.h>

const YAML::Node constants = YAML::LoadFile("scripts/constants.yml");

// World
const auto SEED = constants["seed"].as<unsigned long>();

// Camera
const double FOV = constants["fov"].as<double>();
const double NEAR_CLIP = constants["near clip"].as<double>();
const double FAR_CLIP = constants["far clip"].as<double>();
const double CAMERA_HEIGHT = constants["camera height"].as<double>();
const double CAMERA_OFFSET = constants["camera offset"].as<double>();

// Rendering
const int FULLSCREEN_SIZE_X = constants["fullscreen size x"].as<int>();
const int FULLSCREEN_SIZE_Y = constants["fullscreen size y"].as<int>();
const double ASPECT_RATIO = (double)FULLSCREEN_SIZE_X / (double)FULLSCREEN_SIZE_Y;
const double FIXED_TIMESTEP = constants["fixed timestep"].as<double>();

// Physics
const double USER_RADIUS = constants["user radius"].as<double>();
const double USER_SPEED = constants["user speed"].as<double>();
const double USER_X_ROT_SPEED = constants["user x rot speed"].as<double>();
const double USER_Y_ROT_SPEED = constants["user y rot speed"].as<double>();
