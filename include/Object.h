#pragma once

class Ocean;

class Object
{
public:
    virtual ~Object() = default;

    virtual void update(Ocean& ocean) = 0;
    virtual char symbol() const = 0;
    virtual bool is_alive() const { return true; }

    int x() const { return x_; }
    int y() const { return y_; }

    void set_position(int x, int y)
    {
        x_ = x;
        y_ = y;
    }

private:
    int x_ = 0;
    int y_ = 0;
};
