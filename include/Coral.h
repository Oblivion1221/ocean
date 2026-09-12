#pragma once

#include "Object.h"

class Coral : public Object
{
public:
    explicit Coral(int reproduction_time);

    void update(Ocean& ocean) override;
    char symbol() const override { return 'C'; }

private:
    int reproduction_time_;
    int age_ = 0;
};
