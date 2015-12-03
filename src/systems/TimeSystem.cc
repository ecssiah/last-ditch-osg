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

  return dt;
}
