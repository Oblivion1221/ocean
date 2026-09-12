#include "Config.h"
#include "Coral.h"
#include "Ocean.h"
#include "Predator.h"
#include "Prey.h"
#include "Simulation.h"
#include "Stone.h"

#include <iostream>
#include <memory>

int main(int argc, char *argv[])
{
    static constexpr const char *kConfigPath = "config/config.json";

    std::string config_path = kConfigPath;
    if (argc > 1) 
    {
        config_path = argv[1];
    }
    try
    {
        const SimulationConfig config = ConfigLoader::load(config_path);

        Ocean ocean(config.ocean.width, config.ocean.height);

        // Example initial population.
        // Make them random

        ocean.add_object(std::make_unique<Stone>(), 2, 2);
        ocean.add_object(std::make_unique<Coral>(
            config.coral.reproduction_time), 5, 5);

        // ocean.add_object(std::make_unique<Prey>(
        //     config.prey.lifetime,
        //     config.prey.reproduction_time), 8, 5);

        // ocean.add_object(std::make_unique<Prey>(
        //     config.prey.lifetime,
        //     config.prey.reproduction_time), 9, 5);

        // ocean.add_object(std::make_unique<Predator>(
        //     config.predator.lifetime,
        //     config.predator.reproduction_time,
        //     config.predator.feeding_time), 10, 5);

        Simulation simulation(std::move(ocean));

        //TODO:
        // Simulation simulation(config.simulation);
        simulation.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
