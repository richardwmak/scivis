#pragma once

namespace Config
{
    // SIMULATION PARAMETERS
    const int     GRID_SIZE = 100;                   // size of simulation grid
    const int     NUM_CELLS = GRID_SIZE * GRID_SIZE; // number of nodes we are dealing with
    extern double time_step;                         // simulation time step
    extern float  visc;                              // fluid viscosityx1

    // VISUALIZATION PARAMETERS
    extern int   win_width, win_height; // size of the graphics window, in pixels
    extern bool  color_dir;             // use direction color-coding or not
    extern float vec_scale;             // scaling of hedgehogs
    extern bool  draw_smoke;            // draw the smoke or not
    extern bool  draw_vecs;             // draw the vector field or not
    extern float num_glyphs;            // how many glyphs to draw
    const int   COLOR_BLACKWHITE = 0; // different types of color mapping: black-and-white, rainbow,
    const int   COLOR_RAINBOW    = 1; // banded
    const int   COLOR_RED_WHITE  = 2; //
    extern int  scalar_col;           // method for scalar coloring
    extern bool frozen;               // toggles on/off the animation
    extern bool scaling;              // scaling if true, clamping if false;

    // setting that determines what type of scalar/vector gets drawn
    const int SCALAR_SMOKE    = 0;
    const int SCALAR_FORCE    = 1;
    const int SCALAR_VELOCITY = 2;
    const int VECTOR_VELOCITY = 0;
    const int VECTOR_FORCE    = 1;

    extern int scalar_to_draw;
    extern int vector_to_draw;

    extern bool gradient;  // how to draw color_bar
    extern int  num_verts; //

    const int LOWER_LEFT  = 0;
    const int LOWER_RIGHT = 1;
    const int UPPER_RIGHT = 2;
    const int UPPER_LEFT  = 3;
} // namespace Config
