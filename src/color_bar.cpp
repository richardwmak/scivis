#include "color_bar.h"
#include "FL/gl.h"
#include "FL/glu.h"
#include <iostream>
#include <vector>

ColorBar::ColorBar(int X, int Y, int W, int H) : Fl_Gl_Window(X, Y, W, H), X(X), Y(Y), W(W), H(H) {}

void ColorBar::start_color_bar(Controller *controller)
{
    ptr_controller = controller;
}

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
        ; // divide into num_bands bands and draw each
    }
    glFlush();
}

void ColorBar::draw_rectangle_gradient(int num_verts)
{
    // divide the bar into num_verts - 1 sections
    std::vector<GLfloat> color(3 * num_verts, 0.0);

    int bar_height = pixel_h();
    int bar_width  = pixel_w();

    float sec_height = bar_height / (num_verts - 1);

    switch (Config::scalar_col)
    {
        case Config::COLOR_BLACKWHITE:
        {
            // set top vertices to be black
            for (int i = 0; i < 3; i++)
            {
                color[i + 3] = 1;
            }
            break;
        }
        case Config::COLOR_RAINBOW:
        {
            for (int i = 0; i < num_verts; i++)
            {
            }
            break;
        }
        default:
        {
            std::cout << "Something went wrong" << std::endl;
        };
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_QUADS);

    float h = 0;

    // since we are using GL_QUADS, we need to loop over the nodes in a bit of a weird way
    for (int i = 0; i < num_verts; i += 2, h += 2 * sec_height)
    {
        glColor3f(color[i + 0], color[i + 1], color[i + 2]);
        glVertex2f(bar_width, h);
        glVertex2f(0, h);
        glColor3f(color[i + 3], color[i + 4], color[i + 5]);
        glVertex2f(0, h + sec_height);
        glVertex2f(bar_width, h + sec_height);
    }

    glEnd();
}