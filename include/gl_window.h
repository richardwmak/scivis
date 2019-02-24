#ifndef GL_WINDOW_H
#define GL_WINDOW_H

#include "config.h"
#include "controller.h"
#include "simulation.h"
#include <Fl/Fl_Gl_Window.H>

class Controller;
class Simulation;

class GlWindow : public Fl_Gl_Window
{
  public:
    int         X, Y, W, H;
    const char *L;
    Controller *ptr_controller;
    Simulation *ptr_simulation;

    GlWindow(int X, int Y, int W, int H);

    void draw();
    int  handle(int event);

    void start_gl_window(Controller *controller, Simulation *simulation);
};

#endif