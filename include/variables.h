#ifndef VARIABLES_H
#define VARIABLES_H

#include <rfftw.h>

// SIMULATION PARAMETERS
namespace Config
{
extern const int GRID_SIZE = 50;    // size of simulation grid
extern double time_step; // simulation time step
extern float visc;       // fluid viscosityx1
};                       // namespace GlobalSettings

// Stores all the current simulation data
class SimulationState
{
  public:
    fftw_real velocity[Config::GRID_SIZE][Config::GRID_SIZE]; // velocity field
    fftw_real smoke_density[Config::GRID_SIZE][Config::GRID_SIZE]; // density at each point
    fftw_real force[Config::GRID_SIZE][Config::GRID_SIZE]; // user-applied force

    void initialise()
    {
        for(int i = 0; i < Config::GRID_SIZE; i++)
        {
            for(int j = 0; j < Config::GRID_SIZE; j++)
            {
                velocity[i][j] = 0;
                smoke_density[i][j] = 0;
                force[i][j] = 0;
            };
        };
    };
};

#endif