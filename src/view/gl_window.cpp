// http://seriss.com/people/erco/fltk/opengl-sphere-with-light-old.cxx

#include "gl_window.hpp"
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
    Config::grid_width = (float)H / Config::GRID_SIZE;
}

void GlWindow::draw()
{
    if (!valid())
    {
        valid(1);
        glViewport(0.0f, 0.0f, (GLfloat)Config::win_width, (GLfloat)Config::win_height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        // gluOrtho2D(0.0, (GLdouble)Config::win_width, 0.0, (GLdouble)Config::win_height);
        // glOrtho(0.0, (GLdouble)Config::win_width, 0.0, (GLdouble)Config::win_height, -1, 1);
        gluPerspective(60, 1, -10, 10);
        gluLookAt(Config::win_height / 2,
                  Config::win_width / 2 - 100,
                  900,
                  Config::win_height / 2,
                  Config::win_width / 2,
                  0,
                  0,
                  1,
                  0);
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    visualize();
    glFlush();
}

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

                x_grid_index = std::round(x_glyph_index * glyph_to_grid_ratio);
                y_grid_index = std::round(y_glyph_index * glyph_to_grid_ratio);

                idx = (y_grid_index * Config::GRID_SIZE) + x_grid_index;
                if (Config::vector_color)
                {
                    ColorMapper::set_colormap(scalar_field[idx], RGB);
                }
                glColor3fv(RGB);

                coord start, end;

                start = std::make_pair(
                    (GLfloat)(x_glyph_width + (fftw_real)x_glyph_index * x_glyph_width),
                    (GLfloat)(y_glyph_width + (fftw_real)y_glyph_index * y_glyph_width));
                end = std::make_pair(
                    (GLfloat)((x_glyph_width + (fftw_real)x_glyph_index * x_glyph_width) +
                              Config::vec_scale * vector_field_x[idx]),
                    (GLfloat)((y_glyph_width + (fftw_real)y_glyph_index * y_glyph_width) +
                              Config::vec_scale * vector_field_y[idx]));
                render_vector(start, end);
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
}

void GlWindow::render_vector(coord start, coord end)
{
    // scale it to the grid width
    GLfloat vector_length = std::hypot(end.first - start.first, end.second - start.second);

    if (vector_length > Config::grid_width)
    {
        end.first = start.first + (end.first - start.first) * Config::grid_width / vector_length;
        end.second =
            start.second + (end.second - start.second) * Config::grid_width / vector_length;
    }
    switch (Config::vector_shape)
    {
        case Config::ARROW_2D:
        {
            render_arrow_2d(start, end);
            break;
        }
        case Config::CONE:
        {
            render_cone(start, end);
            break;
        }
        case Config::HEDGEHOG:
        default:
        {
            render_hedgehog(start, end);
            break;
        }
    }
}

void GlWindow::render_hedgehog(coord start, coord end)
{
    glVertex2f(start.first, start.second);
    glVertex2f(end.first, end.second);
}

void GlWindow::render_cone(coord start, coord end)
{
    // https://math.stackexchange.com/questions/1184038/what-is-the-equation-of-a-general-circle-in-3-d-space
    // in the above notation, v1 is (0,0,1), v2 is (y, -x, 0) where (x, y) is end - start

    glVertex3f(end.first, end.second, 0);
    GLfloat cone_length = std::hypot(end.first - start.first, end.second - start.second);
    GLfloat cone_radius = 0.5 * cone_length;

    GLfloat cur_cos, cur_sin;

    int slices = 10;

    std::vector<GLfloat> center = {start.first, start.second, 0};
    std::vector<GLfloat> v1     = {0, 0, 1};
    std::vector<GLfloat> v2     = {
        ((end.second - start.second) / cone_length), ((start.first - end.first) / cone_length), 0};

    std::vector<GLfloat> cur_point = {0, 0, 0};

    GLfloat cur_angle = 0;
    GLfloat increment = M_PI / (float)slices;

    for (int i = 0; i < slices; i++, cur_angle += increment)
    {
        cur_sin = std::sin(cur_angle);
        cur_cos = std::cos(cur_angle);

        for (int j = 0; j < 3; j++)
        {
            cur_point[j] = center[j] + cone_radius * (cur_cos * v1[j] + cur_sin * v2[j]);
        }

        glVertex3f(cur_point[0], cur_point[1], cur_point[2]);
    }

    // draw starting point again to complete the cone
    glVertex3f(center[0] + v1[0], center[1] + v1[1], center[2] + v1[2]);
}

void GlWindow::render_arrow_2d(coord start, coord end)
{
    // https://www.gamedev.net/forums/topic/229253-how-to-draw-arrows-in-opengl/
    // first draw the arrow horizontally along the positive x-axis
    // then rotate it

    GLfloat vector_length     = std::hypot(end.first - start.first, end.second - start.second);
    GLfloat vector_width      = 0.1 * vector_length;
    GLfloat arrow_base_length = 0.7 * vector_length;
    GLfloat arrow_base_width  = 0.4 * vector_width;
    GLfloat arrow_head_width  = vector_width;

    GLfloat vector_angle = std::acos((end.first - start.first) / vector_length);

    coord v1 = rotate_2d(start, start.first, start.second + 0.5 * arrow_base_width, vector_angle);
    coord v2 = rotate_2d(start,
                         start.first + arrow_base_length,
                         start.second + 0.5 * arrow_base_width,
                         vector_angle);
    coord v3 = rotate_2d(start,
                         start.first + arrow_base_length,
                         start.second + 0.5 * arrow_head_width,
                         vector_angle);
    coord v4 = rotate_2d(start, start.first + vector_length, start.second, vector_angle);
    coord v5 = rotate_2d(start,
                         start.first + arrow_base_length,
                         start.second - 0.5 * arrow_head_width,
                         vector_angle);
    coord v6 = rotate_2d(start,
                         start.first + arrow_base_length,
                         start.second - 0.5 * arrow_base_width,
                         vector_angle);
    coord v7 = rotate_2d(start, start.first, start.second - 0.5 * arrow_base_width, vector_angle);

    glVertex2f(v1.first, v1.second);
    glVertex2f(v2.first, v2.second);
    glVertex2f(v3.first, v3.second);
    glVertex2f(v4.first, v4.second);
    glVertex2f(v5.first, v5.second);
    glVertex2f(v6.first, v6.second);
    glVertex2f(v7.first, v7.second);
    glVertex2f(v1.first, v1.second);

    glEnd();
    glBegin(GL_POLYGON);
}

coord GlWindow::rotate_2d(coord center, GLfloat x_vertex, GLfloat y_vertex, GLfloat angle)
{
    GLfloat x_final, y_final;

    GLfloat cos_angle = std::cos(angle);
    GLfloat sin_angle = std::sin(angle);

    coord result;

    // translate back so we are relative to origin
    x_vertex -= center.first;
    y_vertex -= center.second;

    // rotate
    x_final = x_vertex * cos_angle - y_vertex * sin_angle;
    y_final = x_vertex * sin_angle + y_vertex * cos_angle;

    // translate to the right coordinates
    x_final += center.first;
    y_final += center.second;

    result = std::make_pair(x_final, y_final);
    return result;
}