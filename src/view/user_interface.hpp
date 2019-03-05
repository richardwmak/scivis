// generated by Fast Light User Interface Designer (fluid) version 1.0304

#ifndef user_interface_hpp
#define user_interface_hpp
#include <FL/Fl.H>
class GlWindow; class ColorBar; class Controller;
#include <FL/Fl_Double_Window.H>
#include "config.hpp"
#include <iostream>
#include "gl_window.hpp"
#include <FL/Fl_Box.H>
#include "color_bar.hpp"
#include <FL/Fl_Button.H>
#include <FL/Fl_Choice.H>

class UserInterface {
  Controller *ptr_controller; 
public:
  Fl_Double_Window* make_window(Controller *controller);
  Fl_Double_Window *main_window;
  GlWindow *gl_window;
  Fl_Box *separator;
  ColorBar *color_bar;
  Fl_Box *text_toggle;
  Fl_Button *toggle_dir_color;
private:
  inline void cb_toggle_dir_color_i(Fl_Button*, void*);
  static void cb_toggle_dir_color(Fl_Button*, void*);
public:
  Fl_Button *toggle_draw_smoke;
private:
  inline void cb_toggle_draw_smoke_i(Fl_Button*, void*);
  static void cb_toggle_draw_smoke(Fl_Button*, void*);
public:
  Fl_Button *toggle_draw_vecs;
private:
  inline void cb_toggle_draw_vecs_i(Fl_Button*, void*);
  static void cb_toggle_draw_vecs(Fl_Button*, void*);
public:
  Fl_Button *toggle_frozen;
private:
  inline void cb_toggle_frozen_i(Fl_Button*, void*);
  static void cb_toggle_frozen(Fl_Button*, void*);
public:
  Fl_Box *text_scalar_col;
  Fl_Choice *menu_color_map;
  static Fl_Menu_Item menu_menu_color_map[];
  static Fl_Menu_Item *option_black_white;
private:
  inline void cb_option_black_white_i(Fl_Menu_*, void*);
  static void cb_option_black_white(Fl_Menu_*, void*);
public:
  static Fl_Menu_Item *option_rainbow;
private:
  inline void cb_option_rainbow_i(Fl_Menu_*, void*);
  static void cb_option_rainbow(Fl_Menu_*, void*);
public:
  static Fl_Menu_Item *option_bands;
private:
  inline void cb_option_bands_i(Fl_Menu_*, void*);
  static void cb_option_bands(Fl_Menu_*, void*);
public:
  static Fl_Menu_Item *option_red_to_white;
private:
  inline void cb_option_red_to_white_i(Fl_Menu_*, void*);
  static void cb_option_red_to_white(Fl_Menu_*, void*);
public:
  Fl_Box *text_increase_decrease;
  Fl_Button *btn_decrease_time_step;
private:
  inline void cb_btn_decrease_time_step_i(Fl_Button*, void*);
  static void cb_btn_decrease_time_step(Fl_Button*, void*);
public:
  Fl_Box *text_time_step;
  Fl_Button *btn_increase_time_step;
private:
  inline void cb_btn_increase_time_step_i(Fl_Button*, void*);
  static void cb_btn_increase_time_step(Fl_Button*, void*);
public:
  Fl_Button *btn_decrease_visc;
private:
  inline void cb_btn_decrease_visc_i(Fl_Button*, void*);
  static void cb_btn_decrease_visc(Fl_Button*, void*);
public:
  Fl_Box *text_viscosity;
  Fl_Button *btn_increase_visc;
private:
  inline void cb_btn_increase_visc_i(Fl_Button*, void*);
  static void cb_btn_increase_visc(Fl_Button*, void*);
public:
  Fl_Button *btn_decrease_vec_scale;
private:
  inline void cb_btn_decrease_vec_scale_i(Fl_Button*, void*);
  static void cb_btn_decrease_vec_scale(Fl_Button*, void*);
public:
  Fl_Box *text_vec_scale;
  Fl_Button *btn_increase_vec_scale;
private:
  inline void cb_btn_increase_vec_scale_i(Fl_Button*, void*);
  static void cb_btn_increase_vec_scale(Fl_Button*, void*);
public:
  Fl_Button *btn_decrease_num_glyphs;
private:
  inline void cb_btn_decrease_num_glyphs_i(Fl_Button*, void*);
  static void cb_btn_decrease_num_glyphs(Fl_Button*, void*);
public:
  Fl_Box *text_num_glyphs;
  Fl_Button *btn_increase_num_glyphs;
private:
  inline void cb_btn_increase_num_glyphs_i(Fl_Button*, void*);
  static void cb_btn_increase_num_glyphs(Fl_Button*, void*);
public:
  void show();
};
#endif
