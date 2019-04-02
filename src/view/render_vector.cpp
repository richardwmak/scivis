#include "render_vector.hpp"
#include "color_mapper.hpp"
#include "config.hpp"
#include "interpolate.hpp"
#include <FL/gl.h>
#include <math.h>
#include <rfftw.h>
#include <vector>

void RenderVector::render_vector(std::vector<fftw_real> scalar_field,
                                 std::vector<fftw_real> vector_field_x,
                                 std::vector<fftw_real> vector_field_y,
                                 GLfloat                height)
{
    int x_glyph_index, y_glyph_index;
    // this assumes width and height are the same
    float     glyph_to_grid_ratio = (float)Config::GRID_SIZE / (float)Config::num_glyphs;
    double    x_grid, y_grid;
    fftw_real x_glyph_width =
        (fftw_real)Config::win_width / (fftw_real)(Config::num_glyphs - 1); // Grid cell width
    fftw_real y_glyph_width =
        (fftw_real)Config::win_height / (fftw_real)(Config::num_glyphs - 1); // Grid cell height

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

            x_grid = (float)x_glyph_index * glyph_to_grid_ratio;
            y_grid = (float)y_glyph_index * glyph_to_grid_ratio;

            scalar_val   = Interpolate::bilin(x_grid, y_grid, scalar_field);
            vector_x_val = Interpolate::bilin(x_grid, y_grid, vector_field_x);
            vector_y_val = Interpolate::bilin(x_grid, y_grid, vector_field_y);

            if (Config::vector_color)
            {
                ColorMapper::set_colormap(scalar_val, RGB);
            }
            glColor3fv(RGB);

            coord start, end;

            start =
                std::make_pair((GLfloat)(x_glyph_width + (fftw_real)x_glyph_index * x_glyph_width),
                               (GLfloat)(y_glyph_width + (fftw_real)y_glyph_index * y_glyph_width));
            end = std::make_pair(
                (GLfloat)((x_glyph_width + (fftw_real)x_glyph_index * x_glyph_width) +
                          Config::vec_scale * vector_x_val),
                (GLfloat)((y_glyph_width + (fftw_real)y_glyph_index * y_glyph_width) +
                          Config::vec_scale * vector_y_val));
            controller(start, end, height);
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

void RenderVector::controller(coord start, coord end, GLfloat height)
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
            render_arrow_2d(start, end, height);
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
            render_hedgehog(start, end, height);
            break;
        }
    }
}

void RenderVector::render_hedgehog(coord start, coord end, GLfloat height)
{
    glVertex3f(start.first, start.second, height);
    glVertex3f(end.first, end.second, height);
}

void RenderVector::render_cone(coord start, coord end)
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

void RenderVector::render_arrow_2d(coord start, coord end, GLfloat height)
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

    glVertex3f(v1.first, v1.second, height);
    glVertex3f(v2.first, v2.second, height);
    glVertex3f(v3.first, v3.second, height);
    glVertex3f(v4.first, v4.second, height);
    glVertex3f(v5.first, v5.second, height);
    glVertex3f(v6.first, v6.second, height);
    glVertex3f(v7.first, v7.second, height);
    glVertex3f(v1.first, v1.second, height);

    glEnd();
    glBegin(GL_POLYGON);
}

coord RenderVector::rotate_2d(coord center, GLfloat x_vertex, GLfloat y_vertex, GLfloat angle)
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