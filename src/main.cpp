#include "config.h"
#include "gl_window.h"
#include "simulation.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <iostream>
#include <math.h>
#include <rfftw.h>

int main(int argc, char **argv)
{
    Controller controller;
    controller.begin(argc, argv);
    return 0;
}