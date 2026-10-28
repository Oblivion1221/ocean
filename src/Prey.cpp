#include "Prey.h"
#include "Ocean.h"

#include <memory>

Prey::Prey(int lifetime, int reproduction_time, int hunger_time)
    : lifetime_(lifetime),
      reproduction_time_(reproduction_time),
      hunger_time_(hunger_time)
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
    ++age_;
    ++reproduction_age_;
    ++hunger_;

    if(lifetime_ > 0 || age_ >= lifetime_)
    {
        ocean.cell(x(),y()).clear();
        return;
    }

    if(hunger_time_ > 0 && hunger_ >= hunger_time_)
    {
        
        return;
    }

    if(reproduction_time_ > 0 && reproduction_age_ >= reproduction_time_ )
    {

        if(ocean.free_neighbors(x(), y()))
        {
            ocean.add_obgect(* Object.prey, x(), y())
        }
    }
    
}

