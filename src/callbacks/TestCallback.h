#ifndef TESTCALLBACK_H
#define TESTCALLBACK_H

#include <osg/NodeCallback>

using namespace osg;

namespace ld
{

class TestCallback : public osg::NodeCallback
{
public:
  TestCallback() {}

  virtual void operator()(Node* node, NodeVisitor* nv)
  {
    std::cout << "Barky" << std::endl;

    if (nv->getVisitorType() == NodeVisitor::UPDATE_VISITOR)
    {
      std::cout << "Snarky Barky" << std::endl;
    }

    traverse(node, nv);
  }
};

}

#endif /* TESTCALLBACK_H */
