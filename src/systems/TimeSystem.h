#ifndef TIMESYSTEM_H
#define TIMESYSTEM_H

#include <osg/Timer>

namespace ld
{

class TimeSystem
{
public:
  TimeSystem();

  double tick();

private:
  osg::Timer timer;
  osg::Timer_t last_time;

  double dt;
};

}

#endif /* TIMESYSTEM_H */
