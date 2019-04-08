// http://seriss.com/people/erco/fltk/opengl-sphere-with-light-old.cxx

#include "gl_window.hpp"
#include "interpolate.hpp"
#include "render_smoke.hpp"
#include "render_streamline.hpp"
#include "render_vector.hpp"
#include <Fl/Fl.H>
#include <Fl/Fl_Gl_Window.H>
#include <Fl/gl.h>
#include <Fl/glu.h>
#include <iostream>
#include <math.h>
#include <vector>

GlWindow::GlWindow(int X, int Y, int W, int H) : Fl_Gl_Window(X, Y, W, H)
{
    // set double buffered
    mode(FL_RGB | FL_ALPHA | FL_DEPTH | FL_DOUBLE);

    Config::win_height = H;
    Config::win_width  = W;
    Config::grid_width = (float)H / (float)Config::GRID_SIZE;

    eye[0] = (Config::win_height / 2);
    eye[1] = (Config::win_width / 2);
    eye[2] = 730;

    center[0] = (Config::win_height / 2);
    center[1] = (Config::win_width / 2);
    center[2] = 0;

    up[0] = 0;
    up[1] = 1;
    up[2] = 0;

    angle  = 0;
    radius = eye[2];

    // these variable determine the angle relative to the origin, these then will get translated
    relative_x = 0;
    relative_z = eye[2];
}

void GlWindow::draw()
{
    if (!valid())
    {
        valid(1);
        glViewport(0.0f, 0.0f, (GLfloat)Config::win_width, (GLfloat)Config::win_height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(70, 1, 0, 2000);
        gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);
        // https://stackoverflow.com/questions/1617370/how-to-use-alpha-transparency-in-opengl
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if (Config::draw_slices && buffer_scalar_field.size() >= Config::buffer_size)
    {
        visualize_slices();
    }
    else
    {
        visualize(0,
                  buffer_scalar_field.front(),
                  buffer_vector_field_x.front(),
                  buffer_vector_field_y.front(),
                  buffer_vel_field_x.front(),
                  buffer_vel_field_y.front());
    }
    glFlush();
}

int GlWindow::handle(int event)
{
    switch (event)
    {
        case FL_PUSH:
        {
            if (Fl::event_ctrl() && Config::draw_streamline)
            {
                add_seed(Fl::event_x(), Config::win_height - Fl::event_y());
            }
            return 1;
        }
        case FL_KEYUP:
        {
            // down
            if (Fl::event_key() == 65364)
            {
                camera_zoom_out();
                return 1;
            }
            // up
            if (Fl::event_key() == 65362)
            {
                camera_zoom_in();
                return 1;
            }
            // left
            if (Fl::event_key() == 65361)
            {
                camera_rotate_left();
                return 1;
            }
            // right
            if (Fl::event_key() == 65363)
            {
                camera_rotate_right();
                return 1;
            }

            // d
            if (Fl::event_key() == 100)
            {
                eye[0] += 100;
                center[0] += 100;
            }

            // a
            if (Fl::event_key() == 97)
            {
                eye[0] -= 100;
                center[0] -= 100;
            }

            // r -> reset
            if (Fl::event_key() == 114)
            {
                eye[0] = (Config::win_height / 2);
                eye[1] = (Config::win_width / 2);
                eye[2] = 730;

                center[0] = (Config::win_height / 2);
                center[1] = (Config::win_width / 2);
                center[2] = 0;

                up[0] = 0;
                up[1] = 1;
                up[2] = 0;

                angle  = 0;
                radius = eye[2];

                relative_x = 0;
                relative_z = eye[2];
            }

            make_current();
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(70, 1, 0, 2000);
            gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);
            return 1;
        }
    }
    return 0;
}

void GlWindow::add_scalar_data(std::vector<fftw_real> new_scalar_field, fftw_real max_scalar)
{
    if (Config::scaling)
    {
        for (int i = 0; i < Config::NUM_CELLS; i++)
        {
            new_scalar_field[i] /= max_scalar;
        }
    }

    buffer_scalar_field.insert(buffer_scalar_field.begin(), new_scalar_field);
    if (buffer_scalar_field.size() > Config::buffer_size)
    {
        buffer_scalar_field.pop_back();
    }
}

void GlWindow::add_vector_data(std::vector<fftw_real> new_vector_field_x,
                               std::vector<fftw_real> new_vector_field_y)
{
    buffer_vector_field_x.insert(buffer_vector_field_x.begin(), new_vector_field_x);
    buffer_vector_field_y.insert(buffer_vector_field_y.begin(), new_vector_field_y);
    if (buffer_vector_field_x.size() > Config::buffer_size)
    {
        buffer_vector_field_x.pop_back();
    }
    if (buffer_vector_field_y.size() > Config::buffer_size)
    {
        buffer_vector_field_y.pop_back();
    }
}

void GlWindow::add_vel_data(std::vector<fftw_real> new_vel_field_x,
                            std::vector<fftw_real> new_vel_field_y)
{
    buffer_vel_field_x.insert(buffer_vel_field_x.begin(), new_vel_field_x);
    buffer_vel_field_y.insert(buffer_vel_field_y.begin(), new_vel_field_y);
    if (buffer_vel_field_x.size() > Config::buffer_size)
    {
        buffer_vel_field_x.pop_back();
    }
    if (buffer_vel_field_y.size() > Config::buffer_size)
    {
        buffer_vel_field_y.pop_back();
    }
}

void GlWindow::add_seed(GLfloat seed_x, GLfloat seed_y)
{
    coord new_seed;
    new_seed = std::make_pair(seed_x, seed_y);
    if (Config::draw_slices && seeds.size() >= 2)
    {
        seeds.pop_back();
    }
    seeds.insert(seeds.begin(), new_seed);
}

void GlWindow::clear_seeds()
{
    seeds.clear();
}

void GlWindow::visualize(GLfloat                height,
                         std::vector<fftw_real> scalar_field,
                         std::vector<fftw_real> vector_field_x,
                         std::vector<fftw_real> vector_field_y,
                         std::vector<fftw_real> vel_field_x,
                         std::vector<fftw_real> vel_field_y)
{
    if (Config::draw_smoke)
    {
        RenderSmoke::render_smoke(scalar_field, height);
    }

    if (Config::draw_vecs)
    {
        RenderVector::render_vector(scalar_field, vector_field_x, vector_field_y, height);
    }

    if (Config::draw_streamline && !Config::draw_slices)
    {
        RenderStreamline::render_streamlines(vel_field_x, vel_field_y, seeds, height);
    }

    if (Config::draw_streamline && Config::draw_slices)
    {
        RenderStreamline::render_streamsurf(buffer_vel_field_x, buffer_vel_field_y, seeds);
    }
}

void GlWindow::visualize_slices()
{
    GLfloat          height = -(int)((float)Config::buffer_size / (float)Config::buffer_delta) * 20;
    std::vector<int> iter   = {};
    for (int i = 0; i < Config::buffer_size; i += Config::buffer_delta)
    {
        iter.push_back(i);
    }

    for (int i = 0; i < iter.size(); i++, height -= 20)
    {
        visualize(height,
                  buffer_scalar_field[i],
                  buffer_vector_field_x[i],
                  buffer_vector_field_y[i],
                  buffer_vel_field_x[i],
                  buffer_vel_field_y[i]);
    }
}

void GlWindow::camera_zoom_out()
{
    radius += 100;
    eye[0] = -radius * sin(angle);
    eye[2] = radius * cos(angle);

    make_current();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70, 1, 0, 2000);
    // gluPerspective(70, 1, -1000, 1000);
    gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);
}

void GlWindow::camera_zoom_in()
{
    radius -= 100;
    eye[0] = -radius * sin(angle);
    eye[2] = radius * cos(angle);

    make_current();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70, 1, 0, 2000);
    // gluPerspective(70, 1, -1000, 1000);
    gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);
}

void GlWindow::camera_rotate_left()
{
    // https://stackoverflow.com/questions/2259476/rotating-a-point-about-another-point-2d
    angle -= M_PI / 12;
    eye[0] = -radius * sin(angle);
    eye[2] = radius * cos(angle);

    make_current();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70, 1, 0, 2000);
    // gluPerspective(70, 1, -1000, 1000);
    gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);
}

void GlWindow::camera_rotate_right()
{
    angle += M_PI / 12;
    eye[0] = -radius * sin(angle);
    eye[2] = radius * cos(angle);

    make_current();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70, 1, 0, 2000);
    // gluPerspective(70, 1, -1000, 1000);
    gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);
}