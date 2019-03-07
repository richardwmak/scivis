#pragma once

#include "config.hpp"
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>

class ColorBar : public Fl_Gl_Window
{
  public:
    ColorBar(int X, int Y, int W, int H);

    void draw();
    void draw_rectangle_gradient();
    void draw_rectangle_banded();
};
