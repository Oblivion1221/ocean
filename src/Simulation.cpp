#include "Simulation.h"

#include <chrono>
#include <iostream>
#include <thread>

// TODO:
// Simulation::Simulation(const SimulationConfig &cfg)
// {
//     init(cfg);
// }

Simulation::Simulation(Ocean ocean)
    : ocean_(std::move(ocean))
{
}

// TODO:
// void Simulation::init(const SimulationConfig &cfg)
// {
//     // TODO: fills ocean with params from cfg
// }

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
