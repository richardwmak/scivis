#include "color_mapper.hpp"
#include "config.hpp"
#include <Fl/gl.h>
#include <algorithm>
#include <iostream>
#include <math.h>
#include <vector>

void ColorMapper::rainbow(float value, float RGB[3])
{
    const float dx = 0.8;
    value          = (6 - 2 * dx) * value + dx;

    RGB[0] = std::max(0.0f, (3 - std::fabs(value - 4) - std::fabs(value - 5) / 2));
    RGB[1] = std::max(0.0f, (4 - std::fabs(value - 2) - std::fabs(value - 4) / 2));
    RGB[2] = std::max(0.0f, (3 - std::fabs(value - 1) - std::fabs(value - 2) / 2));
}

void ColorMapper::red_white(float value, float RGB[3])
{
    RGB[0] = 1;
    RGB[1] = value;
    RGB[2] = value;
}

void ColorMapper::black_white(float value, float RGB[3])
{
    RGB[0] = value;
    RGB[1] = value;
    RGB[2] = value;
}

void ColorMapper::set_colormap(float value, float RGB[3])
{
    if (!Config::scaling)
    {
        if (value < Config::clamp_min)
        {
            value = Config::clamp_min;
        }
        else if (value > Config::clamp_max)
        {
            value = Config::clamp_max;
        }
    }

    if (!Config::gradient)
    {
        value *= Config::num_bands;
        value = (int)value;
        value /= Config::num_bands;
    }
    switch (Config::scalar_col)
    {
        case Config::COLOR_BLACKWHITE:
        {
            black_white(value, RGB);
            break;
        }
        case Config::COLOR_RAINBOW:
        {
            rainbow(value, RGB);
            break;
        }
        case Config::COLOR_RED_WHITE:
        {
            red_white(value, RGB);
            break;
        }
        default:
        {
            std::cout << "Something went wrong" << std::endl;
        }
    }
}

GLfloat ColorMapper::set_alpha(float value)
{
    if (!Config::draw_slices)
    {
        return 1;
    }
    else
    {
        if (!Config::scaling)
        {
            if (value < Config::clamp_min)
            {
                value = Config::clamp_min;
            }
            else if (value > Config::clamp_max)
            {
                value = Config::clamp_max;
            }
        }

        if (!Config::gradient)
        {
            value *= Config::num_bands;
            value = (int)value;
            value /= Config::num_bands;
        }

        return value;
    }
}

fftw_real ColorMapper::max_scalar = 0;

fftw_real ColorMapper::set_max_scalar(std::vector<fftw_real> scalar_field)
{
    // max_element returns an iterator so we must dereference it
    fftw_real max_value = *std::max_element(scalar_field.begin(), scalar_field.end());
    max_scalar          = std::fabs(max_value);
    return max_scalar;
}
