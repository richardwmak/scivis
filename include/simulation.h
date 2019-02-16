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
    fftw_real velocity_x[Config::NUM_CELLS];    // velocity field
    fftw_real velocity_y[Config::NUM_CELLS];    //
    fftw_real smoke_density[Config::NUM_CELLS]; // density at each point
    fftw_real force_x[Config::NUM_CELLS];       // user-applied force
    fftw_real force_y[Config::NUM_CELLS];

    void initialise();
};

class Simulation
{
  public:
    SimulationState cur_state;
    SimulationState old_state;

    rfftwnd_plan plan_rc, plan_cr; // necessary for fftw to do FFT

    void initialise();

    void fft_r_to_c(void *dataset);

    void fft_c_to_r(void *dataset);

    void compute_next_step();

    void diffuse_matter();

    void set_forces();

    void do_one_simulation_step();
};

#endif