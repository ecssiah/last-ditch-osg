#include "ThirdPersonManipulator.h"

#include <iostream>
#include "systems/CameraSystem.h"

using namespace ld;

ThirdPersonManipulator::ThirdPersonManipulator(CameraSystem& camera_system_)
  : osgGA::CameraManipulator(),
    matrix(),
    mouse_center(0, 0),
    camera_system(camera_system_)
{
  osg::Matrix r;
  r.makeRotate(M_PI / 2, osg::Vec3(1, 0, 0));

  osg::Matrix t;
  t.makeTranslate(osg::Vec3(0, -3, 1.4));

  matrix = r * t;
}


void ThirdPersonManipulator::setByMatrix(const osg::Matrixd& matrix_)
{
  matrix = matrix_;
}


void ThirdPersonManipulator::setByInverseMatrix(const osg::Matrixd& matrix_)
{
  matrix = osg::Matrix::inverse(matrix_);
}


osg::Matrixd ThirdPersonManipulator::getMatrix() const
{
  return matrix;
}


osg::Matrixd ThirdPersonManipulator::getInverseMatrix() const
{
  return osg::Matrix::inverse(matrix);
}


bool ThirdPersonManipulator::handle(
  const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
  switch (ea.getEventType())
  {
  case osgGA::GUIEventAdapter::MOVE:
    return handle_mouse_move(ea, aa);
  default:
    return false;
  }
}


bool ThirdPersonManipulator::handle_mouse_move(
  const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
  if (camera_system.has_active_cursor())
    return false;
  else
    return handle_mouse_delta_movement(ea, aa);
}


bool ThirdPersonManipulator::handle_mouse_delta_movement(
  const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
    float dx = ea.getX() - mouse_center.x();
    float dy = ea.getY() - mouse_center.y();

    if (dx == 0 && dy == 0) return false;

    center_mouse_pointer(ea, aa);

    return perform_mouse_delta_movement(dx, dy);
}


bool ThirdPersonManipulator::perform_mouse_delta_movement(
  const float dx, const float dy)
{
  using namespace osg;

  Matrix m;
  m.makeRotate(dx * .001, Vec3(0, 0, 1));

  matrix *= m;

  return true;
}


void ThirdPersonManipulator::center_mouse_pointer(
  const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
  mouse_center.set(
    (ea.getXmin() + ea.getXmax()) / 2.0f,
    (ea.getYmin() + ea.getYmax()) / 2.0f);

  aa.requestWarpPointer(mouse_center.x(), mouse_center.y());
}
