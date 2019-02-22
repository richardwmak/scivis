// http://seriss.com/people/erco/fltk/opengl-sphere-with-light-old.cxx

#include "gl_window.h"
#include "controller.h"
#include "simulation.h"
#include <Fl/Fl.H>
#include <Fl/Fl_Gl_Window.H>
#include <Fl/gl.h>
#include <iostream>

GlWindow::GlWindow(int X, int Y, int W, int H, const char *L) : Fl_Gl_Window(X, Y, W, H, L) {}

void GlWindow::start_gl_window(Controller *controller,
                               Simulation *simulation,
                               int         argc,
                               char **     argv)
{
    _argc = argc;
    _argv = argv;

    ptr_controller = controller;
    ptr_simulation = simulation;
}

void GlWindow::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    ptr_controller->visualize();
    glFlush();
}

int GlWindow::handle(int event)
{
    switch (event)
    {
    case FL_DRAG:
        ptr_controller->drag(Fl::event_x(), Fl::event_y());
        return 1;
    case FL_KEYBOARD:
        ptr_controller->keyboard((char)Fl::event_key());
        return 1;
    default:
        return 1;
    }
}