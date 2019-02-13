#include <rfftw.h>

// SIMULATION PARAMETERS
const int GRID_SIZE = 50;   // size of simulation grid

// Stores all variables that affect the simulation
struct SimulationVars
{
    static double time_step;           // simulation time step
    static float visc;                 // fluid viscosity
};

// Stores all the current simulation data
struct SimulationState
{
    static fftw_real *vel_x, *vel_y; // (vel_x, vel_y) = velocity field
};

// Stores the smoke density
struct SmokeDensity
{
    static fftw_real *density;
};

// Stores the force the user applies via mouse
struct SimulationForce
{
    static fftw_real *force_x, *force_y;
};
