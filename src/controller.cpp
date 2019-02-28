#include "controller.h"
#include "config.h"
#include "simulation.h"
#include "ui.h"
#include <Fl/Fl.H>
#include <Fl/gl.h>
#include <Fl/glu.h>
#include <iostream>
#include <math.h>
#include <vector>

// https://web.cecs.pdx.edu/~fliu/courses/cs447/tutorial5.html
void idle_callback_sim(void *ptr_data)
{
    if (ptr_data != NULL)
    {
        GlWindow *ptr_gl_window = reinterpret_cast<GlWindow *>(ptr_data);
        if (!Config::frozen)
        {
            ptr_gl_window->ptr_simulation->do_one_simulation_step();
            ptr_gl_window->redraw();
        }
    }
}

Controller::Controller()
{
    simulation = new Simulation();
    window     = new UserInterface();
}

// http://webcache.googleusercontent.com/search?q=cache:MsCYe9ordKkJ:www.fltk.org/strfiles/2590/glut_with_fltk2.cxx
int Controller::begin()
{
    Fl::gl_visual(FL_RGB);
    window->make_window(this);
    // set up main simulation window
    window->gl_window->start_gl_window(this, simulation);
    window->show();
    window->gl_window->show();

    // initialise the color bar
    window->color_bar->start_color_bar(this);
    window->color_bar->show();

    Fl::add_idle(idle_callback_sim, window->gl_window);
    return Fl::run();
}

// most of the handling is done via UI buttons, but we will keep this around in case we want
// keyboard interaction
void Controller::keyboard(unsigned char key)
{
    switch (key)
    {
        case 'q':
            exit(0);
    }
}

void Controller::drag(int mx, int my)
{
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
    {
        X = Config::GRID_SIZE - 1;
    }
    if (Y > (Config::GRID_SIZE - 1))
    {
        Y = Config::GRID_SIZE - 1;
    }
    if (X < 0)
    {
        X = 0;
    }
    if (Y < 0)
    {
        Y = 0;
    }

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

void Controller::rainbow(float value, float RGB[3], int index)
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

    RGB[index + 0] = std::max(0.0f, (3 - std::fabs(value - 4) - std::fabs(value - 5) / 2));
    RGB[index + 1] = std::max(0.0f, (4 - std::fabs(value - 2) - std::fabs(value - 4) / 2));
    RGB[index + 2] = std::max(0.0f, (3 - std::fabs(value - 1) - std::fabs(value - 2) / 2));
}

void Controller::red_to_white(float value, float RGB[3], int index)
{
    if (value < 0)
    {
        value = 0;
    }
    else if (value > 1)
    {
        value = 1;
    }

    RGB[index + 0] = 1;
    RGB[index + 1] = value;
    RGB[index + 2] = value;
}

void Controller::set_colormap(float vy)
{
    float RGB[3] = {0};

    switch (Config::scalar_col)
    {

        case Config::COLOR_BLACKWHITE:
        {
            for (int i = 0; i < 3; i++)
            {
                RGB[i] = vy;
            }
            break;
        }
        case Config::COLOR_RAINBOW:
        {
            rainbow(vy, RGB);
            break;
        }
        case Config::COLOR_RED_TO_WHITE:
        {
            red_to_white(vy, RGB);
            break;
        }

        case Config::COLOR_BANDS:
        {
            const int NLEVELS = 7;
            vy *= NLEVELS;
            vy = (int)(vy);
            vy /= NLEVELS;
            rainbow(vy, RGB);
            break;
        }
        default:
        {
            std::cout << "Something went wrong" << std::endl;
        }
    }

    glColor3fv(RGB);
}

void Controller::direction_to_color(float x, float y, bool method)
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

    float RGB[3] = {r, g, b};

    glColor3fv(RGB);
}

void Controller::visualize()
{
    int       glyph_i, glyph_j, grid_i, grid_j, idx;
    float     glyph_to_grid_ratio = Config::GRID_SIZE / Config::num_glyphs;
    double    pt_x, pt_y;
    fftw_real wn =
        (fftw_real)Config::win_width / (fftw_real)(Config::num_glyphs - 1); // Grid cell width
    fftw_real hn =
        (fftw_real)Config::win_height / (fftw_real)(Config::num_glyphs - 1); // Grid cell heigh

    int win_height = window->gl_window->pixel_h();
    int win_width  = window->gl_window->pixel_w();

    if (Config::draw_smoke)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        for (glyph_j = 0; glyph_j < Config::num_glyphs - 1; glyph_j++) // draw smoke
        {
            glBegin(GL_TRIANGLE_STRIP);

            glyph_i = 0;
            pt_x    = wn + (fftw_real)glyph_i * wn;
            pt_y    = hn + (fftw_real)glyph_j * hn;

            grid_i = std::round(glyph_i * glyph_to_grid_ratio);
            grid_j = std::round(glyph_j * glyph_to_grid_ratio);

            idx = (grid_j * Config::GRID_SIZE) + grid_i;
            glColor3f(simulation->cur_state.smoke_density[idx],
                      simulation->cur_state.smoke_density[idx],
                      simulation->cur_state.smoke_density[idx]);
            glVertex2f(pt_x, pt_y);

            for (glyph_i = 0; glyph_i < Config::num_glyphs - 1; glyph_i++)
            {
                pt_x = wn + (fftw_real)glyph_i * wn;
                pt_y = hn + (fftw_real)(glyph_j + 1) * hn;

                grid_i = std::round(glyph_i * glyph_to_grid_ratio);
                grid_j = std::round(glyph_j * glyph_to_grid_ratio);
                idx    = ((grid_j + 1) * Config::GRID_SIZE) + grid_i;
                set_colormap(simulation->cur_state.smoke_density[idx]);
                glVertex2f(pt_x, pt_y);
                pt_x = wn + (fftw_real)(glyph_i + 1) * wn;
                pt_y = hn + (fftw_real)glyph_j * hn;

                idx = (grid_j * Config::GRID_SIZE) + (grid_i + 1);
                set_colormap(simulation->cur_state.smoke_density[idx]);
                glVertex2f(pt_x, pt_y);
            }

            pt_x = wn + (fftw_real)(Config::GRID_SIZE - 1) * wn;
            pt_y = hn + (fftw_real)(glyph_j + 1) * hn;

            grid_i = std::round(glyph_i * glyph_to_grid_ratio);
            grid_j = std::round(glyph_j * glyph_to_grid_ratio);
            idx    = ((grid_j + 1) * Config::GRID_SIZE) + (Config::GRID_SIZE - 1);
            set_colormap(simulation->cur_state.smoke_density[idx]);
            glVertex2f(pt_x, pt_y);
            glEnd();

            glEnd();
        }
    }

    if (Config::draw_vecs)
    {
        glBegin(GL_LINES); // draw velocities
        for (glyph_i = 0; glyph_i < Config::GRID_SIZE; glyph_i++)
        {
            for (glyph_j = 0; glyph_j < Config::GRID_SIZE; glyph_j++)
            {
                idx = (glyph_j * Config::GRID_SIZE) + glyph_i;
                direction_to_color(simulation->cur_state.velocity_x[idx],
                                   simulation->cur_state.velocity_y[idx],
                                   Config::color_dir);
                glVertex2f(wn + (fftw_real)glyph_i * wn, hn + (fftw_real)glyph_j * hn);

                glVertex2f((wn + (fftw_real)glyph_i * wn) +
                               Config::vec_scale * simulation->cur_state.velocity_x[idx],
                           (hn + (fftw_real)glyph_j * hn) +
                               Config::vec_scale * simulation->cur_state.velocity_y[idx]);
            }
        }
        glEnd();
    }
}

// http://seriss.com/people/erco/fltk/opengl-sphere-with-light-old.cxx
// todo: implement
void Controller::reshape(int w, int h)
{
    glViewport(0.0f, 0.0f, (GLfloat)w, (GLfloat)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
    Config::win_width  = w;
    Config::win_height = h;
}

void Controller::change_map_color(int config_color)
{
    // this does not check whether config_color is properly implemented
    Config::scalar_col = config_color;
    window->color_bar->redraw();
}