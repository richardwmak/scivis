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

void ColorMapper::red_to_white(float value, float RGB[3], int index)
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

void ColorMapper::set_colormap(float vy)
{
    float RGB[3] = {0};

    switch (Config::scalar_col)
    {

        case Config::COLOR_BLACKWHITE:
        {
            for (int i = 0; i < 3; i++)
            {
                RGB[i] = vy;
            }
            break;
        }
        case Config::COLOR_RAINBOW:
        {
            rainbow(vy, RGB);
            break;
        }
        case Config::COLOR_RED_TO_WHITE:
        {
            red_to_white(vy, RGB);
            break;
        }

        case Config::COLOR_BANDS:
        {
            const int NLEVELS = 7;
            vy *= NLEVELS;
            vy = (int)(vy);
            vy /= NLEVELS;
            rainbow(vy, RGB);
            break;
        }
        default:
        {
            std::cout << "Something went wrong" << std::endl;
        }
    }

    glColor3fv(RGB);
}

void ColorMapper::direction_to_color(float x, float y, bool method)
{
    float r, g, b, f;
    if (method)
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

    float RGB[3] = {r, g, b};

    glColor3fv(RGB);
}