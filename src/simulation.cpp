#include "simulation.h"
#include "config.h"
#include <GL/glut.h>
#include <math.h>
#include <rfftw.h>

void SimulationState::initialise()
{
    for (int i = 0; i < Config::GRID_SIZE; i++)
    {
        for (int j = 0; j < Config::GRID_SIZE; j++)
        {
            velocity_x[i + j]    = 0.0;
            velocity_y[i + j]    = 0.0;
            smoke_density[i + j] = 0.0;
            force_x[i + j]       = 0.0;
            force_y[i + j]       = 0.0;
        };
    };
};

void Simulation::initialise()
{
    cur_state.initialise();
    old_state.initialise();

    plan_rc = rfftw2d_create_plan(
        Config::GRID_SIZE, Config::GRID_SIZE, FFTW_REAL_TO_COMPLEX, FFTW_IN_PLACE);
    plan_cr = rfftw2d_create_plan(
        Config::GRID_SIZE, Config::GRID_SIZE, FFTW_COMPLEX_TO_REAL, FFTW_IN_PLACE);
};

void Simulation::fft_r_to_c(void *dataset)
{
    rfftwnd_one_real_to_complex(plan_rc, (fftw_real *)dataset, (fftw_complex *)dataset);
};

void Simulation::fft_c_to_r(void *dataset)
{
    rfftwnd_one_complex_to_real(plan_cr, (fftw_complex *)dataset, (fftw_real *)dataset);
};

void Simulation::compute_next_step()
{

    for (int i = 0; i < Config::NUM_CELLS; i++)
    {
        cur_state.velocity_x[i] += Config::time_step * old_state.velocity_x[i];
        old_state.velocity_x[i] = cur_state.velocity_x[i];

        cur_state.velocity_y[i] += Config::time_step * old_state.velocity_y[i];
        old_state.velocity_y[i] = cur_state.velocity_y[i];
    };

    fftw_real x0, y0, s, t;
    int i0, i1, j0, j1;
    float x, y;
    x = 0.5f / Config::GRID_SIZE;
    y = 0.5f / Config::GRID_SIZE;

    for (int i = 0; i < Config::GRID_SIZE; i++)
    {
        for (int j = 0; j < Config::GRID_SIZE; j++)
        {
            x0 = Config::GRID_SIZE *
                     (x - Config::time_step * old_state.velocity_x[i + Config::GRID_SIZE * j]) -
                 0.5f;
            y0 = Config::GRID_SIZE *
                     (y - Config::time_step * old_state.velocity_y[i + Config::GRID_SIZE * j]) -
                 0.5f;

            i0 = (int)std::floor(x0);
            s  = x0 - i0;
            i0 = (Config::GRID_SIZE + (i0 % Config::GRID_SIZE)) % Config::GRID_SIZE;
            i1 = (i0 + 1) % Config::GRID_SIZE;

            j0 = (int)std::floor(y0);
            t  = y0 - j0;
            j0 = (Config::GRID_SIZE + (j0 % Config::GRID_SIZE)) % Config::GRID_SIZE;
            j1 = (j0 + 1) % Config::GRID_SIZE;

            cur_state.velocity_x[i + Config::GRID_SIZE * j] =
                (1 - s) * ((1 - t) * old_state.velocity_x[i0 + Config::GRID_SIZE * j0] +
                           t * old_state.velocity_x[i0 + Config::GRID_SIZE * j1]) +
                s * ((1 - t) * old_state.velocity_x[i1 + Config::GRID_SIZE * j0] +
                     t * old_state.velocity_x[i1 + Config::GRID_SIZE * j1]);
            cur_state.velocity_y[i + Config::GRID_SIZE * j] =
                (1 - s) * ((1 - t) * old_state.velocity_y[i0 + Config::GRID_SIZE * j0] +
                           t * old_state.velocity_y[i0 + Config::GRID_SIZE * j1]) +
                s * ((1 - t) * old_state.velocity_y[i1 + Config::GRID_SIZE * j0] +
                     t * old_state.velocity_y[i1 + Config::GRID_SIZE * j1]);

            y += 1.0f / Config::GRID_SIZE;
        };
        x += 1.0f / Config::GRID_SIZE;
    };

    for (int i = 0; i < Config::GRID_SIZE; i++)
    {
        for (int j = 0; j < Config::GRID_SIZE; j++)
        {
            old_state.velocity_x[i + (Config::GRID_SIZE + 2) * j] =
                cur_state.velocity_x[i + Config::GRID_SIZE * j];
            old_state.velocity_y[i + (Config::GRID_SIZE + 2) * j] =
                cur_state.velocity_y[i + Config::GRID_SIZE * j];
        };
    };

    Simulation::fft_r_to_c(old_state.velocity_x);
    Simulation::fft_r_to_c(old_state.velocity_y);

    fftw_real r, f, U[2], V[2];

    for (int i = 0; i <= Config::GRID_SIZE; i += 2)
    {
        x = 0.5f * i;
        for (int j = 0; j < Config::GRID_SIZE; j++)
        {
            y = j <= Config::GRID_SIZE / 2 ? (fftw_real)j : (fftw_real)(j - Config::GRID_SIZE);
            r = x * x + y * y;
            if (r == 0.0f)
            {
                continue;
            };

            f    = (fftw_real)exp(-r * Config::time_step * Config::visc);
            U[0] = old_state.velocity_x[i + (Config::GRID_SIZE + 2) * j];
            U[1] = old_state.velocity_x[i + 1 + (Config::GRID_SIZE + 2) * j];

            V[0] = old_state.velocity_y[i + (Config::GRID_SIZE + 2) * j];
            V[1] = old_state.velocity_y[i + 1 + (Config::GRID_SIZE + 2) * j];

            old_state.velocity_x[i + (Config::GRID_SIZE + 2) * j] =
                f * ((1 - x * x / r) * U[0] - x * y / r * V[0]);
            old_state.velocity_x[i + 1 + (Config::GRID_SIZE + 2) * j] =
                f * ((1 - x * x / r) * U[1] - x * y / r * V[1]);
            old_state.velocity_y[i + (Config::GRID_SIZE + 2) * j] =
                f * (-y * x / r * U[0] + (1 - y * y / r) * V[0]);
            old_state.velocity_y[i + 1 + (Config::GRID_SIZE + 2) * j] =
                f * (-y * x / r * U[1] + (1 - y * y / r) * V[1]);
        };
    };

    Simulation::fft_c_to_r(old_state.velocity_x);
    Simulation::fft_c_to_r(old_state.velocity_y);

    f = 1.0 / (Config::NUM_CELLS);
    for (int i = 0; i < Config::GRID_SIZE; i++)
    {
        for (int j = 0; j < Config::GRID_SIZE; j++)
        {
            cur_state.velocity_x[i + Config::GRID_SIZE * j] =
                f * old_state.velocity_x[i + (Config::GRID_SIZE + 2) * j];
            cur_state.velocity_y[i + Config::GRID_SIZE * j] =
                f * old_state.velocity_y[i + (Config::GRID_SIZE + 2) * j];
        };
    };
};

void Simulation::diffuse_matter()
{
    fftw_real x, y, x0, y0, s, t;
    int i0, j0, i1, j1;

    x = 0.5f / Config::GRID_SIZE;
    y = 0.5f / Config::GRID_SIZE;
    for (int i = 0; i < Config::GRID_SIZE; i++)
    {
        for (int j = 0; j < Config::GRID_SIZE; j++)
        {
            x0 = Config::GRID_SIZE *
                     (x - Config::time_step * cur_state.velocity_x[i + Config::GRID_SIZE * j]) -
                 0.5f;
            y0 = Config::GRID_SIZE *
                     (y - Config::time_step * cur_state.velocity_y[i + Config::GRID_SIZE * j]) -
                 0.5f;
            i0 = floor(x0);
            s  = x0 - i0;
            i0 = (Config::GRID_SIZE + (i0 % Config::GRID_SIZE)) % Config::GRID_SIZE;
            i1 = (i0 + 1) % Config::GRID_SIZE;
            j0 = floor(y0);
            t  = y0 - j0;
            j0 = (Config::GRID_SIZE + (j0 % Config::GRID_SIZE)) % Config::GRID_SIZE;
            j1 = (j0 + 1) % Config::GRID_SIZE;
            cur_state.smoke_density[i + Config::GRID_SIZE * j] =
                (1 - s) * ((1 - t) * old_state.smoke_density[i0 + Config::GRID_SIZE * j0] +
                           t * old_state.smoke_density[i0 + Config::GRID_SIZE * j1]) +
                s * ((1 - t) * old_state.smoke_density[i1 + Config::GRID_SIZE * j0] +
                     t * old_state.smoke_density[i1 + Config::GRID_SIZE * j1]);
        };
    };
};

void Simulation::set_forces()
{
    for (int i = 0; i < Config::NUM_CELLS; i++)
    {
        old_state.smoke_density[i] = 0.995 * cur_state.smoke_density[i];
        cur_state.force_x[i] *= 0.85;
        cur_state.force_y[i] *= 0.85;
        old_state.velocity_x[i] = cur_state.force_x[i];
        old_state.velocity_y[i] = cur_state.force_y[i];
    }
}

void Simulation::do_one_simulation_step()
{
    if (!Config::frozen)
    {
        Simulation::set_forces();
        Simulation::compute_next_step();
        Simulation::diffuse_matter();
        glutPostRedisplay();
    };
};