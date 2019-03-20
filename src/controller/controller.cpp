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
    window->make_window();
    window->show();
    // display the OpenGL windows
    window->gl_window->show();
    window->color_bar->show();

    // toggle callbacks
    window->toggle_frozen->callback((Fl_Callback *)cb_toggle_frozen);
    window->toggle_draw_smoke->callback((Fl_Callback *)cb_toggle_smoke);
    window->toggle_vector_color->callback((Fl_Callback *)cb_toggle_vector_color);
    window->toggle_draw_vecs->callback((Fl_Callback *)cb_toggle_vecs);
    window->toggle_parametrize_color_map->callback((Fl_Callback *)cb_toggle_parametrization, this);

    // counter callbacks
    window->counter_num_bands->callback((Fl_Callback *)cb_counter_num_bands, this);
    window->counter_time_step->callback((Fl_Callback *)cb_counter_time_step);
    window->counter_visc->callback((Fl_Callback *)cb_counter_visc);
    window->counter_vec_scale->callback((Fl_Callback *)cb_counter_vec_scale);
    window->counter_num_glyphs->callback((Fl_Callback *)cb_counter_num_glyphs);

    // color map menu callbacks
    window->option_black_white->callback((Fl_Callback *)cb_option_black_white, this);
    window->option_rainbow->callback((Fl_Callback *)cb_option_rainbow, this);
    window->option_red_white->callback((Fl_Callback *)cb_option_red_white, this);
    // scalar choice menus callbacks
    window->option_scalar_smoke->callback((Fl_Callback *)cb_option_scalar_smoke);
    window->option_scalar_velocity->callback((Fl_Callback *)cb_option_scalar_velocity);
    window->option_scalar_force->callback((Fl_Callback *)cb_option_scalar_force);
    // vector choice menu callbacks
    window->option_vector_force->callback((Fl_Callback *)cb_option_vector_force);
    window->option_vector_velocity->callback((Fl_Callback *)cb_option_vector_velocity);
    window->option_vector_gradient_smoke->callback((Fl_Callback *)cb_option_vector_gradient_smoke);
    window->option_vector_gradient_velocity->callback(
        (Fl_Callback *)cb_option_vector_gradient_velocity);
    // vector shape menu callbacks
    window->option_hedgehog->callback((Fl_Callback *)cb_option_hedgehog);
    window->option_cone->callback((Fl_Callback *)cb_option_cone);
    window->option_arrow_2d->callback((Fl_Callback *)cb_option_arrow_2d);

    // button callbacks
    window->button_clamp->callback((Fl_Callback *)cb_button_clamp, this);
    window->button_scale->callback((Fl_Callback *)cb_button_scale, this);

    // value input callbacks
    window->value_clamp_min->callback((Fl_Callback *)cb_value_clamp_min, this);
    window->value_clamp_max->callback((Fl_Callback *)cb_value_clamp_max, this);

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