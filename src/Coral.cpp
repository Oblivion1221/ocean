#include "Coral.h"
#include "Ocean.h"

#include <random>

Coral::Coral(int lifetime, int reproduction_time)
    : lifetime_(lifetime)
    , reproduction_time_(reproduction_time)
{
}

void Coral::update(Ocean& ocean)
{
    ++age_;

    if(age_>= lifetime_)
    {
        alive_ = false;
        ocean.remove_object(*this);
        return;
    }

    if (reproduction_time_ <= 0 || age_ % reproduction_time_ != 0)
        return;

    const auto free = ocean.free_neighbors(x(), y());
    
    if (free.empty())
        return;

    static std::mt19937 gen (std::random_device{}());

    std::uniform_int_distribution<size_t> dist(0, free.size() - 1);

    size_t random_index = dist(gen);

    const auto [nx, ny] = free[random_index];
    ocean.add_object(std::make_unique<Coral>(lifetime_, reproduction_time_), nx, ny);
}
