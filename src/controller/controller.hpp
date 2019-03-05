#pragma once

#include "config.hpp"
#include "simulation.hpp"
#include "ui.hpp"
#include <Fl/Fl_Widget.H>
#include <Fl/Fl_Window.H>
#include <vector>

class GlWindow;
class UserInterface;
class ColorBar;

class Controller
{
  public:
    Controller();

    // functions that handle what happens with keypresses/ draggin the mouse
    void keyboard(unsigned char key);
    void drag(int x_pixel_curr, int y_pixel_prev);

    // this is currently not used, but should implement window resizing
    void reshape(int w, int h);

    // this takes settings done in a dropdown menu and applies the relevant changes
    void change_map_color(int config_color);

    int begin();

    Simulation *   simulation;
    UserInterface *window;

  private:
};
