// http://seriss.com/people/erco/fltk/opengl-sphere-with-light-old.cxx

#include "gl_window.h"
#include "controller.h"
#include "simulation.h"
#include <Fl/Fl.H>
#include <Fl/Fl_Gl_Window.H>
#include <Fl/gl.h>
#include <Fl/glu.h>
#include <iostream>

GlWindow::GlWindow(int X, int Y, int W, int H) : Fl_Gl_Window(X, Y, W, H)
{
    Config::win_height = H;
    Config::win_width  = W;
}

void GlWindow::start_gl_window(Controller *controller, Simulation *simulation)
{
    ptr_controller = controller;
    ptr_simulation = simulation;
}

void GlWindow::draw()
{
    glViewport(0.0f, 0.0f, (GLfloat)Config::win_width, (GLfloat)Config::win_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble)Config::win_width, 0.0, (GLdouble)Config::win_height);

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
        // for the OpenGL drawing the viewport starts in the lower left
        // Fl::event_x() is zero in the left, but
        // Fl::event_y() is zero at the top
        ptr_controller->drag(Fl::event_x(), Fl::event_y());
        return 1;
    case FL_KEYBOARD:
        // Fl::event_key() ignores capitalisation, so we manually capitalise
        unsigned char key;
        if (Fl::event_shift() == 0)
        {
            key = Fl::event_key();
        }
        else
        {
            key = Fl::event_key() - 32;
        }

        ptr_controller->keyboard((char)key);
        return 1;
    default:
        return 1;
    }
}