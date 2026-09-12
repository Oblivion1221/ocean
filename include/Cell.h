#pragma once

class Object;

class Cell
{
public:
    bool empty() const { return object_ == nullptr; }

    Object* object() { return object_; }
    const Object* object() const { return object_; }

    void set_object(Object* object) { object_ = object; }
    void clear() { object_ = nullptr; }

private:
    Object* object_ = nullptr;
};
