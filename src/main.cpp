#include "config.h"
#include "controller.h"

int main(int argc, char **argv)
{
    Controller *controller = new Controller();
    controller->begin();

    return 0;
}