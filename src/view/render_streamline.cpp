#include "render_streamline.hpp"
#include "color_mapper.hpp"
#include "config.hpp"
#include "interpolate.hpp"
#include <iostream>
#include <math.h>
#include <vector>

void RenderStreamline::render_streamlines(std::vector<fftw_real> velocity_x,
                                          std::vector<fftw_real> velocity_y)
{
    float x_pixel, y_pixel;
    if (Config::streamline_options == Config::GLYPH_POINTS)
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
                if (x_pixel > Config::win_width) x_pixel = 1000;
                if (x_pixel < 0) x_pixel = 0;
                if (y_pixel > Config::win_height) y_pixel = 1000;
                if (y_pixel < 0) y_pixel = 0;
                RenderStreamline::render_streamline(x_pixel, y_pixel, velocity_x, velocity_y);
            }
        }
    }
}

void RenderStreamline::render_streamline(float                  x_pixel,
                                         float                  y_pixel,
                                         std::vector<fftw_real> velocity_x,
                                         std::vector<fftw_real> velocity_y)
{
    float     RGB[3]       = {1, 1, 1};
    fftw_real sl_time_step = 1000;
    float     cur_col;
    fftw_real cur_vel_x;
    fftw_real cur_vel_y;
    float     x_grid, y_grid;
    float     pixel_to_grid_ratio = (float)Config::GRID_SIZE / (float)Config::win_width;

    x_grid = x_pixel * pixel_to_grid_ratio;
    y_grid = y_pixel * pixel_to_grid_ratio;

    glBegin(GL_LINE_STRIP);
    glVertex2f(x_pixel, y_pixel);
    for (int i = 0; i < 10; i++)
    {
        cur_vel_x = Interpolate::bilin(x_grid, y_grid, velocity_x);
        cur_vel_y = Interpolate::bilin(x_grid, y_grid, velocity_y);
        cur_col   = std::hypot(cur_vel_x, cur_vel_y);
        ColorMapper::set_colormap(1 - cur_col, RGB);
        glColor3fv(RGB);
        x_pixel += cur_vel_x * sl_time_step;
        y_pixel += cur_vel_y * sl_time_step;
        glVertex2f(x_pixel, y_pixel);
        x_grid = x_pixel * pixel_to_grid_ratio;
        y_grid = y_pixel * pixel_to_grid_ratio;
    }
    glEnd();
}