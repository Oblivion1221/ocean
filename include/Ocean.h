#pragma once

#include "Cell.h"
#include "Object.h"
#include "Config.h"

#include <memory>
#include <vector>

class Object;

class Ocean
{
public:
    // TODO: add constructor config
    // explicit Ocean(const OceanConfig &cfg);
    
    Ocean(int width, int height);

    int width() const { return width_; }
    int height() const { return height_; }

    bool in_bounds(int x, int y) const;
    Cell& cell(int x, int y);
    const Cell& cell(int x, int y) const;

    void add_object(std::unique_ptr<Object> object, int x, int y);
    bool move_object(Object& object, int new_x, int new_y);
    void remove_object(Object& object);

    std::vector<std::pair<int, int>> neighbors(int x, int y) const;
    std::vector<std::pair<int, int>> free_neighbors(int x, int y) const;

    void update();
    void print(std::ostream& out) const;

    bool has_objects() const;
    bool has_only_one_type() const;

private:
    int width_;
    int height_;
    std::vector<std::vector<Cell>> cells_;
    std::vector<std::unique_ptr<Object>> objects_;
};
