#include "config.h"
#include "simulation.h"
#include "view.h"
#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include <rfftw.h>

int main(int argc, char **argv)
{
    Simulation simulation;
    Window     window;

    printf("Fluid Flow Simulation and Visualization\n");
    printf("=======================================\n");
    printf("Click and drag the mouse to steer the flow!\n");
    printf("T/t:   increase/decrease simulation timestep\n");
    printf("S/s:   increase/decrease hedgehog scaling\n");
    printf("c:     toggle direction coloring on/off\n");
    printf("V/v:   increase decrease fluid viscosity\n");
    printf("x:     toggle drawing matter on/off\n");
    printf("y:     toggle drawing hedgehogs on/off\n");
    printf("m:     toggle thru scalar coloring\n");
    printf("a:     toggle the animation on/off\n");
    printf("q:     quit\n\n");

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Real-time smoke simulation and visualization");
    simulation.initialise();
    window.display(simulation);
    // glutDisplayFunc(display);
    // glutReshapeFunc(reshape);
    // glutIdleFunc(do_one_simulation_step);
    // glutKeyboardFunc(keyboard);
    // glutMotionFunc(drag);

    // init_simulation(DIM);	//initialize the simulation data structures
    glutMainLoop(); // calls do_one_simulation_step, keyboard, display, drag, reshape
    return 0;
}