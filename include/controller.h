#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "gl_window.h"
#include "simulation.h"
#include "ui.h"
#include <Fl/Fl_Window.H>

class GlWindow;
class UserInterface;
class ColorBar;

class Controller
{
  public:
    Controller();

    // functions that handle what happens with keypresses/ draggin the mouse
    void keyboard(unsigned char key);
    void drag(int mx, int my);

    // various functions that visualize() uses to draw
    void rainbow(float value, float *R, float *G, float *B);
    void set_colormap(float vy);
    void direction_to_color(float x, float y, bool method);

    // take the information we have and actually use OpenGL to draw it
    void visualize(void);

    // this is currently not used, but should implement window resizing
    void reshape(int w, int h);

    int begin();

    Simulation *simulation;

  private:
    GlWindow *     gl_window;
    UserInterface *window;
};

#endif