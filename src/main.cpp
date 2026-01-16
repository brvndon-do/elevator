#include <print>

#include "elevator.h"

int main()
{
    const int TICKS = 100;

    Elevator* mainElevator = new Elevator();

    // tick-based simulation
    for (int i = 0; i < TICKS; i++)
    {
        if (i == 2)
        {
            mainElevator->send(AddCall { 
                .type = CallType::Hall,
                .direction = Direction::Up,
                .floor = 5
            });
            mainElevator->send(AddCall { 
                .type = CallType::Hall,
                .direction = Direction::Up,
                .floor = 7
            });
            mainElevator->send(AddCall { 
                .type = CallType::Hall,
                .direction = Direction::Up,
                .floor = 9
            });
        }
        if (i == 5)
            mainElevator->send(AddCall { 
                .type = CallType::Hall,
                .direction = Direction::Down,
                .floor = 3
            });
        if (i == 10)
            mainElevator->send(AddCall { 
                .type = CallType::Hall,
                .direction = Direction::Up,
                .floor = 11
            });

        mainElevator->send(Step { });
    }

    mainElevator->send(Shutdown { });

    delete mainElevator;

    return 0;
}