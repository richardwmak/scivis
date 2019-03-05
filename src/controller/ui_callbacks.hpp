#pragma once

#include "config.hpp"
#include <FL/Fl_Button.H>
#include <Fl/Fl_Widget.H>

void idle_callback_sim(void *ptr_data);
void idle_callback_interaction(void *ptr_data);

// https://stackoverflow.com/questions/15644088/declaration-of-function-returning-a-function-pointer
template <typename T>
typedef Fl_Callback (*cb_ptr)(T)

    Fl_Callback (*cb_toggle_frozen())(Fl_Button *, void *);