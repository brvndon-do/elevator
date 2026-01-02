#include "elevator.h"

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
            else if constexpr (std::is_same_v<T, Shutdown>)
            {
                running_ = false;
            }
        }, message);
    }
}