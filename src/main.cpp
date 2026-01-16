#include <print>

#include "elevator.h"

struct ScheduledMessage
{
    int tick;
    Message message;
};

int main()
{
    const int TICKS = 100;

    Elevator* mainElevator = new Elevator(6);

    std::vector<ScheduledMessage> scheduledMessages
    {
        { 2, AddCall { 
                .type = CallType::Hall,
                .direction = Direction::Up,
                .floor = 5
            }
        },
        { 2, AddCall { 
                .type = CallType::Hall,
                .direction = Direction::Up,
                .floor = 7
            }
        },
        { 2, AddCall { 
                .type = CallType::Hall,
                .direction = Direction::Up,
                .floor = 9
            }
        },
        { 5, AddCall { 
                .type = CallType::Hall,
                .direction = Direction::Down,
                .floor = 3
            }
        },
        { 10, AddCall { 
                .type = CallType::Hall,
                .direction = Direction::Up,
                .floor = 11
            }
        },
    };

    // tick-based simulation
    for (int i = 0; i < TICKS; i++)
    {
        for (const auto& message : scheduledMessages)
        {
            if (message.tick == i)
                mainElevator->send(message.message);
        }

        mainElevator->send(Step { });
    }

    mainElevator->send(Shutdown { });

    delete mainElevator;

    return 0;
}