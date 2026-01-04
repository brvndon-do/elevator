#include <print>

#include "elevator.h"

int main()
{
    const int TICKS = 100;

    Elevator* mainElevator = new Elevator();

    for (int i = 0; i < TICKS; i++)
    {
        if (i == 2)
            mainElevator->send(AddStop { .floor = 5 });
        if (i == 5)
            mainElevator->send(AddStop { .floor = 3 });
        if (i == 10)
            mainElevator->send(AddStop { .floor = 7 });

        mainElevator->send(Step { });
    }

    mainElevator->send(Shutdown { });

    delete mainElevator;

    return 0;
}