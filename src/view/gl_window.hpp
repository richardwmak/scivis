#pragma once

#include "config.hpp"
#include "controller.hpp"
#include "simulation.hpp"
#include <Fl/Fl_Gl_Window.H>

class Controller;
class Simulation;

class GlWindow : public Fl_Gl_Window
{
  public:
    int         X, Y, W, H;
    const char *L;

    Simulation *ptr_simulation;
    Controller *ptr_controller;
    void        start_w(Simulation *simulation, Controller *controller);

    GlWindow(int X, int Y, int W, int H);

    void draw();
    // int  handle(int event);
};