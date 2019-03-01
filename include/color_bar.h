#pragma once

#include "config.h"
#include "controller.h"
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>

class Controller;

class ColorBar : public Fl_Gl_Window
{
  public:
    ColorBar(int X, int Y, int W, int H);

    void start_color_bar(Controller *controller);
    void draw();
    void draw_rectangle_gradient();

    int         X, Y, W, H;
    Controller *ptr_controller;
};
