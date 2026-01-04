#include <print>

#include "elevator.h"

Direction calculateDirection(int currentFloor, const std::set<int>& stops)
{
    if (stops.empty() || stops.count(currentFloor))
        return Direction::Nil;
        
    std::set<int>::const_iterator up = stops.lower_bound(currentFloor);
    if (up != stops.end() && *up > currentFloor)
        return Direction::Up;

    if (up != stops.begin())
    {
        std::set<int>::const_iterator down = std::prev(up);
        if (*down < currentFloor)
            return Direction::Down;
    }

    return Direction::Nil;
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

            if constexpr (std::is_same_v<T, AddStop>)
            {
                stops_.insert(static_cast<AddStop>(arg).floor);
            }
            else if constexpr (std::is_same_v<T, Step>)
            {
                direction_ = calculateDirection(currentFloor_, stops_);

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

                if (stops_.count(currentFloor_))
                    stops_.erase(currentFloor_);

                std::println("Elevator is currently on {} floor", currentFloor_);
            }
            else if constexpr (std::is_same_v<T, Shutdown>)
            {
                running_ = false;
            }
        }, message);
    }
}