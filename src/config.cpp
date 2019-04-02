#include "config.hpp"
#include <rfftw.h>

namespace Config
{
    double time_step = 0.4;
    float  visc      = 0.001;
    int    win_width, win_height;
    float  grid_width;
    float  num_glyphs = 50;

    bool color_dir       = false;
    bool draw_smoke      = false;
    bool draw_vecs       = true;
    bool frozen          = false;
    bool vector_color    = false;
    bool draw_streamline = false;
    bool draw_slices     = false;

    float     vec_scale = 1000;
    bool      scaling   = true;
    fftw_real clamp_min = 0;
    fftw_real clamp_max = 1;

    bool gradient  = true;
    int  num_bands = 7;

    int scalar_col = COLOR_BLACKWHITE;

    int scalar_choice = SCALAR_SMOKE;
    int vector_choice = VECTOR_VELOCITY;

    int vector_shape = HEDGEHOG;

    int streamline_options = 0;

    int streamline_max_length = 100;

    long unsigned int buffer_size  = 200;
    int               buffer_delta = 10;

} // namespace Config