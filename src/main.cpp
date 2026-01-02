#include <print>

#include "elevator.h"

int main()
{
    Elevator* mainElevator = new Elevator();

    mainElevator->send(AddStop { .floor = 2 });
    mainElevator->send(AddStop { .floor = 1 });
    mainElevator->send(AddStop { .floor = 3 });
    mainElevator->send(AddStop { .floor = 4 });
    mainElevator->send(Shutdown {});

    delete mainElevator;

    return 0;
}