// http://seriss.com/people/erco/fltk/opengl-sphere-with-light-old.cxx

#include "view.h"
#include "config.h"
#include "controller.h"
#include "simulation.h"
#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include <FL/glu.h>
#include <FL/glut.H>
#include <algorithm>
#include <iostream>
#include <math.h>

Simulation GL_Window::simulation;
Controller GL_Window::controller;

GL_Window::GL_Window(int X, int Y, int W, int H, const char *L, int argc, char **argv) :
    Fl_Gl_Window(X, Y, W, H, L), argc(argc), argv(argv)
{
    Simulation *ptr_simulation = &simulation;
    global_sim::set_ptr(ptr_simulation);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Real-time smoke simulation and visualization");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(simulation.do_one_simulation_step);
    glutKeyboardFunc(controller.keyboard);
    glutMotionFunc(controller.drag);

    glutMainLoop();
}

void GL_Window::draw()
{
    if (!valid())
    {
        valid(1);
        reshape((GLfloat)w(), (GLfloat)h());
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    visualize();
    glFlush();
}

void GL_Window::rainbow(float value, float *R, float *G, float *B)
{
    const float dx = 0.8;
    if (value < 0)
    {
        value = 0;
    }
    else if (value > 1)
    {
        value = 1;
    }

    value = (6 - 2 * dx) * value + dx;

    *R = std::max(0.0f, (3 - std::fabs(value - 4) - std::fabs(value - 5) / 2));
    *G = std::max(0.0f, (4 - std::fabs(value - 2) - std::fabs(value - 4) / 2));
    *B = std::max(0.0f, (3 - std::fabs(value - 1) - std::fabs(value - 2) / 2));
}

void GL_Window::set_colormap(float vy)
{
    float R = 0, G = 0, B = 0;

    if (Config::scalar_col == Config::COLOR_BLACKWHITE)
        R = G = B = vy;
    else if (Config::scalar_col == Config::COLOR_RAINBOW)
        GL_Window::rainbow(vy, &R, &G, &B);
    else if (Config::scalar_col == Config::COLOR_BANDS)
    {
        const int NLEVELS = 7;
        vy *= NLEVELS;
        vy = (int)(vy);
        vy /= NLEVELS;
        GL_Window::rainbow(vy, &R, &G, &B);
    }

    glColor3f(R, G, B);
}

void GL_Window::direction_to_color(float x, float y, int method)
{
    float r, g, b, f;
    if (method)
    {
        f = std::atan2(y, x) / 3.1415927 + 1;
        r = f;
        if (r > 1)
        {
            r = 2 - r;
        }
        g = f + .66667;
        if (g > 2)
        {
            g -= 2;
        }
        if (g > 1)
        {
            g = 2 - g;
        }
        b = f + 2 * .66667;
        if (b > 2)
        {
            b -= 2;
        }
        if (b > 1)
        {
            b = 2 - b;
        }
    }
    else
    {
        r = g = b = 1;
    }
    glColor3f(r, g, b);
    `
}

void GL_Window::visualize()
{
    int       i, j, idx;
    double    px, py;
    fftw_real wn =
        (fftw_real)Config::win_width / (fftw_real)(Config::GRID_SIZE + 1); // Grid cell width
    fftw_real hn =
        (fftw_real)Config::win_height / (fftw_real)(Config::GRID_SIZE + 1); // Grid cell heigh

    if (Config::draw_smoke)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        for (j = 0; j < Config::GRID_SIZE - 1; j++) // draw smoke
        {
            glBegin(GL_TRIANGLE_STRIP);

            i   = 0;
            px  = wn + (fftw_real)i * wn;
            py  = hn + (fftw_real)j * hn;
            idx = (j * Config::GRID_SIZE) + i;
            glColor3f(simulation.cur_state.smoke_density[idx],
                      simulation.cur_state.smoke_density[idx],
                      simulation.cur_state.smoke_density[idx]);
            glVertex2f(px, py);

            for (i = 0; i < Config::GRID_SIZE - 1; i++)
            {
                px  = wn + (fftw_real)i * wn;
                py  = hn + (fftw_real)(j + 1) * hn;
                idx = ((j + 1) * Config::GRID_SIZE) + i;
                set_colormap(simulation.cur_state.smoke_density[idx]);
                glVertex2f(px, py);
                px  = wn + (fftw_real)(i + 1) * wn;
                py  = hn + (fftw_real)j * hn;
                idx = (j * Config::GRID_SIZE) + (i + 1);
                set_colormap(simulation.cur_state.smoke_density[idx]);
                glVertex2f(px, py);
            }

            px  = wn + (fftw_real)(Config::GRID_SIZE - 1) * wn;
            py  = hn + (fftw_real)(j + 1) * hn;
            idx = ((j + 1) * Config::GRID_SIZE) + (Config::GRID_SIZE - 1);
            set_colormap(simulation.cur_state.smoke_density[idx]);
            glVertex2f(px, py);
            glEnd();
        }
    }

    if (Config::draw_vecs)
    {
        glBegin(GL_LINES); // draw velocities
        for (i = 0; i < Config::GRID_SIZE; i++)
            for (j = 0; j < Config::GRID_SIZE; j++)
            {
                idx = (j * Config::GRID_SIZE) + i;
                direction_to_color(simulation.cur_state.velocity_x[idx],
                                   simulation.cur_state.velocity_y[idx],
                                   Config::color_dir);
                glVertex2f(wn + (fftw_real)i * wn, hn + (fftw_real)j * hn);
                glVertex2f((wn + (fftw_real)i * wn) +
                               Config::vec_scale * simulation.cur_state.velocity_x[idx],
                           (hn + (fftw_real)j * hn) +
                               Config::vec_scale * simulation.cur_state.velocity_y[idx]);
            }
        glEnd();
    }
}

void GL_Window::display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    visualize();
    glFlush();
    glutSwapBuffers();
}

// http://seriss.com/people/erco/fltk/opengl-sphere-with-light-old.cxx
void GL_Window::reshape(int w, int h)
{
    glViewport(0.0f, 0.0f, (GLfloat)w, (GLfloat)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
    Config::win_width  = win_w;
    Config::win_height = win_h;
}