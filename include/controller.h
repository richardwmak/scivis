#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "gl_window.h"
#include "simulation.h"
#include <Fl/Fl_Window.H>

class Controller
{
  public:
    Controller();

    void keyboard(unsigned char key, int x, int y);
    void drag(int mx, int my);

    void rainbow(float value, float *R, float *G, float *B);

    void set_colormap(float vy);

    void direction_to_color(float x, float y, int method);

    void visualize(void);

    void display();

    void reshape(int w, int h);

    void begin(int argc, char **argv);

  private:
    Simulation simulation;
    GL_Window  gl_window;
    Fl_Window  window;
};

#endif