#include "config.hpp"
#include "controller.hpp"
#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <Fl/Fl_Gl_Window.H>
#include <iostream>
#include <math.h>

class Controller;

// https://web.cecs.pdx.edu/~fliu/courses/cs447/tutorial5.html
void idle_callback_sim(void *controller)
{
    if (controller != NULL)
    {
        Controller *ptr_controller = reinterpret_cast<Controller *>(controller);

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

void idle_callback_interaction(void *controller)
{
    if (controller != NULL && Fl::event_x() <= Config::win_width &&
        Fl::event_y() <= Config::win_height)
    {
        Controller *ptr_controller = reinterpret_cast<Controller *>(controller);

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

void cb_toggle_frozen(Fl_Button *, void *)
{
    Config::frozen ? Config::frozen = false : Config::frozen = true;
}

void cb_toggle_smoke(Fl_Button *, void *)
{

    if (Config::draw_smoke)
    {
        Config::draw_smoke = false;
        Config::draw_vecs  = true;
    }
    else
    {
        Config::draw_smoke = true;
    }
}

void cb_toggle_vecs(Fl_Button *, void *)
{
    if (Config::draw_vecs)
    {
        Config::draw_vecs  = false;
        Config::draw_smoke = true;
    }
    else
    {
        Config::draw_vecs = true;
    }
}
void cb_toggle_dir_color(Fl_Button *, void *)
{
    Config::color_dir ? Config::color_dir = false : Config::color_dir = true;
}

void cb_counter_time_step(Fl_Counter *w, void *)
{
    Config::time_step = w->value();
}
void cb_counter_visc(Fl_Counter *w, void *)
{
    float new_val  = w->value();
    Config::visc   = new_val;
    float new_step = 0.2 * new_val;
    std::cout << new_val << std::endl;
    std::cout << new_step << std::endl;
    new_step = std::round(1000000 * new_step) / 1000000;
    std::cout << new_step << std::endl;
    w->step(new_step);
}
void cb_counter_vec_scale(Fl_Counter *w, void *)
{
    Config::vec_scale = w->value();
}
void cb_counter_num_glyphs(Fl_Counter *w, void *)
{
    Config::num_glyphs = w->value();
}

void redraw_color_bar(void *controller)
{
    if (controller != NULL)
    {
        Controller *ptr_controller = reinterpret_cast<Controller *>(controller);
        ptr_controller->window->color_bar->redraw();
    }
}

void cb_option_black_white(Fl_Counter *, void *controller)
{
    Config::scalar_col = Config::COLOR_BLACKWHITE;
    redraw_color_bar(controller);
}
void cb_option_rainbow(Fl_Counter *, void *controller)
{
    Config::scalar_col = Config::COLOR_RAINBOW;
    redraw_color_bar(controller);
}
void cb_option_red_white(Fl_Counter *, void *controller)
{
    Config::scalar_col = Config::COLOR_RED_WHITE;
    redraw_color_bar(controller);
}
