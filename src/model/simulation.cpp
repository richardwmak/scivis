#include "simulation.hpp"
#include "config.hpp"
#include "simulation_state.hpp"
#include <algorithm>
#include <fftw.h>
#include <iostream>
#include <math.h>
#include <rfftw.h>

Simulation::Simulation()
{
    cur_state = SimulationState();
    old_state = SimulationState();

    std::vector<fftw_real> gradient_x, gradient_y;

    plan_rc = rfftw2d_create_plan(
        Config::GRID_SIZE, Config::GRID_SIZE, FFTW_REAL_TO_COMPLEX, FFTW_IN_PLACE);
    plan_cr = rfftw2d_create_plan(
        Config::GRID_SIZE, Config::GRID_SIZE, FFTW_COMPLEX_TO_REAL, FFTW_IN_PLACE);
}

void Simulation::FFT(int direction, void *vx)
{
    if (direction == 1)
    {
        rfftwnd_one_real_to_complex(plan_rc, (fftw_real *)vx, (fftw_complex *)vx);
    }
    else
    {
        rfftwnd_one_complex_to_real(plan_cr, (fftw_complex *)vx, (fftw_real *)vx);
    }
}

int Simulation::clamp(float x)
{
    return ((x) >= 0.0 ? ((int)(x)) : (-((int)(1 - (x)))));
}

void Simulation::compute_next_step()
{
    fftw_real x, y, x0, y0, f, r, U[2], V[2], s, t;
    int       i, j, i0, j0, i1, j1;

    for (i = 0; i < Config::NUM_CELLS; i++)
    {
        cur_state.velocity_x[i] += Config::time_step * old_state.velocity_x[i];
        old_state.velocity_x[i] = cur_state.velocity_x[i];
        cur_state.velocity_y[i] += Config::time_step * old_state.velocity_y[i];
        old_state.velocity_y[i] = cur_state.velocity_y[i];
    }

    for (x = 0.5f / Config::GRID_SIZE, i = 0; i < Config::GRID_SIZE;
         i++, x += 1.0f / Config::GRID_SIZE)
    {
        for (y = 0.5f / Config::GRID_SIZE, j = 0; j < Config::GRID_SIZE;
             j++, y += 1.0f / Config::GRID_SIZE)
        {
            x0 = Config::GRID_SIZE *
                     (x - Config::time_step * old_state.velocity_x[i + Config::GRID_SIZE * j]) -
                 0.5f;
            y0 = Config::GRID_SIZE *
                     (y - Config::time_step * old_state.velocity_y[i + Config::GRID_SIZE * j]) -
                 0.5f;
            i0 = clamp(x0);
            s  = x0 - i0;
            i0 = (Config::GRID_SIZE + (i0 % Config::GRID_SIZE)) % Config::GRID_SIZE;
            i1 = (i0 + 1) % Config::GRID_SIZE;
            j0 = clamp(y0);
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
        }
    }

    for (i = 0; i < Config::GRID_SIZE; i++)
    {
        for (j = 0; j < Config::GRID_SIZE; j++)
        {
            old_state.velocity_x[i + (Config::GRID_SIZE + 2) * j] =
                cur_state.velocity_x[i + Config::GRID_SIZE * j];
            old_state.velocity_y[i + (Config::GRID_SIZE + 2) * j] =
                cur_state.velocity_y[i + Config::GRID_SIZE * j];
        }
    }

    FFT(1, old_state.velocity_x);
    FFT(1, old_state.velocity_y);

    for (i = 0; i <= Config::GRID_SIZE; i += 2)
    {
        x = 0.5f * i;
        for (j = 0; j < Config::GRID_SIZE; j++)
        {
            y = j <= Config::GRID_SIZE / 2 ? (fftw_real)j : (fftw_real)j - Config::GRID_SIZE;
            r = x * x + y * y;
            if (r == 0.0f)
            {
                continue;
            }
            f    = (fftw_real)exp(-r * Config::time_step * Config::visc);
            U[0] = old_state.velocity_x[i + (Config::GRID_SIZE + 2) * j];
            V[0] = old_state.velocity_y[i + (Config::GRID_SIZE + 2) * j];
            U[1] = old_state.velocity_x[i + 1 + (Config::GRID_SIZE + 2) * j];
            V[1] = old_state.velocity_y[i + 1 + (Config::GRID_SIZE + 2) * j];

            old_state.velocity_x[i + (Config::GRID_SIZE + 2) * j] =
                f * ((1 - x * x / r) * U[0] - x * y / r * V[0]);
            old_state.velocity_x[i + 1 + (Config::GRID_SIZE + 2) * j] =
                f * ((1 - x * x / r) * U[1] - x * y / r * V[1]);
            old_state.velocity_y[i + (Config::GRID_SIZE + 2) * j] =
                f * (-y * x / r * U[0] + (1 - y * y / r) * V[0]);
            old_state.velocity_y[i + 1 + (Config::GRID_SIZE + 2) * j] =
                f * (-y * x / r * U[1] + (1 - y * y / r) * V[1]);
        }
    }

    FFT(-1, old_state.velocity_x);
    FFT(-1, old_state.velocity_y);

    f = 1.0 / (Config::NUM_CELLS);

    for (i = 0; i < Config::GRID_SIZE; i++)
    {
        for (j = 0; j < Config::GRID_SIZE; j++)
        {
            cur_state.velocity_x[i + Config::GRID_SIZE * j] =
                f * old_state.velocity_x[i + (Config::GRID_SIZE + 2) * j];
            cur_state.velocity_y[i + Config::GRID_SIZE * j] =
                f * old_state.velocity_y[i + (Config::GRID_SIZE + 2) * j];
        }
    }
}

void Simulation::diffuse_matter()
{
    fftw_real x, y, x0, y0, s, t;
    int       i, j, i0, j0, i1, j1;

    for (x = 0.5f / Config::GRID_SIZE, i = 0; i < Config::GRID_SIZE;
         i++, x += 1.0f / Config::GRID_SIZE)
    {
        for (y = 0.5f / Config::GRID_SIZE, j = 0; j < Config::GRID_SIZE;
             j++, y += 1.0f / Config::GRID_SIZE)
        {
            x0 = Config::GRID_SIZE *
                     (x - Config::time_step * cur_state.velocity_x[i + Config::GRID_SIZE * j]) -
                 0.5f;
            y0 = Config::GRID_SIZE *
                     (y - Config::time_step * cur_state.velocity_y[i + Config::GRID_SIZE * j]) -
                 0.5f;
            i0 = clamp(x0);
            s  = x0 - i0;
            i0 = (Config::GRID_SIZE + (i0 % Config::GRID_SIZE)) % Config::GRID_SIZE;
            i1 = (i0 + 1) % Config::GRID_SIZE;
            j0 = clamp(y0);
            t  = y0 - j0;
            j0 = (Config::GRID_SIZE + (j0 % Config::GRID_SIZE)) % Config::GRID_SIZE;
            j1 = (j0 + 1) % Config::GRID_SIZE;
            cur_state.smoke_density[i + Config::GRID_SIZE * j] =
                (1 - s) * ((1 - t) * old_state.smoke_density[i0 + Config::GRID_SIZE * j0] +
                           t * old_state.smoke_density[i0 + Config::GRID_SIZE * j1]) +
                s * ((1 - t) * old_state.smoke_density[i1 + Config::GRID_SIZE * j0] +
                     t * old_state.smoke_density[i1 + Config::GRID_SIZE * j1]);
        }
    }
}

void Simulation::set_forces()
{
    int i;
    for (i = 0; i < Config::NUM_CELLS; i++)
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

        if (Config::vector_choice == Config::VECTOR_GRADIENT_SMOKE ||
            Config::vector_choice == Config::VECTOR_GRADIENT_VELOCITY)
        {
            compute_gradient();
        }
    }
}

void Simulation::compute_gradient()
{
    gradient_x.clear();
    gradient_y.clear();

    fftw_real              grad_x, grad_y;
    std::vector<fftw_real> scalar_field;

    if (Config::vector_choice == Config::VECTOR_GRADIENT_SMOKE)
    {
        for (int i = 0; i < Config::NUM_CELLS; i++)
        {
            scalar_field.push_back(cur_state.smoke_density[i]);
        }
    }
    else if (Config::vector_choice == Config::VECTOR_GRADIENT_VELOCITY)
    {
        for (int i = 0; i < Config::NUM_CELLS; i++)
        {
            scalar_field.push_back(std::hypot(cur_state.velocity_x[i], cur_state.velocity_y[i]));
        }
    }

    for (int y_grid = 0; y_grid < Config::GRID_SIZE; y_grid++)
    {
        for (int x_grid = 0; x_grid < Config::GRID_SIZE; x_grid++)
        {
            int index = x_grid + y_grid * Config::GRID_SIZE;
            if (x_grid == Config::GRID_SIZE - 1)
            {
                grad_x = scalar_field[index + 1 - Config::GRID_SIZE] - scalar_field[index];
            }
            else
            {
                grad_x = scalar_field[index + 1] - scalar_field[index];
            }
            if (y_grid == Config::GRID_SIZE - 1)
            {
                grad_y = scalar_field[x_grid] - scalar_field[index];
            }
            else
            {
                grad_y = scalar_field[index + Config::GRID_SIZE] - scalar_field[index];
            }
            gradient_x.push_back(grad_x);
            gradient_y.push_back(grad_y);
        }
    }
}

std::vector<fftw_real> Simulation::get_scalar_field()
{
    std::vector<fftw_real> scalar_field;
    if (Config::scalar_choice == Config::SCALAR_FORCE)
    {
        for (int i = 0; i < Config::NUM_CELLS; i++)
        {
            scalar_field.push_back(std::hypot(cur_state.force_x[i], cur_state.force_y[i]));
        }
    }

    else if (Config::scalar_choice == Config::SCALAR_VELOCITY)
    {
        for (int i = 0; i < Config::NUM_CELLS; i++)
        {
            scalar_field.push_back(std::hypot(cur_state.velocity_x[i], cur_state.velocity_y[i]));
        }
    }
    else if (Config::scalar_choice == Config::SCALAR_SMOKE)
    {
        // https://stackoverflow.com/questions/259297/how-do-you-copy-the-contents-of-an-array-to-a-stdvector-in-c-without-looping
        std::copy(cur_state.smoke_density,
                  cur_state.smoke_density + Config::NUM_CELLS,
                  std::back_inserter(scalar_field));
    }
    return scalar_field;
}

std::vector<fftw_real> Simulation::get_vector_field_x()
{
    return get_vector_field(cur_state.force_x, cur_state.velocity_x, gradient_x);
}

std::vector<fftw_real> Simulation::get_vector_field_y()
{
    return get_vector_field(cur_state.force_y, cur_state.velocity_y, gradient_y);
}

std::vector<fftw_real>
Simulation::get_vector_field(fftw_real *force, fftw_real *velocity, std::vector<fftw_real> gradient)
{
    switch (Config::vector_choice)
    {
        case Config::VECTOR_FORCE:
        {
            std::vector<fftw_real> vector_field(force, force + Config::NUM_CELLS);
            return vector_field;
        }
        case Config::VECTOR_GRADIENT_SMOKE:
        case Config::VECTOR_GRADIENT_VELOCITY:
        {
            return gradient;
        }
        case Config::VECTOR_VELOCITY:
        default:
        {
            std::vector<fftw_real> vector_field(velocity, velocity + Config::NUM_CELLS);
            return vector_field;
        }
    }
}

std::vector<fftw_real> Simulation::get_vel_field_x()
{
    std::vector<fftw_real> vel_field_x(cur_state.velocity_x,
                                       cur_state.velocity_x + Config::NUM_CELLS);
    return vel_field_x;
}

std::vector<fftw_real> Simulation::get_vel_field_y()
{
    std::vector<fftw_real> vel_field_y(cur_state.velocity_y,
                                       cur_state.velocity_y + Config::NUM_CELLS);
    return vel_field_y;
}