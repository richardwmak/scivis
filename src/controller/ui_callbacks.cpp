#include "config.hpp"
#include "controller.hpp"
#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <iostream>

class Controller;

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

void idle_callback_interaction(void *ptr_data)
{
    if (ptr_data != NULL)
    {
        Controller *ptr_controller = reinterpret_cast<Controller *>(ptr_data);

        switch (Fl::event())
        {
            case FL_DRAG:
            default:
            {
                ptr_controller->drag(Fl::event_x(), Fl::event_y());
            }
        }

        std::cout << Fl::event_button() << std::endl;
    }
}

Fl_Callback (*cb_toggle_frozen())(Fl_Button *, void *)
{
    Config::frozen ? Config::frozen = false : Config::frozen = true;
}