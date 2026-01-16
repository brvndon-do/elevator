#include <print>

#include "elevator.h"

Direction calculateCurrentDirection(
    int currentFloor,
    const std::set<int>& ascendingStops,
    const std::set<int, std::greater<int>>& descendingStops
)
{
    std::set<int>::const_iterator up = ascendingStops.upper_bound(currentFloor);
    if (up != ascendingStops.end())
        return Direction::Up;

    std::set<int>::const_iterator down = descendingStops.upper_bound(currentFloor);
    if (down != descendingStops.end())
        return Direction::Down;

    return Direction::Nil;
}

Direction calculateNewDirection(int currentFloor, int requestedFloor)
{
    if (currentFloor == requestedFloor)
        return Direction::Nil;

    return requestedFloor - currentFloor > 0
        ? Direction::Up
        : Direction::Down;
}

void addStop(
    Direction direction,
    int floor,
    std::set<int>& ascendingStops,
    std::set<int, std::greater<int>>& descendingStops
)
{
    switch (direction)
    {
        case Direction::Down:
            descendingStops.insert(floor);
        break;
        case Direction::Up:
        case Direction::Nil:
            ascendingStops.insert(floor);
            break;
    }
}

Elevator::Elevator()
{
    thread_ = std::thread (&Elevator::run, this);
}

Elevator::~Elevator()
{
    mailbox_.push(Shutdown {});

    if (thread_.joinable())
        thread_.join();
}

void Elevator::send(const Message& message)
{
    mailbox_.push(message);
}

int Elevator::currentFloor() const
{
    return currentFloor_;
}

void Elevator::run()
{
    while (running_)
    {
        Message message = mailbox_.pop_block();
        std::visit([this](auto&& arg)
        {
            using T = std::decay_t<decltype(arg)>;

            if constexpr (std::is_same_v<T, AddCall>)
            {
                AddCall call = static_cast<AddCall>(arg);

                switch (call.type)
                {
                    case CallType::Hall:
                        {
                            addStop(call.direction, call.floor, ascendingStops_, descendingStops_);
                            direction_ = call.direction;
                        }
                        break;
                    case CallType::Car:
                        {
                            Direction newDirection = calculateNewDirection(currentFloor_, call.floor);
                            addStop(newDirection, call.floor, ascendingStops_, descendingStops_);
                            direction_ = newDirection;
                        }
                        break;
                }
            }
            else if constexpr (std::is_same_v<T, Step>)
            {
                direction_ = calculateCurrentDirection(currentFloor_, ascendingStops_, descendingStops_);

                switch (direction_)
                {
                    case Direction::Up:
                        currentFloor_++;
                        break;
                    case Direction::Down:
                        currentFloor_--;
                        break;
                    case Direction::Nil:
                        break;
                }

                if (ascendingStops_.count(currentFloor_))
                    ascendingStops_.erase(currentFloor_);

                if (descendingStops_.count(currentFloor_))
                    descendingStops_.erase(currentFloor_);

                std::println("Elevator is currently on {} floor", currentFloor_);
            }
            else if constexpr (std::is_same_v<T, Shutdown>)
            {
                running_ = false;
            }
        }, message);
    }
}