#include "controller.hpp"
#include "config.hpp"
#include "simulation.hpp"
#include "ui.hpp"
#include <Fl/Fl.H>
#include <Fl/Fl_Widget.H>
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
        Controller *ptr_controller = reinterpret_cast<Controller *>(ptr_data);

        if (!Config::frozen)
        {
            ptr_controller->simulation->do_one_simulation_step();
            ptr_controller->window->gl_window->redraw();
        }
    }
}

void idle_callback_con(void *ptr_data)
{
    if (ptr_data != NULL)
    {
        Controller *ptr_controller = reinterpret_cast<Controller *>(ptr_data);

        switch (Fl::event())
        {
            case FL_DRAG:
            {
                ptr_controller->drag(Fl::event_x(), Fl::event_y());
            }
            default:
            {
                ;
            }
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
    window->gl_window->start_w(simulation, this);
    window->show();
    window->gl_window->show();

    // initialise the color bar
    window->color_bar->start_color_bar(this);
    window->color_bar->show();

    Fl::add_idle(idle_callback_sim, this);
    Fl::add_idle(idle_callback_con, this);
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

void Controller::drag(int x_pixel_curr, int y_pixel_curr)
{
    int        x_grid_index, y_grid_index;
    double     dx, dy, len;
    static int x_pixel_prev = 0, y_pixel_prev = 0; // remembers last mouse location

    y_pixel_curr = Config::win_height - y_pixel_curr;
    // Compute the array index that corresponds to the cursor location
    x_grid_index = (int)std::floor((double)(Config::GRID_SIZE) *
                                   ((double)x_pixel_curr / (double)Config::win_width));
    y_grid_index = (int)std::floor((double)(Config::GRID_SIZE) *
                                   ((double)y_pixel_curr / (double)Config::win_height));

    // clamp index values
    if (x_grid_index > (Config::num_glyphs)) x_grid_index = Config::num_glyphs;
    if (y_grid_index > (Config::num_glyphs)) y_grid_index = Config::num_glyphs;
    if (x_grid_index < 0) x_grid_index = 0;
    if (y_grid_index < 0) y_grid_index = 0;

    // Add force at the cursor location
    dx  = x_pixel_curr - x_pixel_prev;
    dy  = y_pixel_curr - y_pixel_prev;
    len = sqrt(dx * dx + dy * dy);
    if (len != 0.0)
    {
        dx *= 0.1 / len;
        dy *= 0.1 / len;
    }

    simulation->cur_state.force_x[y_grid_index * Config::GRID_SIZE + x_grid_index] += dx;
    simulation->cur_state.force_y[y_grid_index * Config::GRID_SIZE + x_grid_index] += dy;
    simulation->cur_state.smoke_density[y_grid_index * Config::GRID_SIZE + x_grid_index] = 10.0f;

    x_pixel_prev = x_pixel_curr;
    y_pixel_prev = y_pixel_curr;
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
    int x_grid_index, y_grid_index, x_glyph_index, y_glyph_index, idx;
    // this assumes width and height are the same
    float     glyph_to_grid_ratio = (float)Config::GRID_SIZE / (float)Config::num_glyphs;
    double    x_pixel, y_pixel;
    fftw_real x_glyph_width =
        (fftw_real)Config::win_width / (fftw_real)(Config::num_glyphs - 1); // Grid cell width
    fftw_real y_glyph_width =
        (fftw_real)Config::win_height / (fftw_real)(Config::num_glyphs - 1); // Grid cell height

    if (Config::draw_smoke)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        for (y_glyph_index = 0; y_glyph_index - 1 < Config::num_glyphs - 1; y_glyph_index++)
        {
            glBegin(GL_TRIANGLE_STRIP);

            x_glyph_index = 0;
            x_pixel       = x_glyph_width + (fftw_real)x_glyph_index * x_glyph_width;
            y_pixel       = y_glyph_width + (fftw_real)y_glyph_index * y_glyph_width;

            x_grid_index = std::round(x_glyph_index * glyph_to_grid_ratio);
            y_grid_index = std::round(y_glyph_index * glyph_to_grid_ratio);

            idx = (y_grid_index * Config::GRID_SIZE) + x_grid_index;
            glColor3f(simulation->cur_state.smoke_density[idx],
                      simulation->cur_state.smoke_density[idx],
                      simulation->cur_state.smoke_density[idx]);
            glVertex2f(x_pixel, y_pixel);

            for (x_glyph_index = 0; x_glyph_index < Config::num_glyphs - 1; x_glyph_index++)
            {
                x_pixel = x_glyph_width + (fftw_real)x_glyph_index * x_glyph_width;
                y_pixel = y_glyph_width + (fftw_real)(y_glyph_index + 1) * y_glyph_width;

                x_grid_index = std::round(x_glyph_index * glyph_to_grid_ratio);
                y_grid_index = std::round(y_glyph_index * glyph_to_grid_ratio);
                idx          = ((y_grid_index + 1) * Config::GRID_SIZE) + x_grid_index;
                set_colormap(simulation->cur_state.smoke_density[idx]);
                glVertex2f(x_pixel, y_pixel);
                x_pixel = x_glyph_width + (fftw_real)(x_glyph_index + 1) * x_glyph_width;
                y_pixel = y_glyph_width + (fftw_real)y_glyph_index * y_glyph_width;

                idx = (y_grid_index * Config::GRID_SIZE) + (x_grid_index + 1);
                set_colormap(simulation->cur_state.smoke_density[idx]);
                glVertex2f(x_pixel, y_pixel);
            }

            x_pixel = x_glyph_width + (fftw_real)(Config::GRID_SIZE - 1) * x_glyph_width;
            y_pixel = y_glyph_width + (fftw_real)(y_glyph_index + 1) * y_glyph_width;

            x_grid_index = std::round(x_glyph_index * glyph_to_grid_ratio);
            y_grid_index = std::round(y_glyph_index * glyph_to_grid_ratio);
            idx          = ((y_grid_index + 1) * Config::GRID_SIZE) + (Config::GRID_SIZE - 1);
            set_colormap(simulation->cur_state.smoke_density[idx]);
            glVertex2f(x_pixel, y_pixel);

            glEnd();
        }
    }

    if (Config::draw_vecs)
    {
        glBegin(GL_LINES); // draw velocities
        for (x_glyph_index = 0; x_glyph_index < Config::num_glyphs; x_glyph_index++)
        {
            for (y_glyph_index = 0; y_glyph_index < Config::num_glyphs; y_glyph_index++)
            {
                x_grid_index = std::round(x_glyph_index * glyph_to_grid_ratio);
                y_grid_index = std::round(y_glyph_index * glyph_to_grid_ratio);

                idx = (y_grid_index * Config::GRID_SIZE) + x_grid_index;
                direction_to_color(simulation->cur_state.velocity_x[idx],
                                   simulation->cur_state.velocity_y[idx],
                                   Config::color_dir);
                glVertex2f(x_glyph_width + (fftw_real)x_glyph_index * x_glyph_width,
                           y_glyph_width + (fftw_real)y_glyph_index * y_glyph_width);

                glVertex2f((x_glyph_width + (fftw_real)x_glyph_index * x_glyph_width) +
                               Config::vec_scale * simulation->cur_state.velocity_x[idx],
                           (y_glyph_width + (fftw_real)y_glyph_index * y_glyph_width) +
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
