#ifndef DEBUGTEXTCALLBACK_H
#define DEBUGTEXTCALLBACK_H

#include <osg/Drawable>
#include <osgText/Text>
#include "../Debug.h"

using namespace osg;

namespace ld
{

struct DebugTextCallback : public Drawable::DrawCallback
{
  DebugTextCallback() {}

  virtual void drawImplementation(
    RenderInfo& render_info, const Drawable* drawable) const
  {
    osgText::Text* text = (osgText::Text*)drawable;
    text->setText(Debug::instance().text);
    text->drawImplementation(render_info);
  }
};

}

#endif /* DEBUGTEXTCALLBACK_H */
