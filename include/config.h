#ifndef CONFIG_H
#define CONFIG_H

namespace Config
{
// SIMULATION PARAMETERS
const int GRID_SIZE = 50;                    // size of simulation grid
const int NUM_CELLS = GRID_SIZE * GRID_SIZE; // number of nodes we are dealing with
extern double time_step;                     // simulation time step
extern float visc;                           // fluid viscosityx1

//--- VISUALIZATION PARAMETERS ---------------------------------------------------------------------
extern int winWidth, winHeight; // size of the graphics window, in pixels
extern int color_dir;           // use direction color-coding or not
extern float vec_scale;         // scaling of hedgehogs
extern int draw_smoke;          // draw the smoke or not
extern int draw_vecs;           // draw the vector field or not
const int COLOR_BLACKWHITE = 0; // different types of color mapping: black-and-white, rainbow,
                                // banded
const int COLOR_RAINBOW = 1;    //
const int COLOR_BANDS   = 2;    //
extern int scalar_col;          // method for scalar coloring
extern bool frozen;             // toggles on/off the animation
};                              // namespace Config

#endif