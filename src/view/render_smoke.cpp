#include "render_smoke.hpp"
#include "color_mapper.hpp"
#include "config.hpp"
#include <Fl/gl.h>
#include <rfftw.h>
#include <vector>

void RenderSmoke::render_smoke(std::vector<fftw_real> scalar_field, GLfloat height)
{
    int     x_grid_index, y_grid_index, idx;
    GLfloat alpha = 1;
    // this assumes width and height are the same
    double x_pixel, y_pixel;

    fftw_real x_grid_width = (fftw_real)Config::win_width / (fftw_real)(Config::GRID_SIZE - 1);
    fftw_real y_grid_width = (fftw_real)Config::win_height / (fftw_real)(Config::GRID_SIZE - 1);

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
        alpha = ColorMapper::set_alpha(scalar_field[idx]);
        glColor4f(RGB[0], RGB[1], RGB[2], alpha);

        glVertex3f(x_pixel, y_pixel, height);

        for (x_grid_index = 0; x_grid_index < Config::GRID_SIZE - 1; x_grid_index++)
        {
            x_pixel = x_grid_width + (fftw_real)x_grid_index * x_grid_width;
            y_pixel = y_grid_width + (fftw_real)(y_grid_index + 1) * y_grid_width;

            idx = ((y_grid_index + 1) * Config::GRID_SIZE) + x_grid_index;
            ColorMapper::set_colormap(scalar_field[idx], RGB);
            alpha = ColorMapper::set_alpha(scalar_field[idx]);
            glColor4f(RGB[0], RGB[1], RGB[2], alpha);

            glVertex3f(x_pixel, y_pixel, height);
            x_pixel = x_grid_width + (fftw_real)(x_grid_index + 1) * x_grid_width;
            y_pixel = y_grid_width + (fftw_real)y_grid_index * y_grid_width;

            idx = (y_grid_index * Config::GRID_SIZE) + (x_grid_index + 1);
            ColorMapper::set_colormap(scalar_field[idx], RGB);
            alpha = ColorMapper::set_alpha(scalar_field[idx]);
            glColor4f(RGB[0], RGB[1], RGB[2], alpha);

            glVertex3f(x_pixel, y_pixel, height);
        }

        x_pixel = x_grid_width + (fftw_real)(Config::GRID_SIZE - 1) * x_grid_width;
        y_pixel = y_grid_width + (fftw_real)(y_grid_index + 1) * y_grid_width;

        idx = ((y_grid_index + 1) * Config::GRID_SIZE) + (Config::GRID_SIZE - 1);
        ColorMapper::set_colormap(scalar_field[idx], RGB);
        alpha = ColorMapper::set_alpha(scalar_field[idx]);
        glColor4f(RGB[0], RGB[1], RGB[2], alpha);

        glVertex3f(x_pixel, y_pixel, height);

        glEnd();
    }
}