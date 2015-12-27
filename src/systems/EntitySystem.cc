#include "EntitySystem.h"

#include <iostream>
#include "../Constants.h"
#include "../components/DynamicEntity.h"

using namespace ld;
using namespace std;

EntitySystem::EntitySystem(
  std::mt19937& rng_, MapSystem& map_system_
)
  : rng(rng_),
    map_system(map_system_)
{
  setup_users();
  setup_doors();

  printf("Entity System ready\n");
}


void EntitySystem::setup_users()
{
  DynamicEntity user;
  user.name = "kadijah";
  user.collision_active = true;

  users[user.name] = user;
}


void EntitySystem::setup_doors()
{
  const auto& rooms = map_system.get_rooms();

  for (auto floor = 0; floor < NUM_FLOORS; ++floor)
  {
    for (const auto& room : rooms[floor])
    {
      uniform_int_distribution<> num_doors_dist(0, 3);
      auto num_doors(num_doors_dist(rng));

      for (auto i = 0; i < num_doors; ++i)
      {
	uniform_int_distribution<> direction_dist(0, 3);

	auto direction = direction_dist(rng);
	if (direction == 0)
	{
	  auto tx = room.x + room.w - 1;
	  auto ty = room.y + room.h / 2;

	  map_system.create_region(tx - 1, ty, 3, 1, floor, &doors[floor].back());

	  if (tx == room.master->x + room.master->w - 1)
	  {
	    doors[floor].push_back({tx, ty, "a", "door", 270});
	    map_system.set_tile(tx, ty, floor, "a", "door-frame", 270);
	  }
	  else
	  {
	    doors[floor].push_back({tx, ty, "a", "int-door", 270});
	    map_system.set_tile(tx, ty, floor, "a", "int-door-frame", 270);
	  }
	}
	else if (direction == 1)
	{
	  auto tx = room.x + room.w / 2;
	  auto ty = room.y + room.h - 1;

	  map_system.create_region(tx, ty - 1, 1, 3, floor, &doors[floor].back());

	  if (ty == room.master->y + room.master->h - 1)
	  {
	    doors[floor].push_back({tx, ty, "a", "door", 0});
	    map_system.set_tile(tx, ty, floor, "a", "door-frame", 0);
	  }
	  else
	  {
	    doors[floor].push_back({tx, ty, "a", "int-door", 0});
	    map_system.set_tile(tx, ty, floor, "a", "int-door-frame", 0);
	  }
	}
	else if (direction == 2)
	{
	  auto tx = room.x;
	  auto ty = room.y + room.h / 2;

	  map_system.create_region(tx - 1, ty, 3, 1, floor, &doors[floor].back());

	  if (tx == room.master->x)
	  {
	    doors[floor].push_back({tx, ty, "a", "door", 90});
	    map_system.set_tile(tx, ty, floor, "a", "door-frame", 90);
	  }
	  else
	  {
	    doors[floor].push_back({tx, ty, "a", "int-door", 90});
	    map_system.set_tile(tx, ty, floor, "a", "int-door-frame", 90);
	  }
	}
	else if (direction == 3)
	{
	  auto tx = room.x + room.w / 2;
	  auto ty = room.y;

	  map_system.create_region(tx, ty - 1, 1, 3, floor, &doors[floor].back());

	  if (ty == room.master->y)
	  {
	    doors[floor].push_back({tx, ty, "a", "door", 180});
	    map_system.set_tile(tx, ty, floor, "a", "door-frame", 180);
	  }
	  else
	  {
	    doors[floor].push_back({tx, ty, "a", "int-door", 180});
	    map_system.set_tile(tx, ty, floor, "a", "int-door-frame", 180);
	  }
	}
      }
    }
  }
}


void EntitySystem::update()
{

}
