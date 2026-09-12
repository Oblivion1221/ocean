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


// Explanation of unique ptrs
//
// {
//     int *a = new int{}; // a = 0x00000004
//     int *b = a;         // b = 0x00000004
//     delete a;           // a = nullptr, b = 0x00000004
//     print(b);           // BOOM! Segfault
// }

// {
//     std::unique_ptr<int> b;         // b.ptr = nullptr
//     {
//         std::unique_ptr<int> a {new int{}}; // a.ptr = 0x00000004 {156}
//         b = a;                              // b.ptr = 0x00000004
//     }
//     // b.ptr = 0x00000004
//     // print(*b) // BOOM! Segfault

//     // The second variant
//     {
//         std::unique_ptr<int> a {new int{}}; // a.ptr = 0x00000004 {156}
//         b = a;                              // b.ptr = 0x00000008 {156}
//     }
//     // b.ptr = 0x00000008
//     // print(*b) // OK!
//     // But it's not okay! We don't want to allocate and copy objects!

//     // Solution: move constructors, move assignment
//     {
//         std::unique_ptr<int> a {new int{}}; // a.ptr = 0x00000004 {156}
//         b = std::move(a());                 // b.ptr = 0x00000004 {156}, a.ptr = nullptr
//     }
//     // b.ptr = 0x00000004
//     // print(*b) // OK!
// }


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
    // Snapshot prevents newly spawned objects from being updated
    // in the same iteration.
    const std::size_t count = objects_.size();

    for (std::size_t i = 0; i < count && i < objects_.size(); ++i)
    {
        if (objects_[i])
            objects_[i]->update(*this);
    }

    // 0. Было:
    //    [1, 0, 1, 0, 1, 0, 0, 0, 1]
    // 1. auto it = std::remove_if(objects_.begin(), objects_.end(),
    //                  [](const std::unique_ptr<Object>& object)
    //                  {
    //                      return !object->is_alive();
    //                  });
    // 2. Стало:
    //    [1, 1, 1, 1, 0, 0, 0, 0, 0]
    //                it^
    // 3. objects.erase(it, objects.end());
    // 4. Стало
    //    [1, 1, 1, 1]

    objects_.erase(
        std::remove_if(objects_.begin(), objects_.end(),
            [](const std::unique_ptr<Object>& object)
            {
                return !object->is_alive();
            }),
        objects_.end());

    // TODO: clean unused cells
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
    // TODO:
    return true;
}

bool Ocean::has_only_one_type() const
{
    // TODO:
    return true;
}
