#include "mailbox.h"

void Mailbox::push(const Message& message)
{
    std::lock_guard<std::mutex> lock (queue_mutex_);
    queue_.push(message);
    queue_cond_.notify_one();
}

Message Mailbox::pop_block()
{
    std::unique_lock<std::mutex> lock (queue_mutex_);
    queue_cond_.wait(lock, [&]{ return !queue_.empty(); });

    Message message = queue_.front();
    queue_.pop();

    return message;
}