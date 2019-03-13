#pragma once

#include "config.hpp"
#include <FL/Fl_Button.H>
#include <Fl/Fl_Counter.H>
#include <Fl/Fl_Gl_Window.H>
#include <Fl/Fl_Light_Button.H>
#include <Fl/Fl_Menu_Item.H>
#include <Fl/Fl_Widget.H>

void idle_callback_sim(void *controller);
void idle_callback_interaction(void *controller);

void cb_toggle_frozen(Fl_Button *, void *);
void cb_toggle_smoke(Fl_Button *, void *);
void cb_toggle_vecs(Fl_Button *, void *);
void cb_toggle_vector_color(Fl_Button *, void *);
void cb_toggle_parametrization(Fl_Button *, void *);

void cb_counter_num_bands(Fl_Counter *w, void *);
void cb_counter_time_step(Fl_Counter *w, void *);
void cb_counter_visc(Fl_Counter *w, void *);
void cb_counter_vec_scale(Fl_Counter *w, void *);
void cb_counter_num_glyphs(Fl_Counter *w, void *);

// option callbacks

void cb_option_black_white(Fl_Menu_Item *, void *);
void cb_option_rainbow(Fl_Menu_Item *, void *);
void cb_option_red_white(Fl_Menu_Item *, void *);

void cb_option_scalar_smoke(Fl_Menu_Item *, void *);
void cb_option_scalar_velocity(Fl_Menu_Item *, void *);
void cb_option_scalar_force(Fl_Menu_Item *, void *);

void cb_option_vector_force(Fl_Menu_Item *, void *);
void cb_option_vector_velocity(Fl_Menu_Item *, void *);

void cb_option_hedgehog(Fl_Menu_Item *, void *);
void cb_option_cone(Fl_Menu_Item *, void *);
void cb_option_arrow_2d(Fl_Menu_Item *, void *);

void cb_button_scale(Fl_Button *b, void *controller);
void cb_button_clamp(Fl_Button *b, void *controller);

void cb_value_clamp_min(Fl_Valuator *b, void *controller);
void cb_value_clamp_max(Fl_Valuator *b, void *controller);