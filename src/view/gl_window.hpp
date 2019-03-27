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

    void set_scalar_data(std::vector<fftw_real> new_scalar_field, fftw_real max_scalar);
    void set_vector_data(std::vector<fftw_real> new_vector_field_x,
                         std::vector<fftw_real> new_vector_field_y);
    void set_vel_data(std::vector<fftw_real> new_vel_field_x,
                      std::vector<fftw_real> new_vel_field_y);

    // (x,y) should be relative to the grid i.e. (2.5, 2.5) would be the point in the cell defined
    // by (2,2), (2, 3), (3,2), (3,3) in the field vector
    float bilin_interpolate(float x, float y, std::vector<fftw_real> field);

  private:
    std::vector<fftw_real> scalar_field;
    std::vector<fftw_real> vector_field_x;
    std::vector<fftw_real> vector_field_y;
    std::vector<fftw_real> vel_field_x;
    std::vector<fftw_real> vel_field_y;
};