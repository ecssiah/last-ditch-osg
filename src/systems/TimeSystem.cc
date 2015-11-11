#include "TimeSystem.h"

#include <iostream>
#include <math.h>
#include "../Constants.h"

using namespace ld;

TimeSystem::TimeSystem()
  :timer(),
   last_time(timer.tick()),
   iterations(0),
   dt(0),
   accumulated_dt(0)
{
}


double TimeSystem::tick()
{
  auto current = timer.tick();
  double dt = timer.delta_s(last_time, current);
  last_time = current;

  iterations = floor(dt / FIXED_TIMESTEP);
  dt -= iterations * FIXED_TIMESTEP;

  return dt;
}
