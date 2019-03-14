#pragma once

#include "config.hpp"
#include "simulation_state.hpp"
#include <rfftw.h>
#include <vector>

// this is the class that handles the entirety of the actual simulation it stores the data in
// cur_state and old_state and upon running do_one_simulation_step() will compute the next timestep
// in the simulation
class Simulation
{
  public:
    SimulationState cur_state;
    SimulationState old_state;

    fftw_real              max_value;

    rfftwnd_plan plan_rc, plan_cr; // necessary for fftw to do FFT

    Simulation();

    void FFT(int direction, void *vx);

    void apply_force(int, int);

    int clamp(float x);

    void compute_next_step();

    void diffuse_matter();

    void set_forces();

    void do_one_simulation_step();

    std::pair<std::vector<fftw_real>, std::vector<fftw_real>>
    compute_gradient(std::vector<fftw_real> scalar_field);

    std::vector<fftw_real> get_scalar_field();

    std::vector<fftw_real> get_vector_field_x();
    std::vector<fftw_real> get_vector_field_y();
    std::vector<fftw_real> get_vector_field(fftw_real *force, fftw_real *velocity);
};
