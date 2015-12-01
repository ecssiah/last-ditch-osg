#ifndef DEBUG_H
#define DEBUG_H

#include <string>

namespace ld
{

struct Debug
{
  static Debug& instance()
  {
    static Debug instance;

    return instance;
  }

  std::string msg;

private:
  Debug() : msg("default") {}

  Debug(const Debug&) = delete;
  void operator=(const Debug&) = delete;
};

};

#endif /* DEBUG_H */
