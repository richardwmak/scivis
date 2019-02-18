#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "simulation.h"

class Controller
{
  public:
    void keyboard(unsigned char key);
    void drag(int mx, int my, Simulation &simulation);
};

#endif