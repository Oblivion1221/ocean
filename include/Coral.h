#pragma once

#include "Object.h"

class Coral : public Object
{
public:
    explicit Coral(int reproduction_time, int lifetime);

    void update(Ocean& ocean) override;
    char symbol() const override { return 'C'; }

private:
    int lifetime_;
    int reproduction_time_;
    int age_ = 0;
    bool alive_ = true;
};
