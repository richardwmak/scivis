#include "controller.hpp"
#include "config.hpp"
#include "simulation.hpp"
#include "ui_callbacks.hpp"
#include "user_interface.hpp"
#include <Fl/Fl.H>
#include <Fl/Fl_Widget.H>
#include <Fl/gl.h>
#include <Fl/glu.h>
#include <iostream>
#include <math.h>
#include <vector>

Controller::Controller()
{
    simulation = new Simulation();
    window     = new UserInterface();
}

// http://webcache.googleusercontent.com/search?q=cache:MsCYe9ordKkJ:www.fltk.org/strfiles/2590/glut_with_fltk2.cxx
int Controller::begin()
{
    Fl::gl_visual(FL_RGB);
    window->make_window(this);
    window->show();
    // display the OpenGL windows
    window->gl_window->show();
    window->color_bar->show();

    window->toggle_frozen->callback(cb_toggle_frozen);
    Fl::add_idle(idle_callback_sim, this);
    Fl::add_idle(idle_callback_interaction, this);
    return Fl::run();
}

// most of the handling is done via UI buttons, but we will keep this around in case we want
// keyboard interaction
void Controller::keyboard(unsigned char key)
{
    switch (key)
    {
        case 'q':
            exit(0);
    }
}

void Controller::drag(int x_pixel_curr, int y_pixel_curr)
{
    int        x_grid_index, y_grid_index;
    double     dx, dy, len;
    static int x_pixel_prev = 0, y_pixel_prev = 0; // remembers last mouse location

    y_pixel_curr = Config::win_height - y_pixel_curr;
    // Compute the array index that corresponds to the cursor location
    x_grid_index = (int)std::floor((double)(Config::GRID_SIZE) *
                                   ((double)x_pixel_curr / (double)Config::win_width));
    y_grid_index = (int)std::floor((double)(Config::GRID_SIZE) *
                                   ((double)y_pixel_curr / (double)Config::win_height));

    // clamp index values
    if (x_grid_index > (Config::num_glyphs)) x_grid_index = Config::num_glyphs;
    if (y_grid_index > (Config::num_glyphs)) y_grid_index = Config::num_glyphs;
    if (x_grid_index < 0) x_grid_index = 0;
    if (y_grid_index < 0) y_grid_index = 0;

    // Add force at the cursor location
    dx  = x_pixel_curr - x_pixel_prev;
    dy  = y_pixel_curr - y_pixel_prev;
    len = sqrt(dx * dx + dy * dy);
    if (len != 0.0)
    {
        dx *= 0.1 / len;
        dy *= 0.1 / len;
    }

    simulation->cur_state.force_x[y_grid_index * Config::GRID_SIZE + x_grid_index] += dx;
    simulation->cur_state.force_y[y_grid_index * Config::GRID_SIZE + x_grid_index] += dy;
    simulation->cur_state.smoke_density[y_grid_index * Config::GRID_SIZE + x_grid_index] = 10.0f;

    x_pixel_prev = x_pixel_curr;
    y_pixel_prev = y_pixel_curr;
}

// http://seriss.com/people/erco/fltk/opengl-sphere-with-light-old.cxx
// todo: implement
void Controller::reshape(int w, int h)
{
    glViewport(0.0f, 0.0f, (GLfloat)w, (GLfloat)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
    Config::win_width  = w;
    Config::win_height = h;
}

void Controller::change_map_color(int config_color)
{
    // this does not check whether config_color is properly implemented
    Config::scalar_col = config_color;
    window->color_bar->redraw();
}
