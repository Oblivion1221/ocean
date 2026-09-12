#include "Coral.h"
#include "Ocean.h"

#include <random>

Coral::Coral(int reproduction_time)
    : reproduction_time_(reproduction_time)
{
}

void Coral::update(Ocean& ocean)
{
    // TODO: Add restriction for coral's lifetime

    ++age_;

    if (reproduction_time_ <= 0 || age_ < reproduction_time_)
        return;

    age_ = 0;

    const auto free = ocean.free_neighbors(x(), y());
    if (free.empty())
        return;

    const auto [nx, ny] = free.front();
    ocean.add_object(std::make_unique<Coral>(reproduction_time_), nx, ny);
}
