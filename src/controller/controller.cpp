#include "controller.hpp"
#include "config.hpp"
#include "simulation.hpp"
#include "ui.hpp"
#include <Fl/Fl.H>
#include <Fl/Fl_Widget.H>
#include <Fl/gl.h>
#include <Fl/glu.h>
#include <iostream>
#include <math.h>
#include <vector>

// https://web.cecs.pdx.edu/~fliu/courses/cs447/tutorial5.html
void idle_callback_sim(void *ptr_data)
{
    if (ptr_data != NULL)
    {
        Controller *ptr_controller = reinterpret_cast<Controller *>(ptr_data);

        if (!Config::frozen)
        {
            ptr_controller->simulation->do_one_simulation_step();

            // get the new arrays and convert them to vectors for convenience
            std::vector<fftw_real> scalar_field(Config::NUM_CELLS);
            std::vector<fftw_real> vector_field_x(Config::NUM_CELLS);
            std::vector<fftw_real> vector_field_y(Config::NUM_CELLS);

            scalar_field   = ptr_controller->simulation->get_scalar_field();
            vector_field_x = ptr_controller->simulation->get_vector_field_x();
            vector_field_y = ptr_controller->simulation->get_vector_field_y();

            ptr_controller->window->gl_window->set_scalar_data(scalar_field);
            ptr_controller->window->gl_window->set_vector_data(vector_field_x, vector_field_y);
            ptr_controller->window->gl_window->redraw();
        }
    }
}

void idle_callback_con(void *ptr_data)
{
    if (ptr_data != NULL)
    {
        Controller *ptr_controller = reinterpret_cast<Controller *>(ptr_data);

        switch (Fl::event())
        {
            case FL_DRAG:
            {
                ptr_controller->drag(Fl::event_x(), Fl::event_y());
            }
            default:
            {
                ;
            }
        }
    }
}

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
    window->gl_window->show();

    // initialise the color bar
    window->color_bar->start_color_bar(this);
    window->color_bar->show();

    Fl::add_idle(idle_callback_sim, this);
    Fl::add_idle(idle_callback_con, this);
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
