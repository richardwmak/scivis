#include "config.h"

namespace Config
{
    double time_step = 0.4;
    float  visc      = 0.001;
    int    win_width, win_height;
    bool   color_dir  = false;
    float  vec_scale  = 1000;
    bool   draw_smoke = false;
    bool   draw_vecs  = true;
    int    scalar_col = 0;
    bool   frozen     = false;
    bool   gradient   = true;
    int    num_verts  = 2;
} // namespace Config