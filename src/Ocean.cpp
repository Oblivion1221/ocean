#include "Ocean.h"
#include "Object.h"

#include <algorithm>
#include <iostream>
#include <random>
#include <stdexcept>
#include <typeinfo>

namespace
{
std::mt19937& rng()
{
    static std::mt19937 generator{std::random_device{}()};
    return generator;
}
}

Ocean::Ocean(const OceanConfig& cfg)
    : Ocean (cfg.width, cfg.height)
    {    
    }

Ocean::Ocean(int width, int height)
    : width_(width),
      height_(height),
      cells_(height, std::vector<Cell>(width))
{
    if (width <= 0 || height <= 0)
        throw std::invalid_argument("Ocean dimensions must be positive");
}

bool Ocean::in_bounds(int x, int y) const
{
    return x >= 0 && x < width_ && y >= 0 && y < height_;
}

Cell& Ocean::cell(int x, int y)
{
    if (!in_bounds(x, y))
        throw std::out_of_range("Cell coordinates are out of bounds");
    return cells_[y][x];
}

const Cell& Ocean::cell(int x, int y) const
{
    if (!in_bounds(x, y))
        throw std::out_of_range("Cell coordinates are out of bounds");
    return cells_[y][x];
}

void Ocean::add_object(std::unique_ptr<Object> object, int x, int y)
{
    if (!object)
        throw std::invalid_argument("Cannot add null object");

    if (!in_bounds(x, y))
        throw std::out_of_range("Object coordinates are out of bounds");

    if (!cell(x, y).empty())
        throw std::runtime_error("Cell is already occupied");

    object->set_position(x, y);
    cell(x, y).set_object(object.get());
    objects_.push_back(std::move(object));
}

bool Ocean::move_object(Object& object, int new_x, int new_y)
{
    if (!in_bounds(new_x, new_y) || !cell(new_x, new_y).empty())
        return false;

    const int old_x = object.x();
    const int old_y = object.y();

    if (in_bounds(old_x, old_y) && cell(old_x, old_y).object() == &object)
        cell(old_x, old_y).clear();

    object.set_position(new_x, new_y);
    cell(new_x, new_y).set_object(&object);
    return true;
}

void Ocean::remove_object(Object& object)
{
    if (in_bounds(object.x(), object.y()) &&
        cell(object.x(), object.y()).object() == &object)
    {
        cell(object.x(), object.y()).clear();
    }

    objects_.erase(
        std::remove_if(objects_.begin(), objects_.end(),
            [&object](const std::unique_ptr<Object>& ptr)
            {
                return ptr.get() == &object;
            }),
        objects_.end());
}

std::vector<std::pair<int, int>> Ocean::neighbors(int x, int y) const
{
    std::vector<std::pair<int, int>> result;

    for (int dy = -1; dy <= 1; ++dy)
    {
        for (int dx = -1; dx <= 1; ++dx)
        {
            if (dx == 0 && dy == 0)
                continue;

            const int nx = x + dx;
            const int ny = y + dy;

            if (in_bounds(nx, ny))
                result.emplace_back(nx, ny);
        }
    }

    return result;
}

std::vector<std::pair<int, int>> Ocean::free_neighbors(int x, int y) const
{
    std::vector<std::pair<int, int>> result;

    for (const auto& [nx, ny] : neighbors(x, y))
    {
        if (cell(nx, ny).empty())
            result.emplace_back(nx, ny);
    }

    return result;
}

void Ocean::update()
{
    const std::size_t count = objects_.size();

    for (std::size_t i = 0; i < count && i < objects_.size(); ++i)
    {
        if (objects_[i])
            objects_[i]->update(*this);
    }

    for (const auto& object : objects_)
    {
        if (object && !object->is_alive())
        {
            const int x = object->x();
            const int y = object->y();
            if (in_bounds(x, y) && cell(x, y).object() == object.get())
            {
                cell(x, y).clear();
            }
        }
    }

    objects_.erase(
        std::remove_if(objects_.begin(), objects_.end(),
            [](const std::unique_ptr<Object>& object)
            {
                return !object->is_alive();
            }),
        objects_.end());

}

void Ocean::print(std::ostream& out) const
{
    for (int y = 0; y < height_; ++y)
    {
        for (int x = 0; x < width_; ++x)
        {
            const auto* object = cell(x, y).object();
            out << (object ? object->symbol() : '.');
        }
        out << '\n';
    }
}

bool Ocean::has_objects() const
{
    return !objects_.empty();
}

bool Ocean::has_only_one_type() const
{
    {
    const Object* first_live_object = nullptr;
    std::size_t start_index = 0;

    const char stone_symbol = '#';

    for (std::size_t i = 0; i < objects_.size(); ++i)
    {
        if (objects_[i] && objects_[i]->symbol() != stone_symbol) 
        {
            first_live_object = objects_[i].get();
            start_index = i + 1;
            break;
        }
    }

    if (first_live_object == nullptr)
    {
        return false; 
    }

    for (std::size_t i = start_index; i < objects_.size(); ++i)
    {
        if (!objects_[i])continue;

        if (objects_[i]->symbol() == stone_symbol)
        {
            continue;
        }

        if (typeid(*objects_[i]) != typeid(*first_live_object))
        {
            return false;
        }
    }

    return true; 
}
}
