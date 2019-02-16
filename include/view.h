#include "config.h"
#include "simulation.h"
#include <GL/glut.h>

#ifndef VIEW_H
#define VIEW_H

class Window
{
  public:
    void rainbow(float value, float *R, float *G, float *B);

    void set_colormap(float vy);

    void direction_to_color(float x, float y, int method);

    void visualise(Simulation simulation);

    void display(Simulation simulation);
};

#endif