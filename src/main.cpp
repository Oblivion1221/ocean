#include "Config.h"
#include "Simulation.h"

#include <iostream>
#include <string>
#include <exception>

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

        Simulation simulation(config);

        simulation.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Fatal Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}