#include "controller.h"
#include "config.h"
#include "gl_window.h"
#include "simulation.h"
#include <Fl/glu.h>
#include <iostream>
#include <math.h>

Controller::Controller()
{
    simulation = new Simulation();
    gl_window  = new GlWindow(0, 0, 500, 500, 0);
    window     = new Fl_Window(600, 600);
}

int Controller::begin(int argc, char **argv)
{
    window->begin();
    gl_window->start_gl_window(this, simulation, argc, argv);

    Fl::gl_visual(FL_RGB);

    window->end();
    window->show(argc, argv);
    gl_window->show();

    return Fl::run();
}

void Controller::keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 't':
        Config::time_step -= 0.001;
        break;
    case 'T':
        Config::time_step += 0.001;
        break;
    case 'c':
        Config::color_dir = 1 - Config::color_dir;
        break;
    case 'S':
        Config::vec_scale *= 1.2;
        break;
    case 's':
        Config::vec_scale *= 0.8;
        break;
    case 'V':
        Config::visc *= 5;
        break;
    case 'v':
        Config::visc *= 0.2;
        break;
    case 'x':
        Config::draw_smoke = 1 - Config::draw_smoke;
        if (Config::draw_smoke == 0)
            Config::draw_vecs = 1;
        break;
    case 'y':
        Config::draw_vecs = 1 - Config::draw_vecs;
        if (Config::draw_vecs == 0)
            Config::draw_smoke = 1;
        break;
    case 'm':
        Config::scalar_col++;
        if (Config::scalar_col > Config::COLOR_BANDS)
            Config::scalar_col = Config::COLOR_BLACKWHITE;
        break;
    case 'a':
        Config::frozen ? Config::frozen = false : Config::frozen = true;
        break;
    case 'q':
        exit(0);
    }
}

void Controller::drag(int mx, int my)
{
    for (int i = 0; i <= Config::GRID_SIZE; i++)
    {
        for (int j = 0; j <= Config::GRID_SIZE; j++)
        {
            std::cout << simulation->cur_state.velocity_x[i + j];
            std::cout << ", ";
        }
        std::cout << "\n";
    }
    int        xi, yi, X, Y;
    double     dx, dy, len;
    static int lmx = 0, lmy = 0; // remembers last mouse location

    // Compute the array index that corresponds to the cursor location
    xi =
        (int)std::floor((double)(Config::GRID_SIZE + 1) * ((double)mx / (double)Config::win_width));
    yi = (int)std::floor((double)(Config::GRID_SIZE + 1) *
                         ((double)(Config::win_height - my) / (double)Config::win_height));

    X = xi;
    Y = yi;

    if (X > (Config::GRID_SIZE - 1))
        X = Config::GRID_SIZE - 1;
    if (Y > (Config::GRID_SIZE - 1))
        Y = Config::GRID_SIZE - 1;
    if (X < 0)
        X = 0;
    if (Y < 0)
        Y = 0;

    // Add force at the cursor location
    my  = Config::win_height - my;
    dx  = mx - lmx;
    dy  = my - lmy;
    len = sqrt(dx * dx + dy * dy);
    if (len != 0.0)
    {
        dx *= 0.1 / len;
        dy *= 0.1 / len;
    }
    simulation->cur_state.force_x[Y * Config::GRID_SIZE + X] += dx;
    simulation->cur_state.force_y[Y * Config::GRID_SIZE + X] += dy;
    simulation->cur_state.smoke_density[Y * Config::GRID_SIZE + X] = 10.0f;

    lmx = mx;
    lmy = my;
}

void Controller::rainbow(float value, float *R, float *G, float *B)
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

void Controller::set_colormap(float vy)
{
    float R = 0, G = 0, B = 0;

    if (Config::scalar_col == Config::COLOR_BLACKWHITE)
        R = G = B = vy;
    else if (Config::scalar_col == Config::COLOR_RAINBOW)
        rainbow(vy, &R, &G, &B);
    else if (Config::scalar_col == Config::COLOR_BANDS)
    {
        const int NLEVELS = 7;
        vy *= NLEVELS;
        vy = (int)(vy);
        vy /= NLEVELS;
        rainbow(vy, &R, &G, &B);
    }

    glColor3f(R, G, B);
}

void Controller::direction_to_color(float x, float y, int method)
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
}

void Controller::visualize()
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
            glColor3f(simulation->cur_state.smoke_density[idx],
                      simulation->cur_state.smoke_density[idx],
                      simulation->cur_state.smoke_density[idx]);
            glVertex2f(px, py);

            for (i = 0; i < Config::GRID_SIZE - 1; i++)
            {
                px  = wn + (fftw_real)i * wn;
                py  = hn + (fftw_real)(j + 1) * hn;
                idx = ((j + 1) * Config::GRID_SIZE) + i;
                set_colormap(simulation->cur_state.smoke_density[idx]);
                glVertex2f(px, py);
                px  = wn + (fftw_real)(i + 1) * wn;
                py  = hn + (fftw_real)j * hn;
                idx = (j * Config::GRID_SIZE) + (i + 1);
                set_colormap(simulation->cur_state.smoke_density[idx]);
                glVertex2f(px, py);
            }

            px  = wn + (fftw_real)(Config::GRID_SIZE - 1) * wn;
            py  = hn + (fftw_real)(j + 1) * hn;
            idx = ((j + 1) * Config::GRID_SIZE) + (Config::GRID_SIZE - 1);
            set_colormap(simulation->cur_state.smoke_density[idx]);
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
                direction_to_color(simulation->cur_state.velocity_x[idx],
                                   simulation->cur_state.velocity_y[idx],
                                   Config::color_dir);
                glVertex2f(wn + (fftw_real)i * wn, hn + (fftw_real)j * hn);
                glVertex2f((wn + (fftw_real)i * wn) +
                               Config::vec_scale * simulation->cur_state.velocity_x[idx],
                           (hn + (fftw_real)j * hn) +
                               Config::vec_scale * simulation->cur_state.velocity_y[idx]);
            }
        glEnd();
    }
}

void Controller::display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    visualize();
    glFlush();
    glutSwapBuffers();
}

// http://seriss.com/people/erco/fltk/opengl-sphere-with-light-old.cxx
void Controller::reshape(int w, int h)
{
    glViewport(0.0f, 0.0f, (GLfloat)w, (GLfloat)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
    Config::win_width  = w;
    Config::win_height = h;
}

namespace Tramp
{
    Controller *glob_controller = new Controller();

    void t_display()
    {
        return glob_controller->display();
    }
    void t_reshape(int w, int h)
    {
        return glob_controller->reshape(w, h);
    }
    void t_do_one()
    {
        return glob_controller->simulation->do_one_simulation_step();
    }
    void t_keyboard(uchar key, int x, int y)
    {
        return glob_controller->keyboard(key, x, y);
    }
    void t_drag(int x, int y)
    {
        return glob_controller->drag(x, y);
    }
} // namespace Tramp
