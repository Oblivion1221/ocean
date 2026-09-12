#include "Prey.h"
#include "Ocean.h"

#include <memory>

Prey::Prey(int lifetime, int reproduction_time)
    : lifetime_(lifetime),
      reproduction_time_(reproduction_time)
{
}

void Prey::update(Ocean& ocean)
{
    // TODO:
    // │
    // ├── increase age
    // │
    // ├── age >= lifetime || hunger >= hunger_time?
    // │       └── die
    // │
    // ├── age == reprouduce_time
    // |       └── try reproduce
    // │
    // ├── if a neighbor is coral ?
    // |         └── eat coral, reset hunger
    // └── try_move

}
