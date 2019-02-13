#include <rfftw.h>
#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include <variables.h>

SimulationState init_simulation()
{
    SimulationState new_state;

    new_state.initialise();

    return new_state;
};

int main()
{
    SimulationState state = init_simulation();

    std::cout << state.force[1][1];
};