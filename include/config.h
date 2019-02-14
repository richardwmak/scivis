#ifndef CONFIG_H
#define CONFIG_H

// SIMULATION PARAMETERS
namespace Config
{
extern const int GRID_SIZE = 50;       // size of simulation grid
extern const int NUMBER_OF_NODES = GRID_SIZE*GRID_SIZE; // number of nodes we are dealing with
extern double time_step;          // simulation time step
extern float visc;                // fluid viscosityx1
};

#endif