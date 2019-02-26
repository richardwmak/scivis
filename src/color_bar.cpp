#include "color_bar.h"
#include "FL/gl.h"
#include "FL/glu.h"
#include <iostream>

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

void ColorBar::draw_rectangle_gradient()
{
    GLfloat col_top[3], col_bot[3];

    // set col_top and col_bot
    switch (Config::scalar_col)
    {
        case Config::COLOR_BLACKWHITE:
        {
            col_top[0] = col_top[1] = col_top[2] = 0;
            col_bot[0] = col_bot[1] = col_bot[2] = 1;
            break;
        }
        case Config::COLOR_RAINBOW:
        {
            float   R_top, G_top, B_top;
            float   R_bot, G_bot, B_bot;
            GLfloat RGB_top[] = {R_top, G_top, B_top};
            GLfloat RGB_bot[] = {R_bot, G_bot, B_bot};

            ptr_controller->rainbow(0.0, &R_top, &G_top, &B_top);
            ptr_controller->rainbow(1.0, &R_bot, &G_bot, &B_bot);

            for (int i = 0; i < 3; i++)
            {
                col_top[i] = RGB_top[i];
                col_bot[i] = RGB_bot[i];
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

    GLfloat bot_left_x  = 0;
    GLfloat bot_left_y  = 0;
    GLfloat bot_right_x = (GLfloat)pixel_w();
    GLfloat bot_right_y = 0;
    GLfloat top_left_x  = 0;
    GLfloat top_left_y  = (GLfloat)pixel_h();
    GLfloat top_right_x = (GLfloat)pixel_w();
    GLfloat top_right_y = (GLfloat)pixel_h();

    glColor3fv(col_bot);
    glVertex2f(bot_left_x, bot_left_y);
    glVertex2f(bot_right_x, bot_right_y);

    glColor3fv(col_top);
    glVertex2f(top_right_x, top_right_y);
    glVertex2f(top_left_x, top_left_y);
    glEnd();
}