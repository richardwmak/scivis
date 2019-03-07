#pragma once

#include "color_mapper.hpp"
#include "config.hpp"
#include <FL/gl.h>
#include <Fl/Fl_Gl_Window.H>
#include <rfftw.h>
#include <vector>

typedef std::pair<GLfloat, GLfloat> coord;

class GlWindow : public Fl_Gl_Window
{
  public:
    int X, Y, W, H;

    GlWindow(int X, int Y, int W, int H);

    void visualize();

    void draw();
    // int  handle(int event);

    void set_scalar_data(std::vector<fftw_real> new_scalar_field);
    void set_vector_data(std::vector<fftw_real> new_vector_field_x,
                         std::vector<fftw_real> new_vector_field_y);

    void render_vector(coord start, coord end);
    void render_hedgehog(coord start, coord end);
    void render_cone(coord start, coord end);

  private:
    std::vector<fftw_real> scalar_field;
    std::vector<fftw_real> vector_field_x;
    std::vector<fftw_real> vector_field_y;
};