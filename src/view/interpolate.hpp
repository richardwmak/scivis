#pragma once

#include <rfftw.h>
#include <vector>

class Interpolate
{
  public:
    static fftw_real bilin(float x, float y, std::vector<fftw_real> field);
};