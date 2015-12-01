#ifndef DEBUG_H
#define DEBUG_H

#include <string>

namespace ld
{

class Debug
{
  Debug()
    : text("default")
  {}

  Debug(const Debug&) = delete;
  void operator=(const Debug&) = delete;

public:
  static Debug& instance()
  {
    static Debug instance;

    return instance;
  }

  std::string text;
};

};

#endif /* DEBUG_H */
