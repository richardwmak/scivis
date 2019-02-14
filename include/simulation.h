#ifndef SIMULATION_H
#define SIMULATION_H

#include "config.h"
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
    //     };
    // };
    fftw_real velocity[Config::NUM_CELLS];      // velocity field
    fftw_real smoke_density[Config::NUM_CELLS]; // density at each point
    fftw_real force[Config::NUM_CELLS];         // user-applied force

    void initialise();
};

class Simulation
{
  public:
    SimulationState cur_state;
    SimulationState old_state;

    rfftwnd_plan plan_rc, plan_cr; // necessary for fftw to do FFT

    void initialise();

    void fft_r_to_c(fftw_real *old_dataset, fftw_complex *new_dataset);

    void fft_c_to_r(fftw_complex *old_dataset, fftw_real *new_dataset);
};

#endif