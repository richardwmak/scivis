#ifndef CONFIG_H
#define CONFIG_H

// SIMULATION PARAMETERS
namespace Config
{
const int GRID_SIZE = 50;                          // size of simulation grid
const int NUM_CELLS = GRID_SIZE * GRID_SIZE;       // number of nodes we are dealing with
extern double time_step;                                  // simulation time step
extern float visc;                                        // fluid viscosityx1
};                                                        // namespace Config

#endif