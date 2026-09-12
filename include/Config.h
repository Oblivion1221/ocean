#pragma once

#include <string>

struct OceanConfig
{
    int width = 20;
    int height = 10;
};

struct CoralConfig
{
    int reproduction_time = 5;
};

struct PreyConfig
{
    int lifetime = 20;
    int reproduction_time = 5;
};

struct PredatorConfig
{
    int lifetime = 30;
    int reproduction_time = 8;
    int feeding_time = 5;
};

struct SimulationConfig
{
    OceanConfig ocean;
    CoralConfig coral;
    PreyConfig prey;
    PredatorConfig predator;
    int coral_counts = 4;
    int prey_counts = 4;
    int predator_counts = 4;
};

class ConfigLoader
{
public:
    static SimulationConfig load(const std::string& filename);
};
