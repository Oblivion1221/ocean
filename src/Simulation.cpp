#include "Simulation.h"
#include "Stone.h"
#include "Coral.h"
#include "Prey.h"
#include "Predator.h"

#include <chrono>
#include <iostream>
#include <thread>
#include <random>

Simulation::Simulation(const SimulationConfig &cfg)
    : ocean_(cfg.ocean)
{
    init(cfg);
}

Simulation::Simulation(Ocean ocean)
    : ocean_(std::move(ocean))
{
}

void Simulation::init(const SimulationConfig &cfg)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distX(0, cfg.ocean.width - 1);
    std::uniform_int_distribution<int> distY(0, cfg.ocean.height - 1);

    auto add_randomly = [&](std::unique_ptr<Object>obj) 
    {
        constexpr int max_attempts = 1000;
        for(int attempts = 0; attempts < max_attempts; ++attempts)
        {
            int x = distX(gen);
            int y = distY(gen);

            if(ocean_.cell(x, y).empty())
            {
                ocean_.add_object(std::move(obj), x, y);
                return true;
            }
        }
        return false;
    };

    for(int i = 0; i < cfg.stone_counts; ++i)
    {
        add_randomly(std::make_unique<Stone>());
    }

    for(int i = 0; i < cfg.coral_counts; ++i)
    {
        add_randomly(std::make_unique<Coral>(cfg.coral.lifetime, cfg.coral.reproduction_time));
    }

    for(int i = 0; i < cfg.prey_counts; ++i)
    {
        add_randomly(std::make_unique<Prey>(cfg.prey.lifetime, cfg.prey.reproduction_time, cfg.prey.hunger_time));
    }

    for(int i = 0; i < cfg.predator_counts; ++i)
    {
        add_randomly(std::make_unique<Predator>(cfg.predator.lifetime, cfg.predator.reproduction_time, cfg.predator.feeding_time));
    }
}

void Simulation::step()
{
    ocean_.update();
    ++iteration_;
}

bool Simulation::should_stop() const
{
    return !ocean_.has_objects() || ocean_.has_only_one_type();
}

void Simulation::run()
{
    while (!should_stop())
    {
        std::cout << "Iteration: " << iteration_ << '\n';
        ocean_.print(std::cout);
        std::cout << '\n';

        step();

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    std::cout << "Simulation finished after "
              << iteration_ << " iterations.\n";
}