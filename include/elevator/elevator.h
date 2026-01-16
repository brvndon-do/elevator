#pragma once

#include <set>
#include <thread>

#include "messages.h"
#include "mailbox.h"

class Elevator
{
    public:
        Elevator();
        ~Elevator();
        void send(const Message& message);
        int currentFloor() const;

    private:
        void run();

        bool running_ = true;
        int currentFloor_;
        Direction direction_;
        std::set<int> ascendingStops_;
        std::set<int, std::greater<int>> descendingStops_;
        Mailbox mailbox_;
        std::thread thread_;
};