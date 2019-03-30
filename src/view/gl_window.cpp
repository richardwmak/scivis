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
        gluOrtho2D(0.0, (GLdouble)Config::win_width, 0.0, (GLdouble)Config::win_height);
        // gluPerspective(70, 1, -10, 10);
        // gluLookAt(Config::win_height / 2,
        //           Config::win_width / 2,
        //           800,
        //           Config::win_height / 2,
        //           Config::win_width / 2,
        //           0,
        //           0,
        //           1,
        //           0);
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    visualize();
    glFlush();
}

// int GlWindow::handle(int event)
// {
//     switch (event)
//     {
//         case FL_PUSH:
//         {
//             // std::cout << Fl::event_x() << ", " << Fl::event_y() << std::endl;
//             return 1;
//         }
//     }
// }

void GlWindow::set_scalar_data(std::vector<fftw_real> new_scalar_field, fftw_real max_scalar)
{
    if (Config::scaling)
    {
        for (int i = 0; i < Config::NUM_CELLS; i++)
        {
            new_scalar_field[i] /= max_scalar;
        }
    }

    scalar_field = new_scalar_field;
}

void GlWindow::set_vector_data(std::vector<fftw_real> new_vector_field_x,
                               std::vector<fftw_real> new_vector_field_y)
{
    vector_field_x = new_vector_field_x;
    vector_field_y = new_vector_field_y;
}

void GlWindow::set_vel_data(std::vector<fftw_real> new_vel_field_x,
                            std::vector<fftw_real> new_vel_field_y)
{
    vel_field_x = new_vel_field_x;
    vel_field_y = new_vel_field_y;
}

void GlWindow::visualize()
{
    if (Config::draw_smoke)
    {
        RenderSmoke::render_smoke(scalar_field);
    }

    if (Config::draw_vecs)
    {
        RenderVector::render_vector(scalar_field, vector_field_x, vector_field_y);
    }

    if (Config::draw_streamline)
    {
        RenderStreamline::render_streamlines(vel_field_x, vel_field_y);
    }
}
