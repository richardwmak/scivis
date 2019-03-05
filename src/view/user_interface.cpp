// generated by Fast Light User Interface Designer (fluid) version 1.0304

#include "user_interface.hpp"

void UserInterface::cb_toggle_dir_color_i(Fl_Button*, void*) {
  Config::color_dir ? Config::color_dir = false : Config::color_dir = true;
}
void UserInterface::cb_toggle_dir_color(Fl_Button* o, void* v) {
  ((UserInterface*)(o->parent()->user_data()))->cb_toggle_dir_color_i(o,v);
}

void UserInterface::cb_toggle_draw_smoke_i(Fl_Button*, void*) {
  if (Config::draw_smoke)
        {
            Config::draw_smoke = false;
            Config::draw_vecs  = true;
        }
        else
        {
            Config::draw_smoke = true;
        };
}
void UserInterface::cb_toggle_draw_smoke(Fl_Button* o, void* v) {
  ((UserInterface*)(o->parent()->user_data()))->cb_toggle_draw_smoke_i(o,v);
}

void UserInterface::cb_toggle_draw_vecs_i(Fl_Button*, void*) {
  if (Config::draw_vecs)
        {
            Config::draw_vecs  = false;
            Config::draw_smoke = true;
        }
        else
        {
            Config::draw_vecs = true;
        };
}
void UserInterface::cb_toggle_draw_vecs(Fl_Button* o, void* v) {
  ((UserInterface*)(o->parent()->user_data()))->cb_toggle_draw_vecs_i(o,v);
}

void UserInterface::cb_toggle_frozen_i(Fl_Button*, void*) {
  Config::frozen ? Config::frozen = false : Config::frozen = true;
}
void UserInterface::cb_toggle_frozen(Fl_Button* o, void* v) {
  ((UserInterface*)(o->parent()->user_data()))->cb_toggle_frozen_i(o,v);
}

void UserInterface::cb_option_black_white_i(Fl_Menu_*, void*) {
  Config::num_verts = 2;
ptr_controller->change_map_color(Config::COLOR_BLACKWHITE);
}
void UserInterface::cb_option_black_white(Fl_Menu_* o, void* v) {
  ((UserInterface*)(o->parent()->user_data()))->cb_option_black_white_i(o,v);
}

void UserInterface::cb_option_rainbow_i(Fl_Menu_*, void*) {
  Config::num_verts = 11;
ptr_controller->change_map_color(Config::COLOR_RAINBOW);
}
void UserInterface::cb_option_rainbow(Fl_Menu_* o, void* v) {
  ((UserInterface*)(o->parent()->user_data()))->cb_option_rainbow_i(o,v);
}

void UserInterface::cb_option_bands_i(Fl_Menu_*, void*) {
  ptr_controller->change_map_color(Config::COLOR_RED_TO_WHITE);
}
void UserInterface::cb_option_bands(Fl_Menu_* o, void* v) {
  ((UserInterface*)(o->parent()->user_data()))->cb_option_bands_i(o,v);
}

void UserInterface::cb_option_red_to_white_i(Fl_Menu_*, void*) {
  ptr_controller->change_map_color(Config::COLOR_RED_TO_WHITE);
}
void UserInterface::cb_option_red_to_white(Fl_Menu_* o, void* v) {
  ((UserInterface*)(o->parent()->user_data()))->cb_option_red_to_white_i(o,v);
}

Fl_Menu_Item UserInterface::menu_menu_color_map[] = {
 {"Black and white", 0,  (Fl_Callback*)UserInterface::cb_option_black_white, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 14, 0},
 {"Rainbow", 0,  (Fl_Callback*)UserInterface::cb_option_rainbow, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 14, 0},
 {"Banded", 0,  (Fl_Callback*)UserInterface::cb_option_bands, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 14, 0},
 {"Red to white", 0,  (Fl_Callback*)UserInterface::cb_option_red_to_white, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0}
};
Fl_Menu_Item* UserInterface::option_black_white = UserInterface::menu_menu_color_map + 0;
Fl_Menu_Item* UserInterface::option_rainbow = UserInterface::menu_menu_color_map + 1;
Fl_Menu_Item* UserInterface::option_bands = UserInterface::menu_menu_color_map + 2;
Fl_Menu_Item* UserInterface::option_red_to_white = UserInterface::menu_menu_color_map + 3;

void UserInterface::cb_btn_decrease_time_step_i(Fl_Button*, void*) {
  Config::time_step -= 0.001;
}
void UserInterface::cb_btn_decrease_time_step(Fl_Button* o, void* v) {
  ((UserInterface*)(o->parent()->user_data()))->cb_btn_decrease_time_step_i(o,v);
}

void UserInterface::cb_btn_increase_time_step_i(Fl_Button*, void*) {
  Config::time_step += 0.001;
}
void UserInterface::cb_btn_increase_time_step(Fl_Button* o, void* v) {
  ((UserInterface*)(o->parent()->user_data()))->cb_btn_increase_time_step_i(o,v);
}

void UserInterface::cb_btn_decrease_visc_i(Fl_Button*, void*) {
  Config::visc *= 0.2;
}
void UserInterface::cb_btn_decrease_visc(Fl_Button* o, void* v) {
  ((UserInterface*)(o->parent()->user_data()))->cb_btn_decrease_visc_i(o,v);
}

void UserInterface::cb_btn_increase_visc_i(Fl_Button*, void*) {
  Config::visc *= 5;
}
void UserInterface::cb_btn_increase_visc(Fl_Button* o, void* v) {
  ((UserInterface*)(o->parent()->user_data()))->cb_btn_increase_visc_i(o,v);
}

void UserInterface::cb_btn_decrease_vec_scale_i(Fl_Button*, void*) {
  Config::vec_scale *=0.8;
}
void UserInterface::cb_btn_decrease_vec_scale(Fl_Button* o, void* v) {
  ((UserInterface*)(o->parent()->user_data()))->cb_btn_decrease_vec_scale_i(o,v);
}

void UserInterface::cb_btn_increase_vec_scale_i(Fl_Button*, void*) {
  Config::vec_scale *= 1.25;
}
void UserInterface::cb_btn_increase_vec_scale(Fl_Button* o, void* v) {
  ((UserInterface*)(o->parent()->user_data()))->cb_btn_increase_vec_scale_i(o,v);
}

void UserInterface::cb_btn_decrease_num_glyphs_i(Fl_Button*, void*) {
  Config::num_glyphs -= 1;
}
void UserInterface::cb_btn_decrease_num_glyphs(Fl_Button* o, void* v) {
  ((UserInterface*)(o->parent()->user_data()))->cb_btn_decrease_num_glyphs_i(o,v);
}

void UserInterface::cb_btn_increase_num_glyphs_i(Fl_Button*, void*) {
  Config::num_glyphs += 1;
}
void UserInterface::cb_btn_increase_num_glyphs(Fl_Button* o, void* v) {
  ((UserInterface*)(o->parent()->user_data()))->cb_btn_increase_num_glyphs_i(o,v);
}

Fl_Double_Window* UserInterface::make_window(Controller *controller) {
  ptr_controller = controller;
  { main_window = new Fl_Double_Window(1400, 1000, "Smoke");
    main_window->user_data((void*)(this));
    { gl_window = new GlWindow(0, 0, 1000, 1000);
      gl_window->box(FL_NO_BOX);
      gl_window->color(FL_BACKGROUND_COLOR);
      gl_window->selection_color(FL_BACKGROUND_COLOR);
      gl_window->labeltype(FL_NO_LABEL);
      gl_window->labelfont(0);
      gl_window->labelsize(14);
      gl_window->labelcolor(FL_FOREGROUND_COLOR);
      gl_window->align(Fl_Align(FL_ALIGN_CENTER));
      gl_window->when(FL_WHEN_RELEASE);
    } // GlWindow* gl_window
    { separator = new Fl_Box(1000, 0, 10, 1000, "separator");
      separator->box(FL_FLAT_BOX);
      separator->color((Fl_Color)44);
      separator->labeltype(FL_NO_LABEL);
    } // Fl_Box* separator
    { color_bar = new ColorBar(1010, 70, 40, 800);
      color_bar->box(FL_ENGRAVED_BOX);
      color_bar->color(FL_BACKGROUND_COLOR);
      color_bar->selection_color(FL_BACKGROUND_COLOR);
      color_bar->labeltype(FL_NORMAL_LABEL);
      color_bar->labelfont(0);
      color_bar->labelsize(14);
      color_bar->labelcolor(FL_FOREGROUND_COLOR);
      color_bar->align(Fl_Align(FL_ALIGN_CENTER|FL_ALIGN_INSIDE));
      color_bar->when(FL_WHEN_RELEASE);
    } // ColorBar* color_bar
    { text_toggle = new Fl_Box(1135, 50, 200, 20, "Toggle");
      text_toggle->labelfont(1);
    } // Fl_Box* text_toggle
    { toggle_dir_color = new Fl_Button(1135, 130, 200, 20, "Toggle direction colour");
      toggle_dir_color->callback((Fl_Callback*)cb_toggle_dir_color);
    } // Fl_Button* toggle_dir_color
    { toggle_draw_smoke = new Fl_Button(1135, 90, 200, 20, "Toggle smoke");
      toggle_draw_smoke->callback((Fl_Callback*)cb_toggle_draw_smoke);
    } // Fl_Button* toggle_draw_smoke
    { toggle_draw_vecs = new Fl_Button(1135, 110, 200, 20, "Toggle vectors");
      toggle_draw_vecs->callback((Fl_Callback*)cb_toggle_draw_vecs);
    } // Fl_Button* toggle_draw_vecs
    { toggle_frozen = new Fl_Button(1135, 70, 200, 20, "Toggle play");
      toggle_frozen->callback((Fl_Callback*)cb_toggle_frozen);
    } // Fl_Button* toggle_frozen
    { text_scalar_col = new Fl_Box(1135, 170, 200, 20, "Color mapping");
      text_scalar_col->labelfont(1);
    } // Fl_Box* text_scalar_col
    { menu_color_map = new Fl_Choice(1135, 190, 200, 20);
      menu_color_map->down_box(FL_BORDER_BOX);
      menu_color_map->menu(menu_menu_color_map);
    } // Fl_Choice* menu_color_map
    { text_increase_decrease = new Fl_Box(1135, 230, 200, 16, "Increase/decrease values");
      text_increase_decrease->labelfont(1);
    } // Fl_Box* text_increase_decrease
    { btn_decrease_time_step = new Fl_Button(1135, 245, 50, 20, "-");
      btn_decrease_time_step->callback((Fl_Callback*)cb_btn_decrease_time_step);
    } // Fl_Button* btn_decrease_time_step
    { text_time_step = new Fl_Box(1185, 245, 100, 20, "Time step");
    } // Fl_Box* text_time_step
    { btn_increase_time_step = new Fl_Button(1285, 245, 50, 20, "+");
      btn_increase_time_step->callback((Fl_Callback*)cb_btn_increase_time_step);
    } // Fl_Button* btn_increase_time_step
    { btn_decrease_visc = new Fl_Button(1135, 265, 50, 20, "-");
      btn_decrease_visc->callback((Fl_Callback*)cb_btn_decrease_visc);
    } // Fl_Button* btn_decrease_visc
    { text_viscosity = new Fl_Box(1185, 265, 100, 20, "Viscosity");
    } // Fl_Box* text_viscosity
    { btn_increase_visc = new Fl_Button(1285, 265, 50, 20, "+");
      btn_increase_visc->callback((Fl_Callback*)cb_btn_increase_visc);
    } // Fl_Button* btn_increase_visc
    { btn_decrease_vec_scale = new Fl_Button(1135, 285, 50, 20, "-");
      btn_decrease_vec_scale->callback((Fl_Callback*)cb_btn_decrease_vec_scale);
    } // Fl_Button* btn_decrease_vec_scale
    { text_vec_scale = new Fl_Box(1185, 285, 100, 20, "Vector scaling");
    } // Fl_Box* text_vec_scale
    { btn_increase_vec_scale = new Fl_Button(1285, 285, 50, 20, "+");
      btn_increase_vec_scale->callback((Fl_Callback*)cb_btn_increase_vec_scale);
    } // Fl_Button* btn_increase_vec_scale
    { btn_decrease_num_glyphs = new Fl_Button(1135, 305, 50, 20, "-");
      btn_decrease_num_glyphs->callback((Fl_Callback*)cb_btn_decrease_num_glyphs);
    } // Fl_Button* btn_decrease_num_glyphs
    { text_num_glyphs = new Fl_Box(1185, 305, 100, 20, "#glyphs");
    } // Fl_Box* text_num_glyphs
    { btn_increase_num_glyphs = new Fl_Button(1285, 305, 50, 20, "+");
      btn_increase_num_glyphs->callback((Fl_Callback*)cb_btn_increase_num_glyphs);
    } // Fl_Button* btn_increase_num_glyphs
    main_window->end();
  } // Fl_Double_Window* main_window
  return main_window;
}

void UserInterface::show() {
  main_window->show();
}
