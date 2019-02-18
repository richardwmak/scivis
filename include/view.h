#include "config.h"
#include "controller.h"
#include "simulation.h"
#include <FL/Fl_Gl_Window.H>
#include <GL/glut.h>

#ifndef VIEW_H
#define VIEW_H

class GL_Window : public Fl_Gl_Window
{
  public:
    // Controller controller;
    static Simulation simulation;
    static Controller controller;

    int    argc;
    char **argv;

    void draw();
    int  handle(int);

    GL_Window(int X, int Y, int W, int H, const char *L, int argc, char **argv);

    void rainbow(float value, float *R, float *G, float *B);

    void set_colormap(float vy);

    void direction_to_color(float x, float y, int method);

    void visualise();

    void reshape();

    static void idle_function();
};

#endif