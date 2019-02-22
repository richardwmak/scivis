#ifndef GL_WINDOW_H
#define GL_WINDOW_H

#include "config.h"
#include "controller.h"
#include "simulation.h"
#include <FL/glut.H>

class Controller;
class Simulation;

class GlWindow : public Fl_Glut_Window
{
  public:
    int         _argc;
    char **     _argv;
    int         X, Y, W, H;
    const char *L;

    GlWindow(int X, int Y, int W, int H, const char *L);

    void draw();
    int  handle(int event);

    void start_gl_window(Controller *controller, Simulation *simulation, int argc, char **argv);
};

#endif