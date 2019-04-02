// http://seriss.com/people/erco/fltk/opengl-sphere-with-light-old.cxx

#include "gl_window.hpp"
#include "interpolate.hpp"
#include "render_smoke.hpp"
#include "render_streamline.hpp"
#include "render_vector.hpp"
#include <Fl/Fl.H>
#include <Fl/Fl_Gl_Window.H>
#include <Fl/gl.h>
#include <Fl/glu.h>
#include <iostream>
#include <math.h>
#include <vector>

GlWindow::GlWindow(int X, int Y, int W, int H) : Fl_Gl_Window(X, Y, W, H)
{
    Config::win_height = H;
    Config::win_width  = W;
    Config::grid_width = (float)H / (float)Config::GRID_SIZE;
}

void GlWindow::draw()
{
    if (!valid())
    {
        valid(1);
        glViewport(0.0f, 0.0f, (GLfloat)Config::win_width, (GLfloat)Config::win_height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(70, 1, -1000, 1000);
        gluLookAt(Config::win_height / 2,
                  Config::win_width / 2,
                  1000,
                  Config::win_height / 2,
                  Config::win_width / 2,
                  0,
                  0,
                  1,
                  0);
        // gluOrtho2D(0.0, (GLdouble)Config::win_width, 0.0, (GLdouble)Config::win_height);
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if (Config::draw_slices && buffer_scalar_field.size() >= Config::buffer_size)
    {
        visualize_slices();
    }
    else
    {
        visualize(0,
                  buffer_scalar_field.front(),
                  buffer_vector_field_x.front(),
                  buffer_vector_field_y.front(),
                  buffer_vel_field_x.front(),
                  buffer_vel_field_y.front());
    }
    glFlush();
}

void GlWindow::change_perspective()
{
    if (Config::draw_slices)
    {
        make_current();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(70, 1, -10, 10);
        gluLookAt(Config::win_height / 2 + 100,
                  Config::win_width / 2 + 100,
                  1000,
                  Config::win_height / 2,
                  Config::win_width / 2,
                  0,
                  0,
                  1,
                  0);
    }
    else
    {
        make_current();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(70, 1, -1000, 1000);
        gluLookAt(Config::win_height / 2,
                  Config::win_width / 2,
                  1000,
                  Config::win_height / 2,
                  Config::win_width / 2,
                  0,
                  0,
                  1,
                  0);
    }
}

void GlWindow::add_scalar_data(std::vector<fftw_real> new_scalar_field, fftw_real max_scalar)
{
    if (Config::scaling)
    {
        for (int i = 0; i < Config::NUM_CELLS; i++)
        {
            new_scalar_field[i] /= max_scalar;
        }
    }

    buffer_scalar_field.insert(buffer_scalar_field.begin(), new_scalar_field);
    if (buffer_scalar_field.size() > Config::buffer_size)
    {
        buffer_scalar_field.pop_back();
    }
}

void GlWindow::add_vector_data(std::vector<fftw_real> new_vector_field_x,
                               std::vector<fftw_real> new_vector_field_y)
{
    buffer_vector_field_x.insert(buffer_vector_field_x.begin(), new_vector_field_x);
    buffer_vector_field_y.insert(buffer_vector_field_y.begin(), new_vector_field_y);
    if (buffer_vector_field_x.size() > Config::buffer_size)
    {
        buffer_vector_field_x.pop_back();
    }
    if (buffer_vector_field_y.size() > Config::buffer_size)
    {
        buffer_vector_field_y.pop_back();
    }
}

void GlWindow::add_vel_data(std::vector<fftw_real> new_vel_field_x,
                            std::vector<fftw_real> new_vel_field_y)
{
    buffer_vel_field_x.insert(buffer_vel_field_x.begin(), new_vel_field_x);
    buffer_vel_field_y.insert(buffer_vel_field_y.begin(), new_vel_field_y);
    if (buffer_vel_field_x.size() > Config::buffer_size)
    {
        buffer_vel_field_x.pop_back();
    }
    if (buffer_vel_field_y.size() > Config::buffer_size)
    {
        buffer_vel_field_y.pop_back();
    }
}

void GlWindow::visualize(GLfloat                height,
                         std::vector<fftw_real> scalar_field,
                         std::vector<fftw_real> vector_field_x,
                         std::vector<fftw_real> vector_field_y,
                         std::vector<fftw_real> vel_field_x,
                         std::vector<fftw_real> vel_field_y)
{
    if (Config::draw_smoke)
    {
        RenderSmoke::render_smoke(scalar_field, height);
    }

    if (Config::draw_vecs)
    {
        RenderVector::render_vector(scalar_field, vector_field_x, vector_field_y, height);
    }

    if (Config::draw_streamline)
    {
        RenderStreamline::render_streamlines(vel_field_x, vel_field_y, height);
    }
}

void GlWindow::visualize_slices()
{
    GLfloat height = 0;
    for (long unsigned int i = 0; i < Config::buffer_size; i += Config::buffer_delta, height += 20)
    {
        visualize(height,
                  buffer_scalar_field[i],
                  buffer_vector_field_x[i],
                  buffer_vector_field_y[i],
                  buffer_vel_field_x[i],
                  buffer_vel_field_y[i]);
    }
}