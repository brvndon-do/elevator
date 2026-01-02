#pragma once

#include <set>
#include <thread>

#include "messages.h"
#include "mailbox.h"

enum class Direction
{
    Nil,
    Up,
    Down
};

class Elevator
{
    public:
        Elevator();
        ~Elevator();
        void send(const Message& message);

    private:
        void run();

        bool running_ = true;
        int currentFloor_;
        Direction direction_;
        std::set<int> stops_;
        Mailbox mailbox_;
        std::thread thread_;
};