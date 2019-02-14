#include "config.h"
#include "simulation.h"
#include <rfftw.h>

void SimulationState::initialise()
{
    for (int i = 0; i < grid_size; i++)
    {
        for (int j = 0; j < grid_size; j++)
        {
            velocity[i + j] = 0.0;
            smoke_density[i + j] = 0.0;
            force[i + j] = 0.0;
        };
    };
};

void Simulation::initialise()
{
    cur_state.initialise();
    old_state.initialise();

    plan_rc = rfftw2d_create_plan(grid_size, grid_size, FFTW_REAL_TO_COMPLEX, FFTW_IN_PLACE);
    plan_cr = rfftw2d_create_plan(grid_size, grid_size, FFTW_COMPLEX_TO_REAL, FFTW_IN_PLACE);
};

void Simulation::fft_c_to_r(fftw_real *old_dataset, fftw_complex *new_dataset)
{
    rfftwnd_one_real_to_complex(plan_rc, fftw_real * old_dataset, fftw_complex * new_dataset);
};

void Simulation::fft_r_to_c(fftw_complex *old_dataset, fftw_real *new_dataset)
{
    rfftwnd_one_complex_to_real(plan_cr, fftw_complex * old_dataset, fftw_real * new_dataset);
};