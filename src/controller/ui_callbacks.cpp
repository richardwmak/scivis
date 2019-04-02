#include "color_mapper.hpp"
#include "config.hpp"
#include "controller.hpp"
#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <Fl/Fl_Gl_Window.H>
#include <Fl/Fl_Light_Button.H>
#include <Fl/Fl_Menu_Item.H>
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
            std::vector<fftw_real> vel_field_x(Config::NUM_CELLS);
            std::vector<fftw_real> vel_field_y(Config::NUM_CELLS);

            scalar_field   = ptr_controller->simulation->get_scalar_field();
            vector_field_x = ptr_controller->simulation->get_vector_field_x();
            vector_field_y = ptr_controller->simulation->get_vector_field_y();
            vel_field_x    = ptr_controller->simulation->get_vel_field_x();
            vel_field_y    = ptr_controller->simulation->get_vel_field_y();

            fftw_real max_scalar = ColorMapper::set_max_scalar(scalar_field);
            fftw_real min_val, med_val, max_val;

            if (Config::scaling)
            {
                min_val = 0;
                med_val = 0.5;
                max_val = 1;
            }
            else
            {
                min_val = Config::clamp_min;
                med_val = (Config::clamp_max + Config::clamp_min) / 2;
                max_val = Config::clamp_max;
            }

            ptr_controller->window->output_color_bar_max_val->value(max_val);
            ptr_controller->window->output_color_bar_med_val->value(med_val);
            ptr_controller->window->output_color_bar_min_val->value(min_val);

            ptr_controller->window->gl_window->add_scalar_data(scalar_field, max_scalar);
            ptr_controller->window->gl_window->add_vector_data(vector_field_x, vector_field_y);
            ptr_controller->window->gl_window->add_vel_data(vel_field_x, vel_field_y);
            ptr_controller->window->gl_window->redraw();
            ptr_controller->window->color_bar->redraw();
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

void cb_toggle_frozen(Fl_Light_Button *, void *)
{
    Config::frozen ? Config::frozen = false : Config::frozen = true;
}

void cb_toggle_smoke(Fl_Light_Button *, void *)
{
    Config::draw_smoke ? Config::draw_smoke = false : Config::draw_smoke = true;
}

void cb_toggle_vecs(Fl_Light_Button *, void *)
{
    Config::draw_vecs ? Config::draw_vecs = false : Config::draw_vecs = true;
}

void cb_toggle_vector_color(Fl_Light_Button *, void *)
{
    Config::vector_color ? Config::vector_color = false : Config::vector_color = true;
}

void cb_toggle_parametrization(Fl_Light_Button *, void *)
{
    Config::gradient ? Config::gradient = false : Config::gradient = true;
}

void cb_toggle_streamline(Fl_Light_Button *, void *)
{
    Config::draw_streamline ? Config::draw_streamline = false : Config::draw_streamline = true;
}

void cb_toggle_draw_slices(Fl_Light_Button *, void *controller)
{
    Config::draw_slices ? Config::draw_slices = false : Config::draw_slices = true;
    if (controller != NULL)
    {
        Controller *ptr_controller = reinterpret_cast<Controller *>(controller);
        ptr_controller->window->gl_window->change_perspective();
    }
}

void cb_counter_num_bands(Fl_Counter *w, void *)
{
    Config::num_bands = w->value();
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
    new_step       = std::round(1000000 * new_step) / 1000000;
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

void cb_option_black_white(Fl_Menu_Item *, void *)
{
    Config::scalar_col = Config::COLOR_BLACKWHITE;
}
void cb_option_rainbow(Fl_Menu_Item *, void *)
{
    Config::scalar_col = Config::COLOR_RAINBOW;
}
void cb_option_red_white(Fl_Menu_Item *, void *)
{
    Config::scalar_col = Config::COLOR_RED_WHITE;
}

void cb_option_scalar_smoke(Fl_Menu_Item *, void *)
{
    Config::scalar_choice = Config::SCALAR_SMOKE;
}
void cb_option_scalar_velocity(Fl_Menu_Item *, void *)
{
    Config::scalar_choice = Config::SCALAR_VELOCITY;
}
void cb_option_scalar_force(Fl_Menu_Item *, void *)
{
    Config::scalar_choice = Config::SCALAR_FORCE;
}

void cb_option_vector_force(Fl_Menu_Item *, void *)
{
    Config::vector_choice = Config::VECTOR_FORCE;
}

void cb_option_vector_velocity(Fl_Menu_Item *, void *)
{
    Config::vector_choice = Config::VECTOR_VELOCITY;
}

void cb_option_vector_gradient_smoke(Fl_Menu_Item *, void *)
{
    Config::vector_choice = Config::VECTOR_GRADIENT_SMOKE;
}

void cb_option_vector_gradient_velocity(Fl_Menu_Item *, void *)
{
    Config::vector_choice = Config::VECTOR_GRADIENT_VELOCITY;
}

void cb_option_hedgehog(Fl_Menu_Item *, void *)
{
    Config::vector_shape = Config::HEDGEHOG;
}
void cb_option_cone(Fl_Menu_Item *, void *)
{
    Config::vector_shape = Config::CONE;
}
void cb_option_arrow_2d(Fl_Menu_Item *, void *)
{
    Config::vector_shape = Config::ARROW_2D;
}

void cb_option_streamline_grid_points(Fl_Menu_Item *, void *)
{
    Config::streamline_options = Config::GLYPH_POINTS;
}

void cb_button_scale(Fl_Button *b, void *controller)
{
    b->deactivate();
    b->color(FL_GREEN);

    Config::scaling = true;

    Controller *ptr_controller = reinterpret_cast<Controller *>(controller);
    ptr_controller->window->button_clamp->activate();
    ptr_controller->window->button_clamp->color(FL_GRAY);

    ptr_controller->window->value_clamp_min->hide();
    ptr_controller->window->value_clamp_max->hide();
}
void cb_button_clamp(Fl_Button *b, void *controller)
{
    b->deactivate();
    b->color(FL_GREEN);

    Config::scaling = false;

    Controller *ptr_controller = reinterpret_cast<Controller *>(controller);
    ptr_controller->window->button_scale->activate();
    ptr_controller->window->button_scale->color(FL_GRAY);

    ptr_controller->window->value_clamp_min->show();
    ptr_controller->window->value_clamp_max->show();
}

void cb_value_clamp_min(Fl_Valuator *b, void *controller)
{
    Controller *ptr_controller = reinterpret_cast<Controller *>(controller);

    float new_value = b->value();
    float cur_max   = ptr_controller->window->value_clamp_max->value();
    if (cur_max < new_value + 0.1)
    {
        new_value = cur_max - 0.1;
    }
    b->value(new_value);
    Config::clamp_min = new_value;
}

void cb_value_clamp_max(Fl_Valuator *b, void *controller)
{
    Controller *ptr_controller = reinterpret_cast<Controller *>(controller);

    float new_value = b->value();
    float cur_min   = ptr_controller->window->value_clamp_min->value();
    if (cur_min > new_value - 0.1 && new_value != 0)
    {
        new_value = cur_min + 0.1;
    }
    b->value(new_value);
    Config::clamp_max = new_value;
}

void cb_value_streamline_max_length(Fl_Valuator *b, void *)
{
    Config::streamline_max_length = b->value();
}