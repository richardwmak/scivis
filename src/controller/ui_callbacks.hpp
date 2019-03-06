#pragma once

#include "config.hpp"
#include <FL/Fl_Button.H>
#include <Fl/Fl_Counter.H>
#include <Fl/Fl_Gl_Window.H>
#include <Fl/Fl_Widget.H>

void idle_callback_sim(void *controller);
void idle_callback_interaction(void *controller);

void cb_toggle_frozen(Fl_Button *, void *);
void cb_toggle_smoke(Fl_Button *, void *);
void cb_toggle_vecs(Fl_Button *, void *);
void cb_toggle_dir_color(Fl_Button *, void *);

void cb_counter_time_step(Fl_Counter *w, void *);
void cb_counter_visc(Fl_Counter *w, void *);
void cb_counter_vec_scale(Fl_Counter *w, void *);
void cb_counter_num_glyphs(Fl_Counter *w, void *);

void redraw_color_bar(void *controller);

void cb_option_black_white(Fl_Counter *, void *controller);
void cb_option_rainbow(Fl_Counter *, void *controller);
void cb_option_red_white(Fl_Counter *, void *controller);
