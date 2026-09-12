#include "Config.h"

#include <nlohmann/json.hpp>

#include <fstream>
#include <stdexcept>

SimulationConfig ConfigLoader::load(const std::string& filename)
{
    std::ifstream input(filename);
    if (!input)
        throw std::runtime_error("Cannot open config: " + filename);

    nlohmann::json j;
    input >> j;

    SimulationConfig config;


    config.ocean.width = j.at("ocean").at("width").get<int>();
    config.ocean.height = j.at("ocean").at("height").get<int>();

    config.coral.reproduction_time =
        j.at("coral").at("reproduction_time").get<int>();

    config.prey.lifetime =
        j.at("prey").at("lifetime").get<int>();

    config.prey.reproduction_time =
        j.at("prey").at("reproduction_time").get<int>();

    config.predator.lifetime =
        j.at("predator").at("lifetime").get<int>();

    config.predator.reproduction_time =
        j.at("predator").at("reproduction_time").get<int>();

    config.predator.feeding_time =
        j.at("predator").at("feeding_time").get<int>();

    return config;
}
