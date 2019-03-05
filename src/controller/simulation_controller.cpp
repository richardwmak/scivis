#include "simulation_controller.hpp"
#include "config.hpp"
#include <math.h>

std::pair<float, float> SimulationController::force_from_drag(int x_pixel_curr, int y_pixel_curr)
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

    x_pixel_prev = x_pixel_curr;
    y_pixel_prev = y_pixel_curr;

    return std::make_pair(dx, dy);
}