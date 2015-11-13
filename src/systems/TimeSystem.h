#ifndef TIMESYSTEM_H
#define TIMESYSTEM_H

#include <osg/Timer>

namespace ld
{

class TimeSystem
{
  osg::Timer timer;
  osg::Timer_t last_time;

  double dt;

public:
  TimeSystem();

  double tick();
};

}

#endif /* TIMESYSTEM_H */
