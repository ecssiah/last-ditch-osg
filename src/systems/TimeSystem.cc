#include "TimeSystem.h"

#include <iostream>
#include <math.h>
#include <chrono>
#include <thread>
#include "../Constants.h"

using namespace ld;

TimeSystem::TimeSystem()
  :timer(),
   last_time(timer.tick()),
   dt(0)
{
}


double TimeSystem::tick()
{
  auto current = timer.tick();
  auto dt = timer.delta_s(last_time, current);
  last_time = current;

  if (dt < FIXED_TIMESTEP)
  {
    int millis = floor(1000 * (FIXED_TIMESTEP - dt));
    std::this_thread::sleep_for(std::chrono::milliseconds(millis));

    return timer.delta_s(last_time, timer.tick()) + dt;
  }

  return dt;
}
