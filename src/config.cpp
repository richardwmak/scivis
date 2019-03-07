#include "config.hpp"

namespace Config
{
    double time_step = 0.4;
    float  visc      = 0.001;
    int    win_width, win_height;
    float  num_glyphs = 100;

    bool  color_dir  = false;
    bool  draw_smoke = false;
    bool  draw_vecs  = true;
    bool  frozen     = false;
    float vec_scale  = 1000;
    bool  scaling    = false;
    int   clamp_min  = 0;
    int   clamp_max  = 1;

    bool gradient  = true;
    int  num_bands = 7;

    int scalar_col = COLOR_BLACKWHITE;

    int scalar_to_draw = SCALAR_SMOKE;
    int vector_to_draw = VECTOR_VELOCITY;
} // namespace Config