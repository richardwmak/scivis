#include "color_mapper.hpp"
#include "config.hpp"
#include <Fl/gl.h>
#include <Fl/glu.h>
#include <iostream>
#include <math.h>

void ColorMapper::rainbow(float value, float RGB[3], int index)
{
    const float dx = 0.8;
    if (value < 0)
    {
        value = 0;
    }
    else if (value > 1)
    {
        value = 1;
    }

    value = (6 - 2 * dx) * value + dx;

    RGB[index + 0] = std::max(0.0f, (3 - std::fabs(value - 4) - std::fabs(value - 5) / 2));
    RGB[index + 1] = std::max(0.0f, (4 - std::fabs(value - 2) - std::fabs(value - 4) / 2));
    RGB[index + 2] = std::max(0.0f, (3 - std::fabs(value - 1) - std::fabs(value - 2) / 2));
}

void ColorMapper::red_white(float value, float RGB[3], int index)
{
    if (value < 0)
    {
        value = 0;
    }
    else if (value > 1)
    {
        value = 1;
    }

    RGB[index + 0] = 1;
    RGB[index + 1] = value;
    RGB[index + 2] = value;
}

void ColorMapper::black_white(float value, float RGB[3], int index)
{
    RGB[index + 0] = value;
    RGB[index + 1] = value;
    RGB[index + 2] = value;
}

void ColorMapper::set_colormap(float value, float RGB[3], int index)
{
    switch (Config::scalar_col)
    {
        case Config::COLOR_BLACKWHITE:
        {
            black_white(value, RGB, index);
            break;
        }
        case Config::COLOR_RAINBOW:
        {
            rainbow(value, RGB, index);
            break;
        }
        case Config::COLOR_RED_WHITE:
        {
            red_white(value, RGB, index);
            break;
        }
        default:
        {
            std::cout << "Something went wrong" << std::endl;
        }
    }
}

void ColorMapper::direction_to_color(float RGB[3], float x, float y, int index)
{
    float r, g, b, f;
    if (Config::color_dir)
    {
        f = std::atan2(y, x) / 3.1415927 + 1;
        r = f;
        if (r > 1)
        {
            r = 2 - r;
        }
        g = f + .66667;
        if (g > 2)
        {
            g -= 2;
        }
        if (g > 1)
        {
            g = 2 - g;
        }
        b = f + 2 * .66667;
        if (b > 2)
        {
            b -= 2;
        }
        if (b > 1)
        {
            b = 2 - b;
        }
    }
    else
    {
        r = g = b = 1;
    }

    RGB[index + 0] = r;
    RGB[index + 1] = g;
    RGB[index + 2] = b;
}