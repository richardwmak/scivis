
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
    //     }
    // }
    fftw_real *velocity_x;    // velocity field
    fftw_real *velocity_y;    //
    fftw_real *smoke_density; // density at each point
    fftw_real *force_x;       // user-applied force
    fftw_real *force_y;

    SimulationState();
};

class Simulation
{
  public:
    SimulationState cur_state;
    SimulationState old_state;

    rfftwnd_plan plan_rc, plan_cr; // necessary for fftw to do FFT

    Simulation();

    void FFT(int direction, void *vx);

    int clamp(float x);

    void compute_next_step();

    void diffuse_matter();

    void set_forces();

    void do_one_simulation_step();
};

#endif