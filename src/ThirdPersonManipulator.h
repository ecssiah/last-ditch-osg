#ifndef THIRDPERSONMANIPULATOR_H
#define THIRDPERSONMANIPULATOR_H

#include <osgGA/CameraManipulator>
#include "components/Input.h"
#include "systems/CameraSystem.h"

namespace ld
{

class ThirdPersonManipulator : public osgGA::CameraManipulator
{
public:
  ThirdPersonManipulator(
    CameraSystem& camera_system,
    osg::ref_ptr<osg::PositionAttitudeTransform> user_xform);

  virtual void setByMatrix(const osg::Matrixd& matrix);
  virtual void setByInverseMatrix(const osg::Matrixd& matrix);
  virtual osg::Matrixd getMatrix() const;
  virtual osg::Matrixd getInverseMatrix() const;

  virtual bool handle(
    const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

  bool handle_frame(
    const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
  bool handle_mouse_move(
    const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
  bool handle_key_down(
    const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
  bool handle_mouse_delta_movement(
    const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

  bool perform_mouse_delta_movement(const float dx, const float dy);

  void center_mouse_pointer(
    const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

  void update_matrix();

  void setNode(osg::Node*);
  osg::Node* getNode();
  const osg::Node* getNode() const;

protected:
  osg::ref_ptr<osg::PositionAttitudeTransform> user_xform;
  osg::ref_ptr<osg::Node> node;
  osg::Matrix matrix;
  osg::Vec2 mouse_center;

  osg::Matrix base_rotation;
  osg::Matrix base_translate;

  CameraSystem& camera_system;
};

}


#endif /* THIRDPERSONMANIPULATOR_H */
