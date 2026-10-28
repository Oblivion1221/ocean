#pragma once

#include "Object.h"

class Prey : public Object
{
public:
    Prey(int lifetime, int reproduction_time, int hunger_time);

    void update(Ocean& ocean) override;
    char symbol() const override { return 'F'; }
    bool is_alive() const override { return alive_; }

private:
    int lifetime_;
    int reproduction_time_;
    int age_ = 0;
    int hunger_ = 0;
    bool alive_ = true;
    int hunger_time_ = 0;
};
