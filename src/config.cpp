#include "config.hpp"

namespace Config
{
    double time_step = 0.4;
    float  visc      = 0.001;
    int    win_width, win_height;
    bool   color_dir  = false;
    float  vec_scale  = 1000;
    bool   draw_smoke = false;
    bool   draw_vecs  = true;
    int    scalar_col = COLOR_BLACKWHITE;
    bool   frozen     = false;
    bool   gradient   = true;
    int    num_verts  = 2;
    bool   scaling    = false;
    float  num_glyphs = 100;

    int scalar_to_draw = SCALAR_SMOKE;
    int vector_to_draw = VECTOR_VELOCITY;
} // namespace Config