#include "config.h"
#include "controller.h"
#include "simulation.h"
#include <FL/Fl_Gl_Window.H>
#include <FL/glut.H>

#ifndef VIEW_H
#define VIEW_H

void idle_callback(void *change_this_var_name);

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

    void visualize();

    static void display();

    static void reshape(int w, int h);

    static void idle_function();
};

#endif