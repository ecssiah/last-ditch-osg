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
  int get_iterations() { return iterations; }

private:
  osg::Timer timer;
  osg::Timer_t last_time;

  int iterations;
  double dt;
  double accumulated_dt;
};

}

#endif /* TIMESYSTEM_H */
