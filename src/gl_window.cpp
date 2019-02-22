// http://seriss.com/people/erco/fltk/opengl-sphere-with-light-old.cxx

#include "gl_window.h"
#include "controller.h"
#include "simulation.h"
#include <FL/gl.h>
#include <FL/glut.H>
#include <iostream>

GlWindow::GlWindow(int X, int Y, int W, int H, const char *L) : Fl_Glut_Window(X, Y, W, H, L)
{
    reshape = Tramp::t_reshape;
}
void GlWindow::start_gl_window(Controller *controller,
                               Simulation *simulation,
                               int         argc,
                               char **     argv)
{
    _argc = argc;
    _argv = argv;
    glutIdleFunc(Tramp::t_do_one);
}

void GlWindow::draw()
{
    if (!valid())
    {
        valid(1);
        glutInit(&_argc, _argv);
        glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
        glutInitWindowSize(500, 500);
    }
    glutDisplayFunc(Tramp::t_display);
}

int GlWindow::handle(int event)
{
    switch (event)
    {
    case FL_DRAG:
        glutMotionFunc(Tramp::t_drag);
        return 1;
    case FL_KEYBOARD:
        glutKeyboardFunc(Tramp::t_keyboard);
        return 1;
    default:
        return 1;
    }
}