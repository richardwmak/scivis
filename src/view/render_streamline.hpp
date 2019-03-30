#pragma once

#include <Fl/gl.h>
#include <rfftw.h>
#include <vector>

class RenderStreamline
{
  public:
    static void render_streamlines(std::vector<fftw_real> velocity_x,
                                   std::vector<fftw_real> velocity_y);
    static void render_streamline(float                  x_pixel,
                                  float                  y_pixel,
                                  std::vector<fftw_real> velocity_x,
                                  std::vector<fftw_real> velocity_y);
};