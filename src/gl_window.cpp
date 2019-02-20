// http://seriss.com/people/erco/fltk/opengl-sphere-with-light-old.cxx

#include "gl_window.h"
#include "controller.h"
#include "simulation.h"
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include <FL/glut.H>

GL_Window::GL_Window(int X, int Y, int W, int H, const char *L) : X(X), Y(Y), W(W), H(H), L(L) {}

GL_Window::start_gl_window(controller, simulation, argc, argv)
{
    Fl_Gl_Window(X, Y, W, H, L);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Real-time smoke simulation and visualization");
    glutDisplayFunc(controller->display);
    glutReshapeFunc(controller->reshape);
    glutIdleFunc(simulation->do_one_simulation_step);
    glutKeyboardFunc(controller->keyboard);
    glutMotionFunc(controller->drag);

    glutMainLoop();
}
