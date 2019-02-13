#ifndef VARIABLES_H
#define VARIABLES_H

#include <rfftw.h>

// SIMULATION PARAMETERS
namespace Config
{
const int GRID_SIZE = 50;                    // size of simulation grid
int NUMBER_OF_NODES = GRID_SIZE * GRID_SIZE; // number of nodes we are dealing with
double time_step = 0.01;                     // simulation time step
float visc = 0.01;                           // fluid viscosityx1
};                                           // namespace Config

// Stores all the current simulation data
class SimulationState
{
  public:
    // for code readability
    grid_size = Config::GRID_SIZE
        number_of_nodes = Config::NUMBER_OF_NODES;

    // if we do the following type of loop:
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

    void initialise()
    {
        for (int i = 0; i < grid_size; i++)
        {
            for (int j = 0; j < grid_size; j++)
            {
                velocity[i + j] = float 0;
                smoke_density[i + j] = float 0;
                force[i + j] = float 0;
            };
        };
    };
};

class Simulation
{
  public:
    SimulationState cur_state;
    SimulationState old_state;

    rfftwnd_plan plan_rc, plan_cr; // necessary for fftw to do FFT

    // for code readability
    grid_size = Config::GRID_SIZE;
    number_of_nodes = Config::NUMBER_OF_NODES;

    void initialise()
    {
        cur_state.initialise();
        old_state.initialise();

        plan_rc = rfftw2d_create_plan(grid_size, grid_size, FFTW_REAL_TO_COMPLEX, FFTW_IN_PLACE);
        plan_cr = rfftw2d_create_plan(grid_size, grid_size, FFTW_COMPLEX_TO_REAL, FFTW_IN_PLACE);
    };

    void fft_r_to_c(fftw_real *old_dataset, fftw_complex *new_dataset)
    {
        rfftwnd_one_real_to_complex(plan_rc, fftw_real * old_dataset, fftw_complex * new_dataset);
    };

    void fft_c_to_r(fftw_complex *old_dataset, fftw_real *new_dataset)
    {
        rfftwnd_one_complex_to_real(plan_cr, fftw_complex * old_dataset, fftw_real * new_dataset);
    };
};

#endif