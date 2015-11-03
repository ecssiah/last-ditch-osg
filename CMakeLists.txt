cmake_minimum_required(VERSION 3.0)

project(LastDitch)

set(
  HEADERS
  ./LastDitch.h
  ./src/InputAdapter.h
  ./src/systems/CameraSystem.h
  ./src/systems/MapSystem.h
  ./src/systems/RenderSystem.h
  ./src/Constants.h)

set(
  SOURCES
  ./LastDitch.cc
  ./src/InputAdapter.cc
  ./src/systems/CameraSystem.cc
  ./src/systems/MapSystem.cc
  ./src/systems/RenderSystem.cc
  ./src/Constants.cc)

add_executable(LastDitch ${HEADERS} ${SOURCES})

target_compile_features(LastDitch PRIVATE cxx_range_for)

set(
  CMAKE_MODULE_PATH
  "${CMAKE_MODULE_PATH}"
  "${CMAKE_CURRENT_SOURCE_DIR}/dist/cmake/Modules")

set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/dist")

find_package(
  OpenSceneGraph REQUIRED
  osgDB osgGA osgUtil osgText osgViewer)

find_package(YamlCpp REQUIRED)

include_directories(
  ${OPENSCENEGRAPH_INCLUDE_DIRS}
  ${YAMLCPP_INCLUDE_DIR})

target_link_libraries(
  LastDitch
  ${OPENSCENEGRAPH_LIBRARIES}
  ${YAMLCPP_LIBRARY})

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dist/media)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dist/shaders)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dist/scripts)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/dist)

install(
  DIRECTORY ${CMAKE_SOURCE_DIR}/dist/media
  DESTINATION .)

install(
  DIRECTORY ${CMAKE_SOURCE_DIR}/dist/shaders
  DESTINATION .)

install(
  DIRECTORY ${CMAKE_SOURCE_DIR}/dist/scripts
  DESTINATION .)
