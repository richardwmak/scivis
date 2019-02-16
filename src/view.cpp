#include "view.h"
#include "config.h"
#include "simulation.h"
#include <GL/glut.h>
#include <algorithm>
#include <math.h>

void Window::rainbow(float value, float *R, float *G, float *B)
{
    const float dx = 0.8;
    if (value < 0)
    {
        value = 0;
    }
    else if (value > 1)
    {
        value = 1;
    };

    value = (6 - 2 * dx) * value + dx;
    *R    = std::max(0.0f, (3 - std::fabs(value - 4) - std::fabs(value - 5) / 2));
    *G    = std::max(0.0f, (4 - std::fabs(value - 2) - std::fabs(value - 4) / 2));
    *B    = std::max(0.0f, (3 - std::fabs(value - 1) - std::fabs(value - 2) / 2));
};

void Window::set_colormap(float vy)
{
    float R, G, B;

    if (Config::scalar_col == Config::COLOR_BLACKWHITE)
        R = G = B = vy;
    else if (Config::scalar_col == Config::COLOR_RAINBOW)
        Window::rainbow(vy, &R, &G, &B);
    else if (Config::scalar_col == Config::COLOR_BANDS)
    {
        const int NLEVELS = 7;
        vy *= NLEVELS;
        vy = (int)(vy);
        vy /= NLEVELS;
        Window::rainbow(vy, &R, &G, &B);
    }

    glColor3f(R, G, B);
};

void Window::direction_to_color(float x, float y, int method)
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
    };
    glColor3f(r, g, b);
};

void Window::visualise(Simulation simulation)
{
    int       i, idx;
    double    px, py;
    fftw_real wn =
        (fftw_real)Config::win_width / (fftw_real)(Config::GRID_SIZE + 1); // Grid cell width
    fftw_real hn =
        (fftw_real)Config::win_height / (fftw_real)(Config::GRID_SIZE + 1); // Grid cell heigh

    if (Config::draw_smoke)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        for (int j = 0; j < Config::GRID_SIZE - 1; j++) // draw smoke
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
                Window::set_colormap(simulation.cur_state.smoke_density[idx]);
                glVertex2f(px, py);
                px  = wn + (fftw_real)(i + 1) * wn;
                py  = hn + (fftw_real)j * hn;
                idx = (j * Config::GRID_SIZE) + (i + 1);
                Window::set_colormap(simulation.cur_state.smoke_density[idx]);
                glVertex2f(px, py);
            }

            px  = wn + (fftw_real)(Config::GRID_SIZE - 1) * wn;
            py  = hn + (fftw_real)(j + 1) * hn;
            idx = ((j + 1) * Config::GRID_SIZE) + (Config::GRID_SIZE - 1);
            Window::set_colormap(simulation.cur_state.smoke_density[idx]);
            glVertex2f(px, py);
            glEnd();
        }
    }

    if (Config::draw_vecs)
    {
        glBegin(GL_LINES); // draw velocities
        for (int i = 0; i < Config::GRID_SIZE; i++)
            for (int j = 0; j < Config::GRID_SIZE; j++)
            {
                idx = (j * Config::GRID_SIZE) + i;
                Window::direction_to_color(simulation.cur_state.velocity_x[idx],
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

void Window::display(Simulation simulation)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Window::visualise();
    glFlush();
    glutSwapBuffers();
};
