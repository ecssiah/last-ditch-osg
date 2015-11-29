#ifndef DEBUGTEXTCALLBACK_H
#define DEBUGTEXTCALLBACK_H

#include <osg/Drawable>
#include <osgText/Text>
#include "../DebugOut.h"

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
    text->setText(DebugOut::instance().text);
    text->drawImplementation(render_info);
  }
};

}

#endif /* DEBUGTEXTCALLBACK_H */
