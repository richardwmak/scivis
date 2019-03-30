#pragma once

#include <rfftw.h>
#include <vector>

class RenderSmoke
{
  public:
    static void render_smoke(std::vector<fftw_real> scalar_field);
};