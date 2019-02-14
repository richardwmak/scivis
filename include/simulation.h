#ifndef SIMULATION_H
#define SIMULATION_H

#include "config.h"
#include <rfftw.h>

// Stores all the current simulation data
class SimulationState
{
  public:
    // for code readability
    static const int grid_size = Config::GRID_SIZE;
    static const int number_of_nodes = Config::NUMBER_OF_NODES;

    // if we do the following type of loop:
    //
    // for (int i = 0; i < grid_size; i++)
    // {
    //     for (int j = 0; j < grid_size; j++)
    //     {
    //         velocity[i + j] <-- this will be the (i, j)th entry
    //     };
    // };
    fftw_real velocity[number_of_nodes];      // velocity field
    fftw_real smoke_density[number_of_nodes]; // density at each point
    fftw_real force[number_of_nodes];         // user-applied force

    void initialise();
};

class Simulation
{
  public:
    SimulationState cur_state;
    SimulationState old_state;

    rfftwnd_plan plan_rc, plan_cr; // necessary for fftw to do FFT

    // for code readability
    static const int grid_size;
    static const int number_of_nodes;

    void initialise();

    void fft_r_to_c(fftw_real *old_dataset, fftw_complex *new_dataset);

    void fft_c_to_r(fftw_complex *old_dataset, fftw_real *new_dataset);
};

#endif