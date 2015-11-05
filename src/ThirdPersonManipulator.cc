#include "ThirdPersonManipulator.h"

#include <iostream>
#include "systems/CameraSystem.h"

using namespace ld;

ThirdPersonManipulator::ThirdPersonManipulator(CameraSystem& camera_system_)
  : osgGA::StandardManipulator(DEFAULT_SETTINGS),
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


void ThirdPersonManipulator::setTransformation(
  const osg::Vec3d& eye, const osg::Quat& rotation)
{}


void ThirdPersonManipulator::setTransformation(
  const osg::Vec3d& eye, const osg::Vec3d& center, const osg::Vec3d& up)
{}


void ThirdPersonManipulator::getTransformation(
  osg::Vec3d& eye, osg::Quat& rotation) const
{}


void ThirdPersonManipulator::getTransformation(
  osg::Vec3d& eye, osg::Vec3d& center, osg::Vec3d& up) const
{}


bool ThirdPersonManipulator::handleMouseMove(
  const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
  if (camera_system.has_active_cursor())
    return false;
  else
    return handleMouseDeltaMovement(ea, aa);
}


bool ThirdPersonManipulator::performMouseDeltaMovement(
  const float dx, const float dy)
{
  using namespace osg;

  Matrix m;
  m.makeRotate(dx * .001, Vec3(0, 0, 1));

  matrix *= m;

  return true;
}
