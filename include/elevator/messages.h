#pragma once

#include <variant>

struct AddStop
{
    int floor;
};

struct Shutdown {};

using Message = std::variant<AddStop, Shutdown>;