#pragma once
#include <rfftw.h>

namespace Config
{
    // SIMULATION PARAMETERS
    const int     GRID_SIZE = 50;                    // size of simulation grid
    const int     NUM_CELLS = GRID_SIZE * GRID_SIZE; // number of nodes we are dealing with
    extern double time_step;                         // simulation time step
    extern float  visc;                              // fluid viscosityx1
    extern int    win_width, win_height;             // size of the graphics window, in pixels
    extern float  num_glyphs;                        // how many glyphs to draw

    // VISUALIZATION PARAMETERS
    extern bool  draw_smoke;   // draw the smoke or not
    extern bool  draw_vecs;    // draw the vector field or not
    extern bool  frozen;       // toggles on/off the animation
    extern float vec_scale;    // scaling of hedgehogs
    extern bool  vector_color; // color the vectors according to the scalar_col or not

    extern bool      scaling;   // scaling if true, clamping if false;
    extern fftw_real clamp_min; // clamping values if we are clamping
    extern fftw_real clamp_max;

    extern bool gradient;  // draw a gradient if true, use banding if false
    extern int  num_bands; // number of bands if gradient is false

    // what color do we draw the scalar?
    extern int scalar_col;           // method for scalar coloring
    const int  COLOR_BLACKWHITE = 0; // different types of color mapping: black-and-white, rainbow,
    const int  COLOR_RAINBOW    = 1; // banded
    const int  COLOR_RED_WHITE  = 2; //

    // setting that determines what type of scalar/vector gets drawn
    extern int scalar_choice;
    extern int vector_choice;

    const int SCALAR_SMOKE    = 0;
    const int SCALAR_FORCE    = 1;
    const int SCALAR_VELOCITY = 2;
    const int VECTOR_VELOCITY = 0;
    const int VECTOR_FORCE    = 1;

    // what shape of vector do we draw?
    extern int vector_shape;
    const int  HEDGEHOG = 0;
    const int  CONE     = 1;
    const int  ARROW_2D = 2;

} // namespace Config
