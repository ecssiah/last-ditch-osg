#ifndef DEBUGOUT_H
#define DEBUGOUT_H

#include <string>

namespace ld
{

class DebugOut
{
  DebugOut()
    : text("debug")
  {};

  DebugOut(const DebugOut&) = delete;
  void operator=(const DebugOut&) = delete;

public:
  static DebugOut& instance()
  {
    static DebugOut instance;

    return instance;
  }

  std::string text;
};

};

#endif /* DEBUGOUT_H */
