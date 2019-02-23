#include "config.h"
#include "controller.h"

int main(int argc, char **argv)
{
    Controller *controller = new Controller();
    controller->begin(argc, argv);

    return 0;
}