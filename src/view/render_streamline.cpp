#include "render_streamline.hpp"
#include "color_mapper.hpp"
#include "config.hpp"
#include "interpolate.hpp"
#include <iostream>
#include <math.h>
#include <vector>

void RenderStreamline::render_streamlines(std::vector<fftw_real> velocity_x,
                                          std::vector<fftw_real> velocity_y,
                                          std::vector<coord>     seeds,
                                          GLfloat                height)
{
    float x_pixel, y_pixel;
    if (Config::streamline_grid)
    {
        fftw_real x_glyph_width =
            (fftw_real)Config::win_width / (fftw_real)(Config::num_glyphs - 1); // Grid cell width
        fftw_real y_glyph_width =
            (fftw_real)Config::win_height / (fftw_real)(Config::num_glyphs - 1); // Grid cell height

        int x_glyph_index, y_glyph_index;
        for (x_glyph_index = 0; x_glyph_index < Config::num_glyphs; x_glyph_index++)
        {
            for (y_glyph_index = 0; y_glyph_index < Config::num_glyphs; y_glyph_index++)
            {
                x_pixel = (float)(x_glyph_width + (fftw_real)x_glyph_index * x_glyph_width);
                y_pixel = (float)(y_glyph_width + (fftw_real)y_glyph_index * y_glyph_width);
                if (x_pixel > Config::win_width)
                {
                    x_pixel = 1000;
                }
                if (x_pixel < 0)
                {
                    x_pixel = 0;
                }
                if (y_pixel > Config::win_height)
                {
                    y_pixel = 1000;
                }
                if (y_pixel < 0)
                {
                    y_pixel = 0;
                }
                render_streamline(x_pixel, y_pixel, velocity_x, velocity_y, height);
            }
        }
    }
    else
    {
        for (int i = 0; i < seeds.size(); i++)
        {
            render_streamline(seeds[i].first, seeds[i].second, velocity_x, velocity_y, height);
        }
    }
}

void RenderStreamline::render_streamline(float                  x_pixel,
                                         float                  y_pixel,
                                         std::vector<fftw_real> velocity_x,
                                         std::vector<fftw_real> velocity_y,
                                         GLfloat                height)
{
    float     RGB[3]       = {1, 1, 1};
    fftw_real sl_time_step = 1000;
    float     cur_col;
    fftw_real cur_vel_x;
    fftw_real cur_vel_y;
    float     x_grid, y_grid;
    float     pixel_to_grid_ratio = (float)Config::GRID_SIZE / (float)Config::win_width;
    float     length              = 0;
    int       max_iter;

    x_grid = x_pixel * pixel_to_grid_ratio;
    y_grid = y_pixel * pixel_to_grid_ratio;

    glBegin(GL_LINE_STRIP);
    glVertex3f(x_pixel, y_pixel, height);
    Config::streamline_grid ? max_iter = 10
                            : max_iter = 30; // allow more iterations if we have less points
    for (int i = 0; i < max_iter; i++)
    {
        // check length
        if (length > Config::streamline_max_length)
        {
            break;
        }

        if (x_grid > Config::GRID_SIZE)
        {
            x_grid = Config::GRID_SIZE;
        }
        if (x_grid < 0)
        {
            x_grid = 0;
        }
        if (y_grid > Config::GRID_SIZE)
        {
            y_grid = Config::GRID_SIZE;
        }
        if (y_grid < 0)
        {
            y_grid = 0;
        }

        cur_vel_x = Interpolate::bilin(x_grid, y_grid, velocity_x);
        cur_vel_y = Interpolate::bilin(x_grid, y_grid, velocity_y);
        cur_col   = std::hypot(cur_vel_x, cur_vel_y);
        ColorMapper::set_colormap(1 - cur_col, RGB);
        glColor3fv(RGB);
        x_pixel += cur_vel_x * sl_time_step;
        y_pixel += cur_vel_y * sl_time_step;
        glVertex3f(x_pixel, y_pixel, height);
        x_grid = x_pixel * pixel_to_grid_ratio;
        y_grid = y_pixel * pixel_to_grid_ratio;
        length += std::hypot(cur_vel_x * sl_time_step, cur_vel_y * sl_time_step);
    }
    glEnd();
}

void RenderStreamline::render_streamsurf(std::vector<std::vector<fftw_real>> buffer_velocity_x,
                                         std::vector<std::vector<fftw_real>> buffer_velocity_y,
                                         std::vector<coord>                  seeds)
{
    if (seeds.size() < 2)
    {
        return;
    }

    GLfloat max_height = buffer_velocity_x.size();
    GLfloat time_step  = 100;
    GLfloat cur_x1, cur_y1, cur_x2, cur_y2;
    GLfloat cur_vel_x1, cur_vel_y1, cur_vel_x2, cur_vel_y2;
    float   pixel_to_grid_ratio = (float)Config::GRID_SIZE / (float)Config::win_width;

    cur_x1 = seeds[0].first;
    cur_y1 = seeds[0].second;
    cur_x2 = seeds[1].first;
    cur_y2 = seeds[1].second;

    glBegin(GL_LINE_STRIP);
    glVertex3f(cur_x1, cur_y1, 0);
    glVertex3f(cur_x2, cur_y2, 0);

    for (GLfloat height = 0; height < max_height; height++)
    {
        cur_vel_x1 = Interpolate::bilin(
            cur_x1 * pixel_to_grid_ratio, cur_y1 * pixel_to_grid_ratio, buffer_velocity_x[height]);
        cur_vel_x2 = Interpolate::bilin(
            cur_x2 * pixel_to_grid_ratio, cur_y2 * pixel_to_grid_ratio, buffer_velocity_x[height]);
        cur_vel_y1 = Interpolate::bilin(
            cur_x1 * pixel_to_grid_ratio, cur_y1 * pixel_to_grid_ratio, buffer_velocity_y[height]);
        cur_vel_y2 = Interpolate::bilin(
            cur_x2 * pixel_to_grid_ratio, cur_y2 * pixel_to_grid_ratio, buffer_velocity_y[height]);

        cur_x1 += cur_vel_x1 * time_step;
        cur_x2 += cur_vel_x2 * time_step;
        cur_y1 += cur_vel_y1 * time_step;
        cur_y2 += cur_vel_y2 * time_step;

        glVertex3f(cur_x1, cur_y1, (height + 1));
        glVertex3f(cur_x2, cur_y2, (height + 1));
    }
    glEnd();
}