#include "config.h"
#include "simulation.h"
#include "view.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <iostream>
#include <math.h>
#include <rfftw.h>

int main(int argc, char **argv)
{
    Fl::gl_visual(FL_RGB);
    Fl_Window *window    = new Fl_Window(600, 600);
    GL_Window *gl_window = new GL_Window(0, 0, 500, 500, 0, argc, argv);
    window->end();
    window->show(argc, argv);

    return Fl::run();
}