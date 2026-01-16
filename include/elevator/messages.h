#pragma once

#include <variant>

#include "shared.h"

enum class CallType
{
    Hall,
    Car
};

struct AddCall
{
    CallType type;
    Direction direction;
    int floor;
};

struct Step { };

struct Shutdown { };

using Message = std::variant<AddCall, Step, Shutdown>;