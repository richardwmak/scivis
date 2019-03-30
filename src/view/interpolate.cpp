#include "interpolate.hpp"
#include "config.hpp"
#include <math.h>

fftw_real Interpolate::bilin(float x_grid, float y_grid, std::vector<fftw_real> field)
{
    int       x_floor, y_floor;
    int       lower_left, upper_left, upper_right, lower_right;
    fftw_real result;

    x_floor = (int)std::floor(x_grid);
    y_floor = (int)std::floor(y_grid);

    if (x_floor >= (Config::GRID_SIZE - 1) || y_floor >= (Config::GRID_SIZE - 1))
    {
        return 0;
    }

    float x_delta = x_grid - (float)x_floor;
    float y_delta = y_grid - (float)y_floor;

    lower_left  = x_floor + y_floor * Config::GRID_SIZE;
    lower_right = lower_left + 1;
    upper_left  = lower_left + Config::GRID_SIZE;
    upper_right = upper_left + 1;

    // check if we're not actually (almost) at a grid cell
    if (std::round(x_grid) - x_grid < 0.01 && std::round(y_grid) - y_grid < 0.01)
    {
        result = field[lower_left];
    }
    else if (std::round(x_grid) - x_grid < 0.01 && std::round(y_grid) - y_grid >= 0.01)
    {
        result = field[lower_left] * (y_delta) + field[upper_left] * (1 - y_delta);
    }
    else if (std::round(x_grid) - x_grid >= 0.01 && std::round(y_grid) - y_grid < 0.01)
    {
        result = field[lower_left] * (x_delta) + field[lower_right] * (1 - x_delta);
    }
    else
    {
        // https://en.wikipedia.org/wiki/Bilinear_interpolation
        float x_top = field[upper_left] * x_delta + field[upper_right] * (1 - x_delta);
        float x_bot = field[lower_left] * x_delta + field[lower_right] * (1 - x_delta);

        result = x_bot * y_delta + x_top * (1 - y_delta);
    }
    return result;
}