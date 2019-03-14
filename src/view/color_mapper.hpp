#pragma once
#include <rfftw.h>
#include <vector>

class ColorMapper
{
  public:
    // various functions that visualize() uses to draw
    // because of the way C++ works with arrays, actually passing an array RGB of 3 elements is
    // unnecessary. Simply passing the first element of the relevant array is sufficient.
    static void greyscale(float value, float RGB[3]);
    static void rainbow(float value, float RGB[3]);
    static void black_white(float value, float RGB[3]);
    static void red_white(float value, float RGB[3]);
    static void set_colormap(float value, float RGB[3]);
    // static void direction_to_color(float RGB[3], float x, float y);

    static fftw_real max_scalar;
    static fftw_real set_max_scalar(std::vector<fftw_real> scalar_field);
};