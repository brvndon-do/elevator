#pragma once

#include <variant>

struct AddStop
{
    int floor;
};

struct Step { };

struct Shutdown { };

using Message = std::variant<AddStop, Step, Shutdown>;