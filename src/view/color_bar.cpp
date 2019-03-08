#include "color_bar.hpp"
#include "color_mapper.hpp"
#include "config.hpp"
#include <FL/gl.h>
#include <FL/glu.h>
#include <iostream>
#include <vector>

ColorBar::ColorBar(int X, int Y, int W, int H) : Fl_Gl_Window(X, Y, W, H) {}

void ColorBar::draw()
{
    if (!valid())
    {
        valid(1);
        glViewport(0.0f, 0.0f, (GLfloat)pixel_w(), (GLfloat)pixel_h());
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0.0, (GLdouble)pixel_w(), 0.0, (GLdouble)pixel_h());
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (Config::gradient)
    {
        draw_rectangle_gradient(); // draw gradient from top to bottom
    }
    else
    {
        draw_rectangle_banded(); // divide into num_bands bands and draw each
    }
    glFlush();
}

void ColorBar::draw_rectangle_gradient()
{
    int num_verts = 200;

    // divide the bar into num_verts - 1 sections
    std::vector<GLfloat> color(3 * num_verts, 0.0);

    int bar_height = pixel_h();
    int bar_width  = pixel_w();

    float sec_height = bar_height / (num_verts - 1);

    float value_increment = ColorMapper::max_scalar / (float)(num_verts - 1);
    float cur_value       = 0;
    for (int i = 0; i < num_verts; i++, cur_value += value_increment)
    {
        ColorMapper::set_colormap(cur_value, &color[3 * i]);
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_QUADS);

    float h = 0;

    // since we are using GL_QUADS, we need to loop over the nodes in a bit of a weird way
    for (int i = 0; i < num_verts - 1; i++, h += sec_height)
    {
        int index = 3 * i;
        glColor3f(color[index + 0], color[index + 1], color[index + 2]);
        glVertex2f(bar_width, h);
        glVertex2f(0, h);
        glColor3f(color[index + 3], color[index + 4], color[index + 5]);
        glVertex2f(0, h + sec_height);
        glVertex2f(bar_width, h + sec_height);
    }

    glEnd();
}

void ColorBar::draw_rectangle_banded()
{
    std::vector<GLfloat> color(3 * Config::num_bands, 0.0);

    int bar_height = pixel_h();
    int bar_width  = pixel_w();

    float sec_height = bar_height / (Config::num_bands);

    float value_increment = ColorMapper::max_scalar / (float)(Config::num_bands - 1);
    float cur_value       = 0;

    for (int i = 0; i < Config::num_bands; i++, cur_value += value_increment)
    {
        ColorMapper::set_colormap(cur_value, &color[3 * i]);
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_QUADS);

    float h = 0;

    // since we are using GL_QUADS, we need to loop over the nodes in a bit of a weird way
    for (int i = 0; i < Config::num_bands; i++, h += sec_height)
    {
        int index = 3 * i;
        glColor3f(color[index + 0], color[index + 1], color[index + 2]);
        glVertex2f(bar_width, h);
        glVertex2f(0, h);
        glVertex2f(0, h + sec_height);
        glVertex2f(bar_width, h + sec_height);
    }

    glEnd();
}