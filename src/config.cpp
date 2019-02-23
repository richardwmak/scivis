#include "config.h"

namespace Config
{
    double time_step = 0.01;
    float  visc      = 0.01;
    int    win_width, win_height;
    int    color_dir  = 0;
    float  vec_scale  = 1000;
    int    draw_smoke = 0;
    int    draw_vecs  = 1;
    int    scalar_col = 0;
    bool   frozen     = false;
    bool   first_draw = true;
} // namespace Config