#pragma once

#include <Fl/gl.h>
#include <rfftw.h>
#include <vector>

typedef std::pair<GLfloat, GLfloat> coord;

class RenderStreamline
{
  public:
    static void render_streamlines(std::vector<fftw_real> velocity_x,
                                   std::vector<fftw_real> velocity_y,
                                   std::vector<coord>     seeds,
                                   GLfloat                height);
    static void render_streamline(float                  x_pixel,
                                  float                  y_pixel,
                                  std::vector<fftw_real> velocity_x,
                                  std::vector<fftw_real> velocity_y,
                                  GLfloat                height);
    static void render_streamsurf(std::vector<std::vector<fftw_real>> buffer_velocity_x,
                                  std::vector<std::vector<fftw_real>> buffer_velocity_y,
                                  std::vector<coord>                  seeds);
};