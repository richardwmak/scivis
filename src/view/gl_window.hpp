#pragma once

#include "color_mapper.hpp"
#include "config.hpp"
#include <FL/gl.h>
#include <Fl/Fl_Gl_Window.H>
#include <queue>
#include <rfftw.h>
#include <vector>

typedef std::pair<GLfloat, GLfloat> coord;

class GlWindow : public Fl_Gl_Window
{
  public:
    int                X, Y, W, H;
    GLdouble           eye[3];
    GLdouble           center[3];
    GLdouble           up[3];
    GLdouble           angle, radius;
    GLdouble           relative_x, relative_z;
    std::vector<coord> seeds;

    GlWindow(int X, int Y, int W, int H);

    void visualize_slices();
    void visualize(GLfloat                height,
                   std::vector<fftw_real> scalar_field,
                   std::vector<fftw_real> vector_field_x,
                   std::vector<fftw_real> vector_field_y,
                   std::vector<fftw_real> vel_field_x,
                   std::vector<fftw_real> vel_field_y);

    void draw();
    int  handle(int);

    void add_scalar_data(std::vector<fftw_real> new_scalar_field, fftw_real max_scalar);
    void add_vector_data(std::vector<fftw_real> new_vector_field_x,
                         std::vector<fftw_real> new_vector_field_y);
    void add_vel_data(std::vector<fftw_real> new_vel_field_x,
                      std::vector<fftw_real> new_vel_field_y);

    void add_seed(GLfloat seed_x, GLfloat seed_y);
    void clear_seeds();

    void camera_zoom_out();
    void camera_zoom_in();
    void camera_rotate_right();
    void camera_rotate_left();

  private:
    std::vector<std::vector<fftw_real>> buffer_scalar_field;
    std::vector<std::vector<fftw_real>> buffer_vector_field_x;
    std::vector<std::vector<fftw_real>> buffer_vector_field_y;
    std::vector<std::vector<fftw_real>> buffer_vel_field_x;
    std::vector<std::vector<fftw_real>> buffer_vel_field_y;
};