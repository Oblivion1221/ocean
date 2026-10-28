#pragma once

#include "Object.h"

class Predator : public Object
{
public:
    Predator(int lifetime, int reproduction_time, int feeding_time);

    void update(Ocean& ocean) override;
    char symbol() const override { return 'P'; }
    bool is_alive() const override { return alive_; }

private:
    int lifetime_;
    int reproduction_time_;
    int feeding_time_;

    int age_ = 0;
    int reproduction_age_ = 0;
    int hunger_ = 0;
    bool alive_ = true;
};

