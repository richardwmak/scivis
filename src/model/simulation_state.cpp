#include "simulation_state.hpp"
#include "config.hpp"
#include <rfftw.h>

SimulationState::SimulationState()
{
    size_t array_size;
    array_size = Config::GRID_SIZE * 2 * (Config::GRID_SIZE / 2 + 1) * sizeof(fftw_real);
    velocity_x = (fftw_real *)malloc(array_size);
    velocity_y = (fftw_real *)malloc(array_size);

    array_size    = Config::NUM_CELLS * sizeof(fftw_real);
    force_x       = (fftw_real *)malloc(array_size);
    force_y       = (fftw_real *)malloc(array_size);
    smoke_density = (fftw_real *)malloc(array_size);

    for (int i = 0; i < Config::GRID_SIZE; i++)
    {
        for (int j = 0; j < Config::GRID_SIZE; j++)
        {
            velocity_x[i + j]    = 0.0f;
            velocity_y[i + j]    = 0.0f;
            smoke_density[i + j] = 0.0f;
            force_x[i + j]       = 0.0f;
            force_y[i + j]       = 0.0f;
        }
    }
}