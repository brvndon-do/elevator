#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

#include "messages.h"

class Mailbox
{
    public:
        void push(const Message& message);
        Message pop_block();

    private:
        std::queue<Message> queue_;
        std::mutex queue_mutex_;
        std::condition_variable queue_cond_;
};