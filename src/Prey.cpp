#include "Prey.h"
#include "Ocean.h"

#include <vector>
#include <random>
#include <memory>

Prey::Prey(int lifetime, int reproduction_time, int hunger_time)
    : lifetime_(lifetime),
      reproduction_time_(reproduction_time),
      hunger_time_(hunger_time)
{
}

void Prey::update(Ocean& ocean)
{
    ++age_;
    ++hunger_;

    if (age_ >= lifetime_ || hunger_ >= hunger_time_)
    {
        alive_ = false;
        return;
    }

    bool have_coral = false;
    int current_x = x(), current_y = y();

    std::vector<std::pair<int, int>> environment = ocean.neighbors(x(), y());

    for (const auto& [nx, ny] : environment)
    {
        Object* neighbor_obj =ocean.cell(nx, ny).object();

        if (neighbor_obj && neighbor_obj->symbol() == 'C')
        {
            hunger_ = 0;      
            have_coral = true;
            ocean.remove_object(*neighbor_obj);
            break;
        }
    }

    if (!have_coral)
    {
        const auto free = ocean.free_neighbors(x(), y());
        if (!free.empty())
        {
            static std::mt19937 gen(std::random_device{}());
            std::uniform_int_distribution<size_t> dist(0, free.size() - 1);
            const auto [nx, ny] = free[dist(gen)];
            
            ocean.move_object(*this, nx, ny);
            current_x = nx;
            current_y = ny;
        }
    }
    
    if (reproduction_time_ > 0 && age_ % reproduction_time_ == 0)
    {

        const auto free = ocean.free_neighbors(current_x, current_y);
    
        if (free.empty())
        return;

        static std::mt19937 gen (std::random_device{}());
        std::uniform_int_distribution<size_t> dist(0, free.size() - 1);
        size_t random_index = dist(gen);

        const auto [nx, ny] = free[random_index];

        ocean.add_object(std::make_unique<Prey>(lifetime_, reproduction_time_, hunger_time_), nx, ny);
    }

    const auto free = ocean.free_neighbors(current_x, current_y);

    if (!free.empty())
    {
        static std::mt19937 gen(std::random_device{}());
        std::uniform_int_distribution<size_t> dist(0, free.size() - 1);
        const auto [nx, ny] = free[dist(gen)];
            
        ocean.move_object(*this, nx, ny);
    }
}
