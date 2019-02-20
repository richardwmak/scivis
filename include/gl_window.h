#ifndef GL_WINDOW_H
#define GL_WINDOW_H

#include "config.h"
#include "controller.h"
#include "simulation.h"
#include <FL/Fl_Gl_Window.H>
#include <FL/glut.H>

class GL_Window : public Fl_Gl_Window
{
  public:
    int    argc;
    char **argv;

    GL_Window(int X, int Y, int W, int H, const char *L);

    void start_gl_window(Controller *controller, Simulation *simulation, int argc, char **argv);
};

#endif