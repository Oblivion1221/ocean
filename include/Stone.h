#pragma once

#include "Object.h"

class Stone : public Object
{
public:
    void update(Ocean&) override {}
    char symbol() const override { return '#'; }
};
