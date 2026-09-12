#include "Predator.h"
#include "Ocean.h"
#include "Prey.h"

#include <memory>

Predator::Predator(int lifetime, int reproduction_time, int feeding_time)
    : lifetime_(lifetime),
      reproduction_time_(reproduction_time),
      feeding_time_(feeding_time)
{
}

void Predator::update(Ocean& ocean)
{
    // TODO:
    //    │
    //    ├── age++
    //    ├── hunger++
    //    │
    //    ├── if a neighbor is a Prey?
    //    │       │
    //    │       ├── yes → eat
    //    │       │        hunger = 0
    //    │       │
    //    │       └── no → move
    //    ├── if a neighbor is a Coral? TODO:
    //    │
    //    ├── reproduce
    //    │
    //    └── hunger >= feeding_time?
    //    |              └── die
    //    ├── move

}