#include "controller.h"
#include "config.h"
#include "simulation.h"
#include <math.h>

void Controller::keyboard(unsigned char key)
{
    switch (key)
    {
    case 't':
        Config::time_step -= 0.001;
        break;
    case 'T':
        Config::time_step += 0.001;
        break;
    case 'c':
        Config::color_dir = 1 - Config::color_dir;
        break;
    case 'S':
        Config::vec_scale *= 1.2;
        break;
    case 's':
        Config::vec_scale *= 0.8;
        break;
    case 'V':
        Config::visc *= 5;
        break;
    case 'v':
        Config::visc *= 0.2;
        break;
    case 'x':
        Config::draw_smoke = 1 - Config::draw_smoke;
        if (Config::draw_smoke == 0)
            Config::draw_vecs = 1;
        break;
    case 'y':
        Config::draw_vecs = 1 - Config::draw_vecs;
        if (Config::draw_vecs == 0)
            Config::draw_smoke = 1;
        break;
    case 'm':
        Config::scalar_col++;
        if (Config::scalar_col > Config::COLOR_BANDS)
            Config::scalar_col = Config::COLOR_BLACKWHITE;
        break;
    case 'a':
        Config::frozen ? Config::frozen = false : Config::frozen = true;
        break;
    case 'q':
        exit(0);
    }
}

void Controller::drag(int mx, int my, Simulation &simulation)
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
        X = Config::GRID_SIZE - 1;
    if (Y > (Config::GRID_SIZE - 1))
        Y = Config::GRID_SIZE - 1;
    if (X < 0)
        X = 0;
    if (Y < 0)
        Y = 0;

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
    simulation.cur_state.force_x[Y * Config::GRID_SIZE + X] += dx;
    simulation.cur_state.force_y[Y * Config::GRID_SIZE + X] += dy;
    simulation.cur_state.smoke_density[Y * Config::GRID_SIZE + X] = 10.0f;

    lmx = mx;
    lmy = my;
}