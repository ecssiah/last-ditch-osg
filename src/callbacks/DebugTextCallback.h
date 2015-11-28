#ifndef DEBUGTEXTCALLBACK_H
#define DEBUGTEXTCALLBACK_H

#include <osg/Drawable>
#include <osgText/Text>

using namespace osg;

namespace ld
{

struct DebugTextCallback : public osg::Drawable::UpdateCallback
{
public:
  virtual void update(NodeVisitor* nv, Drawable* drawable)
  {
    printf("Stallin\n");

    if (nv->getVisitorType() == NodeVisitor::UPDATE_VISITOR)
    {
      osgText::Text* debug_text_object = dynamic_cast<osgText::Text*>(drawable);
      debug_text_object->setText("Sparky");
    }

    traverse(drawable, nv);
  }
};

}

#endif /* DEBUGTEXTCALLBACK_H */
