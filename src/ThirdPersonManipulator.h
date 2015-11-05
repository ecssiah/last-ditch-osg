#ifndef THIRDPERSONMANIPULATOR_H
#define THIRDPERSONMANIPULATOR_H

#include <osgGA/StandardManipulator>
#include "components/Input.h"
#include "systems/CameraSystem.h"

namespace ld
{

class ThirdPersonManipulator : public osgGA::StandardManipulator
{
public:
  ThirdPersonManipulator(CameraSystem& camera_system);

  virtual void setByMatrix(const osg::Matrixd& matrix);
  virtual void setByInverseMatrix(const osg::Matrixd& matrix);
  virtual osg::Matrixd getMatrix() const;
  virtual osg::Matrixd getInverseMatrix() const;

  virtual void setTransformation(
    const osg::Vec3d& eye, const osg::Quat& rotation);
  virtual void setTransformation(
    const osg::Vec3d& eye, const osg::Vec3d& center, const osg::Vec3d& up);
  virtual void getTransformation(
    osg::Vec3d& eye, osg::Quat& rotation) const;
  virtual void getTransformation(
    osg::Vec3d& eye, osg::Vec3d& center, osg::Vec3d& up) const;

protected:
  virtual bool handleMouseMove(
    const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
  virtual bool performMouseDeltaMovement(const float dx, const float dy);

  osg::Matrix matrix;

private:
  CameraSystem& camera_system;
};

}


#endif /* THIRDPERSONMANIPULATOR_H */
