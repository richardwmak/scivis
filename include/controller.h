#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "gl_window.h"
#include "simulation.h"
#include <Fl/Fl_Window.H>

class GlWindow;

class Controller
{
  public:
    Controller();

    void keyboard(unsigned char key);
    void drag(int mx, int my);

    void rainbow(float value, float *R, float *G, float *B);

    void set_colormap(float vy);

    void direction_to_color(float x, float y, int method);

    void visualize(void);

    void display();

    void reshape(int w, int h);

    int begin(int argc, char **argv);

    Simulation *simulation;

  private:
    GlWindow * gl_window;
    Fl_Window *window;
};

namespace Tramp
{
    extern Controller *glob_controller;

    extern void t_display();
    extern void t_reshape(int, int);
    extern void t_do_one();
    extern void t_keyboard(uchar, int, int);
    extern void t_drag(int, int);
} // namespace Tramp

#endif