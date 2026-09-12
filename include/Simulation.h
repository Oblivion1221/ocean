#pragma once

#include "Ocean.h"
#include "Config.h"

#include <cstddef>

class Simulation
{
public:
    // TODO:
    // explicit Simulation(const SimulationConfig &ocean);
    explicit Simulation(Ocean ocean);

    void run();

private:
    // TODO:
    // void init(const SimulationConfig &cfg);
    void step();
    bool should_stop() const;

    Ocean ocean_;
    std::size_t iteration_ = 0;
};
