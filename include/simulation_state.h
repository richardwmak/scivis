#pragma once

#include <rfftw.h>

// Stores all the current simulation data
class SimulationState
{
  public:
    // if we do the following type of loop:
    //
    // for (int i = 0; i < Config::GRID_SIZE; i++)
    // {
    //     for (int j = 0; j < Config::GRID_SIZE; j++)
    //     {
    //         velocity[i + j] <-- this will be the (i, j)th entry
    //     }
    // }
    fftw_real *velocity_x;    // velocity field
    fftw_real *velocity_y;    //
    fftw_real *smoke_density; // density at each point
    fftw_real *force_x;       // user-applied force
    fftw_real *force_y;

    SimulationState();
};