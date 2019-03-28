// http://seriss.com/people/erco/fltk/opengl-sphere-with-light-old.cxx

#include "gl_window.hpp"
#include "render_vector.hpp"
#include <Fl/Fl.H>
#include <Fl/Fl_Gl_Window.H>
#include <Fl/gl.h>
#include <Fl/glu.h>
#include <iostream>
#include <math.h>
#include <vector>

GlWindow::GlWindow(int X, int Y, int W, int H) : Fl_Gl_Window(X, Y, W, H)
{
    Config::win_height = H;
    Config::win_width  = W;
    Config::grid_width = (float)H / (float)Config::GRID_SIZE;
}

void GlWindow::draw()
{
    if (!valid())
    {
        valid(1);
        glViewport(0.0f, 0.0f, (GLfloat)Config::win_width, (GLfloat)Config::win_height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0.0, (GLdouble)Config::win_width, 0.0, (GLdouble)Config::win_height);
        // gluPerspective(70, 1, -10, 10);
        // gluLookAt(Config::win_height / 2,
        //           Config::win_width / 2,
        //           800,
        //           Config::win_height / 2,
        //           Config::win_width / 2,
        //           0,
        //           0,
        //           1,
        //           0);
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    visualize();
    glFlush();
}

// int GlWindow::handle(int event)
// {
//     switch (event)
//     {
//         case FL_PUSH:
//         {
//             // std::cout << Fl::event_x() << ", " << Fl::event_y() << std::endl;
//             return 1;
//         }
//     }
// }

void GlWindow::set_scalar_data(std::vector<fftw_real> new_scalar_field, fftw_real max_scalar)
{
    if (Config::scaling)
    {
        for (int i = 0; i < Config::NUM_CELLS; i++)
        {
            new_scalar_field[i] /= max_scalar;
        }
    }

    scalar_field = new_scalar_field;
}

void GlWindow::set_vector_data(std::vector<fftw_real> new_vector_field_x,
                               std::vector<fftw_real> new_vector_field_y)
{
    vector_field_x = new_vector_field_x;
    vector_field_y = new_vector_field_y;
}

void GlWindow::set_vel_data(std::vector<fftw_real> new_vel_field_x,
                            std::vector<fftw_real> new_vel_field_y)
{
    vel_field_x = new_vel_field_x;
    vel_field_y = new_vel_field_y;
}

void GlWindow::visualize()
{
    int x_grid_index, y_grid_index, x_glyph_index, y_glyph_index, idx;
    // this assumes width and height are the same
    float     glyph_to_grid_ratio = (float)Config::GRID_SIZE / (float)Config::num_glyphs;
    double    x_pixel, y_pixel;
    fftw_real x_glyph_width =
        (fftw_real)Config::win_width / (fftw_real)(Config::num_glyphs - 1); // Grid cell width
    fftw_real y_glyph_width =
        (fftw_real)Config::win_height / (fftw_real)(Config::num_glyphs - 1); // Grid cell height

    fftw_real x_grid_width = (fftw_real)Config::win_width / (fftw_real)(Config::GRID_SIZE - 1);
    fftw_real y_grid_width = (fftw_real)Config::win_height / (fftw_real)(Config::GRID_SIZE - 1);

    if (Config::draw_smoke)
    {
        float RGB[3] = {0};

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        for (y_grid_index = 0; y_grid_index - 1 < Config::GRID_SIZE - 1; y_grid_index++)
        {
            glBegin(GL_TRIANGLE_STRIP);

            x_grid_index = 0;
            x_pixel      = x_grid_width + (fftw_real)x_grid_index * x_grid_width;
            y_pixel      = y_grid_width + (fftw_real)y_grid_index * y_grid_width;

            idx = (y_grid_index * Config::GRID_SIZE) + x_grid_index;
            ColorMapper::set_colormap(scalar_field[idx], RGB);
            glColor3fv(RGB);

            glVertex2f(x_pixel, y_pixel);

            for (x_grid_index = 0; x_grid_index < Config::GRID_SIZE - 1; x_grid_index++)
            {
                x_pixel = x_grid_width + (fftw_real)x_grid_index * x_grid_width;
                y_pixel = y_grid_width + (fftw_real)(y_grid_index + 1) * y_grid_width;

                idx = ((y_grid_index + 1) * Config::GRID_SIZE) + x_grid_index;
                ColorMapper::set_colormap(scalar_field[idx], RGB);
                glColor3fv(RGB);

                glVertex2f(x_pixel, y_pixel);
                x_pixel = x_grid_width + (fftw_real)(x_grid_index + 1) * x_grid_width;
                y_pixel = y_grid_width + (fftw_real)y_grid_index * y_grid_width;

                idx = (y_grid_index * Config::GRID_SIZE) + (x_grid_index + 1);
                ColorMapper::set_colormap(scalar_field[idx], RGB);
                glColor3fv(RGB);

                glVertex2f(x_pixel, y_pixel);
            }

            x_pixel = x_grid_width + (fftw_real)(Config::GRID_SIZE - 1) * x_grid_width;
            y_pixel = y_grid_width + (fftw_real)(y_grid_index + 1) * y_grid_width;

            idx = ((y_grid_index + 1) * Config::GRID_SIZE) + (Config::GRID_SIZE - 1);
            ColorMapper::set_colormap(scalar_field[idx], RGB);
            glColor3fv(RGB);

            glVertex2f(x_pixel, y_pixel);

            glEnd();
        }
    }

    if (Config::draw_vecs)
    {
        float RGB[3] = {1, 1, 1};
        float scalar_val, vector_x_val, vector_y_val;
        if (Config::vector_shape == Config::HEDGEHOG)
        {
            glBegin(GL_LINES);
        }

        for (x_glyph_index = 0; x_glyph_index < Config::num_glyphs; x_glyph_index++)
        {
            for (y_glyph_index = 0; y_glyph_index < Config::num_glyphs; y_glyph_index++)
            {
                switch (Config::vector_shape)
                {
                    case Config::CONE:
                    {
                        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                        glBegin(GL_TRIANGLE_FAN);
                        break;
                    }
                    case Config::ARROW_2D:
                    {
                        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                        glBegin(GL_POLYGON);
                        break;
                    }
                }

                x_grid_index = x_glyph_index * glyph_to_grid_ratio;
                y_grid_index = y_glyph_index * glyph_to_grid_ratio;

                scalar_val   = bilin_interpolate(x_grid_index, y_grid_index, scalar_field);
                vector_x_val = bilin_interpolate(x_grid_index, y_grid_index, vector_field_x);
                vector_y_val = bilin_interpolate(x_grid_index, y_grid_index, vector_field_y);

                if (Config::vector_color)
                {
                    ColorMapper::set_colormap(scalar_val, RGB);
                }
                glColor3fv(RGB);

                coord start, end;

                start = std::make_pair(
                    (GLfloat)(x_glyph_width + (fftw_real)x_glyph_index * x_glyph_width),
                    (GLfloat)(y_glyph_width + (fftw_real)y_glyph_index * y_glyph_width));
                end = std::make_pair(
                    (GLfloat)((x_glyph_width + (fftw_real)x_glyph_index * x_glyph_width) +
                              Config::vec_scale * vector_x_val),
                    (GLfloat)((y_glyph_width + (fftw_real)y_glyph_index * y_glyph_width) +
                              Config::vec_scale * vector_y_val));
                RenderVector::render_vector(start, end);
                if (Config::vector_shape != Config::HEDGEHOG)
                {
                    glEnd();
                }
            }
        }
        if (Config::vector_shape == Config::HEDGEHOG)
        {
            glEnd();
        }
    }

    if (Config::draw_streamline)
    {
        glBegin(GL_LINES);
        float     RGB[3]       = {1, 1, 1};
        fftw_real sl_time_step = 0.1;
        float     cur_col;
        fftw_real cur_vel_x;
        fftw_real cur_vel_y;
        float     x, y;
        x = Config::win_width / 2;
        y = Config::win_height / 2;

        glVertex2f(x, y);
        for (int i = 0; i < 50; i++)
        {
            cur_vel_x = bilin_interpolate(x, y, vel_field_x);
            cur_vel_y = bilin_interpolate(x, y, vel_field_y);
            std::cout << cur_vel_x * sl_time_step << std::endl;
            cur_col = std::hypot(cur_vel_x, cur_vel_y);
            ColorMapper::set_colormap(cur_col, RGB);
            glVertex2f(x, y);
            x += cur_vel_x * sl_time_step;
            y += cur_vel_y * sl_time_step;
        }
        glEnd();
    }
}

fftw_real GlWindow::bilin_interpolate(float x, float y, std::vector<fftw_real> field)
{
    int       x_floor, y_floor, x_ceil, y_ceil;
    int       lower_left, upper_left, upper_right, lower_right;
    fftw_real result;

    x_floor = (int)std::floor(x);
    y_floor = (int)std::floor(y);

    x_ceil = (int)std::ceil(x);
    y_ceil = (int)std::ceil(y);

    if (x_ceil > Config::GRID_SIZE - 1)
    {
        x_ceil = 0;
    }
    if (y_ceil > Config::GRID_SIZE - 1)
    {
        y_ceil = 0;
    }

    lower_left  = x_floor + y_floor * Config::GRID_SIZE;
    upper_left  = x_floor + y_ceil * Config::GRID_SIZE;
    upper_right = x_ceil + y_ceil * Config::GRID_SIZE;
    lower_right = x_ceil + y_floor * Config::GRID_SIZE;

    // std::cout << field[lower_left] * (float)(x_ceil - x) * (float)(y_ceil - y) << std::endl;

    // check if we're not actually (almost) at a grid cell
    if (((int)x) - x < 0.01 && ((int)y) - y < 0.01)
    {
        result = field[(int)x + Config::GRID_SIZE * (int)y];
    }
    else if (((int)x) - x < 0.01 && ((int)y) - y >= 0.01)
    {
        result = field[lower_left] * (y - y_floor) + field[upper_left] * (y_ceil - y);
    }
    else if (((int)x) - x >= 0.01 && ((int)y) - y < 0.01)
    {
        result = field[lower_left] * (x - x_floor) + field[lower_right] * (x_ceil - x);
    }
    else
    {
        // https://en.wikipedia.org/wiki/Bilinear_interpolation
        result = field[lower_left] * (x_ceil - x) * (y_ceil - y) +
                 field[lower_right] * (x - x_floor) * (y_ceil - y) +
                 field[upper_left] * (x_ceil - x) * (y - x_floor) +
                 field[upper_right] * (x - x_floor) * (y - y_floor);
    }
    return result;
}