#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "simulation.h"

namespace global_sim
{
    Simulation *ptr_simulation;
    void        set_ptr(Simulation *simulation);
} // namespace global_sim
class Controller
{
  public:
    static void keyboard(unsigned char key, int x, int y);
    static void drag(int mx, int my);
};

#endif